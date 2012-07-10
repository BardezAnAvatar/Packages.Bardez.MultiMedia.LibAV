

#include "Container.h"
#include "Error Helper.h"
#include "Frame BGRA.h"
#include "Frame Audio Int16.h"
#include "LibAV Pixel Format.h"
#include "LibAV Picture.h"


using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Bardez::Projects::MultiMedia::LibAV;


#pragma region Properties
/// <summary>Exposes an array of the stream info</summary>
array<StreamInfo^>^ Container::Streams::get()
{
	array<StreamInfo^>^ streams = gcnew array<StreamInfo^>(this->FormatContextPtr->nb_streams);

	for (UInt32 stream = 0; stream < this->FormatContextPtr->nb_streams; ++stream)
		streams[stream] = gcnew StreamInfo(this->FormatContextPtr->streams[stream]);

	return streams;
}

/// <summary>Exposes the base time (in seconds) of the entire media file</summary>
Rational^ Container::TimeBase::get()
{
	return gcnew Rational(1, AV_TIME_BASE);
}
#pragma endregion


#pragma region Methods
/// <summary>Opens the specified path</summary>
/// <param name="path">Path of the file to open</param>
void Container::OpenMediaFile(String^ path)
{
	char* unmanagedPath = reinterpret_cast<char*>(Marshal::StringToHGlobalAnsi(path).ToPointer());
	
	Int32 libavReturnCode;
	AVFormatContext* context = avformat_alloc_context();	//avformat.h @ 1168

	//open file
	libavReturnCode = avformat_open_input(&context, unmanagedPath, NULL, NULL);	//avformat.h @ 1321
	this->FormatContextPtr = context;

	//tidbit: -13 when the path is a directory.
	if (libavReturnCode < 0)
		throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(libavReturnCode, ErrorHelper::LibAvFormat));
}

/// <summary>Scans the container and streams for basic info</summary>
void Container::ReadStreamInfo()
{
	Int32 libavReturnCode = avformat_find_stream_info(this->FormatContextPtr, NULL);
	if (libavReturnCode < 0)
		throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(libavReturnCode, ErrorHelper::LibAvFormat));
}

/// <summary>Loads all the codecs that the caller specifies the indeces of</summary>
/// <param name="streamIndeces">Array of streams for whose codecs to load</param>
void Container::LoadCodecs(array<Int32>^ streamIndeces)
{
	for each(Int32 index in streamIndeces)
	{
		AVCodec* ptrCodec = avcodec_find_decoder(this->FormatContextPtr->streams[index]->codec->codec_id);
		Int32 libavReturnCode = avcodec_open2(this->FormatContextPtr->streams[index]->codec, ptrCodec, NULL);	//have you any option? no? NULL.

		if (libavReturnCode < 0)
			throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(libavReturnCode, ErrorHelper::LibAvCodec));
	}
}

/// <summary>
///		Decodes frames from the container and adds them to the appropriate streams.
///		Sleeps while buffers are full.
/// </summary>
/// <param name="streamBuffers">Collection of stream processing structures, grouped by type</param>
void Container::DecodeStreams(StreamBuffers^ streamBuffers)
{
	//TODO: I need to check for certain error types. Invalid data, perhaps, can continue. EOF stops.

	Boolean error = false;

	//allocate a frame to read
	AVFrame* pFrame = avcodec_alloc_frame();		//avcodec.h @ 3297
	
	//declare a packet; use its reference as a pointer as needed
	AVPacket packet;

	//colorspace conversion
	SwsContext* scalingContext = NULL;

	//return code for erroring libav* libraries
	Int32 libavReturnCode, frameFinished;

	try
	{
		//read all frames and frame types
		while (true)
		{
			//sleep while buffers are full, to allow the calling application to consume them
			while (streamBuffers->BuffersFull)
				System::Threading::Thread::Sleep(5);


			libavReturnCode = av_read_frame(this->FormatContextPtr, &packet); //avformat.h @ 1454
			//MVE seems to give -1 on error; it is not an EOF error, so not sure how to detect


			if (libavReturnCode > -1)
			{
				//frame Processing
				//check the packet's stream index, be it audio or video or whatever, and decode
				switch(this->FormatContextPtr->streams[packet.stream_index]->codec->codec_type)
				{
					case AVMEDIA_TYPE_VIDEO:

						if (streamBuffers->StreamsVideo[packet.stream_index]->Process)
						{
							libavReturnCode = avcodec_decode_video2(this->FormatContextPtr->streams[packet.stream_index]->codec, pFrame, &frameFinished, &packet);

							if (libavReturnCode > -1 && frameFinished != 0)
							{								
								if (scalingContext == NULL)
								{
									scalingContext = this->GenerateScaleContext(this->FormatContextPtr->streams[packet.stream_index]->codec);
								}
	
								Int32 height = this->FormatContextPtr->streams[packet.stream_index]->codec->height, width = this->FormatContextPtr->streams[packet.stream_index]->codec->width;

								//allocate a picture
								LibAVPicture^ picture = nullptr;
								try
								{
									picture = LibAVPicture::BuildPicture(LibAVPixelFormat::PIX_FMT_BGRA, height, width);
								}
								catch (Exception^ ex)
								{
									//deallocate packet
									av_free_packet(&packet);	//avcodec.h @ 3459
									throw;
								}
								
								if (libavReturnCode > -1)
								{
									//convert the frame to BGRA
									libavReturnCode = sws_scale(scalingContext, pFrame->data, pFrame->linesize, 0, this->FormatContextPtr->streams[packet.stream_index]->codec->height, picture->PicturePtr->data, picture->PicturePtr->linesize);
									
									//generate a new frame based off of the scaled frame
									if (libavReturnCode > -1)
										streamBuffers->StreamsVideo[packet.stream_index]->AddFrame(gcnew FrameBGRA(pFrame, picture, &packet));
								}
							}
						}

						break;


					case AVMEDIA_TYPE_AUDIO:

						if (streamBuffers->StreamsAudio[packet.stream_index]->Process)
						{
							libavReturnCode = avcodec_decode_audio4(this->FormatContextPtr->streams[packet.stream_index]->codec, pFrame, &frameFinished, &packet);
					
							if (libavReturnCode > -1 && frameFinished != 0)
								streamBuffers->StreamsAudio[packet.stream_index]->AddFrame(gcnew FrameAudioInt16(pFrame, &packet, this->FormatContextPtr->streams[packet.stream_index]->codec->channels));
						}
						break;

					//case AVMEDIA_TYPE_SUBTITLE:
					//	libavReturnCode = avcodec_decode_subtitle2(this->FormatContextPtr->streams[1]->codec, pFrame, &frameFinished, &packet);
					//	break;

					//case AVMEDIA_TYPE_DATA:
					//case AVMEDIA_TYPE_ATTACHMENT:
					//case AVMEDIA_TYPE_NB:
				}
			}
			else
			{
				AVIOContext* pb = this->FormatContextPtr->pb;
				Int32 temp = pb->eof_reached;
			}

			//free last read packet
			av_free_packet(&packet);	//avcodec.h @ 3459
			
			if (libavReturnCode < 0)
				break;
		}
	}
	finally
	{
		//free the scaling context
		sws_freeContext(scalingContext);

		//free the allocated frames
		av_free(pFrame);

		//close file
		av_close_input_file(this->FormatContextPtr);
		this->FormatContextPtr = NULL;
	}

	//throw any error?
	if (libavReturnCode < 0)
	{
		switch (libavReturnCode)
		{
		case AVERROR_EOF:	//what I would expect
		case -1:			//what MVE parser gives me
			break;
		default:
			throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(libavReturnCode, ErrorHelper::LibAvCodec));
			break;
		}
	}
}

/// <summary>Finds the most probable stream expected for the stream type specified</summary>
/// <param name="type">MediaType of stream to look for</param>
Int32 Container::FindBestStream(MediaType type)
{
	Int32 stream = av_find_best_stream(this->FormatContextPtr, (AVMediaType)(Int32)type, -1, -1, NULL, 0);

	if (stream < 0)
		throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(stream, ErrorHelper::LibAvFormat));

	return stream;
}

/// <summary>Closes the codecs for streams within this Container's AVFormatContext</summary>
void Container::CloseCodecs()
{
	AVFormatContext* pointer = this->FormatContextPtr;
	for (UInt32 index = 0; index < pointer->nb_streams; ++index)
	{
		avcodec_close(pointer->streams[index]->codec);
		pointer->streams[index]->codec = NULL;
	}
}
#pragma endregion


#pragma region Helpers
/// <summary>Generates a scaling context</summary>
/// <param name="codec">Codec context to populate the source to the scaling context</param>
SwsContext* Container::GenerateScaleContext(AVCodecContext* codec)
{
	Int32 width = codec->width, height = codec->height;

	SwsContext* context = NULL;

	//I only want RGB output for Direct2D, so hard code it.
	context = sws_getContext(width, height, codec->pix_fmt, width, height, PIX_FMT_BGRA, SWS_BICUBIC, NULL, NULL, NULL);

	if (context == NULL)
		throw gcnew ApplicationException(String::Format("Could not initialize a scaling context in {0}.", ErrorHelper::LibSwScale));

	return context;
}
#pragma endregion


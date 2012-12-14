

#include "Container.h"
#include "Error Helper.h"
#include "Frame BGRA.h"
#include "Frame Audio Int16.h"
#include "LibAV Pixel Format.h"
#include "LibAV Picture.h"
#include "LibAV Codec ID.h"


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
		AVCodecContext* codecContext = this->FormatContextPtr->streams[index]->codec;

		CodecID codec = codecContext->codec_id;

		//codecContext->codec_id maps to avcodec.h @ line 93
		AVCodec* ptrCodec = avcodec_find_decoder(codec);

		//CODEC_ID_TEXT returns NULL. In this case, do not load a codec and the stream should be decodable past this issue.
		if (ptrCodec != NULL)
		{
			Int32 libavReturnCode = avcodec_open2(codecContext, ptrCodec, NULL);	//have you any option? no? NULL.

			if (libavReturnCode < 0)
				throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(libavReturnCode, ErrorHelper::LibAvCodec));
		}
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

	//return code for erroring libav* libraries
	Int32 libavReturnCode, frameFinished;

	try
	{
		//read all frames and frame types
		while (true)
		{
			//TODO: Let there be some sort of messaging component that allows the Container to have a seek time set
			//	and a stop decoding flag set, so one can break out of this decoding loop and/or seek appropriately


			//sleep while buffers are full, to allow the calling application to consume them
			while (streamBuffers->BuffersFull)
				System::Threading::Thread::Sleep(5);

			//read a frame into a packet
			libavReturnCode = av_read_frame(this->FormatContextPtr, &packet); //avformat.h @ 1454
			//MVE seems to give -1 on error; it is not an EOF error, so not sure how to detect

			//error detection
			if (libavReturnCode > -1)
			{
				//frame Processing
				//check the packet's stream index, be it audio or video or whatever, and decode
				switch(this->FormatContextPtr->streams[packet.stream_index]->codec->codec_type)
				{
					case AVMEDIA_TYPE_VIDEO:

						if (streamBuffers->StreamsVideo[packet.stream_index]->Process)
						{
							//Decode the video frame. Note: this will set the memory pointers found in pFrame->data
							libavReturnCode = avcodec_decode_video2(this->FormatContextPtr->streams[packet.stream_index]->codec, pFrame, &frameFinished, &packet);

							//if there was no error AND the deode operation has finished a frame
							if (libavReturnCode > -1 && frameFinished != 0)
							{
								Int32 height = this->FormatContextPtr->streams[packet.stream_index]->codec->height;
								Int32 width = this->FormatContextPtr->streams[packet.stream_index]->codec->width;

								//allocate a picture
								LibAVPicture^ picture = nullptr;
								try
								{
									LibAVPixelFormat codecPixelFormat = static_cast<LibAVPixelFormat>(this->FormatContextPtr->streams[packet.stream_index]->codec->pix_fmt);
									
									//Documentation suggests the codec will manage that memory;
									//	when constructing the LibAVPicture to extract data, tell it not to dispose memory.
									picture = gcnew LibAVPicture(reinterpret_cast<AVPicture*>(pFrame), LibAVPictureDetail::Build(width, height, codecPixelFormat), true, false);
								}
								catch (Exception^) //name is unused thus warning, but catch apparently requires a reference type to compile.
								{
									//deallocate packet
									av_free_packet(&packet);	//avcodec.h @ 3459
									throw;
								}

								//extract the data
								MemoryStream^ pictureData = picture->Data;

								//generate a new frame based off of the scaled frame
								streamBuffers->StreamsVideo[packet.stream_index]->AddFrame(gcnew FrameBGRA(pFrame, picture->Detail, pictureData, &packet));

								//blank out the pointer so as to not delete pFrame
								picture->PicturePtr = NULL;

								delete picture;
								picture = nullptr;
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

					case AVMEDIA_TYPE_SUBTITLE:

						LibAVCodecID codecId = (LibAVCodecID)reinterpret_cast<CodecID>(this->FormatContextPtr->streams[packet.stream_index]->codec->codec_id);

						//switch on possible behavior for subtitles
						switch (codecId)
						{
							case LibAVCodecID::CODEC_ID_TEXT:
								//scope this to not matter to other cases
								{
									//process the raw text packet
									FrameSubtitleText^ subtitle = this->DecodeSubtitleText(&packet);
								}
								break;

							default:
								//There does not appear to be an API for allocating memory for an AVSubtitle structure
								AVSubtitle subtitleStruct;

								libavReturnCode = avcodec_decode_subtitle2(this->FormatContextPtr->streams[packet.stream_index]->codec, &subtitleStruct, &frameFinished, &packet);

								//if there was no error AND the deode operation has finished a frame
								if (libavReturnCode > -1 && frameFinished != 0)
								{
									//TODO: do something with the decoded subtitle. I think these are video objects.


									//free memory allocated to the subtitle struct
									avsubtitle_free(&subtitleStruct);
								}
								break;
						}

						break;

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


/// <summary>Decodes a text Subtitle</summary>
/// <param name="packet">Packet which contains initial data</param>
/// <returns>A managed FrameSubtitleText instance</returns>
/// <remarks>
///		Copied largely from httr://www.digipedia.pl/usenet/thread/16949/6631/
///		and definately derived from that thread
/// </remarks>
FrameSubtitleText^ Container::DecodeSubtitleText(AVPacket* packet)
{
	//grab a stream reference 
	AVStream* stream = this->FormatContextPtr->streams[packet->stream_index];

	//get the base time
	AVRational timeBase = stream->time_base;

	//get data
	uint8_t * text = packet->data;
	Int32 size = packet->size;

	//get presentation info
	Int64 TimeStampDecode = packet->dts;
	Int64 TimeStampPresentation = packet->pts;
	Int64 duration = packet->duration;
	Int64 convDuration = packet->convergence_duration;

	//now build something with it
	FrameSubtitleText^ subtitle = gcnew FrameSubtitleText(packet);

	return subtitle;
}
#pragma endregion


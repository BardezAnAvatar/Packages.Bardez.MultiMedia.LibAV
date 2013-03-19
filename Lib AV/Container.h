

#ifndef _Container_H_
#define _Container_H_

#include "Library References.h"

extern "C"
{
	#include "libavformat\avformat.h"
	#include "libavcodec\avcodec.h"
	#include "libavutil\avutil.h"
	#include "libswscale\swscale.h"
}

#include "Media Type.h"
#include "Stream Buffers.h"
#include "Stream Info.h"
#include "Stream Processing Info.h"
#include "Frame Subtitle Text.h"


using namespace System::Collections::Generic;
using namespace Bardez::Projects::BasicStructures::Math;


namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>Public-facing container which is used to open an AV file via LibAV</summary>
				public ref class Container
				{
				#pragma region Static fields
				internal:
					/// <summary>Flag indicating whether or not the libavformat's codecs, etc. were registered</summary>
					static System::Boolean Initialized = false;
				#pragma endregion


				#pragma region Fields
				internal:
					/// <summary>Pointer to the libav AVFormatContext used to spawn other references</summary>
					System::IntPtr formatContext;
				#pragma endregion


				#pragma region Properties
				internal:
					/// <summary>Pointer to libavcodec Codec Context, from which to derive public properties</summary>
					property AVFormatContext* FormatContextPtr
					{
						AVFormatContext* get()
						{
							return reinterpret_cast<AVFormatContext*>(this->formatContext.ToPointer());
						}
						void set(AVFormatContext* value)
						{
							this->formatContext = System::IntPtr(value);
						}
					}

				public:
					/// <summary>Exposes an array of the stream info</summary>
					property array<StreamInfo^>^ Streams
					{
						array<StreamInfo^>^ get();
					}

					/// <summary>Exposes the base time (in seconds) of the entire media file</summary>
					property Rational^ TimeBase
					{
						Rational^ get();
					}

					/// <summary>Exposes the number of increments of TimeBase that indicate the file's start time</summary>
					property Int64 StartTime
					{
						Int64 get()
						{
							return this->FormatContextPtr->start_time;
						}
					}

					/// <summary>Exposes the number of increments of TimeBase that indicate the file's duration</summary>
					property Int64 Duration
					{
						Int64 get()
						{
							return this->FormatContextPtr->duration;
						}
					}
				#pragma endregion


				#pragma region Construction
				public:
					/// <summary>Default constructor</summary>
					Container()
					{
						if (!Container::Initialized)
						{
							av_register_all();
							Container::Initialized = true;
						}
						
						this->formatContext = System::IntPtr::Zero;
					}
				#pragma endregion


				#pragma region Destruction
				public:
					/// <summary>Destructor</summary>
					/// <remarks>Dispose()</remarks>
					~Container()
					{
						this->DisposeUnmanaged();
					}
					
					/// <summary>Destructor</summary>
					/// <remarks>Finalize()</remarks>
					!Container()
					{
						this->DisposeUnmanaged();
					}

				internal:
					/// <summary>Destructor logic, disposes the object</summary>
					void DisposeUnmanaged()
					{
						//close the AV format context
						if (this->FormatContextPtr)
						{
							AVFormatContext* nativePtr = this->FormatContextPtr;

							avformat_close_input(&nativePtr);		//access violation after recieving an End Of File error message
							this->formatContext = System::IntPtr::Zero;
						}
					}
				#pragma endregion


				#pragma region Methods
				public:
					/// <summary>Opens the specified path</summary>
					/// <param name="path">Path of the file to open</param>
					void OpenMediaFile(System::String^ path);

					/// <summary>Scans the container and streams for basic info</summary>
					void ReadStreamInfo();

					/// <summary>Loads all the codecs that the caller specifies the indeces of</summary>
					/// <param name="streamIndeces">Array of streams for whose codecs to load</param>
					void LoadCodecs(array<Int32>^ streamIndeces);

					/// <summary>
					///		Decodes frames from the container and adds them to the appropriate streams.
					///		Sleeps while buffers are full.
					/// </summary>
					/// <param name="streamBuffers">Collection of stream processing structures, grouped by type</param>
					void DecodeStreams(StreamBuffers^ streamBuffers);

					/// <summary>Finds the most probable stream expected for the stream type specified</summary>
					/// <param name="type">MediaType of stream to look for</param>
					Int32 FindBestStream(MediaType type);

					/// <summary>Closes the codecs for streams within this Container's AVFormatContext</summary>
					void CloseCodecs();

					/// <summary>Decodes a text Subtitle</summary>
					/// <param name="packet">Packet which contains initial data</param>
					/// <returns>A managed FrameSubtitleText instance</returns>
					/// <remarks>
					///		Copied largely from http://www.digipedia.pl/usenet/thread/16949/6631/
					///		and definitely derived from that thread
					/// </remarks>
					FrameSubtitleText^ DecodeSubtitleText(AVPacket* packet);
				#pragma endregion
				};
			}
		}
	}
}


#endif


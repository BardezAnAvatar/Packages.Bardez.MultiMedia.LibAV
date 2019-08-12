

#ifndef _StreamInfo_H_
#define _StreamInfo_H_


extern "C"
{
	#include "libavformat\avformat.h"
}


#include "Codec Info.h"


using namespace System;
using namespace Bardez::Projects::BasicStructures::Math;
using namespace Bardez::Projects::BasicStructures::Win32::Audio;
using namespace Bardez::Projects::Multimedia::MediaBase::Frame::Image;


namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>Managed class that exposes individual stream information to calling program</summary>
				public ref class StreamInfo
				{
				#pragma region Fields
				internal:
					/// <summary>libavformat Stream to read info data from</summary>
					IntPtr stream;
				#pragma endregion


				#pragma region Properties
				internal:
					/// <summary>Pointer to libavcodec stream, from which to derive public properties</summary>
					property AVStream* StreamPtr
					{
						AVStream* get()
						{
							return reinterpret_cast<AVStream*>(this->stream.ToPointer());
						}
						void set(AVStream* value)
						{
							this->stream = System::IntPtr(value);
						}
					}

				public:
					/// <summary>Index of this stream</summary>
					property System::Int32 Index
					{
						System::Int32 get();
					}

					/// <summary>ID of this stream. Format-specific</summary>
					property System::Int32 ID
					{
						System::Int32 get();
					}

					/// <summary>Reference to codec information about this stream</summary>
					property CodecInfo^ Codec
					{
						CodecInfo^ get();
					}

					/// <summary>Rational number representation of the stream's frame rate</summary>
					property Rational^ FrameRate
					{
						Rational^ get();
					}

					/// <summary>Rational number base of all timestamps</summary>
					property Rational^ TimeBase
					{
						Rational^ get();
					}

					/// <summary>Timestamp for the start time of this stream</summary>
					property System::Int64 StartTime
					{
						System::Int64 get();
					}

					/// <summary>Timestamp for the length of this stream</summary>
					property System::Int64 Duration
					{
						System::Int64 get();
					}

					/// <summary>Total count of frames in this stream</summary>
					/// <value>Can be 0 if unknown</value>
					property System::Int64 FrameCount
					{
						System::Int64 get();
					}

					/// <summary>Rational number representation of the aspect ratio</summary>
					property Rational^ AspectRatio
					{
						Rational^ get();
					}

					/// <summary>Rational number representation of the stream's average frame rate</summary>
					property Rational^ FrameRateAverage
					{
						Rational^ get();
					}
				#pragma endregion


				#pragma region Construction
				public:
					/// <summary>Default constructor</summary>
					StreamInfo()
					{
						this->stream = IntPtr::Zero;
					}

				internal:
					/// <summary>Definition constructor</summary>
					StreamInfo(AVStream* ptrStream)
					{
						this->StreamPtr = ptrStream;
					}
				#pragma endregion


				#pragma region Destruction
				public:
					/// <summary>Destrutor</summary>
					/// <remarks>Dispose()</remarks>
					~StreamInfo()
					{
						this->DisposeUnmanaged();
					}
					
					/// <summary>Destrutor</summary>
					/// <remarks>Finalize()</remarks>
					!StreamInfo()
					{
						this->DisposeUnmanaged();
					}
					
				internal:
					/// <summary>Destrutor logic, disposes the object</summary>
					void DisposeUnmanaged()
					{
						//remove reference to the stream
						if (this->StreamPtr)
							this->stream = IntPtr::Zero;
					}
				#pragma endregion


				#pragma region Metadata generation
				internal:
					/// <summary>Generates audio metadata for the stream</summary>
					/// <returns>A <see cref="WaveFormatEx" /> for the audio stream or null if a non-audio stream</returns>
					WaveFormatEx^ GenerateMetadataAudio();

					/// <summary>Generates image metadata for the stream</summary>
					/// <returns>An <see cref="ImageMetadata" /> for the video stream or null if a non-video stream</returns>
					ImageMetadata^ GenerateMetadataImage();
				#pragma endregion
				};
			}
		}
	}
}

#endif


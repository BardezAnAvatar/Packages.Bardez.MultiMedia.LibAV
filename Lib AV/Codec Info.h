

#ifndef _Components_Codec_Info_H_
#define _Components_Codec_Info_H_

extern "C"
{
	#include "libavcodec\avcodec.h"
}

#include "Audio Sample Format.h"
#include "Codec ID.h"
#include "Media Type.h"
#include "LibAV Pixel Format.h"
#include "Rational.h"

namespace Bardez
{
	namespace Projects
	{
		namespace MultiMedia
		{
			namespace LibAV
			{
				/// <summary>Managed type that exposes info that the using application may be interested in</summary>
				public ref class CodecInfo
				{
				#pragma region Fields
				internal:
					/// <summary>Pointer to libavcodec Codec Context, from which to derive public properties</summary>
					System::IntPtr codecContextIntPointer;
				#pragma endregion

					
				#pragma region Properties
				internal:
					/// <summary>Pointer to libavcodec Codec Context, from which to derive public properties</summary>
					property AVCodecContext* CodecContextPtr
					{
						AVCodecContext* get()
						{
							return reinterpret_cast<AVCodecContext*>(this->codecContextIntPointer.ToPointer());
						}
						void set(AVCodecContext* value)
						{
							this->codecContextIntPointer = System::IntPtr(value);
						}
					}

				public:
					/// <summary>Name of the codec</summary>
					property System::String^ Name
					{
						System::String^ get();
					}

					/// <summary>Descriptive name of the codec</summary>
					property System::String^ LongName
					{
						System::String^ get();
					}

					/// <summary>Type of this codec (Audio, video, etc.)</summary>
					property MediaType CodecType
					{
						MediaType get();
					}

					/// <summary>Unique ID of the codec within Lib AV</summary>
					property CodecID ID
					{
						CodecID get();
					}

					/// <summary>(Possibly erroneously) converted FourCC code from a container</summary>
					property System::String^ FourCC
					{
						System::String^ get();
					}

					/// <summary>The average bitrate</summary>
					property System::Int32 BitRate
					{
						System::Int32 get();
					}

					/// <summary>
					///		This is the fundamental unit of time (in seconds) in terms of which frame
					///		timestamps are represented. For fixed-fps content, timebase should be 1/framerate
					///		and timestamp increments should be identically 1.
					/// </summary>
					property Rational^ TimeBase
					{
						Rational^ get();
					}

					/// <summary>
					///		For some codecs, the time base is closer to the field rate than the frame rate.
					///		Most notably, H.264 and MPEG-2 specify time_base as half of frame duration
					///		if no telecine is used ...
					///		
					///		Set to time_base ticks per frame. Default 1, e.g., H.264/MPEG-2 set it to 2.
					/// </summary>
					property System::Int32 TicksPerFrame
					{
						System::Int32 get();
					}
		
					/// <summary>
					///		Encoding: Number of frames delay there will be from the encoder input to
					///		          the decoder output. (we assume the decoder matches the spec)
					///		Decoding: Number of frames delay in addition to what a standard decoder
					///		          as specified in the spec would produce.
					///		
					///		Video:
					///		  Number of frames the decoded output will be delayed relative to the
					///		  encoded input.
					///		
					///		Audio:
					///		  Number of "priming" samples added to the beginning of the stream
					///		  during encoding. The decoded output will be delayed by this many
					///		  samples relative to the input to the encoder. Note that this field is
					///		  purely informational and does not directly affect the pts output by
					///		  the encoder, which should always be based on the actual presentation
					///		  time, including any delay.
					/// </summary>
					property System::Int32 Delay
					{
						System::Int32 get();
					}

					/// <summary>Picture width</summary>
					property System::Int32 Width
					{
						System::Int32 get();
					}

					/// <summary>Picture height</summary>
					property System::Int32 Height
					{
						System::Int32 get();
					}

					/// <summary>Picture width as coded</summary>
					property System::Int32 CodedWidth
					{
						System::Int32 get();
					}

					/// <summary>Picture height as coded</summary>
					property System::Int32 CodedHeight
					{
						System::Int32 get();
					}

					/// <summary>Output pixel data format</summary>
					property LibAVPixelFormat PixelDataFormat
					{
						LibAVPixelFormat get();
					}

					/// <summary>Aspect ratio of the pixels</summary>
					property Rational^ SampleAspectRatio
					{
						Rational^ get();
					}

					/// <summary>Sample rate of audio</summary>
					property System::Int32 SampleRate
					{
						System::Int32 get();
					}

					/// <summary>Number of audio channels</summary>
					property System::Int32 ChannelCount
					{
						System::Int32 get();
					}

					/// <summary>Output format of audio samples</summary>
					property AuioSampleFormat SampleFormat
					{
						AuioSampleFormat get();
					}
			#pragma endregion


			#pragma region Construction
				public:
					/// <summary>Default constructor</summary>
					CodecInfo() { }
		
				internal:
					/// <summary>Definition constructor</summary>
					/// <param name="context">Unmanaged AVCodecContext to copy from</param>
					CodecInfo(AVCodecContext* context);
			#pragma endregion


			#pragma region Destruction
				public:
					/// <summary>Destructor</summary>
					/// <remarks>Dispose()</remarks>
					~CodecInfo()
					{
						this->DisposeUnmanaged();
					}

					/// <summary>Destructor</summary>
					/// <remarks>Finalize()</remarks>
					!CodecInfo()
					{
						this->DisposeUnmanaged();
					}
		
				internal:
					/// <summary>Destructor logic, disposes the object</summary>
					void DisposeUnmanaged()
					{
						this->codecContextIntPointer = System::IntPtr::Zero;
					}
			#pragma endregion
				};
			}
		}
	}
}

#endif


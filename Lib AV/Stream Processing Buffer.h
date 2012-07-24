

#ifndef _Stream_Processing_Buffer_H_
#define _Stream_Processing_Buffer_H_

extern "C"
{
	#include "libavformat\avformat.h"
}

#include "Codec Info.h"
#include "Frame AV.h"
#include "Rational.h"

using namespace System;
using namespace System::Collections::Generic;

namespace Bardez
{
	namespace Projects
	{
		namespace MultiMedia
		{
			namespace LibAV
			{
				/// <summary>Represents the combination of a buffer and Stream info derived from a pointer</summary>
				generic<class FrameType> where FrameType : FrameAV, ref class
				public ref class StreamProcessingBuffer
				{
				#pragma region Fields
				public:
					/// <summary>Collection of data on this stream, if collected</summary>
					List<FrameType>^ DataStream;

					/// <summary>Flag indicating whether to process data from the stream this info represents.</summary>
					Boolean Process;

					/// <summary>Maximum buffer size for the associated stream</summary>
					Int32 BufferSize;

					/// <summary>The current position within the DataStream indicating where to start retrieving frames</summary>
					Int32 BufferPosition;

					/// <summary>The end position within the DataStream indicating how long the available buffer is</summary>
					Int32 BufferEnd;
				
				private:
					/// <summary>Locking object for pushing/popping frames</summary>
					Object^ lockingObject;

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

					/// <summary>Exposes the size of the buffer available to be read.</summary>
					property Int32 BufferAvailable
					{
						Int32 get();
					}

					/// <summary>Exposes a flag indicating whether or not the buffer is full</summary>
					property Boolean BufferFull
					{
						Boolean get();
					}

					/// <summary>Exposes the start time as a time span</summary>
					property TimeSpan StreamStartTime
					{
						TimeSpan get();
					}

					/// <summary>Exposes the end time as a time span</summary>
					property TimeSpan StreamEndTime
					{
						TimeSpan get();
					}
				#pragma endregion


				#pragma region Construction
				internal:
					/// <summary>Definition constructor</summary>
					/// <param name="ptrStream">Pointer to an AVStream instance</param>
					/// <param name="bufferSize">Size of the buffer to instantiate. less than 1 will default to 30.</param>
					StreamProcessingBuffer(AVStream* ptrStream, Int32 bufferSize)
					{
						this->StreamPtr = ptrStream;
						this->DataStream = gcnew List<FrameType>();
						this->lockingObject = gcnew Object();
						this->Process = false;

						//set the size of the internal frame buffer.
						if (bufferSize < 1)
							bufferSize = 30;
						this->BufferSize = bufferSize;
					}
				#pragma endregion


				#pragma region Destruction
				public:
					/// <summary>Destrutor</summary>
					/// <remarks>Dispose()</remarks>
					~StreamProcessingBuffer()
					{
						this->DisposeUnmanaged();
					}
					
					/// <summary>Destrutor</summary>
					/// <remarks>Finalize()</remarks>
					!StreamProcessingBuffer()
					{
						this->DisposeUnmanaged();
					}

					/// <summary>Destrutor logic, disposes the object</summary>
					void DisposeUnmanaged()
					{
						//remove reference to the stream
						if (this->StreamPtr)
							this->stream = IntPtr::Zero;
					}
				#pragma endregion


				#pragma region Stream Access
				public:
					/// <summary>Adds (pushes) a frame onto the stream</summary>
					/// <param name="frame">Frame to be pushed onto the stream's queue</param>
					void AddFrame(FrameType frame);

					/// <summary>Adds (pushes) a frame onto the stream</summary>
					/// <returns>null if no frames available</returns>
					FrameType PeekFrame();

					/// <summary>Consumes and removes (pops) the next frame from the stream</summary>
					/// <returns>null if no frames available</returns>
					FrameType ConsumeFrame();
				#pragma endregion
				};
			}
		}
	}
}
#endif


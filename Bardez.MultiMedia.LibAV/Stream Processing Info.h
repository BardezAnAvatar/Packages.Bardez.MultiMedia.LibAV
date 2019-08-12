

#ifndef _StreamProcessingInfo_H_
#define _StreamProcessingInfo_H_

#include "Frame AV.h"

using namespace System;
using namespace System::Collections::Generic;

//TODO: Deprecate this code file, as Info is replaced by the actual Buffer, StreamProcessingBuffer

namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>
				///		Processing info for a stream, indicating how to track it, its buffer size, etc.
				///		Meant to be used in conjunction with data reported from a collection of Stream Info,
				///		expressing how the consuming application would like to process A/V streams
				/// </summary>
				/// <remarks>The index of this processing info will implicitly match to the stream number reported from a Container/AVFormatContext</remarks>
				generic<class FrameType> where FrameType : FrameAV, ref class
					public ref class StreamProcessingInfo
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

					/// <summary>Locking object for pushing/popping frames</summary>
					Object^ lockingObject;
				#pragma endregion


				#pragma region Properties
				public:
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
				#pragma endregion


				#pragma region Construction
				public:
					/// <summary>Default constructor</summary>
					StreamProcessingInfo()
					{
						this->DataStream = gcnew List<FrameType>();
						this->lockingObject = gcnew Object();
						this->Process = false;
						this->BufferSize = 30;
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


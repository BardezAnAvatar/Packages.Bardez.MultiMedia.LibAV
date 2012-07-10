
#include "Stream Processing Info.h"

using namespace System;
using namespace System::Threading;
using namespace Bardez::Projects::MultiMedia::LibAV;


#pragma region Properties
/// <summary>Exposes the size of the buffer available to be read.</summary>
generic<class FrameType> where FrameType : FrameAV, ref class
Int32 StreamProcessingInfo<FrameType>::BufferAvailable::get()
{
	return this->BufferEnd - this->BufferPosition;
}

/// <summary>Exposes a flag indicating whether or not the buffer is full</summary>
generic<class FrameType> where FrameType : FrameAV, ref class
Boolean StreamProcessingInfo<FrameType>::BufferFull::get()
{
	//if not processing, do not indicate full.
	//if buffer available > 0, do not indicate full
	return this->Process & (this->BufferAvailable >= this->BufferSize);
}
#pragma endregion


#pragma region Stream Access
/// <summary>Adds (pushes) a frame onto the stream</summary>
/// <param name="frame">Frame to be pushed onto the stream's queue</param>
generic<class FrameType> where FrameType : FrameAV, ref class
	void StreamProcessingInfo<FrameType>::AddFrame(FrameType frame)
{
	Monitor::Enter(this->lockingObject);
	
	this->DataStream->Add(frame);
	++this->BufferEnd;

	Monitor::Exit(this->lockingObject);
}

/// <summary>Adds (pushes) a frame onto the stream</summary>
/// <returns>null if no frames available</returns>
generic<class FrameType> where FrameType : FrameAV, ref class
	FrameType StreamProcessingInfo<FrameType>::PeekFrame()
{
	FrameType frame = FrameType();	//default, should be nullptr

	Monitor::Enter(this->lockingObject);

	if (this->BufferPosition >= this->DataStream->Count)
		frame = this->DataStream[this->BufferPosition];

	Monitor::Exit(this->lockingObject);

	return frame;
}

/// <summary>Consumes and removes (pops) the next frame from the stream</summary>
/// <returns>null if no frames available</returns>
generic<class FrameType> where FrameType : FrameAV, ref class
	FrameType StreamProcessingInfo<FrameType>::ConsumeFrame()
{
	FrameType frame = FrameType();	//default, should be nullptr

	Monitor::Enter(this->lockingObject);

	if (this->BufferPosition < this->DataStream->Count)
	{
		frame = this->DataStream[this->BufferPosition];
		this->DataStream[this->BufferPosition] = FrameType();	//default, should be nullptr
		++this->BufferPosition;
	}

	Monitor::Exit(this->lockingObject);

	return frame;
}
#pragma endregion


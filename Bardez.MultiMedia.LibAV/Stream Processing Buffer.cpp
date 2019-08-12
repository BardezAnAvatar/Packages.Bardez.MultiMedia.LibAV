

#include "Stream Processing Buffer.h"
#include "Rational Extender.h"


using namespace System;
using namespace System::Threading;
using namespace Bardez::Projects::Multimedia::LibAV;


#pragma region Properties
/// <summary>Index of this stream</summary>
generic<class FrameType> where FrameType : FrameAV, ref class
Int32 StreamProcessingBuffer<FrameType>::Index::get()
{
	return this->StreamPtr->index;
}

/// <summary>ID of this stream. Format-specific</summary>
generic<class FrameType> where FrameType : FrameAV, ref class
Int32 StreamProcessingBuffer<FrameType>::ID::get()
{
	return this->StreamPtr->id;
}

/// <summary>Reference to codec information about this stream</summary>
generic<class FrameType> where FrameType : FrameAV, ref class
CodecInfo^ StreamProcessingBuffer<FrameType>::Codec::get()
{
	return gcnew CodecInfo(this->StreamPtr->codec);
}

/// <summary>Rational number representation of the stream's frame rate</summary>
generic<class FrameType> where FrameType : FrameAV, ref class
Rational^ StreamProcessingBuffer<FrameType>::FrameRate::get()
{
	return RationalExtender::ToRational(this->StreamPtr->r_frame_rate);
}

/// <summary>Rational number base of all timestamps</summary>
generic<class FrameType> where FrameType : FrameAV, ref class
Rational^ StreamProcessingBuffer<FrameType>::TimeBase::get()
{
	return RationalExtender::ToRational(this->StreamPtr->time_base);
}

/// <summary>Timestamp for the start time of this stream</summary>
generic<class FrameType> where FrameType : FrameAV, ref class
Int64 StreamProcessingBuffer<FrameType>::StartTime::get()
{
	return this->StreamPtr->start_time;
}

/// <summary>Timestamp for the length of this stream</summary>
generic<class FrameType> where FrameType : FrameAV, ref class
Int64 StreamProcessingBuffer<FrameType>::Duration::get()
{
	return this->StreamPtr->duration;
}

/// <summary>Total count of frames in this stream</summary>
generic<class FrameType> where FrameType : FrameAV, ref class
/// <value>Can be 0 if unknown</value>
Int64 StreamProcessingBuffer<FrameType>::FrameCount::get()
{
	return this->StreamPtr->nb_frames;
}

/// <summary>Rational number representation of the aspect ratio</summary>
generic<class FrameType> where FrameType : FrameAV, ref class
Rational^ StreamProcessingBuffer<FrameType>::AspectRatio::get()
{
	return RationalExtender::ToRational(this->StreamPtr->sample_aspect_ratio);
}

/// <summary>Rational number representation of the stream's average frame rate</summary>
generic<class FrameType> where FrameType : FrameAV, ref class
Rational^ StreamProcessingBuffer<FrameType>::FrameRateAverage::get()
{
	return RationalExtender::ToRational(this->StreamPtr->avg_frame_rate);
}

/// <summary>Exposes the size of the buffer available to be read.</summary>
generic<class FrameType> where FrameType : FrameAV, ref class
Int32 StreamProcessingBuffer<FrameType>::BufferAvailable::get()
{
	return this->BufferEnd - this->BufferPosition;
}

/// <summary>Exposes a flag indicating whether or not the buffer is full</summary>
generic<class FrameType> where FrameType : FrameAV, ref class
Boolean StreamProcessingBuffer<FrameType>::BufferFull::get()
{
	//if not processing, do not indicate full.
	//if buffer available > 0, do not indicate full
	return this->Process & (this->BufferAvailable >= this->BufferSize);
}

/// <summary>Exposes the start time as a time span</summary>
generic<class FrameType> where FrameType : FrameAV, ref class
TimeSpan StreamProcessingBuffer<FrameType>::StreamStartTime::get()
{
	return RationalExtender::GetTimeSpan(this->TimeBase, this->StartTime);
}

/// <summary>Exposes the end time as a time span</summary>
generic<class FrameType> where FrameType : FrameAV, ref class
TimeSpan StreamProcessingBuffer<FrameType>::StreamEndTime::get()
{
	TimeSpan ts = this->StreamStartTime;
	ts += RationalExtender::GetTimeSpan(this->TimeBase, this->Duration);
	return ts;
}
#pragma endregion




#pragma region Stream Access
/// <summary>Adds (pushes) a frame onto the stream</summary>
/// <param name="frame">Frame to be pushed onto the stream's queue</param>
generic<class FrameType> where FrameType : FrameAV, ref class
void StreamProcessingBuffer<FrameType>::AddFrame(FrameType frame)
{
	Monitor::Enter(this->lockingObject);
	
	this->DataStream->Add(frame);
	++this->BufferEnd;

	Monitor::Exit(this->lockingObject);
}

/// <summary>Adds (pushes) a frame onto the stream</summary>
/// <returns>null if no frames available</returns>
generic<class FrameType> where FrameType : FrameAV, ref class
FrameType StreamProcessingBuffer<FrameType>::PeekFrame()
{
	FrameType frame = FrameType();	//default, should be nullptr

	Monitor::Enter(this->lockingObject);

	if (this->BufferPosition < this->DataStream->Count)
		frame = this->DataStream[this->BufferPosition];

	Monitor::Exit(this->lockingObject);

	return frame;
}

/// <summary>Consumes and removes (pops) the next frame from the stream</summary>
/// <returns>null if no frames available</returns>
generic<class FrameType> where FrameType : FrameAV, ref class
FrameType StreamProcessingBuffer<FrameType>::ConsumeFrame()
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




#include "Stream Info.h"


using namespace Bardez::Projects::MultiMedia::LibAV;


#pragma region Properties
/// <summary>Index of this stream</summary>
System::Int32 StreamInfo::Index::get()
{
	return this->StreamPtr->index;
}

/// <summary>ID of this stream. Format-specific</summary>
System::Int32 StreamInfo::ID::get()
{
	return this->StreamPtr->id;
}

/// <summary>Reference to codec information about this stream</summary>
CodecInfo^ StreamInfo::Codec::get()
{
	return gcnew CodecInfo(this->StreamPtr->codec);
}

/// <summary>Rational number representation of the stream's frame rate</summary>
Rational^ StreamInfo::FrameRate::get()
{
	return gcnew Rational(this->StreamPtr->r_frame_rate);
}

/// <summary>Rational number base of all timestamps</summary>
Rational^ StreamInfo::TimeBase::get()
{
	return gcnew Rational(this->StreamPtr->time_base);
}

/// <summary>Timestamp for the start time of this stream</summary>
System::Int64 StreamInfo::StartTime::get()
{
	return this->StreamPtr->start_time;
}

/// <summary>Timestamp for the length of this stream</summary>
System::Int64 StreamInfo::Duration::get()
{
	return this->StreamPtr->duration;
}

/// <summary>Total count of frames in this stream</summary>
/// <value>Can be 0 if unknown</value>
System::Int64 StreamInfo::FrameCount::get()
{
	return this->StreamPtr->nb_frames;
}

/// <summary>Rational number representation of the aspect ratio</summary>
Rational^ StreamInfo::AspectRatio::get()
{
	return gcnew Rational(this->StreamPtr->sample_aspect_ratio);
}

/// <summary>Rational number representation of the stream's average frame rate</summary>
Rational^ StreamInfo::FrameRateAverage::get()
{
	return gcnew Rational(this->StreamPtr->avg_frame_rate);
}
#pragma endregion




#include "Frame LibAV.h"
#include "Rational Extender.h"


using namespace System;
using namespace System::IO;
using namespace Bardez::Projects::Multimedia::LibAV;


#pragma region Properties
/// <summary>Presentation timestap (multiple of least common factor's timebase, where this frame should be presented)</summary>
TimeSpan FrameLibAV::PresentationTimeStamp::get()
{
	return this->presentationTimeStamp;
}

/// <summary>Decoding timestap (multiple of least common factor's timebase, where this frame appeared in the stream)</summary>
TimeSpan FrameLibAV::DecodeTimeStamp::get()
{
	return this->decodeTimeStamp;
}

/// <summary>Indicated whether this frame is a keyframe or not</summary>
Boolean FrameLibAV::KeyFrame::get()
{
	return this->keyFrame;
}

/// <summary>The streaming metadata info</summary>
StreamingMetadata^ FrameLibAV::MetadataStreaming::get()
{
	return this->metadataStreaming;
}
#pragma endregion


#pragma region IMultimediaFrame Properties
/// <summary>The Stream exposed for Frame data</summary>
Stream^ FrameLibAV::Data::get()
{
	return gcnew MemoryStream(this->frameData);
}
#pragma endregion


#pragma region Construction
/// <summary>Definition constructor</summary>
/// <param name="source">Source unmanaged frame to read from</param>
/// <param name="packet">Packet to read timestamps from</param>
/// <param name="stream">Source stream for this Frame</summary>
FrameLibAV::FrameLibAV(AVFrame* source, AVPacket* packet, AVStream* stream)
{
	//This is the fundamental unit of time (in seconds) in terms of which frame
	//timestamps are represented. For fixed-fps content, timebase should be 1/framerate
	//and timestamp increments should be identically 1.
	Rational^ timeBase = RationalExtender::ToRational(stream->time_base);

	this->presentationTimeStamp = RationalExtender::GetTimeSpan(timeBase, packet->pts);
	this->decodeTimeStamp = RationalExtender::GetTimeSpan(timeBase, packet->dts);
	this->keyFrame = Convert::ToBoolean(source->key_frame);

	//duration of the frame's presentation
	TimeSpan durationTimeSpan = RationalExtender::GetTimeSpan(timeBase, packet->duration);
	TimeSpan endTimeStamp = this->presentationTimeStamp + durationTimeSpan;

	//which variant to use? Not even sure why I need it...
	Rational^ sampleRate = gcnew Rational(timeBase->Denominator, timeBase->Numerator);
	//Rational^ sampleRate = RationalExtender::ToRational(stream->avg_frame_rate);

	this->metadataStreaming = gcnew StreamingMetadata(sampleRate, this->presentationTimeStamp, endTimeStamp);
}
#pragma endregion


#pragma region Destruction
/// <summary>Destructor</summary>
/// <remarks>Dispose()</remarks>
FrameLibAV::~FrameLibAV()
{
	this->DisposeUnmanaged();
	GC::SuppressFinalize(this);
}

/// <summary>Destructor logic, disposes the object</summary>
void FrameLibAV::DisposeUnmanaged()
{
}
#pragma endregion




#include "Frame AV.h"

using namespace System;
using namespace Bardez::Projects::MultiMedia::LibAV;


#pragma region Construction
/// <summary>Definition constructor</summary>
/// <param name="source">Source unmanaged frame to read from</param>
/// <param name="packet">Packet to read timestamps from</param>
FrameAV::FrameAV(AVFrame* source, AVPacket* packet)
{
	this->TimeStampDecode = packet->dts;	//drop this?
	this->TimeStampPresentation = packet->pts;
	this->KeyFrame = Convert::ToBoolean(source->key_frame);
}
#pragma endregion


#pragma region Methods
/// <summary>Retrieves a System::TimeSpan instance based off of the timeBase and TimeStampPresentation</summary>
/// <param name="timeBase">Rational number base time of the time stamp</param>
TimeSpan FrameAV::GetPresentationTimeSpan(Rational^ timeBase)
{
	return Rational::GetTimeSpan(timeBase, this->TimeStampPresentation);
}

/// <summary>Retrieves a System::TimeSpan instance based off of the timeBase and TimeStampDecode</summary>
/// <param name="timeBase">Rational number base time of the time stamp</param>
TimeSpan FrameAV::GetDecodeTimeSpan(Rational^ timeBase)
{
	return Rational::GetTimeSpan(timeBase, this->TimeStampDecode);
}
#pragma endregion

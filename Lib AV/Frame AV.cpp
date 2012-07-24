

#include "Frame AV.h"

using namespace System;
using namespace Bardez::Projects::MultiMedia::LibAV;


#pragma region Construction
/// <summary>Definition constructor</summary>
/// <param name="keyFrame">Indicates whether this frame is a keyframe</param>
/// <param name="packet">Packet to read timestamps from</param>
FrameAV::FrameAV(Boolean keyFrame, AVPacket* packet)
{
	this->TimeStampDecode = packet->dts;	//drop this?
	this->TimeStampPresentation = packet->pts;
	this->KeyFrame = keyFrame;
}
#pragma endregion


#pragma region Methods
/// <summary>Retrieves a System::TimeSpan instance based off of the timeBase and TimeStampPresentation</summary>
/// <param name="timeBase">Rational number base time of the time stamp</param>
TimeSpan FrameAV::GetPresentationStartTimeSpan(Rational^ timeBase)
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

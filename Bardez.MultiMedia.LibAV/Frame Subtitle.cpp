

#include "Frame Subtitle.h"
#include "Rational Extender.h"


using namespace System;
using namespace Bardez::Projects::Multimedia::LibAV;


#pragma region Methods
/// <summary>Retrieves a System::TimeSpan instance based off of the timeBase and TimeStampPresentation and TimeStampDuration</summary>
/// <param name="timeBase">Rational number base time of the time stamp</param>
TimeSpan FrameSubtitle::GetPresentationEndTimeSpan(Rational^ timeBase)
{
	return RationalExtender::GetTimeSpan(timeBase, this->TimeStampPresentation + this->TimeStampDuration);
}
#pragma endregion


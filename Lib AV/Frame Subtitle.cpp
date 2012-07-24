

#include "Frame Subtitle.h"


using namespace System;
using namespace Bardez::Projects::MultiMedia::LibAV;


#pragma region Methods
/// <summary>Retrieves a System::TimeSpan instance based off of the timeBase and TimeStampPresentation and TimeStampDuration</summary>
/// <param name="timeBase">Rational number base time of the time stamp</param>
TimeSpan FrameSubtitle::GetPresentationEndTimeSpan(Rational^ timeBase)
{
	return Rational::GetTimeSpan(timeBase, this->TimeStampPresentation + this->TimeStampDuration);
}
#pragma endregion


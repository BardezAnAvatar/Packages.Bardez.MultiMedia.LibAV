

#ifndef	_Frame_Subtitle_H_
#define	_Frame_Subtitle_H_

#include "Frame AV.h"

namespace Bardez
{
	namespace Projects
	{
		namespace MultiMedia
		{
			namespace LibAV
			{
				/// <summary>Frame class derived from FrameAV for a subtitle stream</summary>
				public ref class FrameSubtitle : FrameAV
				{
				#pragma region Fields
				public:
					/// <summary>Duration timestap (multiple of least common factor's timebase, where this frame appeared in the stream)</summary>
					/// <remarks>Added to <see href="FrameAV::TimeStampPresentation">FrameAV::TimeStampPresentation</see> to get the end prensentation timestamp</remarks>
					Int64 TimeStampDuration;
				#pragma endregion


				#pragma region Construction
				internal:
					/// <summary>Definition constructor</summary>
					/// <param name="packet">Packet to read timestamps from</param>
					FrameSubtitle(AVPacket* packet) : FrameAV(true, packet)
					{
						this->TimeStampDuration = packet->duration;
					}
				#pragma endregion


				#pragma region Methods
				public:
					/// <summary>Retrieves a System::TimeSpan instance based off of the timeBase and TimeStampPresentation and TimeStampDuration</summary>
					/// <param name="timeBase">Rational number base time of the time stamp</param>
					TimeSpan GetPresentationEndTimeSpan(Rational^ timeBase);
				#pragma endregion
				};
			}
		}
	}
}

#endif


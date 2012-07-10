

#ifndef _Frame_H_
#define _Frame_H_

using namespace System;

extern "C"
{
	#include "libavcodec\avcodec.h"
}

#include "Rational.h"

namespace Bardez
{
	namespace Projects
	{
		namespace MultiMedia
		{
			namespace LibAV
			{
				/// <summary>Frame class derived from libavcodec's AVFrame</summary>
				public ref class FrameAV abstract
				{
				#pragma region Fields
				public:
					/// <summary>Decoding timestap (multiple of least common factor's timebase, where this frame appeared in the stream)</summary>
					Int64 TimeStampDecode;

					/// <summary>Presentation timestap (multiple of least common factor's timebase, where this frame should be presented)</summary>
					Int64 TimeStampPresentation;

					/// <summary>Indicated whether this frame is a keyframe or not</summary>
					Boolean KeyFrame;
				#pragma endregion


				#pragma region Construction
				public:
					/// <summary>Definition constructor</summary>
					/// <param name="source">Source unmanaged frame to read from</param>
					/// <param name="packet">Packet to read timestamps from</param>
					FrameAV(AVFrame* source, AVPacket* packet);
				#pragma endregion


				#pragma region Methods
				public:
					/// <summary>Retrieves a System::TimeSpan instance based off of the timeBase and TimeStampPresentation</summary>
					/// <param name="timeBase">Rational number base time of the time stamp</param>
					TimeSpan GetPresentationTimeSpan(Rational^ timeBase);

					/// <summary>Retrieves a System::TimeSpan instance based off of the timeBase and TimeStampDecode</summary>
					/// <param name="timeBase">Rational number base time of the time stamp</param>
					TimeSpan GetDecodeTimeSpan(Rational^ timeBase);
				#pragma endregion
				};
			}
		}
	}
}


#endif


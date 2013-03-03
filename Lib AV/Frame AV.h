

#ifndef _Frame_H_
#define _Frame_H_


extern "C"
{
	#include "libavcodec\avcodec.h"
}


using namespace System;
using namespace Bardez::Projects::BasicStructures::Math;


namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
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
				internal:
					/// <summary>Definition constructor</summary>
					/// <param name="keyFrame">Indicates whether this frame is a keyframe</param>
					/// <param name="packet">Packet to read timestamps from</param>
					FrameAV(Boolean keyFrame, AVPacket* packet);
				#pragma endregion


				#pragma region Methods
				public:
					/// <summary>Retrieves a System::TimeSpan instance based off of the timeBase and TimeStampPresentation</summary>
					/// <param name="timeBase">Rational number base time of the time stamp</param>
					TimeSpan GetPresentationStartTimeSpan(Rational^ timeBase);

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


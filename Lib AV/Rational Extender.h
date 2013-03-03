

#ifndef _Rational_Extender_
#define _Rational_Extender_


extern "C"
{
	#include "libavutil\rational.h"
}


using namespace System;
using namespace System::Runtime::CompilerServices;
using namespace Bardez::Projects::BasicStructures::Math;


namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>Provides conversion and generation methods for <see cref="Rational" /> and <see cref="AVRational" /></summary>
				[ExtensionAttribute]
				public ref class RationalExtender abstract sealed
				{
				internal:
					/// <summary>Unmanaged builder</summary>
					/// <param name="source">Unmanaged AVRational to copy from</param>
					[ExtensionAttribute]
					static Rational^ ToRational(AVRational source)
					{
						return gcnew Rational(source.num, source.den);
					}

					/// <summary>Converts a Rational to the LibAV unmanaged type</summary>
					/// <param name="source">Managed Rational to copy from</param>
					[ExtensionAttribute]
					static AVRational ToAVRational(Rational^ source)
					{
						AVRational rational;

						rational.num = source->Numerator;
						rational.den = source->Denominator;

						return rational;
					}


				#pragma region Utility
				public:
					/// <summary>Retrieves a System::TimeSpan instance based off of the timeBase and PresentationTimeStamp</summary>
					/// <param name="timeBase">Rational number base time of the time stamp</param>
					/// <param name="timeStamp">Multiple of timeBase indicating overall time</param>
					[ExtensionAttribute]
					static TimeSpan GetTimeSpan(Rational^ timeBase, Int64 timeStamp)
					{
						if (timeStamp == 0x8000000000000000)	//AV_NOPTS_VALUE avutil.h@ 279
							timeStamp = 0;

						Double base = timeBase->ValueDouble;
						Double seconds = base * timeStamp;
						Double ticks = seconds * 10000000;	//seconds to ticks of 100 nanoseconds
						return TimeSpan(Convert::ToInt64(ticks));
					}
				#pragma endregion
				};
			}
		}
	}
}


#endif


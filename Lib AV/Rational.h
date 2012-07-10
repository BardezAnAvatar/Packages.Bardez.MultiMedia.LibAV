

#ifndef _Components_Rational_H_
#define _Components_Rational_H_

extern "C"
{
	#include "libavutil\rational.h"
}

using namespace System;

namespace Bardez
{
	namespace Projects
	{
		namespace MultiMedia
		{
			namespace LibAV
			{
				/// <summary>Lib AV's representation of numerator/denominator rational numbers</summary>
				public ref class Rational
				{
				#pragma region Fields
				public:
					/// <summary>Numerator of the rational number</summary>
					Int32 Numerator;

					/// <summary>Denominator of the rational number</summary>
					Int32 Denominator;
				#pragma endregion


				#pragma region Properties
				public:
					/// <summary>Exposes the floating-point representation of the rational</summary>
					property Double Value
					{
						Double get()
						{
							return (Convert::ToDouble(this->Numerator) / Convert::ToDouble(this->Denominator));
						}
					}
				#pragma endregion


				#pragma region Construction
				public:
					/// <summary>Default constructor</summary>
					Rational() { }
		
					/// <summary>Definition constructor</summary>
					/// <param name="numerator">Numerator of this rational</param>
					/// <param name="denominator">Denominator of this rational</param>
					Rational(Int32 numerator, Int32 denominator)
					{
						this->Numerator = numerator;
						this->Denominator = denominator;
					}
		
				internal:
					/// <summary>Definition constructor</summary>
					/// <param name="rational">Unmanaged AVRational to copy from</param>
					Rational(AVRational rational)
					{
						this->Numerator = rational.num;
						this->Denominator = rational.den;
					}
				#pragma endregion


				#pragma region Compatability
				internal:
					/// <summary>Converts this Rational to the LibAV unmanaged type</summary>
					AVRational ToUnmanaged()
					{
						AVRational rational;

						rational.num = this->Numerator;
						rational.den = this->Denominator;

						return rational;
					}
				#pragma endregion


				#pragma region Utility
				public:
					/// <summary>Retrieves a System::TimeSpan instance based off of the timeBase and PresentationTimeStamp</summary>
					/// <param name="timeBase">Rational number base time of the time stamp</param>
					/// <param name="timeStamp">Multiple of timeBase indicating overall time</param>
					static TimeSpan GetTimeSpan(Rational^ timeBase, Int64 timeStamp)
					{
						if (timeStamp == 0x8000000000000000)	//AV_NOPTS_VALUE avutil.h@ 279
							timeStamp = 0;

						Double base = timeBase->Value;
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


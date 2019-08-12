

#ifndef _LibAV_Audio_Format_Extender_
#define _LibAV_Audio_Format_Extender_


extern "C"
{
	#include "libavutil\samplefmt.h"
}


using namespace System;
using namespace System::Runtime::CompilerServices;
using namespace Bardez::Projects::BasicStructures::Win32::Audio;


namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>Extension class to convert between <see cref="AudioDataFormat" /> and <see cref="AVSampleFormat" /></summary>
				[ExtensionAttribute]
				public ref class LibAVAudioFormatExtender abstract sealed
				{
				#pragma region Conversion
				internal:
					/// <summary>Extension method to convert <see cref="AudioDataFormat" /> to <see cref="AVSampleFormat" /></summary>
					/// <param name="sourceFormat">Source audio data format</param>
					/// <param name="bits">Number of bits per sample</param>
					[ExtensionAttribute]
					static AVSampleFormat ToAVSampleFormat(AudioDataFormat sourceFormat, Int32 bits);
					
					/// <summary>Extension method to convert <see cref="AVSampleFormat" /> to <see cref="AudioDataFormat" /></summary>
					/// <param name="sourceFormat">Source audio data format</param>
					[ExtensionAttribute]
					static AudioDataFormat ToAudioDataFormat(AVSampleFormat sourceFormat);
					
					/// <summary>Method to get the bit count of samples based off of the <see cref="AVSampleFormat" /></summary>
					/// <param name="sourceFormat">Source audio data format</param>
					[ExtensionAttribute]
					static UInt16 GetBitsPerSample(AVSampleFormat sourceFormat);
				#pragma endregion
				};
			}
		}
	}
}


#endif


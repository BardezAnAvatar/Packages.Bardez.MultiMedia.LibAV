

#ifndef _Frame_Audio_H_
#define _Frame_Audio_H_

#include "Frame AV.h"

using namespace System;
using namespace System::IO;

namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>Frame class derived from libavcodec's AVFrame, expecting audio input from the AVFrame's data as signed 16-bit samples</summary>
				public ref class FrameAudioInt16 : FrameAV
				{
				#pragma region Fields
				public:
					/// <summary>Data of the decoded frame (audio)</summary>
					array<Byte>^ Data;

					/// <summary>Count of channels to this audio stream</summary>
					Int32 CountChannels;

					/// <summary>Count of audio samples in this frame</summary>
					Int32 CountSamples;
				#pragma endregion


				#pragma region Construction
				internal:
					/// <summary>Definition constructor</summary>
					/// <param name="source">Source unmanaged frame to read from</param>
					/// <param name="packet">Packet to read timestamps from</param>
					/// <param name="channels">Number of audio channels</param>
					FrameAudioInt16(AVFrame* source, AVPacket* packet, Int32 channels) : FrameAV(Convert::ToBoolean(source->key_frame), packet)
					{
						this->CountSamples = source->nb_samples;
						this->CountChannels = channels;
						this->CopyData(source);
					}
				#pragma endregion


				#pragma region Helpers
				internal:
					/// <summary>Helper that copies data from the unmanaged pointers to managed set, based on the expected frame type</summary>
					/// <param name="source">Source unmanaged frame to read from</param>
					void CopyData(AVFrame* source);

					/// <summary>Computes the size of an audio sample based on its audio format</summary>
					/// <param name="format">Format of the audio sample from a source frame</param>
					static Int32 SampleSize(Int32 format);
				#pragma endregion
				};
			}
		}
	}
}


#endif


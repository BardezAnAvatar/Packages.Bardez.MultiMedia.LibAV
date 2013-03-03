

#ifndef _Frame_LibAV_Audio_
#define _Frame_LibAV_Audio_


#include "Frame LibAV.h"


using namespace System;
using namespace Bardez::Projects::BasicStructures::Win32::Audio;
using namespace Bardez::Projects::Multimedia::MediaBase::Frame::Audio;


namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>Audio frame for streaming in LibAV</summary>
				public ref class FrameLibAVAudio : FrameLibAV, IMultimediaAudioFrame
				{
				#pragma region Fields
				protected:
					/// <summary>Reference to the audio metadata info</summary>
					WaveFormatEx^ metadataAudio;

					/// <summary>Count of samples in this frame</summary>
					Int32 sampleCount;
					
					/// <summary>Size of the audio sample data</summary>
					Int32 frameDataSize;
				#pragma endregion


				#pragma region Properties
				protected:
					/// <summary>Size of the audio sample data</summary>
					property Int32 FrameDataSize
					{
						Int32 get();
					}
				#pragma endregion


				#pragma region Construction
				internal:
					/// <summary>Definition constructor</summary>
					/// <param name="source">Source unmanaged frame to read from</param>
					/// <param name="packet">Packet to read timestamps from</param>
					/// <param name="stream">Source stream for this Frame</summary>
					FrameLibAVAudio(AVFrame* source, AVPacket* packet, AVStream* stream);
				#pragma endregion


				#pragma region IWaveFormatEx methods
				public:
					/// <summary>Returns a WaveFormatEx instance from this header data</summary>
					/// <returns>A WaveFormatEx instance to submit to API calls</returns>
					virtual WaveFormatEx^ GetWaveFormat();
				#pragma endregion


				#pragma region Helper methods
				protected:
					/// <summary>Copies the data from <see cref="source" /> to <see cref="frameData" /></summary>
					/// <remarks>Data is treated differently between Audio and Video and Subtitle</remarks>
					virtual void CopyData(AVFrame* source) override;
				#pragma endregion
				};
			}
		}
	}
}


#endif


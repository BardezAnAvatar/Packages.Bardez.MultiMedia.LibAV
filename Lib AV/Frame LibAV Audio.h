

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
				/// <summary>Frame basis for streaming in LibAV</summary>
				public ref class FrameLibAVAudio : FrameLibAV, IMultimediaAudioFrame
				{
				#pragma region Fields
				protected:
					/// <summary>Reference to the streaming metadata info</summary>
					WaveFormatEx^ metadataAudio;

					/// <summary>Count of samples in this frame</summary>
					Int32 sampleCount;

				internal:
					/// <summary>Handle to pointer copy of the passed in AVFrame</summary>
					IntPtr ptrAVPicture;
				#pragma endregion


				#pragma region Properties
				internal:
					/// <summary>Pointer exposure of the passed in construction AVFrame</summary>
					property AVPicture* PtrPicture
					{
						AVPicture* get();
						void set(AVPicture* value);
					}

					/// <summary>Size of the audio sample data</summary>
					property Int32 FrameDataSize
					{
						Int32 get();
					}
				#pragma endregion


				#pragma region IMultimediaFrame Properties
				public:
					/// <summary>The streaming metadata info</summary>
					virtual property Stream^ Data
					{
						Stream^ get() override;
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


				#pragma region Destruction
				public:
					/// <summary>Destructor</summary>
					/// <remarks>Dispose()</remarks>
					~FrameLibAVAudio();

				internal:
					/// <summary>Destructor logic, disposes the object</summary>
					virtual void DisposeUnmanaged() override;
				#pragma endregion


				#pragma region IWaveFormatEx methods
				public:
					/// <summary>Returns a WaveFormatEx instance from this header data</summary>
					/// <returns>A WaveFormatEx instance to submit to API calls</returns>
					virtual WaveFormatEx^ GetWaveFormat();
				#pragma endregion
				};
			}
		}
	}
}


#endif


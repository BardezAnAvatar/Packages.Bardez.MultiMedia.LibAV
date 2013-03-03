

#ifndef	_Frame_Subtitle_Text_H_
#define	_Frame_Subtitle_Text_H_


#include "Frame Subtitle.h"


using namespace System;
using namespace System::Runtime::InteropServices;


namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>Frame class derived from libavcodec's AVFrame, expecting RGB input (in the BGRA order) from the AVFrame's data</summary>
				public ref class FrameSubtitleText : FrameSubtitle
				{
				#pragma region Fields
				public:
					/// <summary>Managed String representation of the decoded data from the subtitle stream</summary>
					String^ Data;
				#pragma endregion


				#pragma region Construction
				internal:
					/// <summary>Definition constructor</summary>
					/// <param name="packet">Packet to read timestamps from</param>
					FrameSubtitleText(AVPacket* packet) : FrameSubtitle(packet)
					{
						uint8_t* text = packet->data;
						Int32 size = packet->size;

						this->Data = Marshal::PtrToStringAnsi(IntPtr(text));

						this->TimeStampDuration = packet->duration;
					}
				#pragma endregion
				};
			}
		}
	}
}

#endif


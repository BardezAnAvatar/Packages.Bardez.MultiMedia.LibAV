

#ifndef	_Frame_BGRA_H_
#define	_Frame_BGRA_H_

#include "Frame AV.h"
#include "LibAV Picture.h"

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
				/// <summary>Frame class derived from libavcodec's AVFrame, expecting RGB input (in the BGRA order) from the AVFrame's data</summary>
				public ref class FrameBGRA : FrameAV
				{
				#pragma region Fields
				public:
					/// <summary>Data of the frame</summary>
					MemoryStream^ Data;

				protected:
					/// <summary>Frame's picture detail (height, width, pixel format)</summary>
					LibAVPictureDetail detail;
				#pragma endregion

					
				#pragma region Properties
				public:
					/// <summary>Exposes picture detail (height, width, pixel format)</summary>
					property LibAVPictureDetail Detail
					{
						LibAVPictureDetail get()
						{
							return this->detail;
						}
					}
				#pragma endregion


				#pragma region Construction
				public:
					/// <summary>Definition constructor</summary>
					/// <param name="source">Source unmanaged frame copy values from</param>
					/// <param name="detail">Details of the frame's picture</param>
					/// <param name="picture">Source MemoryStream to read from</param>
					/// <param name="packet">Packet to read timestamps from</param>
					FrameBGRA(AVFrame* source, LibAVPictureDetail detail, MemoryStream^ picture, AVPacket* packet) : FrameAV(Convert::ToBoolean(source->key_frame), packet)
					{
						this->detail = detail;
						this->Data = picture;
					}
				#pragma endregion
				};
			}
		}
	}
}


#endif


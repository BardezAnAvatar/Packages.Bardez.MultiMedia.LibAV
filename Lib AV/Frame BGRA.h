

#ifndef	_Frame_BGRA_H_
#define	_Frame_BGRA_H_

#include "Frame AV.h"
#include "LibAV Picture.h"

namespace Bardez
{
	namespace Projects
	{
		namespace MultiMedia
		{
			namespace LibAV
			{
				/// <summary>Frame class derived from libavcodec's AVFrame, expecting RGB input (in the BGRA order) from the AVFrame's data</summary>
				public ref class FrameBGRA : FrameAV
				{
				#pragma region Fields
				public:
					/// <summary>Height of this frame</summary>
					Int32 Height;

					/// <summary>Width of this frame</summary>
					Int32 Width;

					/// <summary>Data of the frame</summary>
					LibAVPicture^ Data;
				#pragma endregion


				#pragma region Construction
				public:
					/// <summary>Definition constructor</summary>
					/// <param name="source">Source unmanaged frame copy values from</param>
					/// <param name="picture">Source AVPicture to read from</param>
					/// <param name="packet">Packet to read timestamps from</param>
					FrameBGRA(AVFrame* source, LibAVPicture^ picture, AVPacket* packet) : FrameAV(source, packet)
					{
						this->Height = picture->Detail.Height;
						this->Width = picture->Detail.Width;
						this->Data = picture;
					}
				#pragma endregion
				};
			}
		}
	}
}


#endif


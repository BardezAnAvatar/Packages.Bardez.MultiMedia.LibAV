

#ifndef _SWSCALE_Formatting_H_
#define _SWSCALE_Formatting_H_


#include "LibAV Pixel Format.h"


using namespace System;


namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>Represents a section of detail for height, width and pixel format of a source image</summary>
				public value struct LibAVPictureDetail
				{
				#pragma region Fields
				protected:
					/// <summary>Height of the image</summary>
					Int32 height;

					/// <summary>Width of the image</summary>
					Int32 width;

					/// <summary>Pixel format of the image</summary>
					LibAVPixelFormat format;
				#pragma endregion


				#pragma region Properties
				public:
					/// <summary>Height of the image</summary>
					property Int32 Height
					{
						Int32 get()
						{
							return this->height;
						}
						void set(Int32 value)
						{
							this->height = value;
						}
					}

					/// <summary>Width of the image</summary>
					property Int32 Width
					{
						Int32 get()
						{
							return this->width;
						}
						void set(Int32 value)
						{
							this->width = value;
						}
					}
					
					/// <summary>Pixel format of the image</summary>
					property LibAVPixelFormat Format
					{
						LibAVPixelFormat get()
						{
							return this->format;
						}
						void set(LibAVPixelFormat value)
						{
							this->format = value;
						}
					}
				#pragma endregion


				#pragma region Construction
				public:
					/// <summary>Default constructor</summary>
					static LibAVPictureDetail Build()
					{
						LibAVPictureDetail detail;

						detail.Height = -1;
						detail.Width = -1;
						detail.Format = LibAVPixelFormat::PIX_FMT_NONE;

						return detail;
					}

					/// <summary>Definition constructor</summary>
					/// <param name="width">Width of the image</param>
					/// <param name="height">Height of the image</param>
					/// <param name="format">Pixel format of the image</param>
					static LibAVPictureDetail Build(Int32 width, Int32 height, LibAVPixelFormat format)
					{
						LibAVPictureDetail detail;

						detail.Width = width;
						detail.Height = height;
						detail.Format = format;

						return detail;
					}
				#pragma endregion
				};
			}
		}
	}
}

#endif


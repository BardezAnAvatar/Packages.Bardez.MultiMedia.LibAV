

#ifndef _Lib_AV_Picture_H_
#define _Lib_AV_Picture_H_

extern "C"
{
	#include "libavcodec\avcodec.h"
	#include "libavutil\pixfmt.h"
}
#include "LibAV Pixel Format.h"
#include "Error Helper.h"
#include "LibAV Picture Detail.h"

using namespace System;
using namespace System::IO;

namespace Bardez
{
	namespace Projects
	{
		namespace MultiMedia
		{
			namespace LibAV
			{
				/// <summary>Represents a managed instance of an AVPicture structure</summary>
				public ref class LibAVPicture sealed
				{
				#pragma region Fields
				protected:
					/// <summary>Pointer to the AVPicture</summary>
					IntPtr picturePtr;

					/// <summary>Picture detail (height, width, pixel format)</summary>
					LibAVPictureDetail detail;
				#pragma endregion


				#pragma region Properties
				internal:
					/// <summary>Pointer to the AVPicture</summary>
					property AVPicture* PicturePtr
					{
						AVPicture* get() { return reinterpret_cast<AVPicture*>(this->picturePtr.ToPointer()); }
						void set(AVPicture* value) { this->picturePtr = IntPtr(value); }
					}

				public:
					/// <summary>Exposes picture detail (height, width, pixel format)</summary>
					property LibAVPictureDetail Detail
					{
						LibAVPictureDetail get()
						{
							return this->detail;
						}
					}

					/// <summary>Exposes the data used bythis picture as a Stream</summary>
					property MemoryStream^ Data
					{
						MemoryStream^ get()
						{
							return this->GetData();
						}
						void set(MemoryStream^ value)
						{
							this->SetData(value);
						}
					}
				#pragma endregion


				#pragma region Construction
				internal:
					/// <summary>Definition constructor</summary>
					LibAVPicture(AVPicture* pointer, Int32 width, Int32 height, LibAVPixelFormat format)
					{
						this->PicturePtr = pointer;
						this->detail = LibAVPictureDetail::Build(width, height, format);
					}

					/// <summary>Definition constructor</summary>
					LibAVPicture(AVPicture* pointer, LibAVPictureDetail detail)
					{
						this->PicturePtr = pointer;
						this->detail = detail;
					}

				public:
					/// <summary>Static constructor</summary>
					static LibAVPicture^ BuildPicture(LibAVPictureDetail detail);

					/// <summary>Static constructor</summary>
					static LibAVPicture^ BuildPicture(LibAVPixelFormat format, Int32 width, Int32 height);

				internal:
					/// <summary>Static constructor</summary>
					static AVPicture* BuildNativePicture(LibAVPictureDetail detail);

					/// <summary>Static constructor</summary>
					static AVPicture* BuildNativePicture(LibAVPixelFormat format, Int32 width, Int32 height);
				#pragma endregion


				#pragma region Destruction
				public:
					/// <summary>Destructor</summary>
					/// <remarks>Dispose()</remarks>
					~LibAVPicture()
					{
						this->detail = LibAVPictureDetail::Build();
						this->DisposeUnmanaged();
					}
					
					/// <summary>Destructor</summary>
					/// <remarks>Finalize()</remarks>
					!LibAVPicture()
					{
						this->DisposeUnmanaged();
					}

				internal:
					/// <summary>Destructor logic, disposes the object</summary>
					void DisposeUnmanaged()
					{
						//close the AV format context
						if (this->PicturePtr)
						{
							AVPicture* nativePtr = this->PicturePtr;
							avpicture_free(nativePtr);
							delete nativePtr;
							this->PicturePtr = NULL;
						}
					}
				#pragma endregion


				#pragma region Data Access
				protected:
					/// <summary>Sets the data from a .NET Stream to the 8-channel byte arrays used by LibAV</summary>
					/// <param name="source">Source, flattened Byte array of image data</param>
					void SetData(MemoryStream^ source);

					/// <summary>Accesses the data from the 8-channel byte Pointers used by LibAV</summary>
					/// <returns>A Stream of flattened pixel data</returns>
					MemoryStream^ GetData();
				#pragma endregion
				};
			}
		}
	}
}

#endif


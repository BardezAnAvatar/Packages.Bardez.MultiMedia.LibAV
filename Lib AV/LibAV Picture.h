

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
		namespace Multimedia
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

					/// <summary>Flag that indcates whether the memory was assigned by an outside source on construction</summary>
					Boolean assignedDataOnConstruction;

					/// <summary>Flag that indicates whether or not to free memory held by AVPicture on disposal</summary>
					Boolean freeMemoryOnDestruction;
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
					/// <param name="pointer">Pointer to the native AVPicture</param>
					/// <param name="detail">Details of the picture</param>
					/// <param name="assignedDataPointer">Flag that indcates whether the memory was assigned by an outside source on construction</param>
					/// <param name="freeMemory">Flag that indicates whether or not to free memory held by AVPicture on disposal</param>
					LibAVPicture(AVPicture* pointer, LibAVPictureDetail detail, Boolean assignedDataPointer, Boolean freeMemory)
					{
						this->PicturePtr = pointer;
						this->detail = detail;
						this->assignedDataOnConstruction = assignedDataPointer;
						this->freeMemoryOnDestruction = freeMemory;
					}

				public:
					/// <summary>Static constructor</summary>
					/// <param name="detail">Pixel format, width and height of the picture to build</param>
					static LibAVPicture^ BuildPicture(LibAVPictureDetail detail);

					/// <summary>Static constructor</summary>
					/// <param name="detail">Pixel format, width and height of the picture to build</param>
					/// <param name="source">Source data to populate the Picture with</param>
					static LibAVPicture^ BuildPicture(LibAVPictureDetail detail, MemoryStream^ source);

				internal:
					/// <summary>Static constructor that builds an AVPicture instance</summary>
					/// <param name="detail">Detail of the AVPicture to build it from</param>
					static AVPicture* BuildNativePicture(LibAVPictureDetail detail);

					/// <summary>Static constructor that builds an AVPicture instance</summary>
					/// <param name="detail">Detail of the AVPicture to build it from</param>
					/// <param name="source">Source data to populate the Picture with</param>
					static AVPicture* BuildNativePicture(LibAVPictureDetail detail, MemoryStream^ source);

					/// <summary>Clears the contents of an AVPicture's data and linesize arrays. Does not deallocate.</summary>
					/// <param name="picture">AVPicture instance to clear</param>
					static void ClearAvPicture(AVPicture* picture);
				#pragma endregion


				#pragma region Destruction
				public:
					/// <summary>Destructor</summary>
					/// <remarks>Dispose()</remarks>
					~LibAVPicture()
					{
						this->detail = LibAVPictureDetail::Build();
						this->DisposeUnmanaged();
						GC::SuppressFinalize(this);
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
							
							LibAVPicture::DeleteUnmanagedAllocatedMemory(nativePtr);

							delete nativePtr;
							this->PicturePtr = NULL;
						}
					}

					/// <summary>Deletes allocated memory of the AVPicture data member</summary>
					/// <param name="nativePtr">AVPicture instance to clear</param>
					void DeleteUnmanagedAllocatedMemory(AVPicture* nativePtr)
					{
						//is memory held elsewhere?
						if (this->freeMemoryOnDestruction)
						{
							//free memory held by the AVPicure
							if (this->assignedDataOnConstruction)	//if memory was allocated externally
								delete [] nativePtr->data[0];
							else									//memory was allocated by avpicture_alloc()
								avpicture_free(nativePtr);
						}

						//set the pointers & line stride to NULL
						LibAVPicture::ClearAvPicture(nativePtr);
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


				#pragma region Helper Methods
				protected:
					/// <summary>Indicates whether the AVPicture pointer appears to be allocated.</summary>
					/// <param name="picture">AVPicture pointer to assign to</param>
					/// <returns>True if any of the data pointers are non-NULL</returns>
					static Boolean IsPictureAllocated(AVPicture* picture);

					/// <summary>Copies the data pointer from the source array an assigns it to the picturepointer</summary>
					/// <param name="picture">AVPicture pointer to assign to</param>
					/// <param name="detail">Detail of the AVPicture to build it from</param>
					/// <param name="source">Source MemoryStream to copy from</param>
					static void CopyFromStream(AVPicture* picture, LibAVPictureDetail detail, MemoryStream^ source);
				#pragma endregion
				};
			}
		}
	}
}

#endif


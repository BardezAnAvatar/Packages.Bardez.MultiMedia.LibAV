

#include "LibAV Picture.h"
#include "Error Helper.h"	//this errors (in mem.h) when included before another file including it. Weird.


//Bypass windos.h
#define BYTE uint8_t


using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace Bardez::Projects::Multimedia::LibAV;


#pragma region Construction
/// <summary>Static constructor</summary>
/// <param name="detail">Pixel format, width and height of the picture to build</param>
LibAVPicture^ LibAVPicture::BuildPicture(LibAVPictureDetail detail)
{
	AVPicture* picture = LibAVPicture::BuildNativePicture(detail);
	return gcnew LibAVPicture(picture, detail, false, true);
}

/// <summary>Static constructor</summary>
/// <param name="detail">Pixel format, width and height of the picture to build</param>
/// <param name="source">Source data to populate the Picture with</param>
LibAVPicture^ LibAVPicture::BuildPicture(LibAVPictureDetail detail, MemoryStream^ source)
{
	AVPicture* picture = LibAVPicture::BuildNativePicture(detail, source);
	return gcnew LibAVPicture(picture, detail, true, true);
}

/// <summary>Static constructor that builds an AVPicture instance</summary>
/// <param name="detail">Detail of the AVPicture to build it from</param>
AVPicture* LibAVPicture::BuildNativePicture(LibAVPictureDetail detail)
{
	AVPicture* picture = new AVPicture;
	LibAVPicture::ClearAvPicture(picture);	//clear picture arrays

	PixelFormat nativeFormat = static_cast<PixelFormat>(detail.Format);

	//allocate a frame
	Int32 libavReturnCode = avpicture_alloc(picture, nativeFormat, detail.Width, detail.Height);

	//error detection
	if (libavReturnCode < 0)
		throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(libavReturnCode, ErrorHelper::LibAvCodec));

	return picture;
}

/// <summary>Static constructor that builds an AVPicture instance</summary>
/// <param name="detail">Detail of the AVPicture to build it from</param>
/// <param name="source">Source data to populate the Picture with</param>
AVPicture* LibAVPicture::BuildNativePicture(LibAVPictureDetail detail, MemoryStream^ source)
{
	AVPicture* picture = new AVPicture;
	LibAVPicture::ClearAvPicture(picture);
	LibAVPicture::CopyFromStream(picture, detail, source);

	return picture;
}

/// <summary>Clears the contents of an AVPicture's data and linesize arrays. Does not deallocate.</summary>
/// <param name="picture">AVPicture instance to clear</param>
void LibAVPicture::ClearAvPicture(AVPicture* picture)
{
	picture->data[0] = NULL;
	picture->data[1] = NULL;
	picture->data[2] = NULL;
	picture->data[3] = NULL;
	picture->data[4] = NULL;
	picture->data[5] = NULL;
	picture->data[6] = NULL;
	picture->data[7] = NULL;
	
	picture->linesize[0] = 0;
	picture->linesize[1] = 0;
	picture->linesize[2] = 0;
	picture->linesize[3] = 0;
	picture->linesize[4] = 0;
	picture->linesize[5] = 0;
	picture->linesize[6] = 0;
	picture->linesize[7] = 0;
}
#pragma endregion


#pragma region Data Access
/// <summary>Sets the data from a .NET Stream to the 8-channel byte arrays used by LibAV</summary>
/// <param name="source">Source, flattened Byte array of image data</param>
void LibAVPicture::SetData(MemoryStream^ source)
{
	AVPicture* picture = this->PicturePtr;
	LibAVPicture::CopyFromStream(picture, this->detail, source);
}

/// <summary>Accesses the data from the 8-channel byte Pointers used by LibAV</summary>
/// <returns>A Stream of flattened pixel data</returns>
MemoryStream^ LibAVPicture::GetData()
{
	Int32 size = avpicture_get_size((PixelFormat)this->detail.Format, this->detail.Width, this->detail.Height);
	array<Byte>^ data = gcnew array<Byte>(size);
	pin_ptr<Byte> dstData = &data[0];
	Byte* destination = dstData;

	AVPicture* picture = this->PicturePtr;

	Int32 result = avpicture_layout(picture, (PixelFormat)this->detail.Format, this->detail.Width, this->detail.Height, destination, size);

	if (result < 0)	//per API
		throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(result, ErrorHelper::LibAvCodec));

	return gcnew MemoryStream(data);
}
#pragma endregion


#pragma region Helper Methods
/// <summary>Indicates whether the AVPicture pointer appears to be allocated.</summary>
/// <param name="picture">AVPicture pointer to assign to</param>
/// <returns>True if any of the data pointers are non-NULL</returns>
Boolean LibAVPicture::IsPictureAllocated(AVPicture* picture)
{
	return 
		picture->data[0] != NULL ||
		picture->data[1] != NULL ||
		picture->data[2] != NULL ||
		picture->data[3] != NULL ||
		picture->data[4] != NULL ||
		picture->data[5] != NULL ||
		picture->data[6] != NULL ||
		picture->data[7] != NULL;
}

/// <summary>Copies the data pointer from the source array an assigns it to the picturepointer</summary>
/// <param name="picture">AVPicture pointer to assign to</param>
/// <param name="detail">Detail of the AVPicture to build it from</param>
/// <param name="source">Source MemoryStream to copy from</param>
void LibAVPicture::CopyFromStream(AVPicture* picture, LibAVPictureDetail detail, MemoryStream^ source)
{
	//if it appears that the data is currently allocated for the data, unallocate it
	if (LibAVPicture::IsPictureAllocated(picture))
	{
		//TODO: do not rely on avpicture_free(...)
		avpicture_free(picture);
		LibAVPicture::ClearAvPicture(picture);
	}
	
	//get source array
	array<Byte>^ data = source->ToArray();

	//Note: avpicture_fill(...) does not copy data from the source array, but rather copies the source pointer and when splitting data, derives new
	//	addresses off of the main pointer. To allow the picture instance to be independant of the source data, I need to duplicate the source
	//	data entirely. .NET manages binary data in a nightmarish fashion when it comes to native interop.
		
	//the source array can easily end up being too long (such as an extra two Bytes in 1200x1600 RBG24 image), so restrict the copy to what the size needed is going to be.
	Int32 lengthNeeded = avpicture_fill(picture, NULL, static_cast<PixelFormat>(detail.Format), detail.Width, detail.Height);

	//copy the byte array
	BYTE* unmanagedCopy = new BYTE[lengthNeeded];
	Marshal::Copy(data, 0, IntPtr(unmanagedCopy), lengthNeeded);
	
	//fill the AVPicture struct's data pointers & line sizes
	avpicture_fill(picture, unmanagedCopy, static_cast<PixelFormat>(detail.Format), detail.Width, detail.Height);
}
#pragma endregion




#include "LibAV Picture.h"
#include "Error Helper.h"	//this errors (in mem.h) when included before another file including it. Weird.


using namespace System::IO;
using namespace Bardez::Projects::MultiMedia::LibAV;


#pragma region Construction
/// <summary>Static constructor</summary>
LibAVPicture^ LibAVPicture::BuildPicture(LibAVPictureDetail detail)
{
	return LibAVPicture::BuildPicture(detail.Format, detail.Width, detail.Height);
}

/// <summary>Static constructor</summary>
LibAVPicture^ LibAVPicture::BuildPicture(LibAVPixelFormat format, Int32 width, Int32 height)
{
	AVPicture* picture = new AVPicture;

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
	
	PixelFormat nativeFormat = static_cast<PixelFormat>(format);

	//allocate a frame for BGRA transformation
	Int32 libavReturnCode = avpicture_alloc(picture, nativeFormat, width, height);

	if (libavReturnCode < 0)
		throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(libavReturnCode, ErrorHelper::LibAvCodec));

	return gcnew LibAVPicture(picture, height, width, format);
}
#pragma endregion


#pragma region Data Access
/// <summary>Sets the data from a .NET Stream to the 8-channel byte arrays used by LibAV</summary>
/// <param name="source">Source, flattened Byte array of image data</param>
void LibAVPicture::SetData(MemoryStream^ source)
{
	array<Byte>^ data = source->ToArray();
	pin_ptr<Byte> srcData = &data[0];
	Byte* src = srcData;
	
	avpicture_fill(this->PicturePtr, src, (PixelFormat)this->detail.Format, this->detail.Width, this->detail.Height);
}

/// <summary>Accesses the data from the 8-channel byte Pointers used by LibAV</summary>
/// <returns>A Stream of flattened pixel data</returns>
MemoryStream^ LibAVPicture::GetData()
{
	Int32 size = avpicture_get_size((PixelFormat)this->detail.Format, this->detail.Width, this->detail.Height);
	array<Byte>^ data = gcnew array<Byte>(size);
	pin_ptr<Byte> dstData = &data[0];

	Int32 result = avpicture_layout(this->PicturePtr, (PixelFormat)this->detail.Format, this->detail.Width, this->detail.Height, dstData, size);

	if (result < 0)	//per API
		throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(result, ErrorHelper::LibAvCodec));

	return gcnew MemoryStream(data);
}
#pragma endregion


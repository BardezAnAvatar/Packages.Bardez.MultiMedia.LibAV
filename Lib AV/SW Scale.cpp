

#include "SW Scale.h"


using namespace System;
using namespace System::IO;
using namespace Bardez::Projects::MultiMedia::LibAV;


/// <summary>Determines if the format is supported for input</summary>
/// <param name="pix_fmt">Pixel type to check against</param>
/// <returns>A Boolean flag indicating whether the input is supported</returns>
Boolean SWScale::IsSupportedInput(LibAVPixelFormat pix_fmt)
{
	Int32 retVal = sws_isSupportedInput((PixelFormat)pix_fmt);
	return retVal > 0;
}

/// <summary>Determines if the format is supported for input</summary>
/// <param name="input">SwScaleDetail containing pixel type to check against</param>
/// <returns>A Boolean flag indicating whether the input is supported</returns>
Boolean SWScale::IsSupportedInput(LibAVPictureDetail input)
{
	return SWScale::IsSupportedInput(input.Format);
}

/// <summary>Determines if the format is supported for output</summary>
/// <param name="pix_fmt">Pixel type to check against</param>
/// <returns>A Boolean flag indicating whether the input is supported</returns>
Boolean SWScale::IsSupportedOutput(LibAVPixelFormat pix_fmt)
{
	Int32 retVal = sws_isSupportedOutput((PixelFormat)pix_fmt);
	return retVal > 0;
}

/// <summary>Determines if the format is supported for output</summary>
/// <param name="output">SwScaleDetail containing pixel type to check against</param>
/// <returns>A Boolean flag indicating whether the input is supported</returns>
Boolean SWScale::IsSupportedOutput(LibAVPictureDetail output)
{
	return SWScale::IsSupportedOutput(output.Format);
}

/// <summary>Unpacks an 8-bit paletted data stream to a 24-bit pixel data stream</summary>
/// <param name="source">Source data to read from</param>
/// <param name="destination">Destination data to write to</param>
/// <param name="pixelCount">Number of pixels to convert</param>
/// <param name="palette">24-bit Palette data to read from</param>
void SWScale::UnpackPalettedData8BitTo24Bit(array<Byte>^ source, array<Byte>^ destination, Int32 pixelCount, array<Byte>^ palette)
{
	pin_ptr<Byte> srcData = &source[0];
	pin_ptr<Byte> dstData = &destination[0];
	pin_ptr<Byte> palData = &palette[0];

	Byte* src = srcData;
	Byte* dst = dstData;
	Byte* pal = palData;

	sws_convertPalette8ToPacked24(src, dst, pixelCount, pal);
}
					
/// <summary>Unpacks an 8-bit paletted data stream to a 32-bit pixel data stream</summary>
/// <param name="source">Source data to read from</param>
/// <param name="destination">Destination data to write to</param>
/// <param name="pixelCount">Number of pixels to convert</param>
/// <param name="palette">32-bit Palette data to read from</param>
void SWScale::UnpackPalettedData8BitTo32Bit(array<Byte>^ source, array<Byte>^ destination, Int32 pixelCount, array<Byte>^ palette)
{
	pin_ptr<Byte> srcData = &source[0];
	pin_ptr<Byte> dstData = &destination[0];
	pin_ptr<Byte> palData = &palette[0];

	Byte* src = srcData;
	Byte* dst = dstData;
	Byte* pal = palData;

	sws_convertPalette8ToPacked32(src, dst, pixelCount, pal);
}

/// <summary>Performs the constructed SW Scale transformation associated with this context</summary>
/// <param name="source">Source picture to read source data from</summary>
/// <param name="destination">Destination picture to write source data to</summary>
/// <returns>The height of the output image</returns>
Int32 SWScale::Transform(LibAVPicture^ source, LibAVPicture^ destination)
{
	return sws_scale(	this->SWS_Context, source->PicturePtr->data,
						source->PicturePtr->linesize, 0, source->Detail.Height,
						destination->PicturePtr->data, destination->PicturePtr->linesize	);
}

/// <summary>Performs the constructed SW Scale transformation associated with this context</summary>
/// <param name="sourceData">Source data to read from</summary>
/// <param name="destData">Destination Stream to write data to</summary>
/// <returns>The height of the output image</returns>
Int32 SWScale::Transform(MemoryStream^ sourceData, [System::Runtime::InteropServices::Out] MemoryStream^% destData)
{
	LibAVPicture^ source = LibAVPicture::BuildPicture(this->detailSource, sourceData);
	LibAVPicture^ destination = LibAVPicture::BuildPicture(this->detailDestination);

	//Note: sw_scale does not perform any allocation for the destination AVPicture linesize or data arrays.
	//	Ergo, when transforming an input data stream, I need to copy the source pointers for source, but still
	//	allocate memory for destination; the source should *not* be allocated, but instead be filled.
	
	Int32 result = sws_scale(	this->SWS_Context, source->PicturePtr->data,
								source->PicturePtr->linesize, 0, source->Detail.Height,
								destination->PicturePtr->data, destination->PicturePtr->linesize	);

	destData = destination->Data;

	delete source;
	delete destination;
	source = nullptr;
	destination = nullptr;

	return result;
}


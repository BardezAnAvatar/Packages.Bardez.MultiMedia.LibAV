

#include "SW Scale.h"
#include "LibAV Pixel Converter.h"
#include "LibAV Filter Graph.h"
#include "LibAV Pixel Format.h"
#include "LibAV Pixel Format Extender.h"
#include "LibAV Filter Graph VFlip.h"


using namespace System;
using namespace System::IO;
using namespace Bardez::Projects::Multimedia::LibAV;
using namespace Bardez::Projects::Multimedia::MediaBase::Frame::Image;
using namespace Bardez::Projects::Multimedia::MediaBase::Video::Pixels;


#pragma region IPixelConverter implementation
/// <summary>Converts data from the current format to another specified format</summary>
/// <param name="data">Decompressed data to convert</param>
/// <param name="sourceFormat">Format to convert from</param>
/// <param name="destinationFormat">Format to convert to</param>
/// <param name="horizontalPacking">Row packing</param>
/// <param name="verticalPacking">Row count to align to</param>
/// <param name="sourceWidth">Indicates the source width of the image data</param>
/// <param name="sourceHeight">Indicates the source height of the image data</param>
/// <param name="decodedBitDepth">The bits per pixel once decoded</param>
/// <returns>New byte data</returns>
MemoryStream^ LibAVPixelConverter::ConvertData(MemoryStream^ data, Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums::PixelFormat sourceFormat, Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums::PixelFormat destinationFormat, Int32 horizontalPacking, Int32 verticalPacking, Int32 sourceWidth, Int32 sourceHeight, Int32 decodedBitDepth)
{
    MemoryStream^ result = nullptr;

    //translate pixel formats
    LibAVPixelFormat sourceFormatLibAV = LibAVPixelFormatExtender::ToLibAVPixelFormat(sourceFormat);
    LibAVPixelFormat destFormatLibAV = LibAVPixelFormatExtender::ToLibAVPixelFormat(destinationFormat);

	SWScale^ scale = nullptr;
	try
	{
		scale = gcnew SWScale(
            LibAVPictureDetail::Build(sourceWidth, sourceHeight, sourceFormatLibAV),
            LibAVPictureDetail::Build(sourceWidth, sourceHeight, destFormatLibAV),
            ResizeOptions::Lanczos);

        MemoryStream^ outStream = nullptr;
        scale->Transform(data, outStream);
        result = outStream;
	}
	finally
	{
		delete scale;
	}

    return result;
}

/// <summary>Flips pixel data scan line by scan line</summary>
/// <param name="data">Decompressed data to flip</param>
/// <param name="metadata">Image metadata for the source frame</param>
/// <returns>The vertically flipped data</returns>
/// <remarks>Uses the current pixel data's packing, not the destination's</remarks>
MemoryStream^ LibAVPixelConverter::FlipVertical(MemoryStream^ data, ImageMetadata^ metadata)
{
	//create a graph
	LibAVFilterGraphVFlip^ graph = gcnew LibAVFilterGraphVFlip(metadata);

	//create a Picture from the data MemoryStream
	LibAVPixelFormat format = LibAVPixelFormatExtender::ToLibAVPixelFormat(metadata->Format);
	LibAVPictureDetail detail = LibAVPictureDetail::Build(metadata->Width, metadata->Height, format);
	LibAVPicture^ inputPicture = LibAVPicture::BuildPicture(detail, data);

	//push a picture in
	graph->PushLibAVPicture(inputPicture);

	//pull a picture out
	LibAVPicture^ outputPicture = graph->PullLibAVPicture();

	//close the graph
	graph = nullptr;

	//transform the output picture into a memory stream
	MemoryStream^ returnData = outputPicture->Data;

	return returnData;
}

/// <summary>Adjusts the packing bytes of the data</summary>
/// <param name="data">Data to pack</param>
/// <param name="metadata">Image metadata for the source frame</param>
/// <param name="destPackingHorizontal">target horizontal packing</param>
/// <param name="destPackingVertical">target vertical packing</param>
/// <returns>The adjusted packed bytes</returns>
MemoryStream^ LibAVPixelConverter::AdjustForPacking(MemoryStream^ data, ImageMetadata^ metadata, Int32 destPackingHorizontal, Int32 destPackingVertical)
{
	//HACK: This may or may not be a cop-out, but it seems to me that LibAV has not functions to perform this.
	//	so just use the BasicPixelConverter.

	BasicPixelConverter^ bpc = gcnew BasicPixelConverter();

	return bpc->AdjustForPacking(data, metadata, destPackingHorizontal, destPackingVertical);
}
#pragma endregion


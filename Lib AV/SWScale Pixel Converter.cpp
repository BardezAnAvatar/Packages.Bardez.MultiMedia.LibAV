

#include "SW Scale.h"
#include "SWScale Pixel Converter.h"
#include "LibAV Pixel Format.h"
#include "LibAV Pixel Format Extender.h"


using namespace System;
using namespace System::IO;
using namespace Bardez::Projects::Multimedia::LibAV;
using namespace Bardez::Projects::MultiMedia::LibAV;


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
MemoryStream^ SWScalePixelConverter::ConvertData(MemoryStream^ data, Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums::PixelFormat sourceFormat, Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums::PixelFormat destinationFormat, Int32 horizontalPacking, Int32 verticalPacking, Int32 sourceWidth, Int32 sourceHeight, Int32 decodedBitDepth)
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
#pragma endregion


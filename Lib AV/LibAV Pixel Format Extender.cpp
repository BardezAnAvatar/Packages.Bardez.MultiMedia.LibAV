

#include "LibAV Pixel Format Extender.h"


using namespace Bardez::Projects::Multimedia::LibAV;
using namespace Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums;


/// <summary>Converts the PixelFormat instance to a LibAV Pixel Format enumerator</summary>
/// <param name="source">Source PixelFormat to translate</param>
/// <returns>The translated pixel format, or LibAVPixelFormat.PIX_FMT_NONE if no translation available.</returns>
LibAVPixelFormat LibAVPixelFormatExtender::ToLibAVPixelFormat(PixelFormat source)
{
    LibAVPixelFormat format = LibAVPixelFormat::PIX_FMT_NONE;

    switch (source)
    {
	case PixelFormat::RGB_B5G5R5X1:
		format = LibAVPixelFormat::PIX_FMT_BGR555LE;
		break;
	case PixelFormat::RGB_B8G8R8:
		format = LibAVPixelFormat::PIX_FMT_BGR24;
		break;
	case PixelFormat::RGB_R5G5B5X1:
		format = LibAVPixelFormat::PIX_FMT_RGB555LE;
		break;
	case PixelFormat::RGB_R8G8B8:
		format = LibAVPixelFormat::PIX_FMT_RGB24;
		break;
	case PixelFormat::RGBA_B8G8R8A8:
		format = LibAVPixelFormat::PIX_FMT_BGRA;
		break;
	case PixelFormat::RGBA_R8G8B8A8:
		format = LibAVPixelFormat::PIX_FMT_RGBA;
		break;
	case PixelFormat::YUV_YUV_Y4Cb1Cr1:
		format = LibAVPixelFormat::PIX_FMT_YUV420P;
		break;
	case PixelFormat::RGB_32_Paletted:
		format = LibAVPixelFormat::PIX_FMT_PAL8;
		break;
    }

    return format;
}

/// <summary>Converts the LibAV Pixel Format instance to a PixelFormat enumerator</summary>
/// <param name="source">Source LibAVPixelFormat to translate</param>
/// <returns>The translated pixel format, or LibAVPixelFormat.PIX_FMT_NONE if no translation available.</returns>
PixelFormat LibAVPixelFormatExtender::FromLibAVPixelFormat(LibAVPixelFormat source)
{
    PixelFormat format = PixelFormat::Invalid;

    switch (source)
    {
    case LibAVPixelFormat::PIX_FMT_BGR555LE:
        format = PixelFormat::RGB_B5G5R5X1;
        break;
    case LibAVPixelFormat::PIX_FMT_BGR24:
        format = PixelFormat::RGB_B8G8R8;
        break;
    case LibAVPixelFormat::PIX_FMT_RGB555LE:
        format = PixelFormat::RGB_R5G5B5X1;
        break;
    case LibAVPixelFormat::PIX_FMT_RGB24:
        format = PixelFormat::RGB_R8G8B8;
        break;
    case LibAVPixelFormat::PIX_FMT_BGRA:
        format = PixelFormat::RGBA_B8G8R8A8;
        break;
    case LibAVPixelFormat::PIX_FMT_RGBA:
        format = PixelFormat::RGBA_R8G8B8A8;
        break;
    case LibAVPixelFormat::PIX_FMT_YUV420P:
        format = PixelFormat::YUV_YUV_Y4Cb1Cr1;
        break;
    case LibAVPixelFormat::PIX_FMT_PAL8:
        format = PixelFormat::RGB_32_Paletted;
        break;
    }

    return format;
}




#include "Frame LibAV Video.h"
#include "LibAV Pixel Format Extender.h"
#include "Error Helper.h"
#include "Rational Extender.h"

using namespace Bardez::Projects::Multimedia::LibAV;


#pragma region Properties
/// <summary>Size of the audio sample data</summary>
Int32 FrameLibAVVideo::FrameDataSize::get()
{
	//This needs to be the unmanaged PixelFormat. how do I declare this?
	PixelFormat libAV_NativeFormat = (PixelFormat)(LibAVPixelFormatExtender::ToLibAVPixelFormat(this->metadataImage->Format));

	Int32 size = avpicture_get_size(libAV_NativeFormat, this->metadataImage->Width, this->metadataImage->Height);

	if (size < 0)	//per API
		throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(size, ErrorHelper::LibAvCodec));

	return size;
}
#pragma endregion


#pragma region IMultimediaFrame Properties
/// <summary>The group of metadata describing the representation details of the image data stream</summary>
ImageMetadata^ FrameLibAVVideo::MetadataImage::get()
{
	return this->metadataImage;
}
#pragma endregion


#pragma region Construction
/// <summary>Definition constructor</summary>
/// <param name="source">Source unmanaged frame to read from</param>
/// <param name="packet">Packet to read timestamps from</param>
/// <param name="stream">Source stream for this Frame</summary>
FrameLibAVVideo::FrameLibAVVideo(AVFrame* source, AVPacket* packet, AVStream* stream) : FrameLibAV(source, packet, stream)
{
	Int32 height = stream->codec->height;
	Int32 width = stream->codec->width;
	Int32 packingHorizontal = 0;
	Int32 packingVertical = 0;
	Int32 bitsPerPixel = stream->codec->bits_per_raw_sample;
	Int32 originX = 0;
	Int32 originY = 0;
	Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums::PixelFormat format = LibAVPixelFormatExtender::ToPixelFormat((LibAVPixelFormat)(this->metadataImage->Format));
	Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums::ScanLineOrder order = Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums::ScanLineOrder::TopDown;
	Rational^ aspectRatio = RationalExtender::ToRational(stream->codec->sample_aspect_ratio);
	this->metadataImage = gcnew ImageMetadata(height, width, packingHorizontal, packingVertical, bitsPerPixel, originX, originY, format, order, aspectRatio, nullptr);
	
	//Copy the data
	this->CopyData(source);
}
#pragma endregion


#pragma region IMultimediaImageFrame methods
/// <summary>Retrieves the pixel data in the specified format, in the specified scan line order</summary>
/// <param name="pixelConverter">Interface used to convert the pixel data if necessary</param>
/// <param name="format">Expected output format of the data</param>
/// <param name="order">Expected scan line order of the output data</param>
/// <param name="horizontalPacking">Horizontal packing of bytes for output</param>
/// <param name="verticalPacking">Vertical packing of rows for output</param>
/// <returns>Binary pixel data of the converted data</returns>
MemoryStream^ FrameLibAVVideo::GetFormattedData(IPixelConverter^ pixelConverter, 
	Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums::PixelFormat format,
	Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums::ScanLineOrder order,
	Int32 horizontalPacking, Int32 verticalPacking)
{
    MemoryStream^ dataStream = gcnew MemoryStream(this->frameData);

    //flip if necessary
    if (this->MetadataImage->Order != order)
        dataStream = pixelConverter->FlipVertical(dataStream, this->MetadataImage);    //vertically swap each pixel row

    //strip packing
    if (this->MetadataImage->HorizontalPacking != 0 || this->MetadataImage->VerticalPacking != 0)
        dataStream = pixelConverter->AdjustForPacking(dataStream, this->MetadataImage, 0, 0);     // it should know the existing packing for this instance, specify the new packing

    //convert as necessary
    if (format != this->MetadataImage->Format)
        dataStream = pixelConverter->ConvertData(dataStream, this->MetadataImage->Format, format, horizontalPacking, verticalPacking, this->MetadataImage->Width, this->MetadataImage->Height, this->MetadataImage->ExpandedBitsPerPixel);

    //widen data to the destination horizontal and vertical packing specified
    if (horizontalPacking != 0 || verticalPacking != 0)
	{
		ImageMetadata^ metadata = gcnew ImageMetadata(this->MetadataImage->Height, this->MetadataImage->Width, 0, 0, this->MetadataImage->BitsPerDataPixel, this->MetadataImage->OriginX, this->MetadataImage->OriginY, this->MetadataImage->Format, this->MetadataImage->Order, this->MetadataImage->AspectRatio, nullptr);
        dataStream = pixelConverter->AdjustForPacking(dataStream, metadata, horizontalPacking, verticalPacking);     // it should know the existing packing for this instance, specify the new packing
	}

    //in a return state
    return dataStream;
}
#pragma endregion


#pragma region Helper methods
/// <summary>Copies the data from <see cref="source" /> to <see cref="frameData" /></summary>
/// <remarks>Data is treated differently between Audio and Video and Subtitle</remarks>
void FrameLibAVVideo::CopyData(AVFrame* source)
{
	if (source != NULL && source->data != NULL && source->linesize != NULL) 
	{
		//Generate new byte data array, get its pointer to copy to
		Int32 size = this->FrameDataSize;
		this->frameData = gcnew array<Byte>(size);
		pin_ptr<Byte> dstData = &(this->frameData[0]);
		Byte* destination = dstData;

		AVPicture* picture = reinterpret_cast<AVPicture*>(source);
		
		//This needs to be the unmanaged PixelFormat. how do I declare this?
		PixelFormat libAV_NativeFormat = (PixelFormat)(LibAVPixelFormatExtender::ToLibAVPixelFormat(this->metadataImage->Format));
		Int32 result = avpicture_layout(picture, libAV_NativeFormat, this->metadataImage->Width, this->metadataImage->Height, destination, size);

		if (result < 0)	//per API
			throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(result, ErrorHelper::LibAvCodec));
	}
}
#pragma endregion


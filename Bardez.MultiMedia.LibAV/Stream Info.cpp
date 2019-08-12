

#include "Stream Info.h"
#include "LibAV Audio Format Extender.h"
#include "LibAV Pixel Format Extender.h"
#include "Rational Extender.h"


using namespace Bardez::Projects::Multimedia::LibAV;
using namespace Bardez::Projects::Multimedia::MediaBase;
using namespace Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums;


#pragma region Properties
/// <summary>Index of this stream</summary>
System::Int32 StreamInfo::Index::get()
{
	return this->StreamPtr->index;
}

/// <summary>ID of this stream. Format-specific</summary>
System::Int32 StreamInfo::ID::get()
{
	return this->StreamPtr->id;
}

/// <summary>Reference to codec information about this stream</summary>
CodecInfo^ StreamInfo::Codec::get()
{
	return gcnew CodecInfo(this->StreamPtr->codec);
}

/// <summary>Rational number representation of the stream's frame rate</summary>
Rational^ StreamInfo::FrameRate::get()
{
	return RationalExtender::ToRational(this->StreamPtr->r_frame_rate);
}

/// <summary>Rational number base of all timestamps</summary>
Rational^ StreamInfo::TimeBase::get()
{
	return RationalExtender::ToRational(this->StreamPtr->time_base);
}

/// <summary>Timestamp for the start time of this stream</summary>
System::Int64 StreamInfo::StartTime::get()
{
	return this->StreamPtr->start_time;
}

/// <summary>Timestamp for the length of this stream</summary>
System::Int64 StreamInfo::Duration::get()
{
	return this->StreamPtr->duration;
}

/// <summary>Total count of frames in this stream</summary>
/// <value>Can be 0 if unknown</value>
System::Int64 StreamInfo::FrameCount::get()
{
	return this->StreamPtr->nb_frames;
}

/// <summary>Rational number representation of the aspect ratio</summary>
Rational^ StreamInfo::AspectRatio::get()
{
	return RationalExtender::ToRational(this->StreamPtr->sample_aspect_ratio);
}

/// <summary>Rational number representation of the stream's average frame rate</summary>
Rational^ StreamInfo::FrameRateAverage::get()
{
	return RationalExtender::ToRational(this->StreamPtr->avg_frame_rate);
}
#pragma endregion


#pragma region Metadata generation
/// <summary>Generates audio metadata for the stream</summary>
/// <returns>A <see cref="WaveFormatEx" /> for the audio stream or null if a non-audio stream</returns>
WaveFormatEx^ StreamInfo::GenerateMetadataAudio()
{
	WaveFormatEx^ format = nullptr;

	if (this->StreamPtr->codec->codec_type == AVMEDIA_TYPE_AUDIO)
	{
		UInt16 sampleFormat = (UInt16)(LibAVAudioFormatExtender::ToAudioDataFormat(this->StreamPtr->codec->sample_fmt));
		UInt16 channels = Convert::ToUInt16(this->StreamPtr->codec->channels);
		UInt32 sampleRate = Convert::ToUInt32(this->StreamPtr->codec->sample_rate);
		UInt16 bitsPerSample = LibAVAudioFormatExtender::GetBitsPerSample(this->StreamPtr->codec->sample_fmt);

		UInt16 extraSize = 0;
		UInt16 alignment = Convert::ToUInt16((bitsPerSample / 8) * channels);
		UInt32 avgBytesPerSec = sampleRate * alignment;

		format = gcnew WaveFormatEx(sampleFormat, channels, sampleRate, avgBytesPerSec, alignment, bitsPerSample, extraSize);
	}

	return format;
}

/// <summary>Generates image metadata for the stream</summary>
/// <returns>An <see cref="ImageMetadata" /> for the video stream or null if a non-video stream</returns>
ImageMetadata^ StreamInfo::GenerateMetadataImage()
{
	ImageMetadata^ metadata = nullptr;

	if (this->StreamPtr->codec->codec_type == AVMEDIA_TYPE_VIDEO)
	{
		Int32 width = this->StreamPtr->codec->width;
		Int32 height = this->StreamPtr->codec->height;
		Int32 rowBytePadding = 0;	//I believe that LibAV does full data compacting and does not provide any row packing
		Int32 heightRowPadding = 0;	//I believe that LibAV does full data compacting and does not provide any row packing
		Int32 bitsPerPixel = this->StreamPtr->codec->bits_per_raw_sample;
		Int32 originX = 0;
		Int32 originY = 0;
		Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums::PixelFormat format = LibAVPixelFormatExtender::ToPixelFormat((LibAVPixelFormat)(this->StreamPtr->codec->pix_fmt));
		ScanLineOrder order = ScanLineOrder::TopDown;
		Rational^ aspectRatio = RationalExtender::ToRational(this->StreamPtr->codec->sample_aspect_ratio);

		metadata = gcnew ImageMetadata(height, width, rowBytePadding, heightRowPadding, bitsPerPixel, originX, originY, format, order, aspectRatio, nullptr);
	}

	return metadata;
}
#pragma endregion


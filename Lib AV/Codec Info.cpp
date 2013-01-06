

#include "Codec Info.h"
#include "Rational Extender.h"


using namespace System::Runtime::InteropServices;
using namespace Bardez::Projects::BasicStructures::Math;
using namespace Bardez::Projects::Multimedia::LibAV;
using namespace Bardez::Projects::MultiMedia::LibAV;


#pragma region Properties
/// <summary>Name of the codec</summary>
System::String^ CodecInfo::Name::get()
{
	//these want a Character*, not a Byte*; this crashes the compiler
	//this->Name = gcnew System::String(codecContext->codec_name);
	//return Marshal::PtrToStringAnsi((System::IntPtr)(this->CodecContextPtr-codec_name), 32);		//codec_name is (often?) blank, it apears
	return Marshal::PtrToStringAuto((System::IntPtr)(System::Byte*)(this->CodecContextPtr->codec->name), 32);
}

/// <summary>Descriptive name of the codec</summary>
System::String^ CodecInfo::LongName::get()
{
	//these want a Character*, not a Byte*; this crashes the compiler
	//this->LongName = gcnew System::String(CodecContextPtr->codec->long_name);
	return Marshal::PtrToStringAuto((System::IntPtr)(System::Byte*)(this->CodecContextPtr->codec->long_name));
}

/// <summary>Type of this codec (Audio, video, etc.)</summary>
MediaType CodecInfo::CodecType::get()
{
	return (MediaType)(this->CodecContextPtr->codec_type);
}

/// <summary>Unique ID of the codec within Lib AV</summary>
Bardez::Projects::MultiMedia::LibAV::LibAVCodecID CodecInfo::ID::get()
{
	return (Bardez::Projects::MultiMedia::LibAV::LibAVCodecID)(this->CodecContextPtr->codec_id);
}

/// <summary>(Possibly erroneously) converted FourCC code from a container</summary>
System::String^ CodecInfo::FourCC::get()
{
	//not sure which one to use, here
	return System::Text::Encoding::ASCII->GetString(System::BitConverter::GetBytes(this->CodecContextPtr->codec_tag));
	//return System::Text::Encoding::ASCII->GetString(System::BitConverter::GetBytes(this->CodecContextPtr->stream_codec_tag));
}

/// <summary>The average bitrate</summary>
System::Int32 CodecInfo::BitRate::get()
{
	return this->CodecContextPtr->bit_rate;
}

/// <summary>
///		This is the fundamental unit of time (in seconds) in terms of which frame
///		timestamps are represented. For fixed-fps content, timebase should be 1/framerate
///		and timestamp increments should be identically 1.
/// </summary>
Rational^ CodecInfo::TimeBase::get()
{
	return RationalExtender::ToRational(this->CodecContextPtr->time_base);
}

/// <summary>
///		For some codecs, the time base is closer to the field rate than the frame rate.
///		Most notably, H.264 and MPEG-2 specify time_base as half of frame duration
///		if no telecine is used ...
///		
///		Set to time_base ticks per frame. Default 1, e.g., H.264/MPEG-2 set it to 2.
/// </summary>
System::Int32 CodecInfo::TicksPerFrame::get()
{
	return this->CodecContextPtr->ticks_per_frame;
}
		
/// <summary>
///		Encoding: Number of frames delay there will be from the encoder input to
///		          the decoder output. (we assume the decoder matches the spec)
///		Decoding: Number of frames delay in addition to what a standard decoder
///		          as specified in the spec would produce.
///		
///		Video:
///		  Number of frames the decoded output will be delayed relative to the
///		  encoded input.
///		
///		Audio:
///		  Number of "priming" samples added to the beginning of the stream
///		  during encoding. The decoded output will be delayed by this many
///		  samples relative to the input to the encoder. Note that this field is
///		  purely informational and does not directly affect the pts output by
///		  the encoder, which should always be based on the actual presentation
///		  time, including any delay.
/// </summary>
System::Int32 CodecInfo::Delay::get()
{
	return this->CodecContextPtr->delay;
}

/// <summary>Picture width</summary>
System::Int32 CodecInfo::Width::get()
{
	return this->CodecContextPtr->width;
}

/// <summary>Picture height</summary>
System::Int32 CodecInfo::Height::get()
{
	return this->CodecContextPtr->height;
}

/// <summary>Picture width as coded</summary>
System::Int32 CodecInfo::CodedWidth::get()
{
	return this->CodecContextPtr->coded_width;
}

/// <summary>Picture height as coded</summary>
System::Int32 CodecInfo::CodedHeight::get()
{
	return this->CodecContextPtr->coded_height;
}

/// <summary>Output pixel data format</summary>
Bardez::Projects::MultiMedia::LibAV::LibAVPixelFormat CodecInfo::PixelDataFormat::get()
{
	return (Bardez::Projects::MultiMedia::LibAV::LibAVPixelFormat)(this->CodecContextPtr->pix_fmt);
}

/// <summary>Aspect ratio of the pixels</summary>
Rational^ CodecInfo::SampleAspectRatio::get()
{
	return RationalExtender::ToRational(this->CodecContextPtr->sample_aspect_ratio);
}

/// <summary>Sample rate of audio</summary>
System::Int32 CodecInfo::SampleRate::get()
{
	return this->CodecContextPtr->sample_rate;
}

/// <summary>Number of audio channels</summary>
System::Int32 CodecInfo::ChannelCount::get()
{
	return this->CodecContextPtr->channels;
}

/// <summary>Output format of audio samples</summary>
AudioSampleFormat CodecInfo::SampleFormat::get()
{
	return (AudioSampleFormat)(this->CodecContextPtr->sample_fmt);
}
#pragma endregion



#pragma region Construction
/// <summary>Definition constructor</summary>
/// <param name="context">Unmanaged AVCodecContext to copy from</param>
CodecInfo::CodecInfo(AVCodecContext* context)
{
	this->CodecContextPtr = context;
}
#pragma endregion


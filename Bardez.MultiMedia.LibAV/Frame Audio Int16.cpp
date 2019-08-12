

#include "Frame Audio Int16.h"
#include "Audio Sample Format.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace Bardez::Projects::Multimedia::LibAV;


#pragma region Helpers
/// <summary>Helper that copies data from the unmanaged pointers to managed set, based on the expected frame type</summary>
/// <param name="source">Source unmanaged frame to read from</param>
void FrameAudioInt16::CopyData(AVFrame* source)
{
	Int32 formatByteSize = FrameAudioInt16::SampleSize(source->format);
	Int32 dataSize = this->CountSamples * this->CountChannels * formatByteSize;
	this->Data = gcnew array<Byte>(dataSize);

	Marshal::Copy(IntPtr(source->data[0]), this->Data, 0, dataSize);
}

/// <summary>Computes the size of an audio sample based on its audio format</summary>
/// <param name="format">Format of the audio sample from a source frame</param>
Int32 FrameAudioInt16::SampleSize(Int32 format)
{
	Int32 sampleByteSize;

	AudioSampleFormat sampleFormat = (AudioSampleFormat)(format);

	switch (sampleFormat)
	{
		default:
		case AudioSampleFormat::AV_SAMPLE_FMT_NONE:
			sampleByteSize = 0;
			break;
		case AudioSampleFormat::AV_SAMPLE_FMT_U8:
		case AudioSampleFormat::AV_SAMPLE_FMT_U8P:
			sampleByteSize = 1;
			break;
		case AudioSampleFormat::AV_SAMPLE_FMT_S16:
		case AudioSampleFormat::AV_SAMPLE_FMT_S16P:
			sampleByteSize = 2;
			break;
		case AudioSampleFormat::AV_SAMPLE_FMT_S32:
		case AudioSampleFormat::AV_SAMPLE_FMT_S32P:
			sampleByteSize = 4;
			break;
			
		case AudioSampleFormat::AV_SAMPLE_FMT_FLT:
		case AudioSampleFormat::AV_SAMPLE_FMT_FLTP:
			sampleByteSize = sizeof(Single);
			break;
		case AudioSampleFormat::AV_SAMPLE_FMT_DBL:
		case AudioSampleFormat::AV_SAMPLE_FMT_DBLP:
			sampleByteSize = sizeof(Double);
			break;
	}

	return sampleByteSize;
}
#pragma endregion
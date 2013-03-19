

#include "LibAV Audio Format Extender.h"


using namespace System;
using namespace Bardez::Projects::Multimedia::LibAV;


#pragma region Conversion
/// <summary>Extension method to convert <see cref="AudioDataFormat" /> to <see cref="AVSampleFormat" /></summary>
/// <param name="sourceFormat">Source audio data format</param>
/// <param name="bits">Number of bits per sample</param>
AVSampleFormat LibAVAudioFormatExtender::ToAVSampleFormat(AudioDataFormat sourceFormat, Int32 bits)
{
	AVSampleFormat outputFormat = AV_SAMPLE_FMT_NONE;

	if (bits == 8 && sourceFormat == AudioDataFormat::PCM)
		outputFormat = AV_SAMPLE_FMT_U8;
	else if (bits == 16 && sourceFormat == AudioDataFormat::PCM)
		outputFormat = AV_SAMPLE_FMT_S16;
	else if (bits == 32 && sourceFormat == AudioDataFormat::PCM)
		outputFormat = AV_SAMPLE_FMT_S32;
	else if (bits == 16 && sourceFormat == AudioDataFormat::IEEE_FP)
		outputFormat = AV_SAMPLE_FMT_FLT;
	else if (bits == 32 && sourceFormat == AudioDataFormat::IEEE_FP)
		outputFormat = AV_SAMPLE_FMT_DBL;

	return outputFormat;
}

/// <summary>Extension method to convert <see cref="AVSampleFormat" /> to <see cref="AudioDataFormat" /></summary>
/// <param name="sourceFormat">Source audio data format</param>
AudioDataFormat LibAVAudioFormatExtender::ToAudioDataFormat(AVSampleFormat sourceFormat)
{
	AudioDataFormat outputFormat = AudioDataFormat::Unknown;

	switch (sourceFormat)
	{
	case AV_SAMPLE_FMT_U8:
	case AV_SAMPLE_FMT_S16:
	case AV_SAMPLE_FMT_S32:
		outputFormat = AudioDataFormat::PCM;
		break;
	case AV_SAMPLE_FMT_FLT:
	case AV_SAMPLE_FMT_DBL:
		outputFormat = AudioDataFormat::IEEE_FP;
		break;
	}

	return outputFormat;
}

/// <summary>Method to get the bit count of samples based off of the <see cref="AVSampleFormat" /></summary>
/// <param name="sourceFormat">Source audio data format</param>
UInt16 LibAVAudioFormatExtender::GetBitsPerSample(AVSampleFormat sourceFormat)
{
	UInt16 bps = 0;

	switch (sourceFormat)
	{
	case AV_SAMPLE_FMT_U8:
		bps = 8;
		break;
	case AV_SAMPLE_FMT_S16:
	case AV_SAMPLE_FMT_FLT:
		bps = 16;
		break;
	case AV_SAMPLE_FMT_S32:
	case AV_SAMPLE_FMT_DBL:
		bps = 32;
		break;
	}

	return bps;
}
#pragma endregion


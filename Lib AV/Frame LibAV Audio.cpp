

#include "Frame LibAV Audio.h"
#include "LibAV Audio Format Extender.h"


using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace Bardez::Projects::BasicStructures::Win32::Audio;
using namespace Bardez::Projects::Multimedia::LibAV;


#pragma region Properties
/// <summary>Size of the audio sample data</summary>
Int32 FrameLibAVAudio::FrameDataSize::get()
{
	/* original, probably hacked and assumption-based: */
	//UInt16 formatByteSize = this->metadataAudio->BitsPerSample / 8;
	//UInt16 channels = this->metadataAudio->NumberChannels;

	//Int32 dataSize = Convert::ToInt32(this->sampleCount * channels * formatByteSize);
	//return dataSize;


	
	/* newer LibAV-based: */
	return this->frameDataSize;
}
#pragma endregion


#pragma region Construction
/// <summary>Definition constructor</summary>
/// <param name="source">Source unmanaged frame to read from</param>
/// <param name="packet">Packet to read timestamps from</param>
/// <param name="stream">Source stream for this Frame</param>
FrameLibAVAudio::FrameLibAVAudio(AVFrame* source, AVPacket* packet, AVStream* stream) : FrameLibAV(source, packet, stream)
{
	this->sampleCount = source->nb_samples;

	//build WaveFormatEx
	AudioDataFormat format = LibAVAudioFormatExtender::ToAudioDataFormat(stream->codec->sample_fmt);
	UInt16 bitsPerSample = LibAVAudioFormatExtender::GetBitsPerSample(stream->codec->sample_fmt);
	UInt16 channels = Convert::ToUInt16(stream->codec->channels);
	UInt32 sampleRate = Convert::ToUInt32(stream->codec->sample_rate);
	UInt16 alignment = Convert::ToUInt16((bitsPerSample / 8) * channels);
	UInt32 avgBytesPerSec = sampleRate * alignment;
	this->metadataAudio = gcnew WaveFormatEx((UInt16)(format), channels, sampleRate, avgBytesPerSec, alignment, bitsPerSample, 0);


	//store the frame size
	//per AVFrame definition, only value is 0th index for Audio frames; see avcodec.h, line 941
	//	For planar audio, each channel has a separate data pointer, and all are the same size;
	this->frameDataSize = source->linesize[0];
	//TODO: figure something out to determine the size of the data for planar audio


	//Copy the data
	this->CopyData(source);
}
#pragma endregion


#pragma region IWaveFormatEx methods
/// <summary>Returns a WaveFormatEx instance from this header data</summary>
/// <returns>A WaveFormatEx instance to submit to API calls</returns>
WaveFormatEx^ FrameLibAVAudio::GetWaveFormat()
{
	return this->metadataAudio;
}
#pragma endregion


#pragma region Helper methods
/// <summary>Copies the data from <see cref="source" /> to <see cref="frameData" /></summary>
/// <param name="source">Source AVFrame to copy data from</param>
/// <remarks>Data is treated differently between Audio and Video and Subtitle</remarks>
void FrameLibAVAudio::CopyData(AVFrame* source)
{
	if (source != NULL && source->data != NULL && source->linesize != NULL) 
	{
		Int32 size = this->FrameDataSize;
		this->frameData = gcnew array<Byte>(size);
		
		Marshal::Copy(IntPtr(source->data[0]), this->frameData, 0, size);
	}
}
#pragma endregion


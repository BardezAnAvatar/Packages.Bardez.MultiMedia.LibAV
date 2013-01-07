

#include "Frame LibAV Audio.h"
#include "LibAV Audio Format Extender.h"


using namespace System::IO;
using namespace Bardez::Projects::BasicStructures::Win32::Audio;
using namespace Bardez::Projects::Multimedia::LibAV;


#pragma region IMultimediaFrame Properties
/// <summary>The streaming metadata info</summary>
Stream^ FrameLibAVAudio::Data::get()
{
	IntPtr unmanagedData = IntPtr(this->PtrPicture->data[0]);
	Int32 length = this->FrameDataSize;

	Byte* pointer = reinterpret_cast<Byte*>(unmanagedData.ToPointer());	//get an internal pointer?
	UnmanagedMemoryStream^ dataStream = gcnew UnmanagedMemoryStream(pointer, length);

	return dataStream;
}
#pragma endregion


#pragma region Properties
/// <summary>Pointer exposure of the passed in construction AVFrame</summary>
AVPicture* FrameLibAVAudio::PtrPicture::get()
{
	return reinterpret_cast<AVPicture*>(this->ptrAVPicture.ToPointer());
}

/// <summary>Pointer exposure of the passed in construction AVFrame</summary>
void FrameLibAVAudio::PtrPicture::set(AVPicture* value)
{
	this->ptrAVPicture = IntPtr(value);
}

/// <summary>Size of the audio sample data</summary>
Int32 FrameLibAVAudio::FrameDataSize::get()
{
	UInt16 formatByteSize = this->metadataAudio->BitsPerSample / 8;
	UInt16 channels = this->metadataAudio->NumberChannels;

	Int32 dataSize = Convert::ToInt32(this->sampleCount * channels * formatByteSize);
	return dataSize;
}
#pragma endregion


#pragma region Construction
/// <summary>Definition constructor</summary>
/// <param name="source">Source unmanaged frame to read from</param>
/// <param name="packet">Packet to read timestamps from</param>
/// <param name="stream">Source stream for this Frame</summary>
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


	//declare a new picture copy with which to access data
	AVPicture* picture = new AVPicture;
	av_picture_copy(picture, reinterpret_cast<AVPicture*>(source), stream->codec->pix_fmt, stream->codec->width, stream->codec->height);	//see avcodec.h @ 4263
	this->ptrAVPicture = IntPtr(picture);
}
#pragma endregion


#pragma region Destruction
/// <summary>Destructor</summary>
/// <remarks>Dispose()</remarks>
FrameLibAVAudio::~FrameLibAVAudio()
{
	this->DisposeUnmanaged();
	GC::SuppressFinalize(this);
}

/// <summary>Destructor logic, disposes the object</summary>
void FrameLibAVAudio::DisposeUnmanaged()
{
	FrameLibAV::DisposeUnmanaged();

	if (this->PtrPicture != NULL)
	{
		avpicture_free(PtrPicture);	//see avcodec.h @ 4198
		this->PtrPicture = NULL;
	}
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


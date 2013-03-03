

#include "Stream Buffers.h"


using namespace System;
using namespace System::Collections::Generic;
using namespace Bardez::Projects::Multimedia::LibAV;


#pragma region Properties
/// <summary>Exposes a flag indicating whether the streams' buffers are full</summary>
/// <value>True indicates that the buffers are full and should not be added to; false indicates that room still exists</value>
Boolean StreamBuffers::BuffersFull::get()
{
	Boolean full = false;

	//check video
	for each(Int32 key in this->StreamsVideo->Keys)
		if (this->StreamsVideo[key]->BufferFull)
		{
			full = true;
			break;
		}
		
	//check audio
	if (!full)
	{
		for each(Int32 key in this->StreamsAudio->Keys)
			if (this->StreamsAudio[key]->BufferFull)
			{
				full = true;
				break;
			}
	}

	return full;
}
#pragma endregion


#pragma region Helpers
/// <summary>Initializes the dictionaries from a collection of StreamInfo</summary>
void StreamBuffers::InitializeDictionaries(IList<StreamInfo^>^ streams)
{
	this->StreamsVideo = gcnew Dictionary<Int32, StreamProcessingBuffer<FrameBGRA^>^>();
	this->StreamsAudio = gcnew Dictionary<Int32, StreamProcessingBuffer<FrameAudioInt16^>^>();
	this->Streams = streams;

	for (Int32 index = 0; index < streams->Count; ++index)
	{
		switch (streams[index]->Codec->CodecType)
		{
			case MediaType::AVMEDIA_TYPE_VIDEO:
				this->StreamsVideo->Add(index, gcnew StreamProcessingBuffer<FrameBGRA^>(streams[index]->StreamPtr, this->DefaultBuffserSize));
				break;

			case MediaType::AVMEDIA_TYPE_AUDIO:
				this->StreamsAudio->Add(index, gcnew StreamProcessingBuffer<FrameAudioInt16^>(streams[index]->StreamPtr, this->DefaultBuffserSize));
				break;

			case MediaType::AVMEDIA_TYPE_SUBTITLE:
			default:
				break;
		}
	}
}
#pragma endregion


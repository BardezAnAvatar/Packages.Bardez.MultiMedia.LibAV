

#ifndef _Stream_Buffers_H_
#define _Stream_Buffers_H_

using namespace System;
using namespace System::Collections::Generic;

#include "Frame Audio Int16.h"
#include "Frame BGRA.h"
#include "Stream Info.h"
#include "Stream Processing Buffer.h"
#include "Stream Processing Info.h"

namespace Bardez
{
	namespace Projects
	{
		namespace MultiMedia
		{
			namespace LibAV
			{
				public ref class StreamBuffers
				{
				#pragma region Fields
				public:
					/// <summary>Dictionary of video stream processing records, the key being the stream number</summary>
					Dictionary<Int32, StreamProcessingBuffer<FrameBGRA^>^>^ StreamsVideo;
					
					/// <summary>Dictionary of audio stream processing records, the key being the stream number</summary>
					Dictionary<Int32, StreamProcessingBuffer<FrameAudioInt16^>^>^ StreamsAudio;

					/// <summary>Represents a collection of stream info keyed to the stream index number</summary>
					IList<StreamInfo^>^ Streams;
				#pragma endregion


				#pragma region Properties
				public:
					/// <summary>Exposes a flag indicating whether the streams' buffers are full</summary>
					/// <value>True indicates that the buffers are full and should not be added to; false indicates that room still exists</value>
					property Boolean BuffersFull
					{
						Boolean get();
					}
				#pragma endregion


				#pragma region Construction
				public:
					/// <summary>Default constructor</summary>
					StreamBuffers() { }

					/// <summary>Definition constructor</summary>
					StreamBuffers(IList<StreamInfo^>^ streams)
					{
						this->InitializeDictionaries(streams);
					}
				#pragma endregion


				#pragma region Helpers
				protected:
					/// <summary>Initializes the dictionaries from a collection of StreamInfo</summary>
					void InitializeDictionaries(IList<StreamInfo^>^ streams);
				#pragma endregion
				};
			}
		}
	}
}

#endif


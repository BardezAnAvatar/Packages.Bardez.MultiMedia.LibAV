

#ifndef _Frame_LibAV_
#define _Frame_LibAV_


extern "C"
{
	#include "libavcodec\avcodec.h"
	#include "libavformat\avformat.h"
}


using namespace System;
using namespace System::IO;
using namespace Bardez::Projects::BasicStructures::Math;
using namespace Bardez::Projects::Multimedia::MediaBase::Frame;


namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>Frame basis for streaming in LibAV</summary>
				public ref class FrameLibAV abstract : IMultimediaStreamingFrame
				{
				#pragma region Fields
				protected:
					/// <summary>Presentation timestap (multiple of least common factor's timebase, where this frame should be presented)</summary>
					TimeSpan presentationTimeStamp;

					/// <summary>Decoding timestap (multiple of least common factor's timebase, where this frame appeared in the stream)</summary>
					TimeSpan decodeTimeStamp;

					/// <summary>Indicated whether this frame is a keyframe or not</summary>
					Boolean keyFrame;

					/// <summary>Reference to the streaming metadata info</summary>
					StreamingMetadata^ metadataStreaming;
				#pragma endregion


				#pragma region Properties
				public:
					/// <summary>Presentation timestap (multiple of least common factor's timebase, where this frame should be presented)</summary>
					property TimeSpan PresentationTimeStamp
					{
						TimeSpan get();
					}

					/// <summary>Decoding timestap (multiple of least common factor's timebase, where this frame appeared in the stream)</summary>
					property TimeSpan DecodeTimeStamp
					{
						TimeSpan get();
					}

					/// <summary>Indicated whether this frame is a keyframe or not</summary>
					property Boolean KeyFrame
					{
						Boolean get();
					}
				#pragma endregion


				#pragma region IMultimediaStreamingFrame Properties
				public:
					/// <summary>The streaming metadata info</summary>
					virtual property Bardez::Projects::Multimedia::MediaBase::Frame::StreamingMetadata^ StreamingMetadata
					{
						Bardez::Projects::Multimedia::MediaBase::Frame::StreamingMetadata^ get();
					}
				#pragma endregion


				#pragma region IMultimediaFrame Properties
				public:
					/// <summary>The streaming metadata info</summary>
					virtual property Stream^ Data
					{
						Stream^ get() abstract;
					}
				#pragma endregion


				#pragma region Construction
				internal:
					/// <summary>Definition constructor</summary>
					/// <param name="source">Source unmanaged frame to read from</param>
					/// <param name="packet">Packet to read timestamps from</param>
					/// <param name="stream">Source stream for this Frame</summary>
					FrameLibAV(AVFrame* source, AVPacket* packet, AVStream* stream);
				#pragma endregion


				#pragma region Destruction
				public:
					/// <summary>Destructor</summary>
					/// <remarks>Dispose()</remarks>
					~FrameLibAV();

					//No pointers locally, so no Finalizer needed.

				internal:
					/// <summary>Destructor logic, disposes the object</summary>
					virtual void DisposeUnmanaged();
				#pragma endregion
				};
			}
		}
	}
}


#endif


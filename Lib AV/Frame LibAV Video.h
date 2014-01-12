

#ifndef _Frame_LibAV_Video_
#define _Frame_LibAV_Video_


#include "Frame LibAV.h"


using namespace System;
using namespace System::IO;
using namespace Bardez::Projects::Multimedia::MediaBase::Data::Pixels;
using namespace Bardez::Projects::Multimedia::MediaBase::Frame::Image;
using namespace Bardez::Projects::Multimedia::MediaBase::Frame::Video;


namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>Video frame for streaming in LibAV</summary>
				public ref class FrameLibAVVideo : FrameLibAV, IMultimediaVideoFrame
				{
				#pragma region Fields
				protected:
					/// <summary>Reference to the image metadata info</summary>
					ImageMetadata^ metadataImage;
				#pragma endregion


				#pragma region Properties
				protected:
					/// <summary>Size of the audio sample data</summary>
					property Int32 FrameDataSize
					{
						Int32 get();
					}
				#pragma endregion


				#pragma region IMultimediaFrame Properties
				public:
					/// <summary>The group of metadata describing the representation details of the image data stream</summary>
					virtual property ImageMetadata^ MetadataImage
					{
						ImageMetadata^ get();
					}
				#pragma endregion


				#pragma region Construction
				internal:
					/// <summary>Definition constructor</summary>
					/// <param name="source">Source unmanaged frame to read from</param>
					/// <param name="packet">Packet to read timestamps from</param>
					/// <param name="stream">Source stream for this Frame</param>
					FrameLibAVVideo(AVFrame* source, AVPacket* packet, AVStream* stream);
				#pragma endregion


				#pragma region IMultimediaImageFrame methods
				public:
					/// <summary>Retrieves the pixel data in the specified format, in the specified scan line order</summary>
					/// <param name="pixelConverter">Interface used to convert the pixel data if necessary</param>
					/// <param name="format">Expected output format of the data</param>
					/// <param name="order">Expected scan line order of the output data</param>
					/// <param name="horizontalPacking">Horizontal packing of bytes for output</param>
					/// <param name="verticalPacking">Vertical packing of rows for output</param>
					/// <returns>Binary pixel data of the converted data</returns>
					virtual MemoryStream^ GetFormattedData(IPixelConverter^ pixelConverter, 
						Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums::PixelFormat format,
						Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums::ScanLineOrder order,
						Int32 horizontalPacking, Int32 verticalPacking);

					/// <summary>Gets a sub-image of the current image</summary>
					/// <param name="x">Source X position</param>
					/// <param name="y">Source Y position</param>
					/// <param name="width">Width of sub-image</param>
					/// <param name="height">Height of sub-image</param>
					/// <returns>The requested sub-image</returns>
					/// <remarks>The generated point of origin should be (0,0).</remarks>
					virtual IMultimediaImageFrame^ GetSubImage(Int32 x, Int32 y, Int32 width, Int32 height);
				#pragma endregion


				#pragma region Helper methods
				protected:
					/// <summary>Copies the data from to <see cref="frameData" /></summary>
					/// <param name="source">Source AVFrame to copy data from</param>
					/// <remarks>Data is treated differently between Audio and Video and Subtitle</remarks>
					virtual void CopyData(AVFrame* source) override;
				#pragma endregion
				};
			}
		}
	}
}


#endif


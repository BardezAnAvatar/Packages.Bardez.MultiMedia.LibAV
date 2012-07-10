

#ifndef _SWSCALE_H_
#define _SWSCALE_H_

extern "C"
{
	#include "libswscale\swscale.h"
}

#include "Resize Options.h"
#include "LibAV Picture Detail.h"
#include "LibAV Picture.h"


using namespace System;


namespace Bardez
{
	namespace Projects
	{
		namespace MultiMedia
		{
			namespace LibAV
			{
				public ref class SWScale
				{
				#pragma region Fields
				internal:
					/// <summary>Pointer to the associated SW Scale context</summary>
					IntPtr context;

				protected:
					/// <summary>Represents the source detail for scaling</summary>
					LibAVPictureDetail detailSource;

					/// <summary>Represents the destination detail for scaling</summary>
					LibAVPictureDetail detailDestination;

					/// <summary>Resizing options for scaling</summary>
					ResizeOptions options;
				#pragma endregion


				#pragma region Properties
				internal:
					/// <summary>Exposes a pointer to the SW Scale context</summary>
					property SwsContext* SWS_Context
					{
						SwsContext* get()
						{
							return reinterpret_cast<SwsContext*>(this->context.ToPointer());
						}
						void set(SwsContext* value)
						{
							this->context = System::IntPtr(value);
						}
					}

				public:
					/// <summary>Represents the source detail for scaling</summary>
					property LibAVPictureDetail DetailSource
					{
						LibAVPictureDetail get()
						{
							return this->detailSource;
						}
					}

					/// <summary>Represents the destination detail for scaling</summary>
					property LibAVPictureDetail DetailDestination
					{
						LibAVPictureDetail get()
						{
							return this->detailDestination;
						}
					}

					/// <summary>Resizing options for scaling</summary>
					property ResizeOptions Options
					{
						ResizeOptions get()
						{
							return this->options;
						}
					}
				#pragma endregion


				#pragma region Construction
				public:
					/// <summary>Definition constructor</summary>
					/// <param name="source">Source image format details</param>
					/// <param name="destination">Destination image format details</param>
					/// <param name="options">Resizing options</param>
					SWScale(LibAVPictureDetail source, LibAVPictureDetail destination, ResizeOptions options)
					{
						//store input for possible reference
						this->detailSource = source;
						this->detailDestination = destination;
						this->options = options;

						//generate the context
						SwsContext* context = sws_getContext(	this->detailSource.Width, this->detailSource.Height, (PixelFormat)this->detailSource.Format,
																this->detailDestination.Width, this->detailDestination.Height, (PixelFormat)this->detailDestination.Format,
																(Int32)options, NULL, NULL, NULL);

						if (context == NULL)
							throw gcnew ApplicationException(String::Format("Could not initialize a scaling context in {0}.", ErrorHelper::LibSwScale));

						//store the generated context
						this->SWS_Context = context;
					}
				#pragma endregion


				#pragma region Destruction
				public:
					/// <summary>Destructor</summary>
					/// <remarks>Dispose()</remarks>
					~SWScale()
					{
						this->DisposeUnmanaged();
					}
					
					/// <summary>Destructor</summary>
					/// <remarks>Finalize()</remarks>
					!SWScale()
					{
						this->DisposeUnmanaged();
					}

				internal:
					/// <summary>Destructor logic, disposes the object</summary>
					void DisposeUnmanaged()
					{
						//close the AV format context
						if (this->SWS_Context)
						{
							SwsContext* nativePtr = this->SWS_Context;
							
							//free the scaling context
							sws_freeContext(nativePtr);

							this->context = System::IntPtr::Zero;
						}
					}
				#pragma endregion


				#pragma region Static Methods
				public:
					/// <summary>Determines if the format is supported for input</summary>
					/// <param name="pix_fmt">Pixel type to check against</param>
					/// <returns>A Boolean flag indicating whether the input is supported</returns>
					static Boolean IsSupportedInput(LibAVPixelFormat pix_fmt);

					/// <summary>Determines if the format is supported for input</summary>
					/// <param name="input">SwScaleDetail containing pixel type to check against</param>
					/// <returns>A Boolean flag indicating whether the input is supported</returns>
					static Boolean IsSupportedInput(LibAVPictureDetail input);

					/// <summary>Determines if the format is supported for output</summary>
					/// <param name="pix_fmt">Pixel type to check against</param>
					/// <returns>A Boolean flag indicating whether the input is supported</returns>
					static Boolean IsSupportedOutput(LibAVPixelFormat pix_fmt);

					/// <summary>Determines if the format is supported for output</summary>
					/// <param name="output">SwScaleDetail containing pixel type to check against</param>
					/// <returns>A Boolean flag indicating whether the input is supported</returns>
					static Boolean IsSupportedOutput(LibAVPictureDetail output);

					/// <summary>Unpacks an 8-bit paletted data stream to a 24-bit pixel data stream</summary>
					/// <param name="source">Source data to read from</param>
					/// <param name="destination">Destination data to write to</param>
					/// <param name="pixelCount">Number of pixels to convert</param>
					/// <param name="palette">24-bit palette data to read from</param>
					static void UnpackPalettedData8BitTo24Bit(array<Byte>^ source, array<Byte>^ destination, Int32 pixelCount, array<Byte>^ palette);
					
					/// <summary>Unpacks an 8-bit paletted data stream to a 32-bit pixel data stream</summary>
					/// <param name="source">Source data to read from</param>
					/// <param name="destination">Destination data to write to</param>
					/// <param name="pixelCount">Number of pixels to convert</param>
					/// <param name="palette">32-bit palette data to read from</param>
					static void UnpackPalettedData8BitTo32Bit(array<Byte>^ source, array<Byte>^ destination, Int32 pixelCount, array<Byte>^ palette);
				#pragma endregion


				#pragma region Transformation method
				public:
					/// <summary>Performs the constructed SW Scale transformation associated with this context</summary>
					/// <param name="source">Source picture to read source data from</summary>
					/// <param name="destination">Destination picture to write source data to</summary>
					/// <returns>The height of the output image</returns>
					Int32 Transform(LibAVPicture^ source, LibAVPicture^ destination);
				#pragma endregion
				};
			}
		}
	}
}

#endif


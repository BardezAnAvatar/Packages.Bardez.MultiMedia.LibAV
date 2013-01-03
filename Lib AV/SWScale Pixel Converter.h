

#ifndef _SWScale_Pixel_Converter_
#define _SWScale_Pixel_Converter_


using namespace System;
using namespace System::IO;
using namespace Bardez::Projects::Multimedia::MediaBase::Data::Pixels;


namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>Pixel converter that utilizes SWScale from LibAV</summary>
				public ref class SWScalePixelConverter : IPixelConverter
				{
				#pragma region IPixelConverter implementation
				public:
					/// <summary>Converts data from the current format to another specified format</summary>
					/// <param name="data">Decompressed data to convert</param>
					/// <param name="sourceFormat">Format to convert from</param>
					/// <param name="destinationFormat">Format to convert to</param>
					/// <param name="horizontalPacking">Row packing</param>
					/// <param name="verticalPacking">Row count to align to</param>
					/// <param name="sourceWidth">Indicates the source width of the image data</param>
					/// <param name="sourceHeight">Indicates the source height of the image data</param>
					/// <param name="decodedBitDepth">The bits per pixel once decoded</param>
					/// <returns>New byte data</returns>
					virtual MemoryStream^ ConvertData(MemoryStream^ data, Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums::PixelFormat sourceFormat, Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums::PixelFormat destinationFormat, Int32 horizontalPacking, Int32 verticalPacking, Int32 sourceWidth, Int32 sourceHeight, Int32 decodedBitDepth);
				#pragma endregion
				};
			}
		}
	}
}


#endif




#ifndef _Pixel_Format_Extender_
#define _Pixel_Format_Extender_


#include "LibAV Pixel Format.h"


using namespace System::Runtime::CompilerServices;
using namespace Bardez::Projects::MultiMedia::LibAV;
using namespace Bardez::Projects::Multimedia::MediaBase::Data::Pixels::Enums;


namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>Extension class for converting the PixelFormat enum to other destination pixel format enumerators</summary>
				[ExtensionAttribute]
				public ref class LibAVPixelFormatExtender abstract sealed
				{
				#pragma region 
				public:
					/// <summary>Converts the PixelFormat instance to a LibAV Pixel Format enumerator</summary>
					/// <param name="source">Source PixelFormat to translate</param>
					/// <returns>The translated pixel format, or LibAVPixelFormat.PIX_FMT_NONE if no translation available.</returns>
					[ExtensionAttribute]
					static LibAVPixelFormat ToLibAVPixelFormat(PixelFormat source);

					/// <summary>Converts the LibAV Pixel Format instance to a PixelFormat enumerator</summary>
					/// <param name="source">Source LibAVPixelFormat to translate</param>
					/// <returns>The translated pixel format, or LibAVPixelFormat.PIX_FMT_NONE if no translation available.</returns>
					[ExtensionAttribute]
					static PixelFormat ToPixelFormat(LibAVPixelFormat source);
				#pragma endregion
				};
			}
		}
	}
}

#endif


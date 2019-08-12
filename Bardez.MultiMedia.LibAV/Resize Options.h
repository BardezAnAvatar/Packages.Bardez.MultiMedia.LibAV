

#ifndef _Enums_ResizeMethod_H_
#define _Enums_ResizeMethod_H_

using namespace System;

namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>CLR-exposed enumerator describing resize methods</summary>
				[FlagsAttribute]
				public enum struct ResizeOptions : System::Int32
				{
					None			= 0x000,
					FastBilinear	= 0x001,
					Bilinear		= 0x002,
					Bicubic			= 0x004,
					X				= 0x008,
					PointTransform	= 0x010,
					AreaTransform	= 0x020,
					BicubicLinear	= 0x040,
					Guassian		= 0x080,
					Sinc			= 0x100,
					Lanczos			= 0x200,
					Spline			= 0x400,
				};
			}
		}
	}
}


#endif


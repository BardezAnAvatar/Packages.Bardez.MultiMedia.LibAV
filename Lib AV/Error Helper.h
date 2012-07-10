
#ifndef	_Error_Helper_H_
#define	_Error_Helper_H_

extern "C"
{
	#include "libavutil\error.h"
}

using namespace System;
using namespace System::Runtime::InteropServices;

namespace Bardez
{
	namespace Projects
	{
		namespace MultiMedia
		{
			namespace LibAV
			{
				/// <summary>Frame class derived from libavcodec's AVFrame, expecting RGB input (in the BGRA order) from the AVFrame's data</summary>
				private ref class ErrorHelper
				{
				#pragma region Constants
				public:
					/// <summary>Name of the container library</summary>
					literal System::String^ LibAvFormat = "libavformat";

					/// <summary>Name of the codec library</summary>
					literal System::String^ LibAvCodec = "libavcodec";

					/// <summary>Name of the scaling library</summary>
					literal System::String^ LibSwScale = "libswscale";
				#pragma endregion


				internal:
					/// <summary>Gets a descriptive string of an error number</summary>
					/// <param name="errorNo">Error number to look up</param>
					/// <param name="library">Name of the libav* library in which the error occurred</param>
					static String^ GetErrorCodeDescription(Int32 errorNo, String^ library)
					{
						char errorDescription[255];
						Int32 result = av_strerror(errorNo, errorDescription, 255);

						System::String^ errorString = "Error encountered from {0}: \"{1}\"";
						if (result < 0)
							errorString = "Could not find an error description in {0}: \"{1}\"";

						String^ error = Marshal::PtrToStringAnsi((IntPtr)(Byte*)errorDescription);
						return String::Format(errorString, library, error);
					}
				};
			}
		}
	}
}

#endif



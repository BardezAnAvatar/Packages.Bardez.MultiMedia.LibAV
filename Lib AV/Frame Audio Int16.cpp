

#include "Frame Audio Int16.h"

using namespace System;
using namespace Bardez::Projects::MultiMedia::LibAV;




#pragma region Helpers
/// <summary>Helper that copies data from the unmanaged pointers to managed set, based on the expected frame type</summary>
/// <param name="source">Source unmanaged frame to read from</param>
void FrameAudioInt16::CopyData(AVFrame* source)
{
	Int32 dataSize = this->CountSamples * this->CountChannels * 2;
	this->Data = gcnew array<Byte>(dataSize);

	System::Runtime::InteropServices::Marshal::Copy(this->Data, 0, IntPtr(source->data[0]), dataSize);
}
#pragma endregion
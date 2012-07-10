

#include "Frame BGRA.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace Bardez::Projects::MultiMedia::LibAV;


//#pragma region Helpers
///// <summary>Helper that copies data from the unmanaged pointers to managed set, based on the expected frame type</summary>
///// <param name="source">Source unmanaged picture to read from</param>
//void FrameBGRA::CopyData(AVPicture* source)
//{
//	Int32 dataSize = this->Height * (source->linesize[0] + source->linesize[1] + source->linesize[2] + source->linesize[3]);
//	this->Data = gcnew array<Byte>(dataSize);
//
//	Marshal::Copy(IntPtr(source->data[0]), this->Data, 0, dataSize);
//
//	//Int32 index = 0;
//	//for (Int32 row = 0; row < this->Height; ++row)
//	//{
//	//	index = row * source->linesize[0];
//
//	//	for (Int32 stride = 0; stride < source->linesize[0]; ++stride, ++index)
//	//	{
//	//		Int32 destIndex = (index * 4);
//
//	//		this->Data[destIndex    ] = source->data[0][index + stride];
//	//		this->Data[destIndex + 1] = source->data[1][index + stride];
//	//		this->Data[destIndex + 2] = source->data[2][index + stride];
//	//		this->Data[destIndex + 3] = source->data[3][index + stride];
//	//	}
//	//}
//}
//#pragma endregion




#ifndef LibAV_Filter_Graph_VFlip
#define LibAV_Filter_Graph_VFlip


#include "LibAV Filter Graph Buffer.h"


using namespace System;
using namespace Bardez::Projects::Multimedia::MediaBase::Frame::Image;


namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>Managed class wrapping a LibAV filter graph that performs a vertical flip</summary>
				//Cannot be internal in C++/CLI. Use Private instead. Weird.
				private ref class LibAVFilterGraphVFlip : LibAVFilterGraphBuffer
				{
				#pragma region Construction
				public:
					/// <summary>Default constructor</summary>
					/// <param name="metadata">Metadata to instantiate the source and sink AVFilterContexts with</param>
					LibAVFilterGraphVFlip(ImageMetadata^ metadata) : LibAVFilterGraphBuffer(metadata)
					{
						//configure the graph
						Int32 result_code = avfilter_graph_parse(this->Graph, "vflip", this->InputChain, this->OutputChain, NULL);
						if (result_code < 0)
							throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(result_code, "LibAVFilter"));

						result_code = avfilter_graph_config(this->Graph, NULL);
						if (result_code < 0)
							throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(result_code, "LibAVFilter"));
					}
				#pragma endregion
				};
			}
		}
	}
}


#endif


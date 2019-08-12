

#ifndef LibAV_Filter_Graph_Buffer
#define LibAV_Filter_Graph_Buffer


#include "LibAV Filter Graph.h"
#include "LibAV Pixel Format Extender.h"


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
				/// <summary>Managed class wrapping a source and sink LibAV filter graph</summary>
				private ref class LibAVFilterGraphBuffer abstract : LibAVFilterGraph
				{
				#pragma region Construction
				public:
					/// <summary>Default constructor</summary>
					/// <param name="metadata">Metadata to instantiate the source and sink Filter contexts with</param>
					LibAVFilterGraphBuffer(ImageMetadata^ metadata);

				protected:
					/// <summary>Builds the source and sink filter contexts</summary>
					/// <param name="metadata">Meta data to instantiate the source and sink Filter contexts with</param>
					void BuildSourceAndSinkFilterContexts(ImageMetadata^ metadata);
				#pragma endregion
				};
			}
		}
	}
}

#endif


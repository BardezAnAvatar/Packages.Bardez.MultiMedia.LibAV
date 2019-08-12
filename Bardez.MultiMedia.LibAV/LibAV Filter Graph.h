

#ifndef LibAV_Filter_Graph
#define LibAV_Filter_Graph


extern "C"
{
	#include "libavfilter\avfiltergraph.h"
	#include "libavcodec\avcodec.h"
}

#include "Error Helper.h"
#include "LibAV Picture.h"
#include "Library References.h"	//this is to link to LibAVFilter


using namespace System;
using namespace System::IO;
using namespace Bardez::Projects::Multimedia::MediaBase::Data::Pixels;
using namespace Bardez::Projects::Multimedia::MediaBase::Frame::Image;


namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>Managed class wrapping a LibAV filter graph, its source and destination filters</summary>
				/// <remarks>I think that this should be abstract and to let implementations say "source filter X" and "sink filter Y"</remarks>
				//Cannot be internal in C++/CLI. Use Private instead. Weird.
				private ref class LibAVFilterGraph abstract
				{
				#pragma region Static fields
				internal:
					/// <summary>Flag indicating whether or not the libavformat's codecs, etc. were registered</summary>
					static Boolean Initialized = false;
				#pragma endregion


				#pragma region Fields
				protected:
					/// <summary>Pointer to the graph object</summary>
					IntPtr graph;

					/// <summary>The inputs source filter</summary>
					IntPtr filterSource;

					/// <summary>The outputs destination filter</summary>
					IntPtr filterSink;

					/// <summary>The input source filter context</summary>
					IntPtr filterContextSource;

					/// <summary>The outputs destination filter context</summary>
					IntPtr filterContextSink;

					/// <summary>The input chain</summary>
					IntPtr inputs;

					/// <summary>The output chain</summary>
					IntPtr outputs;
				#pragma endregion


				#pragma region Properties
				public:
					/// <summary>Pointer to the graph object</summary>
					property AVFilterGraph* Graph
					{
						AVFilterGraph* get()
						{
							return reinterpret_cast<AVFilterGraph*>(this->graph.ToPointer());
						}
						void set(AVFilterGraph* value)
						{
							this->graph = IntPtr(value);
						}
					}

					/// <summary>The inputs source filter</summary>
					property AVFilter* FilterSource
					{
						AVFilter* get()
						{
							return reinterpret_cast<AVFilter*>(this->filterSource.ToPointer());
						}
						void set(AVFilter* value)
						{
							this->filterSource = IntPtr(value);
						}
					}

					/// <summary>The outputs destination filter</summary>
					property AVFilter* FilterSink
					{
						AVFilter* get()
						{
							return reinterpret_cast<AVFilter*>(this->filterSink.ToPointer());
						}
						void set(AVFilter* value)
						{
							this->filterSink = IntPtr(value);
						}
					}

					/// <summary>The input source filter context</summary>
					property AVFilterContext* FilterContextSource
					{
						AVFilterContext* get()
						{
							return reinterpret_cast<AVFilterContext*>(this->filterContextSource.ToPointer());
						}
						void set(AVFilterContext* value)
						{
							this->filterContextSource = IntPtr(value);
						}
					}

					/// <summary>The outputs destination filter context</summary>
					property AVFilterContext* FilterContextSink
					{
						AVFilterContext* get()
						{
							return reinterpret_cast<AVFilterContext*>(this->filterContextSink.ToPointer());
						}
						void set(AVFilterContext* value)
						{
							this->filterContextSink = IntPtr(value);
						}
					}

					/// <summary>The input chain</summary>
					property AVFilterInOut* InputChain
					{
						AVFilterInOut* get()
						{
							return reinterpret_cast<AVFilterInOut*>(this->inputs.ToPointer());
						}
						void set(AVFilterInOut* value)
						{
							this->inputs = IntPtr(value);
						}
					}

					/// <summary>The output chain</summary>
					property AVFilterInOut* OutputChain
					{
						AVFilterInOut* get()
						{
							return reinterpret_cast<AVFilterInOut*>(this->outputs.ToPointer());
						}
						void set(AVFilterInOut* value)
						{
							this->outputs = IntPtr(value);
						}
					}
				#pragma endregion


				#pragma region Construction
				public:
					/// <summary>Default constructor</summary>
					LibAVFilterGraph()
					{
						//initialize LibAVFilter
						LibAVFilterGraph::Initialize();

						//default pointer values
						this->Graph = NULL;
						this->FilterSource = NULL;
						this->FilterSink = NULL;
						this->FilterContextSource = NULL;
						this->FilterContextSink = NULL;
						this->InputChain = NULL;
						this->OutputChain = NULL;

						//allocate the basic structures
						this->Graph = avfilter_graph_alloc();
						this->InputChain = avfilter_inout_alloc();
						this->OutputChain = avfilter_inout_alloc();

						if (this->Graph == NULL)
							throw gcnew NullReferenceException("Unmanaged AVFilterGraph \"Graph\" is NULL.");
						else if (this->InputChain == NULL)
							throw gcnew NullReferenceException("Unmanaged AVFilterInOut \"InputChain\" is NULL.");
						else if (this->OutputChain == NULL)
							throw gcnew NullReferenceException("Unmanaged AVFilterInOut \"OutputChain\" is NULL.");
					}

					/// <summary>Initializes the LibAV filters</summary>
					static void Initialize()
					{
						if (!LibAVFilterGraph::Initialized)
						{
							avfilter_register_all();
							LibAVFilterGraph::Initialized = true;
						}
					}
				#pragma endregion


				#pragma region Destruction
				public:
					/// <summary>Destructor</summary>
					/// <remarks>Dispose()</remarks>
					~LibAVFilterGraph()
					{
						this->DisposeUnmanaged();
						GC::SuppressFinalize(this);
					}

					/// <summary>Destructor</summary>
					/// <remarks>Finalize()</remarks>
					!LibAVFilterGraph()
					{
						this->DisposeUnmanaged();
					}

				internal:
					/// <summary>Destructor logic, disposes the object</summary>
					void DisposeUnmanaged()
					{
						if (this->Graph != NULL)
						{
							AVFilterGraph* graph = this->Graph;
							avfilter_graph_free(&graph);
							this->Graph = graph;	//should be NULL
						}

						if (this->InputChain != NULL)
						{
							AVFilterInOut* chain = this->InputChain;
							avfilter_inout_free(&chain);
							this->InputChain = chain;	//should be NULL
						}

						if (this->OutputChain != NULL)
						{
							AVFilterInOut* chain = this->OutputChain;
							avfilter_inout_free(&chain);
							this->OutputChain = chain;	//should be NULL
						}

						if (this->FilterContextSource != NULL)
						{
							AVFilterContext* context = this->FilterContextSource;
							avfilter_free(context);
							this->FilterContextSource = context;	//should be NULL
						}

						if (this->FilterContextSink != NULL)
						{
							AVFilterContext* context = this->FilterContextSink;
							avfilter_free(context);
							this->FilterContextSink = context;	//should be NULL
						}
						
						this->FilterSource = NULL;
						this->FilterSink = NULL;
					}
				#pragma endregion


				#pragma region Methods
				public:
					/// <summary>Pushes a new picture into the source filter</summary>
					/// <param name="source">Source frame to push onto the filter queue</param>
					void PushLibAVPicture(LibAVPicture^ source);

					/// <summary>Pulls a frame from the filter stack</summary>
					/// <returns>The next frame in the filter queue</returns>
					LibAVPicture^ PullLibAVPicture();
				#pragma endregion


				#pragma region Helper Methods
				protected:
					/// <summary>Builds an AVFrame from the AVPicture source</summary>
					/// <param name="source">Reference handle to the managed LibAVPicture instance</param>
					/// <returns>An unmanaged instance of a frame referencing the same data</returns>
					AVFrame* GetAVFrame(LibAVPicture^ source);

					/// <summary>Builds a LibAVPicture reference from the source AVFilterBufferRef instance</summary>
					/// <param name="picref">Buffer to copy data from</param>
					/// <returns>The built LibAVPicture instance</returns>
					LibAVPicture^ BuildPictureFromBuffer(AVFilterBufferRef* picref);
				#pragma endregion
				};
			}
		}
	}
}

#endif


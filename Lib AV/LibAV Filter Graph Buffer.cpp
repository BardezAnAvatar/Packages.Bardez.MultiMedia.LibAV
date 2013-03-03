

extern "C"
{
	#include "libavfilter\avfilter.h"
}
#include "LibAV Filter Graph Buffer.h"


#pragma region Construction
/// <summary>Default constructor</summary>
/// <param name="metadata">Metadata to instantiate the source and sink Filter contexts with</param>
LibAVFilterGraphBuffer::LibAVFilterGraphBuffer(ImageMetadata^ metadata) : LibAVFilterGraph()
{
	this->FilterSource = avfilter_get_by_name("buffer");
	this->FilterSink = avfilter_get_by_name("buffersink");

	if (this->FilterSource == NULL)
		throw gcnew NullReferenceException("Unmanaged AVFilter \"filter_source\" is NULL.");
	else if (this->FilterSink == NULL)
		throw gcnew NullReferenceException("Unmanaged AVFilter \"filter_sink\" is NULL.");

	//build out the filter contexts for the source and the sink
	this->BuildSourceAndSinkFilterContexts(metadata);
						
	//set up the outputs and inputs
	AVFilterInOut* inputs = this->InputChain;
	AVFilterInOut* outputs = this->OutputChain;
						
	outputs->name       = av_strdup("in");
	outputs->filter_ctx = this->FilterContextSource;
	outputs->pad_idx    = 0;
	outputs->next       = NULL;

	inputs->name		= av_strdup("out");
	inputs->filter_ctx	= this->FilterContextSink;
	inputs->pad_idx		= 0;
	inputs->next		= NULL;
}

/// <summary>Builds the source and sink filter contexts</summary>
/// <param name="metadata">Meta data to instantiate the source and sink Filter contexts with</param>
void LibAVFilterGraphBuffer::BuildSourceAndSinkFilterContexts(ImageMetadata^ metadata)
{
	/*
	*	Create the arguments needed for the input filter's context.
	*
	*	These arguments are in the following format:
	*	"A:B:C:D:E:F:G"
	*	Where
	*		A = Width
	*		B = height
	*		C = pixel format
	*		D = Time base's numerator
	*		E = Time base's denominator
	*		F = Aspect ratio's numerator
	*		G = Aspect ratio's denominator
	*
	*	Example (for 640x480 24fps film 1:1 pixels): "640:480:yuv410p:1:24:1:1"
	*/
	LibAVPixelFormat pixelFormat = LibAVPixelFormatExtender::ToLibAVPixelFormat(metadata->Format);
	enum PixelFormat libavFormat = (enum PixelFormat)(pixelFormat);
	
	//C++/CLI does not support the "params" keyword so overloads don't match
	array<Object^>^ parameters = gcnew array<Object^> { metadata->Width, metadata->Height, pixelFormat, 1, 1, metadata->AspectRatio->Numerator, metadata->AspectRatio->Denominator };
	String^ args = String::Format("{0}:{1}:{2}:{3}:{4}:{5}:{6}", parameters);
	IntPtr argsP = Marshal::StringToHGlobalAnsi(args);	//convert the args into an unmanged string

	try
	{
		char* unmanagedArgs = reinterpret_cast<char*>(argsP.ToPointer());
							
		AVFilterContext* source = NULL;
		AVFilterContext* sink = NULL;

		Int32 result_code = avfilter_graph_create_filter(&source, this->FilterSource, "in", unmanagedArgs, NULL, this->Graph);
		if (result_code < 0)
			throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(result_code, "LibAVFilter"));
	
		//create an array of appropriate formats
		enum PixelFormat supportedFormats[] = { libavFormat, PIX_FMT_NONE };
		result_code = avfilter_graph_create_filter(&sink, this->FilterSink, "out", NULL, supportedFormats, this->Graph);
		if (result_code < 0)
			throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(result_code, "LibAVFilter"));

		//error check
		if (source == NULL)
			throw gcnew NullReferenceException("Unmanaged AVFilterContext \"source\" is NULL.");
		else if (sink == NULL)
			throw gcnew NullReferenceException("Unmanaged AVFilterContext \"sink\" is NULL.");

		//assign the pointers to the local members
		this->FilterContextSource = source;
		this->FilterContextSink = sink;
		source = NULL;
		sink = NULL;
	}
	finally	//free up the unamanged string memory
	{
		Marshal::FreeHGlobal(argsP); //clean up that arguments string
	}
}
#pragma endregion


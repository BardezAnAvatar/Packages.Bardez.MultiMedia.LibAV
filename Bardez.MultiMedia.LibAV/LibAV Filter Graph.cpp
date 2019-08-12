

extern "C"
{
	#include "libavfilter\vsrc_buffer.h"
}
#include "LibAV Filter Graph.h"


using namespace Bardez::Projects::Multimedia::LibAV;


#pragma region Methods
/// <summary>Pushes a new frame into the source filter</summary>
/// <param name="source">Source frame to push onto the filter queue</param>
void LibAVFilterGraph::PushLibAVPicture(LibAVPicture^ source)
{
	//get an AVFrame from the LibAVPicture, somehow
	AVFrame* pFrame = NULL;

	//define the presentation timestamp
	Int64 presentationTimeStamp = 1L;	//arbitrary

	//source buffer context, pointer to data, presentation timestamp, aspect ratio
	AVRational aspectRatio = { 1, 1 };
	Int32 result = av_vsrc_buffer_add_frame(this->FilterContextSource, pFrame, presentationTimeStamp, aspectRatio);

	//regardless of the result, free pFrame
    av_free(pFrame);

	//check for errored result
	if (result < 0)
		throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(result, ErrorHelper::LibAvFilter));
}

/// <summary>Pulls a frame from the filter stack</summary>
/// <returns>The next frame in the filter queue</returns>
LibAVPicture^ LibAVFilterGraph::PullLibAVPicture()
{
	//check to see if a frame is available
    Int32 result = avfilter_poll_frame(this->FilterContextSink->inputs[0]);
    if (result < 0)
		throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(result, ErrorHelper::LibAvFilter));
	else if (result == 0)
		throw gcnew ApplicationException(String::Format("Encountered error in {0}: no frames available for Filter Sink output", ErrorHelper::LibAvFilter));

	//request the frame to be processed
	result = avfilter_request_frame(this->FilterContextSink->inputs[0]);
    if (result < 0)
		throw gcnew ApplicationException(ErrorHelper::GetErrorCodeDescription(result, ErrorHelper::LibAvFilter));

	//get the buffer reference
    AVFilterBufferRef* picref = this->FilterContextSink->inputs[0]->cur_buf;
	if (picref == NULL)
		throw gcnew ApplicationException(String::Format("Encountered error in {0}: Filter Sink output has a NULL buffer", ErrorHelper::LibAvFilter));
	
	//copy the image
	LibAVPicture^ outputImage = this->BuildPictureFromBuffer(picref);

	//I will leave this here, but in source code and this code, there is no accompanying avfilter_ref_buffer call,
	//	which makes me to wonder whether this call should be here.
	avfilter_unref_buffer(picref);

	return outputImage;
}
#pragma endregion


#pragma region Helper Methods
/// <summary>Builds an AVFrame from the AVPicture source</summary>
/// <param name="source">Reference handle to the managed LibAVPicture instance</param>
/// <returns>An unmanaged instance of a frame referencing the same data</returns>
AVFrame* LibAVFilterGraph::GetAVFrame(LibAVPicture^ source)
{
	AVFrame* frame = avcodec_alloc_frame();
	AVPicture* picture = source->PicturePtr;

	//copy data
	for (Int32 index = 0; index < AV_NUM_DATA_POINTERS; ++index)
	{
		frame->data[index] = picture->data[index];
		frame->linesize[index] = picture->linesize[index];
	}

	return frame;
}

/// <summary>Builds a LibAVPicture reference from the source AVFilterBufferRef instance</summary>
/// <param name="picref">Buffer to copy data from</param>
/// <returns>The built LibAVPicture instance</returns>
LibAVPicture^ LibAVFilterGraph::BuildPictureFromBuffer(AVFilterBufferRef* picref)
{
	Int32 height = picref->video->h;
	Int32 width = picref->video->w;
	LibAVPixelFormat codecPixelFormat = static_cast<LibAVPixelFormat>(picref->format);

	AVPicture* picture = new AVPicture;

	//copy data
	for (Int32 index = 0; index < AV_NUM_DATA_POINTERS; ++index)
	{
		picture->data[index] = picref->data[index];
		picture->linesize[index] = picref->linesize[index];
	}
	
	//	when constructing the LibAVPicture to extract data, tell it to dispose memory when finished.
	LibAVPicture^ managedPicture = gcnew LibAVPicture(picture, LibAVPictureDetail::Build(width, height, codecPixelFormat), true, true);

	delete picture;

	return managedPicture;
}
#pragma endregion


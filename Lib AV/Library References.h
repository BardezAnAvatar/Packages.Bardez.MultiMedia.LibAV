

#ifndef _LibraryReferences_H_
#define	_LibraryReferences_H_

//used to force the linker to link to the correct library for C extern calls
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "avfilter.lib")

#endif


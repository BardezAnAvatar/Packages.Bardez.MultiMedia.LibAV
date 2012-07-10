

#ifndef _Enums_PixelFormat_H_
#define _Enums_PixelFormat_H_

namespace Bardez
{
	namespace Projects
	{
		namespace MultiMedia
		{
			namespace LibAV
			{
				/// <summary>CLR-exposed enumerator describing a codec's output Pixel Format</summary>
				public enum struct LibAVPixelFormat : System::Int32
				{
					/// <summary>None</summary>
					PIX_FMT_NONE = -1,

					/// <summary>planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)</summary>
					PIX_FMT_YUV420P,

					/// <summary>packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr</summary>
					PIX_FMT_YUYV422,

					/// <summary>packed RGB 8:8:8, 24bpp, RGBRGB...</summary>
					PIX_FMT_RGB24,

					/// <summary>packed RGB 8:8:8, 24bpp, BGRBGR...</summary>
					PIX_FMT_BGR24,

					/// <summary>planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)</summary>
					PIX_FMT_YUV422P,

					/// <summary>planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples)</summary>
					PIX_FMT_YUV444P,

					/// <summary>planar YUV 4:1:0,  9bpp, (1 Cr & Cb sample per 4x4 Y samples)</summary>
					PIX_FMT_YUV410P,

					/// <summary>planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples)</summary>
					PIX_FMT_YUV411P,

					/// <summary>Y        ,  8bpp</summary>
					PIX_FMT_GRAY8,

					/// <summary>Y        ,  1bpp, 0 is white, 1 is black, in each byte pixels are ordered from the msb to the lsb</summary>
					PIX_FMT_MONOWHITE,

					/// <summary>Y        ,  1bpp, 0 is black, 1 is white, in each byte pixels are ordered from the msb to the lsb</summary>
					PIX_FMT_MONOBLACK,

					/// <summary>8 bit with PIX_FMT_RGB32 palette</summary>
					PIX_FMT_PAL8,

					/// <summary>planar YUV 4:2:0, 12bpp, full scale (JPEG), deprecated in favor of PIX_FMT_YUV420P and setting color_range</summary>
					PIX_FMT_YUVJ420P,

					/// <summary>planar YUV 4:2:2, 16bpp, full scale (JPEG), deprecated in favor of PIX_FMT_YUV422P and setting color_range</summary>
					PIX_FMT_YUVJ422P,

					/// <summary>planar YUV 4:4:4, 24bpp, full scale (JPEG), deprecated in favor of PIX_FMT_YUV444P and setting color_range</summary>
					PIX_FMT_YUVJ444P,

					/// <summary>XVideo Motion Acceleration via common packet passing</summary>
					PIX_FMT_XVMC_MPEG2_MC,

					/// <remarks>The only one with no comment?</remarks>
					PIX_FMT_XVMC_MPEG2_IDCT,

					/// <summary>packed YUV 4:2:2, 16bpp, Cb Y0 Cr Y1</summary>
					PIX_FMT_UYVY422,

					/// <summary>packed YUV 4:1:1, 12bpp, Cb Y0 Y1 Cr Y2 Y3</summary>
					PIX_FMT_UYYVYY411,

					/// <summary>packed RGB 3:3:2,  8bpp, (msb)2B 3G 3R(lsb)</summary>
					PIX_FMT_BGR8,

					/// <summary>packed RGB 1:2:1 bitstream,  4bpp, (msb)1B 2G 1R(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits</summary>
					PIX_FMT_BGR4,

					/// <summary>packed RGB 1:2:1,  8bpp, (msb)1B 2G 1R(lsb)</summary>
					PIX_FMT_BGR4_BYTE,

					/// <summary>packed RGB 3:3:2,  8bpp, (msb)2R 3G 3B(lsb)</summary>
					PIX_FMT_RGB8,

					/// <summary>packed RGB 1:2:1 bitstream,  4bpp, (msb)1R 2G 1B(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits</summary>
					PIX_FMT_RGB4,

					/// <summary>packed RGB 1:2:1,  8bpp, (msb)1R 2G 1B(lsb)</summary>
					PIX_FMT_RGB4_BYTE,

					/// <summary>planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 plane for the UV components, which are interleaved (first byte U and the following byte V)</summary>
					PIX_FMT_NV12,

					/// <summary>as above, but U and V bytes are swapped</summary>
					PIX_FMT_NV21,

					/// <summary>packed ARGB 8:8:8:8, 32bpp, ARGBARGB...</summary>
					PIX_FMT_ARGB,

					/// <summary>packed RGBA 8:8:8:8, 32bpp, RGBARGBA...</summary>
					PIX_FMT_RGBA,

					/// <summary>packed ABGR 8:8:8:8, 32bpp, ABGRABGR...</summary>
					PIX_FMT_ABGR,

					/// <summary>packed BGRA 8:8:8:8, 32bpp, BGRABGRA...</summary>
					PIX_FMT_BGRA,

					/// <summary>Y        , 16bpp, big-endian</summary>
					PIX_FMT_GRAY16BE,

					/// <summary>Y        , 16bpp, little-endian</summary>
					PIX_FMT_GRAY16LE,

					/// <summary>planar YUV 4:4:0 (1 Cr & Cb sample per 1x2 Y samples)</summary>
					PIX_FMT_YUV440P,

					/// <summary>planar YUV 4:4:0 full scale (JPEG), deprecated in favor of PIX_FMT_YUV440P and setting color_range</summary>
					PIX_FMT_YUVJ440P,

					/// <summary>planar YUV 4:2:0, 20bpp, (1 Cr & Cb sample per 2x2 Y & A samples)</summary>
					PIX_FMT_YUVA420P,

					/// <summary>H.264 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers</summary>
					PIX_FMT_VDPAU_H264,

					/// <summary>MPEG-1 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers</summary>
					PIX_FMT_VDPAU_MPEG1,

					/// <summary>MPEG-2 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers</summary>
					PIX_FMT_VDPAU_MPEG2,

					/// <summary>WMV3 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers</summary>
					PIX_FMT_VDPAU_WMV3,

					/// <summary>VC-1 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers</summary>
					PIX_FMT_VDPAU_VC1,

					/// <summary>packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the 2-byte value for each R/G/B component is stored as big-endian</summary>
					PIX_FMT_RGB48BE,

					/// <summary>packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the 2-byte value for each R/G/B component is stored as little-endian</summary>
					PIX_FMT_RGB48LE,

					/// <summary>packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), big-endian</summary>
					PIX_FMT_RGB565BE,

					/// <summary>packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), little-endian</summary>
					PIX_FMT_RGB565LE,

					/// <summary>packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G 5B(lsb), big-endian, most significant bit to 0</summary>
					PIX_FMT_RGB555BE,

					/// <summary>packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G 5B(lsb), little-endian, most significant bit to 0</summary>
					PIX_FMT_RGB555LE,

					/// <summary>packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), big-endian</summary>
					PIX_FMT_BGR565BE,

					/// <summary>packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), little-endian</summary>
					PIX_FMT_BGR565LE,

					/// <summary>packed BGR 5:5:5, 16bpp, (msb)1A 5B 5G 5R(lsb), big-endian, most significant bit to 1</summary>
					PIX_FMT_BGR555BE,

					/// <summary>packed BGR 5:5:5, 16bpp, (msb)1A 5B 5G 5R(lsb), little-endian, most significant bit to 1</summary>
					PIX_FMT_BGR555LE,

					/// <summary>HW acceleration through VA API at motion compensation entry-point, Picture.data[3] contains a vaapi_render_state struct which contains macroblocks as well as various fields extracted from headers</summary>
					PIX_FMT_VAAPI_MOCO,

					/// <summary>HW acceleration through VA API at IDCT entry-point, Picture.data[3] contains a vaapi_render_state struct which contains fields extracted from headers</summary>
					PIX_FMT_VAAPI_IDCT,

					/// <summary>HW decoding through VA API, Picture.data[3] contains a vaapi_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers</summary>
					PIX_FMT_VAAPI_VLD,

					/// <summary>planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian</summary>
					PIX_FMT_YUV420P16LE,

					/// <summary>planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian</summary>
					PIX_FMT_YUV420P16BE,

					/// <summary>planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian</summary>
					PIX_FMT_YUV422P16LE,

					/// <summary>planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian</summary>
					PIX_FMT_YUV422P16BE,

					/// <summary>planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian</summary>
					PIX_FMT_YUV444P16LE,

					/// <summary>planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian</summary>
					PIX_FMT_YUV444P16BE,

					/// <summary>MPEG4 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers</summary>
					PIX_FMT_VDPAU_MPEG4,

					/// <summary>HW decoding through DXVA2, Picture.data[3] contains a LPDIRECT3DSURFACE9 pointer</summary>
					PIX_FMT_DXVA2_VLD,

					/// <summary>packed RGB 4:4:4, 16bpp, (msb)4A 4R 4G 4B(lsb), little-endian, most significant bits to 0</summary>
					PIX_FMT_RGB444LE,

					/// <summary>packed RGB 4:4:4, 16bpp, (msb)4A 4R 4G 4B(lsb), big-endian, most significant bits to 0</summary>
					PIX_FMT_RGB444BE,

					/// <summary>packed BGR 4:4:4, 16bpp, (msb)4A 4B 4G 4R(lsb), little-endian, most significant bits to 1</summary>
					PIX_FMT_BGR444LE,

					/// <summary>packed BGR 4:4:4, 16bpp, (msb)4A 4B 4G 4R(lsb), big-endian, most significant bits to 1</summary>
					PIX_FMT_BGR444BE,

					/// <summary>8bit gray, 8bit alpha</summary>
					PIX_FMT_GRAY8A,

					/// <summary>packed RGB 16:16:16, 48bpp, 16B, 16G, 16R, the 2-byte value for each R/G/B component is stored as big-endian</summary>
					PIX_FMT_BGR48BE,

					/// <summary>packed RGB 16:16:16, 48bpp, 16B, 16G, 16R, the 2-byte value for each R/G/B component is stored as little-endian</summary>
					PIX_FMT_BGR48LE,


					//the following 10 formats have the disadvantage of needing 1 format for each bit depth, thus
					//If you want to support multiple bit depths, then using PIX_FMT_YUV420P16* with the bpp stored seperately
					//is better

					/// <summary>planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian</summary>
					PIX_FMT_YUV420P9BE,

					/// <summary>planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian</summary>
					PIX_FMT_YUV420P9LE,

					/// <summary>planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian</summary>
					PIX_FMT_YUV420P10BE,

					/// <summary>planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian</summary>
					PIX_FMT_YUV420P10LE,

					/// <summary>planar YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian</summary>
					PIX_FMT_YUV422P10BE,

					/// <summary>planar YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian</summary>
					PIX_FMT_YUV422P10LE,

					/// <summary>planar YUV 4:4:4, 27bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian</summary>
					PIX_FMT_YUV444P9BE,

					/// <summary>planar YUV 4:4:4, 27bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian</summary>
					PIX_FMT_YUV444P9LE,

					/// <summary>planar YUV 4:4:4, 30bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian</summary>
					PIX_FMT_YUV444P10BE,

					/// <summary>planar YUV 4:4:4, 30bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian</summary>
					PIX_FMT_YUV444P10LE,

					/// <summary>planar YUV 4:2:2, 18bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian</summary>
					PIX_FMT_YUV422P9BE,

					/// <summary>planar YUV 4:2:2, 18bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian</summary>
					PIX_FMT_YUV422P9LE,


					/// <summary>hardware decoding through VDA</summary>
					PIX_FMT_VDA_VLD,

					/// <summary>planar GBR 4:4:4 24bpp</summary>
					PIX_FMT_GBRP,

					/// <summary>planar GBR 4:4:4 27bpp, big endian</summary>
					PIX_FMT_GBRP9BE,

					/// <summary>planar GBR 4:4:4 27bpp, little endian</summary>
					PIX_FMT_GBRP9LE,

					/// <summary>planar GBR 4:4:4 30bpp, big endian</summary>
					PIX_FMT_GBRP10BE,

					/// <summary>planar GBR 4:4:4 30bpp, little endian</summary>
					PIX_FMT_GBRP10LE,

					/// <summary>planar GBR 4:4:4 48bpp, big endian</summary>
					PIX_FMT_GBRP16BE,

					/// <summary>planar GBR 4:4:4 48bpp, little endian</summary>
					PIX_FMT_GBRP16LE,

					/// <summary>packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian</summary>
					PIX_FMT_RGBA64BE=0x123,

					/// <summary>packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian</summary>
					PIX_FMT_RGBA64LE,

					/// <summary>packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian</summary>
					PIX_FMT_BGRA64BE,

					/// <summary>packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian</summary>
					PIX_FMT_BGRA64LE,

					/// <summary>packed RGB 8:8:8, 32bpp, 0RGB0RGB...</summary>
					PIX_FMT_0RGB=0x123+4,

					/// <summary>packed RGB 8:8:8, 32bpp, RGB0RGB0...</summary>
					PIX_FMT_RGB0,

					/// <summary>packed BGR 8:8:8, 32bpp, 0BGR0BGR...</summary>
					PIX_FMT_0BGR,

					/// <summary>packed BGR 8:8:8, 32bpp, BGR0BGR0...</summary>
					PIX_FMT_BGR0,

					/// <summary>planar YUV 4:4:4 32bpp, (1 Cr & Cb sample per 1x1 Y & A samples)</summary>
					PIX_FMT_YUVA444P,
				};
			}
		}
	}
}


#endif


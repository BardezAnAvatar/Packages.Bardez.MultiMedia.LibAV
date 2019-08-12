

#ifndef _ENUMS_MediaType_
#define _ENUMS_MediaType_

namespace Bardez
{
	namespace Projects
	{
		namespace Multimedia
		{
			namespace LibAV
			{
				/// <summary>CLR-exposed enumerator describing a the type of media a codec outputs</summary>
				public enum struct MediaType : System::Int32
				{
					/// <summary>Usually treated as AVMEDIA_TYPE_DATA</summary>
					AVMEDIA_TYPE_UNKNOWN = -1,
					AVMEDIA_TYPE_VIDEO,
					AVMEDIA_TYPE_AUDIO,

					/// <summary>Opaque data information usually continuous</summary>
					AVMEDIA_TYPE_DATA,
					AVMEDIA_TYPE_SUBTITLE,

					/// <summary>Opaque data information usually sparse</summary>
					AVMEDIA_TYPE_ATTACHMENT,
					AVMEDIA_TYPE_NB
				};
			}
		}
	}
}


#endif


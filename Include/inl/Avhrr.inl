#ifndef __INCLUDED_AGMRS_AVHRR_INL__
#define __INCLUDED_AGMRS_AVHRR_INL__

namespace Agmrs { namespace Avhrr
{
//---------------------------------------------------------
}}// namespace Agmrs::Avhrr - end
//---------------------------------------------------------
using namespace Agmrs::Avhrr;

namespace Agmrs { namespace Avhrr
{

//----------------------------------------------------------
// AVHRR 常量定义 - Beg
//--------------------
//#define AGMRS_AVHRR_DATA_MARKER				AGMRS_LOCAL_DATA_MARKER
#define AGMRS_AVHRR_DATA_CHANNEL_NUM_MAX	20						// AVHRR 数据通道数最大值
#define AGMRS_AVHRR_DATA_CHANNEL_NUM		10						// AVHRR 数据通道数
#define AGMRS_AVHRR_DATA_VALUE_MAX			0x0FFF					// AVHRR 通道数据最大值

//--------------------
// AVHRR 常量定义 - End
//----------------------------------------------------------

#pragma pack(1)

	struct AvhrrChannel
	{
		enum
		{
			Red = 1,	// 0.645um - 红外通道
			Nir = 2,	// 0.859um - 近红外通道
			T3930nm = 4,	// 3930nm	- 3930nm通道(3b)
			T11um = 5,	// 11.00um - 热红外通道
			T12um = 6,	// 12.00um - 热红外通道
		};
	};

#pragma pack()

//---------------------------------------------------------
}}// namespace Agmrs::Avhrr - end
//---------------------------------------------------------

#endif	//#ifndef __INCLUDED_AGMRS_AVHRR_INL__

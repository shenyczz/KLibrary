#ifndef __INCLUDED_AGMRS_VIRR_INL__
#define __INCLUDED_AGMRS_VIRR_INL__

namespace Agmrs { namespace Virr
{
//---------------------------------------------------------
}}// namespace Agmrs::Virr - end
//---------------------------------------------------------
using namespace Agmrs::Virr;

namespace Agmrs { namespace Virr
{

//----------------------------------------------------------
// VIRR 常量定义 - Beg
//--------------------
//#define AGMRS_VIRR_DATA_MARKER				AGMRS_LOCAL_DATA_MARKER
#define AGMRS_VIRR_DATA_CHANNEL_NUM_MAX		20						// VIRR 数据通道数最大值
#define AGMRS_VIRR_DATA_CHANNEL_NUMBERS		10						// VIRR 数据通道数
#define AGMRS_VIRR_DATA_VALUE_MAX			0x0FFF					// VIRR 通道数据最大值

//--------------------
// AVHRR 常量定义 - End
//----------------------------------------------------------

#pragma pack(1)

	struct VirrChannel
	{
		enum
		{
			Red = 1,	// 0.645um - 红外通道
			Nir = 2,	// 0.859um - 近红外通道
			Blu = 7,	// 0.469um - 蓝光通道
			Grn = 9,	// 0.555um - 绿光通道
			T3930nm = 3,	// 3930nm	- 3930nm通道
			T11um = 4,	// 11.00um - 热红外通道
			T12um = 5,	// 12.00um - 热红外通道
		};
	};

#pragma pack()

//---------------------------------------------------------
}}// namespace Agmrs::Virr - end
//---------------------------------------------------------

#endif	//#ifndef __INCLUDED_AGMRS_VIRR_INL__

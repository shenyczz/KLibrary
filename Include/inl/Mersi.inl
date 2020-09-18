#ifndef __INCLUDED_AGMRS_MERSI_INL__
#define __INCLUDED_AGMRS_MERSI_INL__

namespace Agmrs { namespace Mersi
{
//---------------------------------------------------------
}}// namespace Agmrs::Mersi - end
//---------------------------------------------------------
using namespace Agmrs::Mersi;

namespace Agmrs { namespace Mersi
{

//----------------------------------------------------------
// MERSI 常量定义 - Beg
//--------------------
//#define AGMRS_MERSI_DATA_MARKER					AGMRS_LOCAL_DATA_MARKER
#define AGMRS_MERSI_DATA_CHANNEL_NUM_MAX		20						// MERSI数据通道数最大值
#define AGMRS_MERSI_DATA_CHANNEL_NUMBERS		20						// MERSI数据通道数
#define AGMRS_MERSI_DATA_VALUE_MAX				0x0FFF					// MERSI通道数据最大值

//--------------------
// MERSI 常量定义 - End
//----------------------------------------------------------

#pragma pack(1)

	struct MersiChannel
	{
		enum
		{
			Red = 3,		// 0.645um - 红外通道
			Nir = 4,		// 0.859um - 近红外通道
			Blu = 1,		// 0.469um - 蓝光通道
			Grn = 2,		// 0.555um - 绿光通道
			C940nm = 16,	// 940nm - 940um通道
			T11um = 5,		// 11.00um - 热红外通道
		};
	};

#pragma pack()

//---------------------------------------------------------
}}// namespace Agmrs::Mersi - end
//---------------------------------------------------------

#endif	//#ifndef __INCLUDED_AGMRS_MERSI_INL__

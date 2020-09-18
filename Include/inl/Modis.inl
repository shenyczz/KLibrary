#ifndef __INCLUDED_AGMRS_MODIS_INL__
#define __INCLUDED_AGMRS_MODIS_INL__

namespace Agmrs { namespace Modis
{
//---------------------------------------------------------
}}// namespace Agmrs::Modis - end
//---------------------------------------------------------
using namespace Agmrs::Modis;

namespace Agmrs { namespace Modis
{

//----------------------------------------------------------
// MODIS常量定义 - Beg
//--------------------
#define AGMRS_MODIS_DATA_VALUE_MAX				0x0FFF						// MODIS通道数据最大值
#define AGMRS_MODIS_DATA_INVALID				0x0FFF						// MODIS通道数据无效值
#define AGMRS_MODIS_DATA_INVALID2				0x0FFE						// MODIS通道数据无效值2

#define AGMRS_MODIS_DATA_CHANNEL_NUM_MAX		40							// MODIS数据通道数最大值
#define AGMRS_MODIS_DATA_CHANNEL_NUM			36							// MODIS数据通道数
//--------------------
// MODIS常量定义 - End
//----------------------------------------------------------

#pragma pack(1)

	struct ModisChannel
	{
		enum
		{
			Red = 1,	// 0.645um - 红外通道
			Nir = 2,	// 0.859um - 近红外通道
			Blu = 3,	// 0.469um - 蓝光通道
			Grn = 4,	// 0.555um - 绿光通道
			C940nm = 19,	// 940nm	- 940nm通道
			T3930nm = 21,	// 3930nm	- 3930nm通道
			T11um = 31,	// 11.00um - 热红外通道
			T12um = 32,	// 12.00um - 热红外通道
		};
	};

#pragma pack()

//---------------------------------------------------------
}}// namespace Agmrs::Modis - end
//---------------------------------------------------------

#endif	//#ifndef __INCLUDED_AGMRS_MODIS_INL__

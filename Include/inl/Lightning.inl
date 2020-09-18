#ifndef __INCLUDED_LIGHTNING_INL__
#define __INCLUDED_LIGHTNING_INL__

// 雷电
namespace Lightning
{
//---------------------------------------------------------
}// namespace Lightning - end
//---------------------------------------------------------
using namespace Lightning;

#define LIGHTNING_PRODUCT_CODE_BEG					5000		// 雷电产品代码 - BEG

#define LIGHTNING_PRODUCT_CODE_MONITOR_RECORD		5001		// 雷电监测记录数据(每雷电一个记录)
#define LIGHTNING_PRODUCT_CODE_RECORD_COUNT			5002		// 雷电记录统计数据(雷电数量)

#define LIGHTNING_PRODUCT_CODE_POTENTIAL_FORECAST	5010		// 雷电潜势预报(概率)
#define LIGHTNING_PRODUCT_CODE_NOWCAST				5011		// 雷电临近预报

#define LIGHTNING_PRODUCT_CODE_END					5999		// 雷电产品代码 - END

#pragma pack(1)
#pragma pack()

#endif	//#ifndef __INCLUDED_LIGHTNING_INL__

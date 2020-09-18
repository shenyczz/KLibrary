#ifndef __INCLUDED_RADAR_INL__
#define __INCLUDED_RADAR_INL__

namespace Radar {}
using namespace Radar;

namespace Radar
{

	// À×´ïÐÍºÅ
	enum RadarType
	{
		eRadarType_None = 0,
		eRadarType_Wsr98d_Sa,			// 1
		eRadarType_Wsr98d_Sb,			// 2
		eRadarType_Wsr98d_Cb,			// 3
	};

	enum RadarDataOperatorType
	{
		eRadarDataOperator_Auto = 0,
		eRadarDataOperator_Reader,
		eRadarDataOperator_Writer,
		eRadarDataOperator_Render,
	};

//---------------------------------------------------------
}// namespace Radar - end
//---------------------------------------------------------


#pragma pack(1)
#pragma pack()

#include ".\Radar\Wsr98d.inl"



#endif	//#ifndef __INCLUDED_RADAR_INL__

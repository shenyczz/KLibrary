#include "stdafx.h"
#include "KAgmrsThreshold.h"

namespace Agmrs
{

#pragma region KAgmrsThreshold

	KAgmrsThreshold::KAgmrsThreshold()
	{
		Init();
	}

	KAgmrsThreshold::~KAgmrsThreshold()
	{
	}

	void KAgmrsThreshold::Init()
	{
		InitThreshold_Cloud();
		InitThreshold_Water();
		InitThreshold_Fire();
		InitThreshold_Snow();
	}

	//初始化云体判别阈值
	void KAgmrsThreshold::InitThreshold_Cloud()
	{
		dCloudRed = 20;
		dCloudT11um = 273;
		dCloudNirRed = 0;
		dCloudNirRedMax = 2;

		dNightCloudT12umLo = 265;
		dNightCloudT12umHi = 289;
	}

	//初始化水体判别阈值
	void KAgmrsThreshold::InitThreshold_Water()
	{
		dWaterRed = 15;
		dWaterNir = 12;
		dWaterNirRed = 0;
		dWaterNirRedMin = -2;
	}

	//初始化陆地判别阈值
	void KAgmrsThreshold::InitThreshold_Land()
	{
		dLandRed = 20;
		dLandNir = 15;
		dLandNirRed = 0;
	}

	//初始化火点判别阈值
	void KAgmrsThreshold::InitThreshold_Fire()
	{
		dFiringRed = -1;
		dFiringNir = -1;
		dFiringT3930nm = -1;
		dFiringT3930nm_T11um = -1;
		dFiringDeltBkgrd = 2;

		dNightFiringT3930nm = -1;
		dNightFiringT3930nm_T11um = -1;
	}

	//初始化积雪判别阈值
	void KAgmrsThreshold::InitThreshold_Snow()
	{
		dNdsi = 0.39;
		dSnowCh2 = 11;
		dSnowCh4 = 30;
		dSnowCh26 = 10;
		dSnowCh20Ch31 = 0;
		dSnowCh29Ch31 = 0;
	}

#pragma endregion

#pragma region KAvhrrThreshold

	void KAvhrrThreshold::InitThreshold_Fire()
	{
		dFiringRed = 20;
		dFiringNir = 25;
		dFiringT3930nm = 315;
		dFiringT3930nm_T11um = 10;
		dFiringDeltBkgrd = 2;

		dNightFiringT3930nm = 305;
		dNightFiringT3930nm_T11um = 8;
	}

#pragma endregion

#pragma region KModisThreshold

	void KModisThreshold::InitThreshold_Fire()
	{
		dFiringRed = 20;
		dFiringNir = 25;
		dFiringT3930nm = 315;
		dFiringT3930nm_T11um = 10;
		dFiringDeltBkgrd = 2;

		dNightFiringT3930nm = 305;
		dNightFiringT3930nm_T11um = 8;
	}

#pragma endregion

#pragma region KMersiThreshold

#pragma endregion

#pragma region KVirrThreshold

#pragma endregion

};

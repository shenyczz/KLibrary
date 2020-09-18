#pragma once

namespace Agmrs
{

	class KIND_EXT_CLASS KAgmrsThreshold
	{
	public:
		KAgmrsThreshold();
		virtual ~KAgmrsThreshold();

	public:
		void Init();

	protected:
		virtual void InitThreshold_Cloud();
		virtual void InitThreshold_Water();
		virtual void InitThreshold_Land();
		virtual void InitThreshold_Fire();
		virtual void InitThreshold_Snow();

	public:
		// 1.0 白天云判别阈值
		//
		// 【判别方法】
		//		dDataRed			> Threshold.dCloudRed
		//		&&
		//		dDataNir-dDataRed	> Threshold.dCloudNirRed && dDataNir-dDataRed < Threshold.dCloudNirRedHi
		//
		//		&&(如果存在T11um通道)
		//		dDataT11um			< Threshold.dCloudT11um		(Default: 273)
		//
		double dCloudRed;				// % 红光通道反射率
		double dCloudT11um;				// K 波长11um通道温度
		double dCloudNirRed;			// % 近红外和红光通道反射率之差下限
		double dCloudNirRedMax;			// % 近红外和红光通道反射率之差上限

		// 1.1 晚上云判别阈值
		//
		// 【判别方法】
		//  满足条件是晴空(IsClearNight)
		//		dDataT12um > Threshold.dNightCloudT12umLo
		//		&&
		//		dDataT12um < Threshold.dNightCloudT12umHi
		//
		double dNightCloudT12umLo;		// K 
		double dNightCloudT12umHi;		// K 

		// 2.0 水体判别阈值
		// 
		// 【判别方法】
		//		dDataRed			< Threshold.dWaterRed
		//		&&
		//		dDataNir			< Threshold.dWaterNir
		//		&&
		//		dDataNir-dDataRed	> Threshold.dWaterNirRedLo && dDataNir-dDataRed < Threshold.dWaterNirRedHi
		//
		double dWaterRed;				// % 红光通道反射率
		double dWaterNir;				// % 近红外通道反射率
		double dWaterNirRed;			// % 近红外和红光通道反射率之差
		double dWaterNirRedMin;			// % 近红外和红光通道反射率之差

		// 3.0 积雪判断
		double dNdsi;					// 积雪指数
		double dSnowCh2;				// %
		double dSnowCh4;				// %
		double dSnowCh26;				// K
		double dSnowCh20Ch31;			// K
		double dSnowCh29Ch31;			// K

		// 4.0 陆地判别阈值(没有确定)
		double dLandRed;				// % 20
		double dLandNir;				// % ?(没有确定)
		double dLandNirRed;				// % 0

		// 5.0 白天火点
		double dFiringRed;				// %
		double dFiringNir;				// % 
		double dFiringT3930nm;			// K
		double dFiringT3930nm_T11um;	// K 
		double dFiringDeltBkgrd;		// K 背景温度标准偏差>=2K

		// 5.1 晚上火点		  
		double dNightFiringT3930nm;			// K 
		double dNightFiringT3930nm_T11um;	// K 
	};

	class KIND_EXT_CLASS KAvhrrThreshold : public KAgmrsThreshold
	{
	protected:
		//virtual void InitThreshold_Cloud();
		//virtual void InitThreshold_Water();
		//virtual void InitThreshold_Land();
		virtual void InitThreshold_Fire();
		//virtual void InitThreshold_Snow();
	};

	class KIND_EXT_CLASS KModisThreshold : public KAgmrsThreshold
	{
	protected:
		//virtual void InitThreshold_Cloud();
		//virtual void InitThreshold_Water();
		//virtual void InitThreshold_Land();
		virtual void InitThreshold_Fire();
		//virtual void InitThreshold_Snow();
	};

	class KIND_EXT_CLASS KMersiThreshold : public KAgmrsThreshold
	{
	protected:
		//virtual void InitThreshold_Cloud();
		//virtual void InitThreshold_Water();
		//virtual void InitThreshold_Land();
		//virtual void InitThreshold_Fire();
		//virtual void InitThreshold_Snow();
	};

	class KIND_EXT_CLASS KVirrThreshold : public KAgmrsThreshold
	{
	protected:
		//virtual void InitThreshold_Cloud();
		//virtual void InitThreshold_Water();
		//virtual void InitThreshold_Land();
		//virtual void InitThreshold_Fire();
		//virtual void InitThreshold_Snow();
	};

}

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
		// 1.0 �������б���ֵ
		//
		// ���б𷽷���
		//		dDataRed			> Threshold.dCloudRed
		//		&&
		//		dDataNir-dDataRed	> Threshold.dCloudNirRed && dDataNir-dDataRed < Threshold.dCloudNirRedHi
		//
		//		&&(�������T11umͨ��)
		//		dDataT11um			< Threshold.dCloudT11um		(Default: 273)
		//
		double dCloudRed;				// % ���ͨ��������
		double dCloudT11um;				// K ����11umͨ���¶�
		double dCloudNirRed;			// % ������ͺ��ͨ��������֮������
		double dCloudNirRedMax;			// % ������ͺ��ͨ��������֮������

		// 1.1 �������б���ֵ
		//
		// ���б𷽷���
		//  �������������(IsClearNight)
		//		dDataT12um > Threshold.dNightCloudT12umLo
		//		&&
		//		dDataT12um < Threshold.dNightCloudT12umHi
		//
		double dNightCloudT12umLo;		// K 
		double dNightCloudT12umHi;		// K 

		// 2.0 ˮ���б���ֵ
		// 
		// ���б𷽷���
		//		dDataRed			< Threshold.dWaterRed
		//		&&
		//		dDataNir			< Threshold.dWaterNir
		//		&&
		//		dDataNir-dDataRed	> Threshold.dWaterNirRedLo && dDataNir-dDataRed < Threshold.dWaterNirRedHi
		//
		double dWaterRed;				// % ���ͨ��������
		double dWaterNir;				// % ������ͨ��������
		double dWaterNirRed;			// % ������ͺ��ͨ��������֮��
		double dWaterNirRedMin;			// % ������ͺ��ͨ��������֮��

		// 3.0 ��ѩ�ж�
		double dNdsi;					// ��ѩָ��
		double dSnowCh2;				// %
		double dSnowCh4;				// %
		double dSnowCh26;				// K
		double dSnowCh20Ch31;			// K
		double dSnowCh29Ch31;			// K

		// 4.0 ½���б���ֵ(û��ȷ��)
		double dLandRed;				// % 20
		double dLandNir;				// % ?(û��ȷ��)
		double dLandNirRed;				// % 0

		// 5.0 ������
		double dFiringRed;				// %
		double dFiringNir;				// % 
		double dFiringT3930nm;			// K
		double dFiringT3930nm_T11um;	// K 
		double dFiringDeltBkgrd;		// K �����¶ȱ�׼ƫ��>=2K

		// 5.1 ���ϻ��		  
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

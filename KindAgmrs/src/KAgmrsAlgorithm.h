/******************************************************************************
 ** KAgmrsAlgorithm.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KAgmrsAlgorithm - 农业气象遥感数据反演算法
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 

 ** Modifier: 
 ** DateTime: 
 **  Explain: 

 **    Usage: 

              //1. 
              use

******************************************************************************/

#pragma once

namespace Agmrs
{

	class KIND_EXT_CLASS KAgmrsAlgorithm : public KDataAlgorithm
	{
	public:
		KAgmrsAlgorithm(void);
		virtual ~KAgmrsAlgorithm(void);

	public:
		enum _channel_
		{
			None = 0,
			Red,		// 0.645um - 红外通道
			Nir,		// 0.859um - 近红外通道
			Blu,		// 0.469um - 蓝光通道
			Grn,		// 0.555um - 绿光通道
			C940,		// 0.940um - 0.940um通道
			T11um,		// 11.00um - 热红外通道
			T12um,		// 12.00um - 热红外通道
		};

	public:
		// 是否有效数据
		virtual BOOL IsValidData(double dValue) { return (fabs(dValue-(double)AGMRS_INVALID_DATA)<EPS); }

		// 用于计算指数的数据
		void SetData(void* pData) { m_pData = pData; }
		void* GetData() { return m_pData; }

		// LAI配置操作
		void SetLaiConfig(const KStringArray& lc) { m_LaiConfig = lc; }
		const KStringArray& GetLaiConfig() { return m_LaiConfig; }
		bool ConfigLai();

	public:
		//-------------------------------------------------
		///植被指数
		//-------------------------------------------------
		// 归一化植被指数
		virtual double Ndvi() = 0;
		// 比值植被指数
		virtual double Rvi() = 0;
		// 差值植被指数
		virtual double Dvi() = 0;
		// 土壤调整植被指数
		virtual double Savi() = 0;
		// 增强型植被指数
		virtual double Evi() = 0;

		// 植被指数导出产品
		virtual double Lai() = 0;
		//-------------------------------------------------
		///干旱指数
		//-------------------------------------------------
		// 植被供水指数
		virtual double Vswi() = 0;
		// 地表含水量指数
		virtual double Swci() = 0;
		//-------------------------------------------------
		///其他指数
		//-------------------------------------------------
		// 归一化积雪指数
		virtual double Ndsi() = 0;
		//-------------------------------------------------
		/// 地表温度
		//-------------------------------------------------
		virtual double Lst(int iMethod = 0) = 0;


	protected:
		// 数据指针
		void* m_pData;

		// LAI 配置数据
		KStringArray m_LaiConfig;	// 字符串数组{"vix","equ",...}
		KString m_strVix;			// 指数名称字符串：NDVI、RIV、DIV、SAVI等
		KString m_strEqu;			// 指数对应的方程
	};

//---------------------------------------------------------
}// namespace Agmrs - end
//---------------------------------------------------------

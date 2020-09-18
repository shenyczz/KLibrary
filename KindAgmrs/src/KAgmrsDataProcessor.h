/******************************************************************************
 ** KAgmrsDataProcessor.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KAgmrsDataProcessor - 农业气象遥感数据处理器
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

              //1. 设置数据对象

******************************************************************************/

#pragma once

#include "KAgmrsAlgorithm.h"

namespace Agmrs
{
	typedef vector<FirePoint> FirePointVector;

	class KIND_EXT_CLASS KAgmrsDataProcessor : public KDataProcessor
	{
	public:

	public:
		KAgmrsDataProcessor(void);
		virtual ~KAgmrsDataProcessor(void);

	public:
		// 计算TVDI
		BOOL Dix_Tvdi(KDataInfo* pDataInfo,float* pDataNdvi,float* pDataLst,float*& pDataTvdi);

	public:
		// 产品
		PFLOAT* GetProductData() { return m_ppfProduct; }
		// 取得火点集合
		FirePointVector* GetFirePointVector() { return m_pFirePointVector; }
		// 算法对象指针
		//KAgmrsAlgorithm* GetAgmrsAlgorithm() { return m_pAgmrsAlgorithm; }

	#pragma region --纯虚函数--

	public:
		// 数据转换为 DIB
		virtual BOOL DataToDIB(int iRChannel = 0,int iGChannel = 0,int iBChannel = 0) = 0;

		// 产品生成 - 全照率
		virtual BOOL Abe(int iProductID) = 0;
		// 产品生成 - 地表温度
		virtual BOOL Lst(int iMethod) = 0;
		// 产品生成 - 植被指数
		virtual BOOL Vix(int iProductID) = 0;
		// 产品生成 - 干旱指数
		virtual BOOL Dix(int iProductID) = 0;
		// 单项监测 - SIM (Single Item Monitor)
		virtual BOOL Sim(int iProductID) = 0;

		// 取得像元类型
		virtual int GetPixelType(int i,int j) = 0;
		// 是否晴空(无云)
		virtual BOOL IsClearNight(int i,int j) = 0;

		// 合并数据通道
		virtual BOOL ChannelCombine(ChannelCombineInfo* pChannelCombineInfo1,
			ChannelCombineInfo* pChannelCombineInfo2,
			LPCTSTR lpszOutputFile,
			KExtents* pExtents) = 0;

	#pragma endregion

	protected:
		// 格点数据转换为颜色
		virtual COLORREF* Data2Colors(COLORREF clrBack);

		// 编组
		void Grouping(FirePointVector* pFirePointVector);
		BOOL grouping(FirePoint* pFirePoint, FirePointVector* pGroupPointVector);

		// 提取背景温度
		// va - 数组
		// na - 数组维数
		void GetBackgroundTemperature(double* va, int& na, int ch, int row0, int col0, int rows, int cols);

	protected:
		// 产品数据
		PFLOAT* m_ppfProduct;
		// 数据转换为灰度值使用的参数结构数组(按500通道)
		Data2GrayParam* m_pData2GrayParam;
		// 火点集合
		FirePointVector* m_pFirePointVector;
		// 算法对象
		//KAgmrsAlgorithm* m_pAgmrsAlgorithm;

		// 通道数据索引数组
		int m_DataIndexOfChannel[200];
	};

//---------------------------------------------------------
}// namespace Agmrs - end
//---------------------------------------------------------

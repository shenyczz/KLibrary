/******************************************************************************
 ** KWsr98Data.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KWsr98Data -  WSR98D 数据类
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

#include "KRadarData.h"

namespace Wsr98d
{

	class KIND_EXT_CLASS KWsr98Data : public KRadarData
	{
	public:
		KWsr98Data(void);
		virtual ~KWsr98Data(void);

	public:
		// 清空数据
		virtual void Empty();

		// 装载数据
		virtual BOOL LoadData(LPCTSTR lpszFile, LPVOID lpvData = 0);

		// 转换到距离库链表 m_pRangeBinList
		virtual BOOL ToRangeBin() { Assert(FALSE); return FALSE; }

		// 取得单位
		virtual LPCTSTR GetUnit();

	public:
		// 取得产品5大数据块结构指针
		MHB* MHBPtr() { return &m_oMhb; }
		PDB* PDBPtr() { return &m_oPdb; }
		PSB* PSBPtr() { return &m_oPsb; }
		GAB* GABPtr() { return &m_oGab; }
		TAB* TABPtr() { return &m_oTab; }

		// 取得 PSB 数据块偏移
		const long PSBOffset() {return m_oPdb.lOffsetSymbology;}
		// 取得 GAB 数据块偏移
		const long GABOffset() {return m_oPdb.lOffsetGraphic;}
		// 取得 TAB 数据块偏移
		const long TABOffset() {return m_oPdb.lOffsetTabular;}

		// 取得产品代码
		int GetProductCode();
		// 取得产品数据级别
		int GetProductDataLevel(int iProductCode);
		// 取得产品仰角
		double GetProductElevation(int iProductCode);
		// 取得产品名称信息
		ProductNameInfo* GetProductNameInfo(int iProductCode);
		// 取得覆盖范围(km)
		double GetProductRange(int iProductCode);
		// 取得产品精度(一个像素点表示的km)
		double GetProductResolution(int iProductCode);
		// 取得产品类型
		ProductType GetProductType(int iProductCode);
		// 取得产品单位
		LPCTSTR GetProductUnit(int iProductCode);
		// 生成图例信息
		void MakeLegendInfo(int iProductCode);
		// 填充数据级别
		void MakeDataLevelThreshold(int iProductCode);

	public:
		// 设置/获取阈值
		void SetThreshold(int iThreshold_min,int iThreshold_max);
		void GetThreshold(int* iThreshold_min,int* iThreshold_max);

		// 数据级别阈值
		DataLevelThreshold* GetDataLevelThreshold() { return &m_DataLevelThreshold[0]; }
		DataLevelThreshold* GetDataLevelThreshold(int index) { return &m_DataLevelThreshold[index]; }

		// 距离库链表
		KRangeBinList* GetRangeBinList() { return m_pRangeBinList; }

		// 
		KRangeBinPerRadialList* GetRangeBinPerRadialList() { return m_pRangeBinPerRadialList; }

		// 根据雷达站ID返回雷达站名称
		LPCTSTR GetRadarStationName(int id);

		// 取得每公里像素
		double GetPixelPerKm();

		// 填写雷达信息
		virtual void FillRadarInfo(RadarInfo* pRadarInfo);

		// 填充数据信息
		virtual void FillDataInfo(LPVOID lpvData);

	protected:
		// 初始化颜色表
		virtual void InitColorTable(int iProductCode);

		// 填充雷达产品信息5个特殊信息字段
		void MakeProductSpecialInfo(RadarInfo *pRadarInfo);

	public:
		// 颜色转换为要素值
		double Color2Value(COLORREF clr);

		// 要素值转换为颜色
		COLORREF Value2Color(double value);

		// 要素值转换为颜色索引
		int Value2ColorIndex(double value);

	protected:
		// 产品5大数据块结构
		MHB	m_oMhb;		//1 消息头块
		PDB	m_oPdb;		//2 产品描述
		PSB	m_oPsb;		//3 形象产品
		GAB	m_oGab;		//4 数字图解产品
		TAB	m_oTab;		//5 数字图表产品

		// 产品名称信息
		ProductNameInfo m_ProductNameInfo;

		// 根据产品依赖字段设置数据级别属性
		DataLevelThreshold m_DataLevelThreshold[256];

		// 雷达站名称
		TCHAR m_szStationName[32];

		// 产品数据阈值
		int m_iThreshold_max;	// 上阈值
		int m_iThreshold_min;	// 下阈值

		// 距离库链表
		KRangeBinList* m_pRangeBinList;

		// 每条径向射线上的距离库链表
		KRangeBinPerRadialList* m_pRangeBinPerRadialList;

	public:
		static bool IsWsr98Data(LPCTSTR lpszFile);

	};

//---------------------------------------------------------
}// namespace Wsr98d - end
//---------------------------------------------------------

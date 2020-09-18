/******************************************************************************
 ** KRpgData.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.
              All rights reserved.

 **     Name: KRpgData - RPG 数据类
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

		//1.装载数据
			KRpgData rpgData;
			rpgData.Load(fileName);

******************************************************************************/

#pragma once

#include "KWsr98Data.h"

namespace Wsr98d { namespace Rpg
{

	class KIND_EXT_CLASS KRpgData : public KWsr98Data
	{
	public:
		KRpgData(void);
		virtual ~KRpgData(void);

	public:
		// 清空数据
		virtual void Empty();

	public:
		// [1] 装载数据
		virtual BOOL LoadData(LPCTSTR lpszFile, LPVOID lpvData = 0);

		// [2] 生成图像
		BOOL BuildImage(float fZoom = 1.0f, BOOL bDrawCoordinate = TRUE, BOOL bDrawInfoArea = TRUE);

		// [3] 保存图像
		BOOL SaveImage(LPCTSTR lpszFile);

		// [4] 生成格点数据
		BOOL ToGrid(LPCTSTR lpszFile, int iThreshold_min = 0, int iThreshold_max = 15, int skip = 0);
		BOOL ToGrid(double*& pData, KDataInfo& dataInfo, int iThreshold_min = 0, int iThreshold_max = 15, int skip = 0);

		// [5] 生成地理坐标数据(距离库链表)
		virtual BOOL ToRangeBin();

		// [6] 保存雷达产品属性数据
		BOOL SavePSBTextData(LPCTSTR lpszFile);
		BOOL SaveGABTextData(LPCTSTR lpszFile);
		BOOL SaveTABTextData(LPCTSTR lpszFile);

		// [7] 判断数据块是否存在
		BOOL ExistPSB();
		BOOL ExistGAB();
		BOOL ExistTAB();

		// [8] 拼图
		BOOL Union(double*& pData, KDataInfo* pDataInfo,TCollection<KString>* pFileArray);
		BOOL Union(double*& pData, KDataInfo* pDataInfo,LPCTSTR pszFile[],int nFile);
		BOOL Union(double*& pData, KDataInfo* pDataInfo, double*& pData1, KDataInfo* pDataInfo1, double*& pData2, KDataInfo* pDataInfo2);

		//[9] 转换
		//根据数值转换为灰度图s
		HBITMAP ToGrayImage();

	public:
		//取得图像的中心经纬度
		void GetImageCenter(double* x,double* y);

	public:
		// 径向数据图像的数据包头
		RadialDataPacketHeader* GetRadialDataPacketHeader() { return &m_RadialDataPacketHeader; }
		// 栅格数据图像的数据包头
		RasterDataPacketHeader* GetRasterDataPacketHeader() { return &m_RasterDataPacketHeader; }

		//FMT.01 单色文本数据指针
		//     Mono Text Data
		//     Packet Code = 1
		KMonoTextList* GetMonoTextList() { return &m_MonoTextList; }

		//FMT.02 特殊符号数据指针
		//     Special Symbol Data
		//     Packet Code = 2
		KSpecilaSymbolList* GetSpecilaSymbolList() { return &m_SpecilaSymbolList; }

		//FMT.04 风钩数据指针
		//     Wind Barb Data
		//     Packet Code = 4
		KWindBarbList* GetWindBarbList() { return &m_WindBarbList; }

		//FMT.06 单色连接矢量数据指针
		//     Linked Vector Data
		//     Packet Code = 6
		KMonoLinkedVectorList* GetMonoLinkedVectorList() { return &m_MonoLinkedVectorList; }

		//FMT.07 单色非连接矢量数据指针(一组线段)
		//     Unlinked Vector Data
		//     Packet Code = 7
		KMonoUnlinkedVectorList* GetMonoUnlinkedVectorList() { return &m_MonoUnlinkedVectorList; }

		//FMT.08 彩色文本数据指针
		//     Color Text Data
		//     Packet Code = 8
		KColorTextList* GetColorTextList() { return &m_ColorTextList; }

		//FMT.09 彩色连接矢量数据指针
		//     Linked Vector Data
		//     Packet Code = 9
		KColorLinkedVectorList* GetColorLinkedVectorList() { return &m_ColorLinkedVectorList; }

		//FMT.10 彩色非连接矢量数据指针
		//     Unlinked Vector Data
		//     Packet Code=10
		KColorUnlinkedVectorList* GetColorUnlinkedVectorList() { return &m_ColorUnlinkedVectorList; }

		//FMT.15 风暴ID数据指针
		//     Storm Id Data
		//     Packet Code = 15
		KStormIdList* GetStormIdList() { return &m_StormIdList; }

		//FMT.19 冰雹概率数据指针
		//     HDA Hail Data
		//     Packet Code = 19
		KHdaHailList* GetHdaHailList() { return &m_HdaHailList; }

		//FMT.23 风暴中心路径信息过去数据指针
		//     SCIT Past Data
		//     Packet Code = 23
		KScitPastList* GetScitPastList() { return &m_ScitPastList; }

		//FMT.24 风暴中心路径信息预报数据指针
		//     SCIT Forecast Data
		//     Packet Code = 24
		KScitForecastList* GetScitForecastList() { return &m_ScitForecastList; }

		//FMT.25 风暴路径信息圈数据指针
		//     STI Circle Data
		//     Packet Code = 25
		KStiCircleList* GetStiCircleList() { return &m_StiCircleList; }

		//FMT.x0E03 连接等值线矢量数据指针
		//        Linked Contour Vectors Data
		//        Packet Code = 0x0E03
		KLinkedContourVectorList* GetLinkedContourVectorList() { return &m_LinkedContourVectorList; }

		//FMT.xAF1F 径向数据图像数据指针
		//        Radial Data
		//        Packet Code = 0xAF1F
		KRadialList* GetRadialList() { return &m_RadialList; }

		//FMT.xBA07 栅格数据图像数据指针
		//        Raster Data
		//        Packet Code = 0xBA07
		//FMT.xBA0F 栅格数据图像数据
		//        Raster Data Packet
		//        Packet Code = 0xBA0F
		KRasterRowList* GetRasterRowList() { return &m_RasterRowList; }

		// 多层切片图
		//FMT.xBA07 栅格数据图像数据指针
		//        Raster Data
		//        Packet Code = 0xBA07
		//FMT.xBA0F 栅格数据图像数据
		//        Raster Data Packet
		//        Packet Code = 0xBA0F
		KRasterRowList*	GetRasterRowList_Slice(int i)					{ return &m_RasterRowList_Slice[i]; }			// 多层切片图
		KMonoUnlinkedVectorList* GetMonoUnLinkedVectorList_Slice(int i)	{ return &m_MonoUnLinkedVectorList_Slice[i]; }	// 各切片坐标数据
		KMonoTextList* GetMonoTextList_Slice(int i)						{ return &m_MonoTextList_Slice[i]; }			// 各切片标注数据

		//Other
		//
		//
		KWriteTextList* GetPSBWriteTextList()				{ return &m_PSBWriteTextList; }			// PSB字符串链表(针对62和82)
		KWriteTextList* GetGABWriteTextList()				{ return &m_GABWriteTextList; }			// GAB字符串链表
		KWriteTextList* GetTABWriteTextList()				{ return &m_TABWriteTextList; }			// TAB字符串链表

		KColorUnlinkedVectorList* GetGABUnlinkedVectorList() { return &m_GABUnlinkedVectorList; }	// GABGAB非连接矢量链表

	//protected:
	//	// 填充数据信息
	//	virtual void FillDataInfo(LPVOID lpvData) {}

	private:
		// 径向数据图像的数据包头
		RadialDataPacketHeader m_RadialDataPacketHeader;
		// 栅格数据图像的数据包头
		RasterDataPacketHeader m_RasterDataPacketHeader;

		//FMT.01 单色文本数据
		//     Mono Text Data
		//     Packet Code = 1
		KMonoTextList m_MonoTextList;

		//FMT.02 特殊符号数据
		//     Special Symbol Data
		//     Packet Code = 2
		KSpecilaSymbolList m_SpecilaSymbolList;

		//FMT.04 风钩数据
		//     Wind Barb Data
		//     Packet Code = 4
		KWindBarbList m_WindBarbList;

		//FMT.06 单色连接矢量数据
		//     Linked Vector Data
		//     Packet Code = 6
		KMonoLinkedVectorList m_MonoLinkedVectorList;

		//FMT.07 单色非连接矢量数据(一组线段)
		//     Unlinked Vector Data
		//     Packet Code = 7
		KMonoUnlinkedVectorList m_MonoUnlinkedVectorList;

		//FMT.08 彩色文本数据
		//     Color Text Data
		//     Packet Code = 8
		KColorTextList m_ColorTextList;

		//FMT.09 彩色连接矢量数据
		//     Linked Vector Data
		//     Packet Code = 9
		KColorLinkedVectorList m_ColorLinkedVectorList;

		//FMT.10 彩色非连接矢量数据
		//     Unlinked Vector Data
		//     Packet Code=10
		KColorUnlinkedVectorList m_ColorUnlinkedVectorList;

		//FMT.15 风暴ID数据
		//     Storm Id Data
		//     Packet Code = 15
		KStormIdList m_StormIdList;

		//FMT.19 冰雹概率数据
		//     HDA Hail Data
		//     Packet Code = 19
		KHdaHailList m_HdaHailList;

		//FMT.23 风暴中心路径信息过去数据
		//     SCIT Past Data
		//     Packet Code = 23
		KScitPastList  m_ScitPastList;

		//FMT.24 风暴中心路径信息预报数据
		//     SCIT Forecast Data
		//     Packet Code = 24
		KScitForecastList m_ScitForecastList;

		//FMT.25 风暴路径信息圈数据
		//     STI Circle Data
		//     Packet Code = 25
		KStiCircleList m_StiCircleList;

		//FMT.x0E03 连接等值线矢量数据
		//        Linked Contour Vectors Data
		//        Packet Code = 0x0E03
		KLinkedContourVectorList m_LinkedContourVectorList;

		//FMT.xAF1F 读取径向数据图像数据
		//        Radial Data
		//        Packet Code = 0xAF1F
		KRadialList m_RadialList;

		//FMT.xBA07 读取栅格数据图像数据
		//        Raster Data
		//        Packet Code = 0xBA07
		//FMT.xBA0F 读取栅格数据图像数据
		//        Raster Data Packet
		//        Packet Code = 0xBA0F
		KRasterRowList m_RasterRowList;

		//FMT.xBA07 读取栅格数据图像数据
		//        Raster Data
		//        Packet Code = 0xBA07
		//FMT.xBA0F 读取栅格数据图像数据
		//        Raster Data
		//        Packet Code = 0xBA0F
		KRasterRowList			m_RasterRowList_Slice[20];			// 多层切片图
		KMonoUnlinkedVectorList	m_MonoUnLinkedVectorList_Slice[20];	// 各切片坐标数据
		KMonoTextList			m_MonoTextList_Slice[20];			// 各切片标注数据

		//Other
		//
		//
		KWriteTextList m_PSBWriteTextList;					// PSB字符串链表(针对62和82)
		KWriteTextList m_GABWriteTextList;					// GAB字符串链表
		KWriteTextList m_TABWriteTextList;					// TAB字符串链表

		KColorUnlinkedVectorList m_GABUnlinkedVectorList;	// GAB非连接矢量链表


	public:
		/// 静态函数
		static bool IsRpgData(LPCTSTR lpszFile);
	};

//---------------------------------------------------------
}}// namespace Wsr98d::Rpg - end
//---------------------------------------------------------

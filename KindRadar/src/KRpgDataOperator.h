/******************************************************************************
 ** header.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.
              All rights reserved.

 **     Name: Kind - 
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

#include "KWsr98DataOperator.h"

namespace Wsr98d { namespace Rpg
{

	class KIND_EXT_CLASS KRpgDataOperator : public KWsr98DataOperator
	{
	public:
		KRpgDataOperator(void);
		virtual ~KRpgDataOperator(void);

#pragma region 读取数据 - LoadData

	///----读取数据 - beg -------------------------------------
	public:
		// 装载数据
		virtual BOOL LoadData(LPCTSTR lpszFile,LPVOID lpvData = 0);

	private:
		/// 装载5大数据块 由 LoadData() 调用
		BOOL loadMHB(LPCTSTR lpszFile);		//1. MHB
		BOOL loadPDB(LPCTSTR lpszFile);		//2. PDB
		BOOL loadPSB(LPCTSTR lpszFile);		//3. PSB
		BOOL loadGAB(LPCTSTR lpszFile);		//4. GAB
		BOOL loadTAB(LPCTSTR lpszFile);		//5. TAB

		///装载 PSB 数据块的8种数据
		//3.1 装载 Radial Image Data
		BOOL psb_LoadRadialImageData(LPCTSTR lpszFile,long lOffset);
		//3.2 装载 Raster Image Data
		BOOL psb_LoadRasterImageData(LPCTSTR lpszFile,long lOffset);
		//3.3 装载 Linked Contour Vector Image Data
		BOOL psb_LoadLinkedContourVectorImageData(LPCTSTR lpszFile,long lOffset);
		//3.4 装载 Geographic Alphanumeric Image Data
		BOOL psb_LoadGeographicAlphanumericImageData(LPCTSTR lpszFile,long lOffset);
		//3.5 装载 Non Geographic Alphanumeric Image Data
		BOOL psb_LoadNonGeographicAlphanumericImageData(LPCTSTR lpszFile,long lOffset);
		//3.6 装载 Alphanumeric Image Data
		BOOL psb_LoadAlphanumericImageData(LPCTSTR lpszFile,long lOffset);
		//3.7 装载 Raster Image Data (Section)
		BOOL psb_LoadRasterImageData_Section(LPCTSTR lpszFile,long lOffset);
		//3.8 装载 Raster Image Data (Slice)
		BOOL psb_LoadRasterImageData_Slice(LPCTSTR lpszFile,long lOffset);

		///装载 GAB 数据块文本数据
		// 4.1 
		BOOL gab_LoadWriteTextData(LPCTSTR lpszFile,long lOffset);

		///装载 TAB 数据块文本数据
		// 5.1 
		BOOL tab_LoadWriteTextData(LPCTSTR lpszFile,long lOffset);

		// 辅助函数
		BOOL ReadPSBWriteTextData(LPCTSTR lpszFile,long lOffset);
		BOOL ReadGABWriteTextData(LPCTSTR lpszFile,long lOffset);
		BOOL ReadTABWriteTextData(LPCTSTR lpszFile,long lOffset);

		///装载各种格式数据
		//FMT.01 读取单色文本数据
		//     Mono Text Data
		//     Packet Code = 1
		BOOL ReadMonoTextData(KMonoTextList* pMonoTextList);

		//FMT.02 读取特殊符号数据
		//     Special Symbol Data
		//     Packet Code = 2
		BOOL ReadSpecilaSymbolData(KSpecilaSymbolList* pSpecilaSymbolList);

		//FMT.04 读取风钩数据
		//     Wind Barb Data
		//     Packet Code = 4
		BOOL ReadWindBarbData(KWindBarbList* pWindBarbList);

		//FMT.06 读取单色连接矢量数据
		//     Linked Vector Data
		//     Packet Code = 6
		BOOL ReadMonoLinkedVectorData(KMonoLinkedVectorList* pMonoLinkedVectorList);

		//FMT.07 读取单色非连接矢量数据(一组线段)
		//     Unlinked Vector Data
		//     Packet Code = 7
		BOOL ReadMonoUnLinkedVectorData(KMonoUnlinkedVectorList* pMonoUnlinkedVectorList);

		//FMT.08 读取彩色文本数据
		//     Color Text Data
		//     Packet Code = 8
		BOOL ReadColorTextData(KColorTextList* pColorTextList);

		//FMT.09 读取彩色连接矢量数据
		//     Linked Vector Data
		//     Packet Code = 9
		BOOL ReadColorLinkedVectorData(KColorLinkedVectorList* pColorLinkedVectorList);

		//FMT.10 读取彩色非连接矢量数据
		//     Unlinked Vector Data
		//     Packet Code=10
		BOOL ReadColorUnLinkedVectorData(KColorUnlinkedVectorList* pColorUnlinkedVectorList);

		//FMT.15 读取风暴ID数据
		//     Storm Id Data
		//     Packet Code = 15
		BOOL ReadStormIdData(KStormIdList* pStormIdList);

		//FMT.19 读取冰雹概率数据
		//     HDA Hail Data
		//     Packet Code = 19
		BOOL ReadHdaHailData(KHdaHailList* pHdaHailList);

		//FMT.23 读取风暴中心路径信息过去数据
		//     SCIT Past Data
		//     Packet Code = 23
		BOOL ReadScitPastData(KScitPastList* pScitPastList);

		//FMT.24 读取风暴中心路径信息预报数据
		//     SCIT Forecast Data
		//     Packet Code = 24
		BOOL ReadScitForecastData(KScitForecastList* pScitForecastList);

		//FMT.25 读取风暴路径信息圈数据
		//     STI Circle Data
		//     Packet Code = 25
		BOOL ReadStiCircleData(KStiCircleList* pStiCircleList);

		//FMT.x0E03 读取连接等值线矢量数据
		//        Linked Contour Vectors Data
		//        Packet Code = 0x0E03
		BOOL ReadLinkedContourVectorData(KLinkedContourVectorList* pLinkedContourVectorList);

		//FMT.xAF1F 读取径向数据图像数据
		//        Radial Data
		//        Packet Code = 0xAF1F
		BOOL ReadRadialImageData(KRadialList* pRadialList);

		//FMT.xBA07 读取栅格数据图像数据
		//        Raster Data
		//        Packet Code = 0xBA07
		//FMT.xBA0F 读取栅格数据图像数据
		//        Raster Data Packet
		//        Packet Code = 0xBA0F
		BOOL ReadRasterImageData(KRasterRowList* pRasterRowList);

		//FMT.xBA07 读取栅格数据图像数据
		//        Raster Data
		//        Packet Code = 0xBA07
		//FMT.xBA0F 读取栅格数据图像数据
		//        Raster Data
		//        Packet Code = 0xBA0F
		BOOL ReadRasterImageData_Section(KRasterRowList* pRasterRowList);

		//FMT.xBA07 读取栅格数据图像数据
		//        Raster Data
		//        Packet Code = 0xBA07
		//FMT.xBA0F 读取栅格数据图像数据
		//        Raster Data
		//        Packet Code = 0xBA0F
		BOOL ReadRasterImageData_Slice(KRasterRowList* pRasterRowList);

		//FMT.x 读取 TAB 块的一页文本
		//
		//
		BOOL ReadTABWriteTextPage(KWriteTextList* pWriteTextList);

		///读取对应的数据包
		//PACKET.01 单色文本数据包
		//     Mono Text Data Packet
		//     Packet Code = 1
		BOOL ReadMonoTextDataPacket(MonoText* pMonoText);

		//PACKET.02 特殊符号数据包
		//     Special Symbol Packet
		//     Packet Code = 2
		BOOL ReadSpecilaSymbolDataPacket(SpecilaSymbol* pSpecilaSymbol);

		//PACKET.03 中气旋数据包 (参看PACKET.11)

		//PACKET.04 风钩数据包
		//     Wind Barb Data Packet
		//     Packet Code = 4
		BOOL ReadWindBarbDataPacket(WindBarb* pWindBarb);

		//PACKET.05 风矢数据包
		//     Vector Arrow Data Packet
		//     Packet Code = 5
		BOOL ReadVectorArrowDataPacket(VectorArrow* pVectorArrow);

		//PACKET.06 单色连接矢量数据包
		//     Linked Vector Packet
		//     Packet Code = 6
		BOOL ReadMonoLinkedVectorDataPacket(MonoLinkedVector* pMonoLinkedVector);

		//PACKET.07 单色非连接矢量数据包
		//     Unlinked Vector Packet
		//     Packet Code = 7
		BOOL ReadMonoUnlinkedVectorDataPacket(MonoUnlinkedVector* pMonoUnlinkedVector);

		//PACKET.08 彩色文本数据包
		//     Color Text Data Packet
		//     Packet Code = 8
		BOOL ReadColorTextDataPacket(ColorText* pColorText);

		//PACKET.09 彩色连接矢量数据包头
		//     Linked Vector Packet
		//     Packet Code = 9
		BOOL ReadColorLinkedVectorDataPacket(ColorLinkedVector* pColorLinkedVector);

		//PACKET.10 彩色非连接矢量数据包头
		//     Unlinked Vector Packet
		//     Packet Code=10
		BOOL ReadColorUnlinkedVectorDataPacket(ColorUnlinkedVector* pColorUnlinkedVector);

		//PACKET.11 中气旋数据包头
		//     Mesocyclone
		//     Packet Code = 11
		BOOL ReadMesocycloneDataPacket(Mesocyclone* pMesocyclone);

		//PACKET.12 龙卷蜗旋特征数据包头
		//     Tornado Vortex Signature
		//     Packet Code = 12
		BOOL ReadTornadoVortexSignatureDataPacket(TornadoVortexSignature* pTornadoVortexSignature);

		//PACKET.13 冰雹实况数据包头
		//     Hail Positive
		//     Packet Code = 13
		BOOL ReadHailPositiveDataPacket(HailPositive* pHailPositive);

		//PACKET.14 冰雹概率数据包
		//     Hail Probable
		//     Packet Code = 14
		BOOL ReadHailProbableDataPacket(HailProbable* pHailProbable);

		//PACKET.15 风暴ID数据包
		//     Storm ID
		//     Packet Code = 15
		BOOL ReadStormIdDataPacket(StormId* pStormId);

		//PACKET.16 数字径向数据阵列数据包
		//     Digital Radial Data Array Packet
		//     Packet Code = 16
		BOOL ReadDigitalRadialDataArrayDataPacket(DigitalRadialDataArray* pDigitalRadialDataArray);

		//PACKET.17 数字降水数据阵列数据包
		//     Digital Precipatation Data Array Packet
		//     Packet Code = 17
		BOOL ReadDigitalPrecipitationDataArrayDataPacket(DigitalPrecipitationDataArray* pDigitalPrecipitationDataArray);

		//PACKET.18 降水率数据阵列数据包
		//     Precipitation Rate Data Array Packet
		//     Packet Code = 18
		BOOL ReadPrecipitationRateDataArrayDataPacket(PrecipitationRateDataArray* pPrecipitationRateDataArray);

		//PACKET.19 冰雹概率数据包
		//     HDA Hail
		//     Packet Code = 19
		BOOL ReadHdaHailDataPacket(HdaHail* pHdaHail);

		//PACKET.20 待定数据包

		//PACKET.21 待定数据包

		//PACKET.22 待定数据包

		//PACKET.23 风暴中心路径信息过去数据包
		//     SCIT Past Data
		//     Packet Code = 23
		BOOL ReadScitPastDataPacket(ScitPast* pScitPast);

		//PACKET.24 风暴中心路径信息预报数据包
		//     SCIT Forecast Data
		//     Packet Code = 24
		BOOL ReadScitForecastDataPacket(ScitForecast* pScitForecast);

		//PACKET.25 风暴路径信息圈数据包
		//     STI Circle
		//     Packet Code = 25
		BOOL ReadStiCircleDataPacket(StiCircle* pStiCircle);

		//PACKET.x3501 非连接等值线矢量数据包
		//        UnLinked Contour Vectors Packet
		//        Packet Code = 0x3501
		BOOL ReadUnLinkedContourVectorDataPacket(UnlinkedContourVector* pUnlinkedContourVector);

		//PACKET.x0E03 连接等值线矢量数据包
		//        Linked Contour Vectors Packet
		//        Packet Code = 0x0E03
		BOOL ReadLinkedContourVectorDataPacket(LinkedContourVector* pLinkedContourVector);

		//PACKET.xAF1F 径向数据包
		//        Radial Data Packet
		//        Packet Code = 0xAF1F
		BOOL ReadRadialDataPacket(Radial* pRadial);

		//PACKET.xBA07 栅格数据包
		//        Raster Data Packet
		//        Packet Code = 0xBA07
		//PACKET.xBA0F 栅格数据包
		//        Raster Data Packet
		//        Packet Code = 0xBA0F
		BOOL ReadRasterRowDataPacket(RasterRow* pRasterRow);
	///----读取数据 - end -------------------------------------

#pragma endregion

#pragma region 生成图像 - BuildImage

	///----生成图像 - beg -------------------------------------
	public:
		// 生成图像
		BOOL BuildImage(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);

	private:
		// 生成图像 - 分类
		BOOL BuildImage_Radial(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);				// 1
		BOOL BuildImage_Raster(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);				// 2
		BOOL BuildImage_LinkedContourVector(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);	// 3
		BOOL BuildImage_GeographicAlpha(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);		// 4
		BOOL BuildImage_NonGeographicAlpha(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);	// 5
		BOOL BuildImage_Alphanumeric(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);			// 6
		BOOL BuildImage_Raster_Section(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);		// 7
		BOOL BuildImage_Raster_Slice(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);			// 8

		/// 具体绘制函数(函数重载机制) - beg
		// 绘制文本数据包链表 [P3-108][P3-110][P3-109][P3-130]
		//     Write Text Data Packet
		//     Packet Code = 1,2,8,15
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KWriteTextList* pWriteTextList,BOOL bSpecilaSymbol=FALSE);

		// 中气旋数据包 [P3-129]
		//     Mesocyclone
		//     Packet Code = 3,11
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KMesocycloneList* pMesocycloneList);

		// 风钩数据包 [P3-128]
		//     Wind Barb Data Packet
		//     Packet Code = 4
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KWindBarbList* pWindBarbList);

		// 风矢数据包 [P3-127]
		//     Vector Arrow Data Packet
		//     Packet Code = 5
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KVectorArrowList* pVectorArrowList);

		// 绘制连接矢量数据包 [P3- 97][P3-105][P3-105]
		//     Linked Vector Packet
		//     Packet Code = 6,9,0x0e03
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KLinkedVectorList* pLinkedVectorList);

		// 绘制非连接的矢量数据图像 [P3-101][P3-102][P3-105]
		//     Unlinked Vector Packet
		//     Packet Code = 7,10,x3501
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KUnlinkedVectorList* pUnlinkedVectorList);

		// 龙卷蜗旋特征数据包 [P3-129]
		//     Tornado Vortex Signature
		//     Packet Code = 12

		// 冰雹实况数据包 [P3-129]
		//     Hail Positive
		//     Packet Code = 13

		// 冰雹概率数据包 [P3-129]
		//     Hail Probable
		//     Packet Code = 14

		// 径向数据包 [P3-125][P3-117]
		//     Radial Data Packet
		//     Packet Code = 16,0xAF1F
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KRadialList* pRadialList);

		// 栅格数据包 [P3-121][P3-123][P3-119]
		//     Raster Data Packet
		//     Packet Code = 17,18,xBA07
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KRasterRowList* pRasterRowList);

		// 冰雹概率数据包 [P3-130]
		//     HDA Hail
		//     Packet Code = 19
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KHdaHailList* pHdaHailList);

		//4.20 待定数据包

		//4.21 待定数据包

		//4.22 待定数据包

		// 风暴中心信息路径数据包 [P3-130]
		//     SCIT Past Data
		//     Packet Code = 23,24
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KScitList* pScitList);

		// 风暴路径信息圈数据包 [P3-130]
		//     STI Circle
		//     Packet Code = 25
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KStiCircleList* pStiCircleList);

		// 绘制距离库链表
		virtual BOOL DrawImage(HDC hDC, LPRECT lpRect, float fZoom, KRangeBinList* pRangeBinList);

		BOOL DrawImage(HDC hDC, LPRECT lpRect, float fZoom, KRangeBinPerRadialList* pRangeBinPerRadialList);

		BOOL out(LPCTSTR lpszFile, KRangeBinPerRadialList* pRangeBinPerRadialList);
		/// 具体绘制函数(函数重载机制) - end

		// 绘制极坐标
		void DrawPolar(HDC hDC, LPRECT pRect,float fZoom);
		// 绘制笛卡尔坐标(剖面图)
		void DrawDescartes(HDC hDC, LPRECT pRect,float fZoom,KUnlinkedVectorList* pUnlinkedVectorList,KWriteTextList* pWriteTextList);
		// 绘制产品信息
		void DrawInfoArea(HDC hDC, LPRECT pRect);
		// 绘制产品信息文本
		void DrawInfoText(HDC hDC, LPRECT pRect);
		// 绘制产品信息色标
		void DrawInfoHue(HDC hDC, LPRECT pRect);

	///----生成图像 - end -------------------------------------

#pragma endregion

#pragma region 数据转换 - ToGrid、Union等

	///----数据转换 - beg -------------------------------------
	public:
		// 生成格点数据
		BOOL ToGrid(double*& pData, KDataInfo& dataInfo, int iThreshold_min = 0, int iThreshold_max = 15, int skip = 0);
		BOOL ToGrid(LPCTSTR lpszFile, int iThreshold_min = 0, int iThreshold_max = 15, int skip = 0);

		// 根据数值转换为灰度图
		HBITMAP ToGrayImage();


		// 拼图
		BOOL Union(double*& pData, KDataInfo* pDataInfo,TCollection<KString>* pFileArray);
		BOOL Union(double*& pData, KDataInfo* pDataInfo,LPCTSTR pszFile[],int nFile);
		BOOL Union(double*& pData, KDataInfo* pDataInfo, double*& pData1, KDataInfo* pDataInfo1, double*& pData2, KDataInfo* pDataInfo2);

		// 径向数据转换为距离库链表
		BOOL Radial2RangeBin(KRangeBinList* pRangeBinList, KRangeBinPerRadialList* pRangeBinPerRadialList);

		// 栅格数据转换为距离库链表
		BOOL Raster2RangeBin(KRangeBinList* pRangeBinList);

	private:
		//行列与经纬度转换
		BOOL LonLat2RowCol(KDataInfo* pDataInfo, double lon, double lat, int& row, int& col);
		BOOL RowCol2LonLat(KDataInfo* pDataInfo, int row, int col, double& lon, double& lat);

	private:
		// 生成调色板
		void GeneratePalette();

	///----数据转换 - end -------------------------------------

#pragma endregion

#pragma region 写文本字符到磁盘文件

	public:
		//写文本字符到磁盘文件
		BOOL WriteTextData(LPCTSTR lpszFile,KWriteTextList* pWriteTextList);

#pragma endregion

	private:
		BOOL m_bScitPast;		// 过去
		BOOL m_bScitForecast;	// 预报


	};

//---------------------------------------------------------
}}// namespace Wsr98d::Rpg - end
//---------------------------------------------------------

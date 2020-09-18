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

 **   Author: ������.֣�� (shenyczz@163.com)
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

#pragma region ��ȡ���� - LoadData

	///----��ȡ���� - beg -------------------------------------
	public:
		// װ������
		virtual BOOL LoadData(LPCTSTR lpszFile,LPVOID lpvData = 0);

	private:
		/// װ��5�����ݿ� �� LoadData() ����
		BOOL loadMHB(LPCTSTR lpszFile);		//1. MHB
		BOOL loadPDB(LPCTSTR lpszFile);		//2. PDB
		BOOL loadPSB(LPCTSTR lpszFile);		//3. PSB
		BOOL loadGAB(LPCTSTR lpszFile);		//4. GAB
		BOOL loadTAB(LPCTSTR lpszFile);		//5. TAB

		///װ�� PSB ���ݿ��8������
		//3.1 װ�� Radial Image Data
		BOOL psb_LoadRadialImageData(LPCTSTR lpszFile,long lOffset);
		//3.2 װ�� Raster Image Data
		BOOL psb_LoadRasterImageData(LPCTSTR lpszFile,long lOffset);
		//3.3 װ�� Linked Contour Vector Image Data
		BOOL psb_LoadLinkedContourVectorImageData(LPCTSTR lpszFile,long lOffset);
		//3.4 װ�� Geographic Alphanumeric Image Data
		BOOL psb_LoadGeographicAlphanumericImageData(LPCTSTR lpszFile,long lOffset);
		//3.5 װ�� Non Geographic Alphanumeric Image Data
		BOOL psb_LoadNonGeographicAlphanumericImageData(LPCTSTR lpszFile,long lOffset);
		//3.6 װ�� Alphanumeric Image Data
		BOOL psb_LoadAlphanumericImageData(LPCTSTR lpszFile,long lOffset);
		//3.7 װ�� Raster Image Data (Section)
		BOOL psb_LoadRasterImageData_Section(LPCTSTR lpszFile,long lOffset);
		//3.8 װ�� Raster Image Data (Slice)
		BOOL psb_LoadRasterImageData_Slice(LPCTSTR lpszFile,long lOffset);

		///װ�� GAB ���ݿ��ı�����
		// 4.1 
		BOOL gab_LoadWriteTextData(LPCTSTR lpszFile,long lOffset);

		///װ�� TAB ���ݿ��ı�����
		// 5.1 
		BOOL tab_LoadWriteTextData(LPCTSTR lpszFile,long lOffset);

		// ��������
		BOOL ReadPSBWriteTextData(LPCTSTR lpszFile,long lOffset);
		BOOL ReadGABWriteTextData(LPCTSTR lpszFile,long lOffset);
		BOOL ReadTABWriteTextData(LPCTSTR lpszFile,long lOffset);

		///װ�ظ��ָ�ʽ����
		//FMT.01 ��ȡ��ɫ�ı�����
		//     Mono Text Data
		//     Packet Code = 1
		BOOL ReadMonoTextData(KMonoTextList* pMonoTextList);

		//FMT.02 ��ȡ�����������
		//     Special Symbol Data
		//     Packet Code = 2
		BOOL ReadSpecilaSymbolData(KSpecilaSymbolList* pSpecilaSymbolList);

		//FMT.04 ��ȡ�繳����
		//     Wind Barb Data
		//     Packet Code = 4
		BOOL ReadWindBarbData(KWindBarbList* pWindBarbList);

		//FMT.06 ��ȡ��ɫ����ʸ������
		//     Linked Vector Data
		//     Packet Code = 6
		BOOL ReadMonoLinkedVectorData(KMonoLinkedVectorList* pMonoLinkedVectorList);

		//FMT.07 ��ȡ��ɫ������ʸ������(һ���߶�)
		//     Unlinked Vector Data
		//     Packet Code = 7
		BOOL ReadMonoUnLinkedVectorData(KMonoUnlinkedVectorList* pMonoUnlinkedVectorList);

		//FMT.08 ��ȡ��ɫ�ı�����
		//     Color Text Data
		//     Packet Code = 8
		BOOL ReadColorTextData(KColorTextList* pColorTextList);

		//FMT.09 ��ȡ��ɫ����ʸ������
		//     Linked Vector Data
		//     Packet Code = 9
		BOOL ReadColorLinkedVectorData(KColorLinkedVectorList* pColorLinkedVectorList);

		//FMT.10 ��ȡ��ɫ������ʸ������
		//     Unlinked Vector Data
		//     Packet Code=10
		BOOL ReadColorUnLinkedVectorData(KColorUnlinkedVectorList* pColorUnlinkedVectorList);

		//FMT.15 ��ȡ�籩ID����
		//     Storm Id Data
		//     Packet Code = 15
		BOOL ReadStormIdData(KStormIdList* pStormIdList);

		//FMT.19 ��ȡ������������
		//     HDA Hail Data
		//     Packet Code = 19
		BOOL ReadHdaHailData(KHdaHailList* pHdaHailList);

		//FMT.23 ��ȡ�籩����·����Ϣ��ȥ����
		//     SCIT Past Data
		//     Packet Code = 23
		BOOL ReadScitPastData(KScitPastList* pScitPastList);

		//FMT.24 ��ȡ�籩����·����ϢԤ������
		//     SCIT Forecast Data
		//     Packet Code = 24
		BOOL ReadScitForecastData(KScitForecastList* pScitForecastList);

		//FMT.25 ��ȡ�籩·����ϢȦ����
		//     STI Circle Data
		//     Packet Code = 25
		BOOL ReadStiCircleData(KStiCircleList* pStiCircleList);

		//FMT.x0E03 ��ȡ���ӵ�ֵ��ʸ������
		//        Linked Contour Vectors Data
		//        Packet Code = 0x0E03
		BOOL ReadLinkedContourVectorData(KLinkedContourVectorList* pLinkedContourVectorList);

		//FMT.xAF1F ��ȡ��������ͼ������
		//        Radial Data
		//        Packet Code = 0xAF1F
		BOOL ReadRadialImageData(KRadialList* pRadialList);

		//FMT.xBA07 ��ȡդ������ͼ������
		//        Raster Data
		//        Packet Code = 0xBA07
		//FMT.xBA0F ��ȡդ������ͼ������
		//        Raster Data Packet
		//        Packet Code = 0xBA0F
		BOOL ReadRasterImageData(KRasterRowList* pRasterRowList);

		//FMT.xBA07 ��ȡդ������ͼ������
		//        Raster Data
		//        Packet Code = 0xBA07
		//FMT.xBA0F ��ȡդ������ͼ������
		//        Raster Data
		//        Packet Code = 0xBA0F
		BOOL ReadRasterImageData_Section(KRasterRowList* pRasterRowList);

		//FMT.xBA07 ��ȡդ������ͼ������
		//        Raster Data
		//        Packet Code = 0xBA07
		//FMT.xBA0F ��ȡդ������ͼ������
		//        Raster Data
		//        Packet Code = 0xBA0F
		BOOL ReadRasterImageData_Slice(KRasterRowList* pRasterRowList);

		//FMT.x ��ȡ TAB ���һҳ�ı�
		//
		//
		BOOL ReadTABWriteTextPage(KWriteTextList* pWriteTextList);

		///��ȡ��Ӧ�����ݰ�
		//PACKET.01 ��ɫ�ı����ݰ�
		//     Mono Text Data Packet
		//     Packet Code = 1
		BOOL ReadMonoTextDataPacket(MonoText* pMonoText);

		//PACKET.02 ����������ݰ�
		//     Special Symbol Packet
		//     Packet Code = 2
		BOOL ReadSpecilaSymbolDataPacket(SpecilaSymbol* pSpecilaSymbol);

		//PACKET.03 ���������ݰ� (�ο�PACKET.11)

		//PACKET.04 �繳���ݰ�
		//     Wind Barb Data Packet
		//     Packet Code = 4
		BOOL ReadWindBarbDataPacket(WindBarb* pWindBarb);

		//PACKET.05 ��ʸ���ݰ�
		//     Vector Arrow Data Packet
		//     Packet Code = 5
		BOOL ReadVectorArrowDataPacket(VectorArrow* pVectorArrow);

		//PACKET.06 ��ɫ����ʸ�����ݰ�
		//     Linked Vector Packet
		//     Packet Code = 6
		BOOL ReadMonoLinkedVectorDataPacket(MonoLinkedVector* pMonoLinkedVector);

		//PACKET.07 ��ɫ������ʸ�����ݰ�
		//     Unlinked Vector Packet
		//     Packet Code = 7
		BOOL ReadMonoUnlinkedVectorDataPacket(MonoUnlinkedVector* pMonoUnlinkedVector);

		//PACKET.08 ��ɫ�ı����ݰ�
		//     Color Text Data Packet
		//     Packet Code = 8
		BOOL ReadColorTextDataPacket(ColorText* pColorText);

		//PACKET.09 ��ɫ����ʸ�����ݰ�ͷ
		//     Linked Vector Packet
		//     Packet Code = 9
		BOOL ReadColorLinkedVectorDataPacket(ColorLinkedVector* pColorLinkedVector);

		//PACKET.10 ��ɫ������ʸ�����ݰ�ͷ
		//     Unlinked Vector Packet
		//     Packet Code=10
		BOOL ReadColorUnlinkedVectorDataPacket(ColorUnlinkedVector* pColorUnlinkedVector);

		//PACKET.11 ���������ݰ�ͷ
		//     Mesocyclone
		//     Packet Code = 11
		BOOL ReadMesocycloneDataPacket(Mesocyclone* pMesocyclone);

		//PACKET.12 ���������������ݰ�ͷ
		//     Tornado Vortex Signature
		//     Packet Code = 12
		BOOL ReadTornadoVortexSignatureDataPacket(TornadoVortexSignature* pTornadoVortexSignature);

		//PACKET.13 ����ʵ�����ݰ�ͷ
		//     Hail Positive
		//     Packet Code = 13
		BOOL ReadHailPositiveDataPacket(HailPositive* pHailPositive);

		//PACKET.14 �����������ݰ�
		//     Hail Probable
		//     Packet Code = 14
		BOOL ReadHailProbableDataPacket(HailProbable* pHailProbable);

		//PACKET.15 �籩ID���ݰ�
		//     Storm ID
		//     Packet Code = 15
		BOOL ReadStormIdDataPacket(StormId* pStormId);

		//PACKET.16 ���־��������������ݰ�
		//     Digital Radial Data Array Packet
		//     Packet Code = 16
		BOOL ReadDigitalRadialDataArrayDataPacket(DigitalRadialDataArray* pDigitalRadialDataArray);

		//PACKET.17 ���ֽ�ˮ�����������ݰ�
		//     Digital Precipatation Data Array Packet
		//     Packet Code = 17
		BOOL ReadDigitalPrecipitationDataArrayDataPacket(DigitalPrecipitationDataArray* pDigitalPrecipitationDataArray);

		//PACKET.18 ��ˮ�������������ݰ�
		//     Precipitation Rate Data Array Packet
		//     Packet Code = 18
		BOOL ReadPrecipitationRateDataArrayDataPacket(PrecipitationRateDataArray* pPrecipitationRateDataArray);

		//PACKET.19 �����������ݰ�
		//     HDA Hail
		//     Packet Code = 19
		BOOL ReadHdaHailDataPacket(HdaHail* pHdaHail);

		//PACKET.20 �������ݰ�

		//PACKET.21 �������ݰ�

		//PACKET.22 �������ݰ�

		//PACKET.23 �籩����·����Ϣ��ȥ���ݰ�
		//     SCIT Past Data
		//     Packet Code = 23
		BOOL ReadScitPastDataPacket(ScitPast* pScitPast);

		//PACKET.24 �籩����·����ϢԤ�����ݰ�
		//     SCIT Forecast Data
		//     Packet Code = 24
		BOOL ReadScitForecastDataPacket(ScitForecast* pScitForecast);

		//PACKET.25 �籩·����ϢȦ���ݰ�
		//     STI Circle
		//     Packet Code = 25
		BOOL ReadStiCircleDataPacket(StiCircle* pStiCircle);

		//PACKET.x3501 �����ӵ�ֵ��ʸ�����ݰ�
		//        UnLinked Contour Vectors Packet
		//        Packet Code = 0x3501
		BOOL ReadUnLinkedContourVectorDataPacket(UnlinkedContourVector* pUnlinkedContourVector);

		//PACKET.x0E03 ���ӵ�ֵ��ʸ�����ݰ�
		//        Linked Contour Vectors Packet
		//        Packet Code = 0x0E03
		BOOL ReadLinkedContourVectorDataPacket(LinkedContourVector* pLinkedContourVector);

		//PACKET.xAF1F �������ݰ�
		//        Radial Data Packet
		//        Packet Code = 0xAF1F
		BOOL ReadRadialDataPacket(Radial* pRadial);

		//PACKET.xBA07 դ�����ݰ�
		//        Raster Data Packet
		//        Packet Code = 0xBA07
		//PACKET.xBA0F դ�����ݰ�
		//        Raster Data Packet
		//        Packet Code = 0xBA0F
		BOOL ReadRasterRowDataPacket(RasterRow* pRasterRow);
	///----��ȡ���� - end -------------------------------------

#pragma endregion

#pragma region ����ͼ�� - BuildImage

	///----����ͼ�� - beg -------------------------------------
	public:
		// ����ͼ��
		BOOL BuildImage(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);

	private:
		// ����ͼ�� - ����
		BOOL BuildImage_Radial(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);				// 1
		BOOL BuildImage_Raster(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);				// 2
		BOOL BuildImage_LinkedContourVector(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);	// 3
		BOOL BuildImage_GeographicAlpha(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);		// 4
		BOOL BuildImage_NonGeographicAlpha(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);	// 5
		BOOL BuildImage_Alphanumeric(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);			// 6
		BOOL BuildImage_Raster_Section(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);		// 7
		BOOL BuildImage_Raster_Slice(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea);			// 8

		/// ������ƺ���(�������ػ���) - beg
		// �����ı����ݰ����� [P3-108][P3-110][P3-109][P3-130]
		//     Write Text Data Packet
		//     Packet Code = 1,2,8,15
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KWriteTextList* pWriteTextList,BOOL bSpecilaSymbol=FALSE);

		// ���������ݰ� [P3-129]
		//     Mesocyclone
		//     Packet Code = 3,11
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KMesocycloneList* pMesocycloneList);

		// �繳���ݰ� [P3-128]
		//     Wind Barb Data Packet
		//     Packet Code = 4
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KWindBarbList* pWindBarbList);

		// ��ʸ���ݰ� [P3-127]
		//     Vector Arrow Data Packet
		//     Packet Code = 5
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KVectorArrowList* pVectorArrowList);

		// ��������ʸ�����ݰ� [P3- 97][P3-105][P3-105]
		//     Linked Vector Packet
		//     Packet Code = 6,9,0x0e03
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KLinkedVectorList* pLinkedVectorList);

		// ���Ʒ����ӵ�ʸ������ͼ�� [P3-101][P3-102][P3-105]
		//     Unlinked Vector Packet
		//     Packet Code = 7,10,x3501
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KUnlinkedVectorList* pUnlinkedVectorList);

		// ���������������ݰ� [P3-129]
		//     Tornado Vortex Signature
		//     Packet Code = 12

		// ����ʵ�����ݰ� [P3-129]
		//     Hail Positive
		//     Packet Code = 13

		// �����������ݰ� [P3-129]
		//     Hail Probable
		//     Packet Code = 14

		// �������ݰ� [P3-125][P3-117]
		//     Radial Data Packet
		//     Packet Code = 16,0xAF1F
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KRadialList* pRadialList);

		// դ�����ݰ� [P3-121][P3-123][P3-119]
		//     Raster Data Packet
		//     Packet Code = 17,18,xBA07
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KRasterRowList* pRasterRowList);

		// �����������ݰ� [P3-130]
		//     HDA Hail
		//     Packet Code = 19
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KHdaHailList* pHdaHailList);

		//4.20 �������ݰ�

		//4.21 �������ݰ�

		//4.22 �������ݰ�

		// �籩������Ϣ·�����ݰ� [P3-130]
		//     SCIT Past Data
		//     Packet Code = 23,24
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KScitList* pScitList);

		// �籩·����ϢȦ���ݰ� [P3-130]
		//     STI Circle
		//     Packet Code = 25
		virtual BOOL DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KStiCircleList* pStiCircleList);

		// ���ƾ��������
		virtual BOOL DrawImage(HDC hDC, LPRECT lpRect, float fZoom, KRangeBinList* pRangeBinList);

		BOOL DrawImage(HDC hDC, LPRECT lpRect, float fZoom, KRangeBinPerRadialList* pRangeBinPerRadialList);

		BOOL out(LPCTSTR lpszFile, KRangeBinPerRadialList* pRangeBinPerRadialList);
		/// ������ƺ���(�������ػ���) - end

		// ���Ƽ�����
		void DrawPolar(HDC hDC, LPRECT pRect,float fZoom);
		// ���Ƶѿ�������(����ͼ)
		void DrawDescartes(HDC hDC, LPRECT pRect,float fZoom,KUnlinkedVectorList* pUnlinkedVectorList,KWriteTextList* pWriteTextList);
		// ���Ʋ�Ʒ��Ϣ
		void DrawInfoArea(HDC hDC, LPRECT pRect);
		// ���Ʋ�Ʒ��Ϣ�ı�
		void DrawInfoText(HDC hDC, LPRECT pRect);
		// ���Ʋ�Ʒ��Ϣɫ��
		void DrawInfoHue(HDC hDC, LPRECT pRect);

	///----����ͼ�� - end -------------------------------------

#pragma endregion

#pragma region ����ת�� - ToGrid��Union��

	///----����ת�� - beg -------------------------------------
	public:
		// ���ɸ������
		BOOL ToGrid(double*& pData, KDataInfo& dataInfo, int iThreshold_min = 0, int iThreshold_max = 15, int skip = 0);
		BOOL ToGrid(LPCTSTR lpszFile, int iThreshold_min = 0, int iThreshold_max = 15, int skip = 0);

		// ������ֵת��Ϊ�Ҷ�ͼ
		HBITMAP ToGrayImage();


		// ƴͼ
		BOOL Union(double*& pData, KDataInfo* pDataInfo,TCollection<KString>* pFileArray);
		BOOL Union(double*& pData, KDataInfo* pDataInfo,LPCTSTR pszFile[],int nFile);
		BOOL Union(double*& pData, KDataInfo* pDataInfo, double*& pData1, KDataInfo* pDataInfo1, double*& pData2, KDataInfo* pDataInfo2);

		// ��������ת��Ϊ���������
		BOOL Radial2RangeBin(KRangeBinList* pRangeBinList, KRangeBinPerRadialList* pRangeBinPerRadialList);

		// դ������ת��Ϊ���������
		BOOL Raster2RangeBin(KRangeBinList* pRangeBinList);

	private:
		//�����뾭γ��ת��
		BOOL LonLat2RowCol(KDataInfo* pDataInfo, double lon, double lat, int& row, int& col);
		BOOL RowCol2LonLat(KDataInfo* pDataInfo, int row, int col, double& lon, double& lat);

	private:
		// ���ɵ�ɫ��
		void GeneratePalette();

	///----����ת�� - end -------------------------------------

#pragma endregion

#pragma region д�ı��ַ��������ļ�

	public:
		//д�ı��ַ��������ļ�
		BOOL WriteTextData(LPCTSTR lpszFile,KWriteTextList* pWriteTextList);

#pragma endregion

	private:
		BOOL m_bScitPast;		// ��ȥ
		BOOL m_bScitForecast;	// Ԥ��


	};

//---------------------------------------------------------
}}// namespace Wsr98d::Rpg - end
//---------------------------------------------------------

/******************************************************************************
 ** KRpgData.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.
              All rights reserved.

 **     Name: KRpgData - RPG ������
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

		//1.װ������
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
		// �������
		virtual void Empty();

	public:
		// [1] װ������
		virtual BOOL LoadData(LPCTSTR lpszFile, LPVOID lpvData = 0);

		// [2] ����ͼ��
		BOOL BuildImage(float fZoom = 1.0f, BOOL bDrawCoordinate = TRUE, BOOL bDrawInfoArea = TRUE);

		// [3] ����ͼ��
		BOOL SaveImage(LPCTSTR lpszFile);

		// [4] ���ɸ������
		BOOL ToGrid(LPCTSTR lpszFile, int iThreshold_min = 0, int iThreshold_max = 15, int skip = 0);
		BOOL ToGrid(double*& pData, KDataInfo& dataInfo, int iThreshold_min = 0, int iThreshold_max = 15, int skip = 0);

		// [5] ���ɵ�����������(���������)
		virtual BOOL ToRangeBin();

		// [6] �����״��Ʒ��������
		BOOL SavePSBTextData(LPCTSTR lpszFile);
		BOOL SaveGABTextData(LPCTSTR lpszFile);
		BOOL SaveTABTextData(LPCTSTR lpszFile);

		// [7] �ж����ݿ��Ƿ����
		BOOL ExistPSB();
		BOOL ExistGAB();
		BOOL ExistTAB();

		// [8] ƴͼ
		BOOL Union(double*& pData, KDataInfo* pDataInfo,TCollection<KString>* pFileArray);
		BOOL Union(double*& pData, KDataInfo* pDataInfo,LPCTSTR pszFile[],int nFile);
		BOOL Union(double*& pData, KDataInfo* pDataInfo, double*& pData1, KDataInfo* pDataInfo1, double*& pData2, KDataInfo* pDataInfo2);

		//[9] ת��
		//������ֵת��Ϊ�Ҷ�ͼs
		HBITMAP ToGrayImage();

	public:
		//ȡ��ͼ������ľ�γ��
		void GetImageCenter(double* x,double* y);

	public:
		// ��������ͼ������ݰ�ͷ
		RadialDataPacketHeader* GetRadialDataPacketHeader() { return &m_RadialDataPacketHeader; }
		// դ������ͼ������ݰ�ͷ
		RasterDataPacketHeader* GetRasterDataPacketHeader() { return &m_RasterDataPacketHeader; }

		//FMT.01 ��ɫ�ı�����ָ��
		//     Mono Text Data
		//     Packet Code = 1
		KMonoTextList* GetMonoTextList() { return &m_MonoTextList; }

		//FMT.02 �����������ָ��
		//     Special Symbol Data
		//     Packet Code = 2
		KSpecilaSymbolList* GetSpecilaSymbolList() { return &m_SpecilaSymbolList; }

		//FMT.04 �繳����ָ��
		//     Wind Barb Data
		//     Packet Code = 4
		KWindBarbList* GetWindBarbList() { return &m_WindBarbList; }

		//FMT.06 ��ɫ����ʸ������ָ��
		//     Linked Vector Data
		//     Packet Code = 6
		KMonoLinkedVectorList* GetMonoLinkedVectorList() { return &m_MonoLinkedVectorList; }

		//FMT.07 ��ɫ������ʸ������ָ��(һ���߶�)
		//     Unlinked Vector Data
		//     Packet Code = 7
		KMonoUnlinkedVectorList* GetMonoUnlinkedVectorList() { return &m_MonoUnlinkedVectorList; }

		//FMT.08 ��ɫ�ı�����ָ��
		//     Color Text Data
		//     Packet Code = 8
		KColorTextList* GetColorTextList() { return &m_ColorTextList; }

		//FMT.09 ��ɫ����ʸ������ָ��
		//     Linked Vector Data
		//     Packet Code = 9
		KColorLinkedVectorList* GetColorLinkedVectorList() { return &m_ColorLinkedVectorList; }

		//FMT.10 ��ɫ������ʸ������ָ��
		//     Unlinked Vector Data
		//     Packet Code=10
		KColorUnlinkedVectorList* GetColorUnlinkedVectorList() { return &m_ColorUnlinkedVectorList; }

		//FMT.15 �籩ID����ָ��
		//     Storm Id Data
		//     Packet Code = 15
		KStormIdList* GetStormIdList() { return &m_StormIdList; }

		//FMT.19 ������������ָ��
		//     HDA Hail Data
		//     Packet Code = 19
		KHdaHailList* GetHdaHailList() { return &m_HdaHailList; }

		//FMT.23 �籩����·����Ϣ��ȥ����ָ��
		//     SCIT Past Data
		//     Packet Code = 23
		KScitPastList* GetScitPastList() { return &m_ScitPastList; }

		//FMT.24 �籩����·����ϢԤ������ָ��
		//     SCIT Forecast Data
		//     Packet Code = 24
		KScitForecastList* GetScitForecastList() { return &m_ScitForecastList; }

		//FMT.25 �籩·����ϢȦ����ָ��
		//     STI Circle Data
		//     Packet Code = 25
		KStiCircleList* GetStiCircleList() { return &m_StiCircleList; }

		//FMT.x0E03 ���ӵ�ֵ��ʸ������ָ��
		//        Linked Contour Vectors Data
		//        Packet Code = 0x0E03
		KLinkedContourVectorList* GetLinkedContourVectorList() { return &m_LinkedContourVectorList; }

		//FMT.xAF1F ��������ͼ������ָ��
		//        Radial Data
		//        Packet Code = 0xAF1F
		KRadialList* GetRadialList() { return &m_RadialList; }

		//FMT.xBA07 դ������ͼ������ָ��
		//        Raster Data
		//        Packet Code = 0xBA07
		//FMT.xBA0F դ������ͼ������
		//        Raster Data Packet
		//        Packet Code = 0xBA0F
		KRasterRowList* GetRasterRowList() { return &m_RasterRowList; }

		// �����Ƭͼ
		//FMT.xBA07 դ������ͼ������ָ��
		//        Raster Data
		//        Packet Code = 0xBA07
		//FMT.xBA0F դ������ͼ������
		//        Raster Data Packet
		//        Packet Code = 0xBA0F
		KRasterRowList*	GetRasterRowList_Slice(int i)					{ return &m_RasterRowList_Slice[i]; }			// �����Ƭͼ
		KMonoUnlinkedVectorList* GetMonoUnLinkedVectorList_Slice(int i)	{ return &m_MonoUnLinkedVectorList_Slice[i]; }	// ����Ƭ��������
		KMonoTextList* GetMonoTextList_Slice(int i)						{ return &m_MonoTextList_Slice[i]; }			// ����Ƭ��ע����

		//Other
		//
		//
		KWriteTextList* GetPSBWriteTextList()				{ return &m_PSBWriteTextList; }			// PSB�ַ�������(���62��82)
		KWriteTextList* GetGABWriteTextList()				{ return &m_GABWriteTextList; }			// GAB�ַ�������
		KWriteTextList* GetTABWriteTextList()				{ return &m_TABWriteTextList; }			// TAB�ַ�������

		KColorUnlinkedVectorList* GetGABUnlinkedVectorList() { return &m_GABUnlinkedVectorList; }	// GABGAB������ʸ������

	//protected:
	//	// ���������Ϣ
	//	virtual void FillDataInfo(LPVOID lpvData) {}

	private:
		// ��������ͼ������ݰ�ͷ
		RadialDataPacketHeader m_RadialDataPacketHeader;
		// դ������ͼ������ݰ�ͷ
		RasterDataPacketHeader m_RasterDataPacketHeader;

		//FMT.01 ��ɫ�ı�����
		//     Mono Text Data
		//     Packet Code = 1
		KMonoTextList m_MonoTextList;

		//FMT.02 �����������
		//     Special Symbol Data
		//     Packet Code = 2
		KSpecilaSymbolList m_SpecilaSymbolList;

		//FMT.04 �繳����
		//     Wind Barb Data
		//     Packet Code = 4
		KWindBarbList m_WindBarbList;

		//FMT.06 ��ɫ����ʸ������
		//     Linked Vector Data
		//     Packet Code = 6
		KMonoLinkedVectorList m_MonoLinkedVectorList;

		//FMT.07 ��ɫ������ʸ������(һ���߶�)
		//     Unlinked Vector Data
		//     Packet Code = 7
		KMonoUnlinkedVectorList m_MonoUnlinkedVectorList;

		//FMT.08 ��ɫ�ı�����
		//     Color Text Data
		//     Packet Code = 8
		KColorTextList m_ColorTextList;

		//FMT.09 ��ɫ����ʸ������
		//     Linked Vector Data
		//     Packet Code = 9
		KColorLinkedVectorList m_ColorLinkedVectorList;

		//FMT.10 ��ɫ������ʸ������
		//     Unlinked Vector Data
		//     Packet Code=10
		KColorUnlinkedVectorList m_ColorUnlinkedVectorList;

		//FMT.15 �籩ID����
		//     Storm Id Data
		//     Packet Code = 15
		KStormIdList m_StormIdList;

		//FMT.19 ������������
		//     HDA Hail Data
		//     Packet Code = 19
		KHdaHailList m_HdaHailList;

		//FMT.23 �籩����·����Ϣ��ȥ����
		//     SCIT Past Data
		//     Packet Code = 23
		KScitPastList  m_ScitPastList;

		//FMT.24 �籩����·����ϢԤ������
		//     SCIT Forecast Data
		//     Packet Code = 24
		KScitForecastList m_ScitForecastList;

		//FMT.25 �籩·����ϢȦ����
		//     STI Circle Data
		//     Packet Code = 25
		KStiCircleList m_StiCircleList;

		//FMT.x0E03 ���ӵ�ֵ��ʸ������
		//        Linked Contour Vectors Data
		//        Packet Code = 0x0E03
		KLinkedContourVectorList m_LinkedContourVectorList;

		//FMT.xAF1F ��ȡ��������ͼ������
		//        Radial Data
		//        Packet Code = 0xAF1F
		KRadialList m_RadialList;

		//FMT.xBA07 ��ȡդ������ͼ������
		//        Raster Data
		//        Packet Code = 0xBA07
		//FMT.xBA0F ��ȡդ������ͼ������
		//        Raster Data Packet
		//        Packet Code = 0xBA0F
		KRasterRowList m_RasterRowList;

		//FMT.xBA07 ��ȡդ������ͼ������
		//        Raster Data
		//        Packet Code = 0xBA07
		//FMT.xBA0F ��ȡդ������ͼ������
		//        Raster Data
		//        Packet Code = 0xBA0F
		KRasterRowList			m_RasterRowList_Slice[20];			// �����Ƭͼ
		KMonoUnlinkedVectorList	m_MonoUnLinkedVectorList_Slice[20];	// ����Ƭ��������
		KMonoTextList			m_MonoTextList_Slice[20];			// ����Ƭ��ע����

		//Other
		//
		//
		KWriteTextList m_PSBWriteTextList;					// PSB�ַ�������(���62��82)
		KWriteTextList m_GABWriteTextList;					// GAB�ַ�������
		KWriteTextList m_TABWriteTextList;					// TAB�ַ�������

		KColorUnlinkedVectorList m_GABUnlinkedVectorList;	// GAB������ʸ������


	public:
		/// ��̬����
		static bool IsRpgData(LPCTSTR lpszFile);
	};

//---------------------------------------------------------
}}// namespace Wsr98d::Rpg - end
//---------------------------------------------------------

/******************************************************************************
 ** KWsr98Data.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KWsr98Data -  WSR98D ������
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

#include "KRadarData.h"

namespace Wsr98d
{

	class KIND_EXT_CLASS KWsr98Data : public KRadarData
	{
	public:
		KWsr98Data(void);
		virtual ~KWsr98Data(void);

	public:
		// �������
		virtual void Empty();

		// װ������
		virtual BOOL LoadData(LPCTSTR lpszFile, LPVOID lpvData = 0);

		// ת������������� m_pRangeBinList
		virtual BOOL ToRangeBin() { Assert(FALSE); return FALSE; }

		// ȡ�õ�λ
		virtual LPCTSTR GetUnit();

	public:
		// ȡ�ò�Ʒ5�����ݿ�ṹָ��
		MHB* MHBPtr() { return &m_oMhb; }
		PDB* PDBPtr() { return &m_oPdb; }
		PSB* PSBPtr() { return &m_oPsb; }
		GAB* GABPtr() { return &m_oGab; }
		TAB* TABPtr() { return &m_oTab; }

		// ȡ�� PSB ���ݿ�ƫ��
		const long PSBOffset() {return m_oPdb.lOffsetSymbology;}
		// ȡ�� GAB ���ݿ�ƫ��
		const long GABOffset() {return m_oPdb.lOffsetGraphic;}
		// ȡ�� TAB ���ݿ�ƫ��
		const long TABOffset() {return m_oPdb.lOffsetTabular;}

		// ȡ�ò�Ʒ����
		int GetProductCode();
		// ȡ�ò�Ʒ���ݼ���
		int GetProductDataLevel(int iProductCode);
		// ȡ�ò�Ʒ����
		double GetProductElevation(int iProductCode);
		// ȡ�ò�Ʒ������Ϣ
		ProductNameInfo* GetProductNameInfo(int iProductCode);
		// ȡ�ø��Ƿ�Χ(km)
		double GetProductRange(int iProductCode);
		// ȡ�ò�Ʒ����(һ�����ص��ʾ��km)
		double GetProductResolution(int iProductCode);
		// ȡ�ò�Ʒ����
		ProductType GetProductType(int iProductCode);
		// ȡ�ò�Ʒ��λ
		LPCTSTR GetProductUnit(int iProductCode);
		// ����ͼ����Ϣ
		void MakeLegendInfo(int iProductCode);
		// ������ݼ���
		void MakeDataLevelThreshold(int iProductCode);

	public:
		// ����/��ȡ��ֵ
		void SetThreshold(int iThreshold_min,int iThreshold_max);
		void GetThreshold(int* iThreshold_min,int* iThreshold_max);

		// ���ݼ�����ֵ
		DataLevelThreshold* GetDataLevelThreshold() { return &m_DataLevelThreshold[0]; }
		DataLevelThreshold* GetDataLevelThreshold(int index) { return &m_DataLevelThreshold[index]; }

		// ���������
		KRangeBinList* GetRangeBinList() { return m_pRangeBinList; }

		// 
		KRangeBinPerRadialList* GetRangeBinPerRadialList() { return m_pRangeBinPerRadialList; }

		// �����״�վID�����״�վ����
		LPCTSTR GetRadarStationName(int id);

		// ȡ��ÿ��������
		double GetPixelPerKm();

		// ��д�״���Ϣ
		virtual void FillRadarInfo(RadarInfo* pRadarInfo);

		// ���������Ϣ
		virtual void FillDataInfo(LPVOID lpvData);

	protected:
		// ��ʼ����ɫ��
		virtual void InitColorTable(int iProductCode);

		// ����״��Ʒ��Ϣ5��������Ϣ�ֶ�
		void MakeProductSpecialInfo(RadarInfo *pRadarInfo);

	public:
		// ��ɫת��ΪҪ��ֵ
		double Color2Value(COLORREF clr);

		// Ҫ��ֵת��Ϊ��ɫ
		COLORREF Value2Color(double value);

		// Ҫ��ֵת��Ϊ��ɫ����
		int Value2ColorIndex(double value);

	protected:
		// ��Ʒ5�����ݿ�ṹ
		MHB	m_oMhb;		//1 ��Ϣͷ��
		PDB	m_oPdb;		//2 ��Ʒ����
		PSB	m_oPsb;		//3 �����Ʒ
		GAB	m_oGab;		//4 ����ͼ���Ʒ
		TAB	m_oTab;		//5 ����ͼ���Ʒ

		// ��Ʒ������Ϣ
		ProductNameInfo m_ProductNameInfo;

		// ���ݲ�Ʒ�����ֶ��������ݼ�������
		DataLevelThreshold m_DataLevelThreshold[256];

		// �״�վ����
		TCHAR m_szStationName[32];

		// ��Ʒ������ֵ
		int m_iThreshold_max;	// ����ֵ
		int m_iThreshold_min;	// ����ֵ

		// ���������
		KRangeBinList* m_pRangeBinList;

		// ÿ�����������ϵľ��������
		KRangeBinPerRadialList* m_pRangeBinPerRadialList;

	public:
		static bool IsWsr98Data(LPCTSTR lpszFile);

	};

//---------------------------------------------------------
}// namespace Wsr98d - end
//---------------------------------------------------------

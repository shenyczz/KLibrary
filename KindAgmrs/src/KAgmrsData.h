/******************************************************************************
 ** KAgmrsData.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I know nothing.

 **     Name: KAgmrsData - ũҵ����ң������
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

#include "KAxinData.h"
#include "KAgmrsDataProcessor.h"
#include "KAgmrsThreshold.h"

namespace Agmrs
{
	class KIND_EXT_CLASS KAgmrsData : public KAxinData
	{
	public:
		KAgmrsData(void);
		virtual ~KAgmrsData(void);

	public:
		// ��������
		virtual BOOL SaveData(LPCTSTR lpszFile);

	protected:
		// װ������
		virtual BOOL LoadData_bin(LPCTSTR lpszFile);

	public:
		// �ڴ�����
		virtual void Empty();

		// ����/��ȡ����ͨ��
		virtual void SetDataChannel(int iRChannel,int iGChannel,int iBChannel);
		virtual void GetDataChannel(int* iRChannel,int* iGChannel,int* iBChannel);

		// ����/��ȡĬ������ͨ��
		virtual void SetDataChannelDef(int iRChannel,int iGChannel,int iBChannel);
		virtual void GetDataChannelDef(int* iRChannel,int* iGChannel,int* iBChannel);

		// ȡ�ò�Ʒ����
		virtual PFLOAT* GetProductData();

		// ����ת��Ϊ DIB
		virtual BOOL DataToDIB(int iRChannel = 0,int iGChannel = 0,int iBChannel = 0);

		// �ϲ�����ͨ��
		virtual BOOL ChannelCombine(ChannelCombineInfo* pChannelCombineInfo1,
			ChannelCombineInfo* pChannelCombineInfo2,
			LPCTSTR lpszOutputFile,
			KExtents* pExtents);

	public:
		// �Ƿ��
		BOOL IsEmpty() { return m_bEmpty; }

		// ����ָ��
		PWORD* GetData() { return m_ppwData; }
		// ���ݵĻҶ�ָ��
		PBYTE* GetDataGray() { return m_ppucDataGray; }

		// ȡ���ļ�ͷ
		PVOID FileHeader() { return m_pFileHeader; }

		// ȡ��ͨ��ӳ��
		AgmrsDataChannelMap* GetAgmrsDataChannelMap() { return &m_AgmrsDataChannelMap; }

		// ȡ��ָ�����ȫ��ͨ������
		ChannelInfoVector* GetChannelInfo(double lon, double lat); 

	public:
		// ���ũ��ң������
		BOOL IsAgmrsData();
		BOOL IsAvhrrData()	{ return AgmrsDataSource::Avhrr	== this->GetAgmrsDataSource(); }
		BOOL IsModisData()	{ return AgmrsDataSource::Modis	== this->GetAgmrsDataSource(); }
		BOOL IsMersiData()	{ return AgmrsDataSource::Mersi	== this->GetAgmrsDataSource(); }
		BOOL IsVirrData()	{ return AgmrsDataSource::Virr	== this->GetAgmrsDataSource(); }

		// ȡ��ң������Դ
		int GetAgmrsDataSource();

	public:
		// ���� ATI ʱʹ��
		// ���ð��������ļ�
		LPCTSTR GetDayFile()				{ return m_szDayFile; }
		void SetDayFile(LPCTSTR lpszFile)	{ _tcscpy(m_szDayFile,lpszFile); }

		// �������������ļ�
		LPCTSTR GetNightFile()				{ return m_szNightFile; }
		void SetNightFile(LPCTSTR lpszFile)	{ _tcscpy(m_szNightFile,lpszFile); }

	public:
		// ��γ��ת��Ϊ�������
		virtual BOOL LonLat2xy(double dLon,double dLat,int& x,int& y, int flag = 0);
		// �������ת��Ϊ��γ������
		virtual BOOL xy2LonLat(int x,int y,double& lon,double& lat, int flag = 0);
		// ȡ�ò�Ʒ���ֵ
		virtual float GetProductValue(double lon, double lat);

	public:
		// ��Ʒ���� - ȫ����
		BOOL Abe(int iProductID);
		// ��Ʒ���� - �ر��¶�
		BOOL Lst(int iMethod);
		// ��Ʒ���� - ֲ��ָ��
		BOOL Vix(int iProductID);
		// ��Ʒ���� - �ɺ�ָ��
		BOOL Dix(int iProductID);
		// ������ - SIM (Single Item Monitor)
		BOOL Sim(int iProductID);

		// ȡ����Ԫ����
		int GetPixelType(int i,int j);
		// �Ƿ����(����)
		BOOL IsClearNight(int i,int j);

		// ����TVDI
		BOOL Dix_Tvdi(KDataInfo* pDataInfo,float* pDataNdvi,float* pDataLst,float*& pDataTvdi);

		// ������Ʒ
		virtual LPCTSTR NameProduct(int iProductID);

	public:
		///------------------------------------------------
		///���麯�� - Beg
		///---------------
		// ���������Ϣ
		virtual void FillDataInfo(LPVOID lpvData) = 0;
		// ӳ��ͨ��(ͨ���ź�����������Ӧ)
		virtual void MapChannel() = 0;
		// ����ͨ������ȡ��ͨ����������
		virtual int GetDataIndex(int iChannelCode) = 0;
		// ͨ������ת��Ϊͨ������
		virtual int ChannelName2ChannelCode(LPCTSTR lpszChannelName) = 0;

		// ȡ���б���ֵ
		virtual void* GetThreshold() = 0;
		// ����Ĭ�ϵ���ʾͨ��
		virtual void SetDefaultDisplayChannel() = 0;
		// ͨ����ת��Ϊͨ������
		virtual int Channel2ChannelCode(int iChannel) = 0;
		// ͨ������ת��Ϊͨ����
		virtual int ChannelCode2Channel(int iChannelCode) = 0;
		// ͨ������ת��Ϊͨ������
		virtual LPCTSTR ChannelCode2String(int iChannelCode) = 0;

		// �ǰ�������
		virtual BOOL IsDay() = 0;
		// ����������
		virtual BOOL IsNight() = 0;
		///---------------
		///���麯�� - Beg
		///------------------------------------------------

	public:

	protected:
		BOOL SaveData_LD2(LPCTSTR lpszFile);
		BOOL SaveData_LD3(LPCTSTR lpszFile);

	protected:
		//-------------------------------------------------
		// ͨ�����ݺͲ�Ʒ����
		PWORD* m_ppwData;			// ����ͨ������
		PBYTE* m_ppucDataGray;		// ����ͨ�����ݻҶ�
		
		PVOID m_pFileHeader;		// �ļ�ͷ
		//-------------------------------------------------
		int m_iRChannel;			//��ɫͨ��
		int m_iGChannel;			//��ɫͨ��
		int m_iBChannel;			//��ɫͨ��

		int m_iRChannelDef;			//��ɫͨ��Ĭ��
		int m_iGChannelDef;			//��ɫͨ��Ĭ��
		int m_iBChannelDef;			//��ɫͨ��Ĭ��
		//-------------------------------------------------
		// ͨ������
		TCHAR m_szChannel[MAX_PATH];

		// �����ļ�
		TCHAR m_szDayFile[MAX_PATH];
		// �����ļ�
		TCHAR m_szNightFile[MAX_PATH];

		BOOL m_bEmpty;
		//-------------------------------------------------
		TCHAR m_szTemp[MAX_PATH];

	private:
		// ͨ��ӳ��
		AgmrsDataChannelMap m_AgmrsDataChannelMap;
		// ͨ����Ϣ����
		ChannelInfoVector m_ChannelInfoVector;

#pragma region --��̬��Ա--

	public:	// static function begin

		// ȡ����Ԫ�����(km^2)
		static double GetPixelArea(double lat, double resolution);

		static BOOL IsLd2File(LPCTSTR lpszFile);
		static BOOL IsLd3File(LPCTSTR lpszFile);

	public:
		static BOOL IsAgmrsData(LPCTSTR lpszFile);
		static int GetAgmrsDataSource(LPCTSTR lpszFile);
		static int GetAgmrsDataSource(WORD wSatelliteID);

		static BOOL IsAvhrrData(LPCTSTR lpszFile)	{ return AgmrsDataSource::Avhrr	== GetAgmrsDataSource(lpszFile); }
		static BOOL IsModisData(LPCTSTR lpszFile)	{ return AgmrsDataSource::Modis	== GetAgmrsDataSource(lpszFile); }
		static BOOL IsMersiData(LPCTSTR lpszFile)	{ return AgmrsDataSource::Mersi	== GetAgmrsDataSource(lpszFile); }
		static BOOL IsVirrData(LPCTSTR lpszFile)	{ return AgmrsDataSource::Virr	== GetAgmrsDataSource(lpszFile); }

	public:
		static KAvhrrThreshold s_AvhrrThreshold;
		static KAvhrrThreshold* GetAvhrrThreshold();
		static void SetAvhrrThreshold(KAvhrrThreshold* pAvhrrThreshold);

		static KModisThreshold s_ModisThreshold;
		static KModisThreshold* GetModisThreshold();
		static void SetModisThreshold(KModisThreshold* pModisThreshold);

		static KMersiThreshold s_MersiThreshold;
		static KMersiThreshold* GetMersiThreshold();
		static void SetMersiThreshold(KMersiThreshold* pMersiThreshold);

		static KVirrThreshold s_VirrThreshold;
		static KVirrThreshold* GetVirrThreshold();
		static void SetVirrThreshold(KVirrThreshold* pVirrThreshold);


	public:
		static KStdPalette		s_StdPalette;
		static KIdxPalette		s_IdxPalette;
		static KRbwPalette		s_RbwPalette;
		static KSegPalette		s_SegPalette;
		static KLinearPalette	s_LinearPalette;

		// ȡ��ũ��Ҫ�ɲ�Ʒ��ɫ��
		static KPalette* GetAgmrsProductPalette(int iProductCode, PaletteType ePaletteType = PaletteType_Segment);

#pragma endregion

	};

//---------------------------------------------------------
}// namespace Agmrs - end
//---------------------------------------------------------

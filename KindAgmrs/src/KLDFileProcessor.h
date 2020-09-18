#pragma once

#include "KAgmrsDataProcessor.h"

namespace Agmrs { namespace LDFile
{

	class KIND_EXT_CLASS KLDFileProcessor : public KAgmrsDataProcessor
	{
	public:
		KLDFileProcessor(void);
		~KLDFileProcessor(void);

	public:
		// ����ת��Ϊ DIB
		virtual BOOL DataToDIB(int iRChannel = 0,int iGChannel = 0,int iBChannel = 0);

	protected:
		// ����ת��Ϊ�Ҷ�
		void Data2Gray();
		// ����ͨ������ת��Ϊ�Ҷ����ݵĲ���
		void CalcData2GrayParam();
		// ͨ������ת��Ϊ�Ҷ�
		void ChannelData2Gray(Data2GrayParam* pData2GrayParam);

	public:
		// ��Ʒ���� - ֲ��ָ��
		virtual BOOL Vix(int iProductID);
		// ��Ʒ���� - �ɺ�ָ��
		virtual BOOL Dix(int iProductID);

		// ������ - SIM (Single Item Monitor)
		virtual BOOL Sim(int iProductID) { return FALSE; }

		// ȡ����Ԫ����
		virtual int GetPixelType(int i,int j){ return PixelType::Unknown; }
		// �Ƿ����(����)
		virtual BOOL IsClearNight(int i,int j) { return FALSE; }

		// �ϲ�����ͨ��
		virtual BOOL ChannelCombine(ChannelCombineInfo* pChannelCombineInfo1,
			                        ChannelCombineInfo* pChannelCombineInfo2,
									LPCTSTR lpszOutputFile,
									KExtents* pExtents) { return FALSE; }

		// ��Ʒ���� - �ر��¶�
		virtual BOOL Lst(int iMethod) { return FALSE; }
		// ��Ʒ���� - ȫ����
		virtual BOOL Abe(int iProductID) { return FALSE; }

	protected:
		virtual BOOL Dix_Def(int iProductID) { return FALSE; }
		virtual BOOL Dix_Ati(int iProductID) { return FALSE; }
		virtual BOOL Dix_Tvdi(int iProductID);
		virtual BOOL Dix_Pdi(int iProductID) { return FALSE; }
		virtual BOOL Dix_Mpdi(int iProductID) { return FALSE; }
		virtual BOOL Dix_Vswi(int iProductID) { return FALSE; }
	};

//---------------------------------------------------------
}}// namespace Agmrs::LDFile - end
//---------------------------------------------------------

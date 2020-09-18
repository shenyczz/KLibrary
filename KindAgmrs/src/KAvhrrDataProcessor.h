#pragma once

#include "KLDFileProcessor.h"

namespace Agmrs { namespace Avhrr
{

	class KIND_EXT_CLASS KAvhrrDataProcessor : public KLDFileProcessor
	{
	public:
		KAvhrrDataProcessor(void);
		virtual ~KAvhrrDataProcessor(void);

	public:
		// ����ת��Ϊ DIB
		virtual BOOL DataToDIB(int iRChannel = 0,int iGChannel = 0,int iBChannel = 0);

	public:
		// ��Ʒ���� - ȫ����
		virtual BOOL Abe(int iProductID) { return FALSE; }
		// ��Ʒ���� - �ر��¶�
		virtual BOOL Lst(int iMethod) { return FALSE; }
		// ��Ʒ���� - ֲ��ָ��
		virtual BOOL Vix(int iProductID);
		// ��Ʒ���� - �ɺ�ָ��
		virtual BOOL Dix(int iProductID) { return FALSE; }
		// ������ - SIM (Single Item Monitor)
		virtual BOOL Sim(int iProductID);

		// ȡ����Ԫ����
		virtual int GetPixelType(int i,int j);
		// �Ƿ����(����)
		virtual BOOL IsClearNight(int i,int j) { return FALSE; }

	private:
		// ����ת��Ϊ�Ҷ�
		void Data2Gray();
		// ����ͨ������ת��Ϊ�Ҷ����ݵĲ���
		void CalcData2GrayParam();
		// ͨ������ת��Ϊ�Ҷ�
		void ChannelData2Gray(Data2GrayParam* pData2GrayParam);
	};

//---------------------------------------------------------
}}// namespace Agmrs::Avhrr - end
//---------------------------------------------------------

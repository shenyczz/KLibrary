#pragma once

#include "KLDFile.h"

namespace Agmrs { namespace Avhrr
{

	class KIND_EXT_CLASS KAvhrrData : public KLDFile
	{
	public:
		KAvhrrData(void);
		virtual ~KAvhrrData(void);

	public:
		// ȡ���б���ֵ
		virtual void* GetThreshold(){ return GetAvhrrThreshold(); }

	protected:
		// װ������
		virtual BOOL LoadData_bin(LPCTSTR lpszFile);
		private:
		BOOL LoadData_bin_ld2(LPCTSTR lpszFile);
		BOOL LoadData_bin_ld3(LPCTSTR lpszFile);

	public:
		// ӳ��ͨ��(ͨ���ź�����������Ӧ)
		//virtual void MapChannel();
		// ����ͨ����������ͨ����������
		//virtual int GetDataIndex(int iChannelCode);

		// ����Ĭ�ϵ���ʾͨ��
		virtual void SetDefaultDisplayChannel();
		// ͨ����ת��Ϊͨ������
		virtual int Channel2ChannelCode(int iChannel);
		// ͨ������ת��Ϊͨ����
		virtual int ChannelCode2Channel(int iChannelCode);
		// ͨ��ת��Ϊ�ַ���
		virtual LPCTSTR ChannelCode2String(int iChannelCode);
	};

//---------------------------------------------------------
}}// namespace Agmrs::Avhrr - end
//---------------------------------------------------------

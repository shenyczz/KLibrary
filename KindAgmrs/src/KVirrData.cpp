#include "StdAfx.h"
#include "KVirrData.h"
#include "KVirrDataProcessor.h"

namespace Agmrs { namespace Virr
{

	KVirrData::KVirrData(void)
	{
		this->SetType(AgmrsDataSource::Virr);

		m_pDataProcessor = new KVirrDataProcessor();
		m_pDataProcessor->SetOwner(this);
	}

	KVirrData::~KVirrData(void)
	{
	}

	// ����Ĭ�ϵ���ʾͨ��
	void KVirrData::SetDefaultDisplayChannel()
	{
		KLDFile::SetDefaultDisplayChannel();

		// ������Ϣ
		KDataInfo* pDataInfo = GetDataInfo();

		// ͨ������
		int iChannelNums = pDataInfo->ChannelNumbers();

		if(this->IsDay())
		{// ����
			SetDataChannelDef(1,2,1);
		}
		else
		{// ����
			int ch = this->ChannelCode2Channel(pDataInfo->ChannelCode(0));
			SetDataChannelDef(ch,ch,ch);
			SetDataChannelDef(3,4,3);
		}

		return;
	}

	// ͨ����ת��Ϊͨ������
	int KVirrData::Channel2ChannelCode(int iChannel)
	{
		return iChannel + 2000;
	}

	// ͨ������ת��Ϊͨ����
	int KVirrData::ChannelCode2Channel(int iChannelCode)
	{
		return iChannelCode - 2000;
	}

	// ͨ��ת��Ϊ�ַ���
	LPCTSTR KVirrData::ChannelCode2String(int iChannelCode)
	{
		KDataInfo* pDataInfo = this->GetDataInfo();
		int iChannelNums = pDataInfo->ChannelNumbers();
		_stprintf(m_szChannel,_T("%2d ͨ��"),this->ChannelCode2Channel(iChannelCode));
		return m_szChannel;
	}

//---------------------------------------------------------
}}// namespace Agmrs::Virr - end
//---------------------------------------------------------

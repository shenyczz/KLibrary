#include "StdAfx.h"
#include "KModisData.h"
#include "KModisDataProcessor.h"

namespace Agmrs { namespace Modis 
{

	KModisData::KModisData(void)
	{
		this->SetType(AgmrsDataSource::Modis);

		m_pDataProcessor = new KModisDataProcessor();
		m_pDataProcessor->SetOwner(this);
	}

	KModisData::~KModisData(void)
	{
	}

	// ����Ĭ�ϵ���ʾͨ��
	void KModisData::SetDefaultDisplayChannel()
	{
		KLDFile::SetDefaultDisplayChannel();

		// ������Ϣ
		KDataInfo* pDataInfo = GetDataInfo();

		// ͨ������
		int iChannelNums = pDataInfo->ChannelNumbers();

		if(IsDay())
		{// ����
			if(iChannelNums==1)
			{
				int ch = pDataInfo->ChannelCode(0);
				SetDataChannelDef(ch,ch,ch);
			}
			else if(iChannelNums==2)
			{
				int ch1 = pDataInfo->ChannelCode(0);
				int ch2 = pDataInfo->ChannelCode(1);
				SetDataChannelDef(ch1,ch2,ch1);
			}
			else
			{
				SetDataChannelDef(1,2,1);
			}
		}
		else
		{// ����
			if(iChannelNums==38)
			{
				SetDataChannelDef(32,32,31);
			}
			else
			{
				int ch = pDataInfo->ChannelCode(0);
				SetDataChannelDef(ch,ch,ch);
			}
		}

		return;
	}


	// �� 1-36 ת��Ϊ 1-38
	// 1 2 .............. 12     13           14      15 ................ 36		// ϰ��ͨ��
	// 1 2 .............. 12  [13 + 14]   [15 + 16]   17 ................ 38		// ʵ��ͨ��
	// ͨ����ת��Ϊͨ������
	int KModisData::Channel2ChannelCode(int iChannel)
	{
		int ch36 = iChannel;
		int ch38(-1);

		if(ch36<=12) ch38 = ch36;
		if(ch36>=15) ch38 = ch36+2;

		// 13 14 -> 13l 13h
		// 15 16 -> 14l 14h
		if(ch36==13) ch38 = 13;	// or 14
		if(ch36==14) ch38 = 15;	// or 16

		return ch38;
	}

	// �� 1-38 ת��Ϊ 1-36
	// 1 2 .............. 12     13           14      15 ................ 36		// ϰ��ͨ��
	// 1 2 .............. 12  [13 + 14]   [15 + 16]   17 ................ 38		// ʵ��ͨ��
	// ͨ������ת��Ϊͨ����
	int KModisData::ChannelCode2Channel(int iChannelCode)
	{
		int ch36(-1);
		int ch38 = iChannelCode;

		if(ch38<=12) ch36 = ch38;
		if(ch38>=17) ch36 = ch38-2;

		// 13 14 -> 13l 13h
		// 15 16 -> 14l 14h
		if(ch38==13 || ch38==14) ch36 = 13;
		if(ch38==15 || ch38==16) ch36 = 14;

		return ch36;
	}

	// ͨ������ת��Ϊͨ������(ch38 -> Channelname)
	LPCTSTR KModisData::ChannelCode2String(int iChannelCode)
	{
		KDataInfo* pDataInfo = this->GetDataInfo();
		int iChannelNums = pDataInfo->ChannelNumbers();
		//--------------------------------------------
		int ch38 = iChannelCode;
		if(ch38<=12)
		{
			_stprintf(m_szChannel,_T("%2d ͨ��"),ch38);
		}
		else if(ch38>=17)
		{
			_stprintf(m_szChannel,_T("%2d ͨ��"),ch38-2);
		}
		else
		{
			if(ch38==13) _stprintf(m_szChannel,_T("13lo ͨ��"));
			if(ch38==14) _stprintf(m_szChannel,_T("13hi ͨ��"));

			if(ch38==15) _stprintf(m_szChannel,_T("14lo ͨ��"));
			if(ch38==16) _stprintf(m_szChannel,_T("14hi ͨ��"));
		}

		//--------------------------------------------
		return m_szChannel;
	}

}}// namespace Agmrs::Modis - end
//---------------------------------------------------------

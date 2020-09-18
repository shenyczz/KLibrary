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

	// 设置默认的显示通道
	void KVirrData::SetDefaultDisplayChannel()
	{
		KLDFile::SetDefaultDisplayChannel();

		// 数据信息
		KDataInfo* pDataInfo = GetDataInfo();

		// 通道数量
		int iChannelNums = pDataInfo->ChannelNumbers();

		if(this->IsDay())
		{// 白天
			SetDataChannelDef(1,2,1);
		}
		else
		{// 晚上
			int ch = this->ChannelCode2Channel(pDataInfo->ChannelCode(0));
			SetDataChannelDef(ch,ch,ch);
			SetDataChannelDef(3,4,3);
		}

		return;
	}

	// 通道号转换为通道代码
	int KVirrData::Channel2ChannelCode(int iChannel)
	{
		return iChannel + 2000;
	}

	// 通道代码转换为通道号
	int KVirrData::ChannelCode2Channel(int iChannelCode)
	{
		return iChannelCode - 2000;
	}

	// 通道转换为字符串
	LPCTSTR KVirrData::ChannelCode2String(int iChannelCode)
	{
		KDataInfo* pDataInfo = this->GetDataInfo();
		int iChannelNums = pDataInfo->ChannelNumbers();
		_stprintf(m_szChannel,_T("%2d 通道"),this->ChannelCode2Channel(iChannelCode));
		return m_szChannel;
	}

//---------------------------------------------------------
}}// namespace Agmrs::Virr - end
//---------------------------------------------------------

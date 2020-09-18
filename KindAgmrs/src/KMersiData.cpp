#include "StdAfx.h"
#include "KMersiData.h"
#include "KMersiDataProcessor.h"

namespace Agmrs { namespace Mersi
{

	KMersiData::KMersiData(void)
	{
		this->SetType(AgmrsDataSource::Mersi);

		m_pDataProcessor = new KMersiDataProcessor();
		m_pDataProcessor->SetOwner(this);
	}

	KMersiData::~KMersiData(void)
	{
	}

	// 设置默认的显示通道
	void KMersiData::SetDefaultDisplayChannel()
	{
		KLDFile::SetDefaultDisplayChannel();

		// 数据信息
		KDataInfo* pDataInfo = GetDataInfo();

		// 通道数量
		int iChannelNums = pDataInfo->ChannelNumbers();

		if(this->IsDay())
		{// 白天
			SetDataChannelDef(3,4,3);
		}
		else
		{// 晚上
			int ch = this->ChannelCode2Channel(pDataInfo->ChannelCode(0));
			SetDataChannelDef(ch,ch,ch);
		}

		return;
	}

	// 通道转换为通道代码
	int KMersiData::Channel2ChannelCode(int iChannel)
	{
		return iChannel + 50;
	}

	// 通道代码转换为通道
	int KMersiData::ChannelCode2Channel(int iChannelCode)
	{
		return iChannelCode - 50;
	}

	// 通道转换为字符串
	LPCTSTR KMersiData::ChannelCode2String(int iChannelCode)
	{
		KDataInfo* pDataInfo = this->GetDataInfo();
		int iChannelNums = pDataInfo->ChannelNumbers();
		_stprintf(m_szChannel,_T("%2d 通道"),ChannelCode2Channel(iChannelCode));
		return m_szChannel;
	}

//---------------------------------------------------------
}}// namespace Agmrs::Mersi - end
//---------------------------------------------------------

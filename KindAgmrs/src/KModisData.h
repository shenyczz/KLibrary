/******************************************************************************
 ** KModisData.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KModisData - MODIS ����
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

		// 1. ���ɶ���
		KModisData* pModisData = new KModisData();
		// 1.1 װ������
		pModisData->LoadData(pszFile);

		// 2. ���ݴ�����
		KModisDataProcessor* pModisDataProcessor = (KModisDataProcessor*)pModisData->GetDataProcessor();
		// 2.1 ����ת��Ϊͼ��
		pModisDataProcessor->DataToDIB();
		// 2.2 ���ɲ�Ʒ
		pModisDataProcessor->Lst(iMethod);

		// 3. ��Ʒ����
		PWORD* ppwProduct = pAgmrsDataProcessor->GetProductData();
		KDataInfo* pDataInfo = pAgmrsData->GetDataInfo();
		KdHelper::CreateKdFile(fileName, *ppwProduct, pDataInfo, palFile)

******************************************************************************/

#pragma once

#include "KLDFile.h"

namespace Agmrs { namespace Modis 
{

	class KIND_EXT_CLASS KModisData : public KLDFile
	{
	public:
		KModisData(void);
		virtual ~KModisData(void);

	public:
		// ȡ���б���ֵ
		virtual void* GetThreshold() { return GetModisThreshold(); }

	public:
		// ����Ĭ�ϵ���ʾͨ��
		virtual void SetDefaultDisplayChannel();
		///===========================================
		// 1 2 .............. 12     13           14      15 ................ 36		// ϰ��ͨ��
		// 1 2 .............. 12  [13 + 14]   [15 + 16]   17 ................ 38		// ʵ��ͨ��
		// 0 1 .............. 11  [12 + 13]   [14 + 15]   16 ................ 37		// ����ͨ��
		///===========================================
		// ͨ����ת��Ϊͨ������(1-36 to 1-38)
		virtual int Channel2ChannelCode(int iChannel);
		// ͨ������ת��Ϊͨ����(1-38 to 1-36)
		virtual int ChannelCode2Channel(int iChannelCode);
		// ͨ������ת��Ϊͨ������
		LPCTSTR ChannelCode2String(int iChannelCode);

	};

//---------------------------------------------------------
}}// namespace Agmrs::Modis - end
//---------------------------------------------------------

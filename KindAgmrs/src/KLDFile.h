/******************************************************************************
 ** KLDFile.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KLDFile - ũҵ����ң�����ݱ����ļ�
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
/******************************************************************************
����ͨ����ͨ�����롢����������˵��
ͨ��		- Channel
ͨ������	- ChannelCode
��������	- DataIndex

��ͨ�����ǿ�����Ա���е�����ͨ��
��ͨ�����롿�ֵ��ļ�ͨ�����������м�¼�Ĵ��롣Mersi ��1ͨ����ͨ��������51
�������������Ǳ������ڴ��еĶ�Ӧͨ������������׵�ַ������ֵ
******************************************************************************/

#pragma once

#include "KAgmrsData.h"

namespace Agmrs { namespace LDFile
{

	class KIND_EXT_CLASS KLDFile : public KAgmrsData
	{
	public:
		KLDFile(void);
		~KLDFile(void);

	protected:
		// װ������
		virtual BOOL LoadData_bin(LPCTSTR lpszFile);
		private:
			BOOL LoadData_bin_ld2(LPCTSTR lpszFile);
			BOOL LoadData_bin_ld3(LPCTSTR lpszFile);

	public:
		// ���������Ϣ
		virtual void FillDataInfo(LPVOID lpvData);

	public:
		// �ǰ�������
		virtual BOOL IsDay() { return ((LDFHeaderCommon*)m_pFileHeader)->wDayOrNight == 0; }
		// ����������
		virtual BOOL IsNight() { return ((LDFHeaderCommon*)m_pFileHeader)->wDayOrNight == 1; }
		// ȡ���б���ֵ
		virtual void* GetThreshold() { return NULL; }

	public:
		// ӳ��ͨ��(ͨ���ź�����������Ӧ)
		virtual void MapChannel();
		// ����Ĭ�ϵ���ʾͨ��
		virtual void SetDefaultDisplayChannel();
		// ����ͨ������ȡ��ͨ����������
		virtual int GetDataIndex(int iChannelCode);
		// ͨ������ת��Ϊͨ����(1-38)
		int ChannelName2ChannelCode(LPCTSTR lpszChannelName);
	};

//---------------------------------------------------------
}}// namespace Agmrs::LDFile - end
//---------------------------------------------------------

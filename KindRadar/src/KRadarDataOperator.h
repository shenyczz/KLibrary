/******************************************************************************
 ** KRadarDataOperator.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.
              All rights reserved.

 **     Name: KRadarDataOperator - �״����ݲ�����
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

class KIND_EXT_CLASS KRadarDataOperator : public KDataProcessor
{
public:
	KRadarDataOperator(void);
	virtual ~KRadarDataOperator(void);

public:
	// ��������(��1970.1.1���������)ת����������������
	void JulianDate2GMTDate(LONG lJDate,int* iYear,int* iMonth,int* iDay);
	// ������ʱ��ת����24Сʱ��ʱ��
	void JulianTime2GMTTime(LONG lJTime,int* iHour,int* iMinute,int* iSecond);

public:
	// ����/��ȡ�ļ�ָ��
	void SetFilePtr(FILE* fp)	{ m_fp = fp; }
	FILE* GetFilePtr()			{ return m_fp; }

	// ����/��ȡ�ļ�ָ��ƫ�Ƶ�ַ
	void SetFilePtrOffset(LONG lOffset)	{ m_lOffset = lOffset; }
	LONG GetFilePtrOffset()				{ return m_lOffset; }

protected:
	// �ļ�ָ��
	FILE* m_fp;
	// �ļ�ָ��ƫ��
	LONG m_lOffset;
	//�Ҷ�ͼ
	KDib m_dibGray;

};

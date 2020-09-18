/******************************************************************************
 ** KGridContourData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KGridContourData
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: ������.֣�� (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

#include "KGridData.h"

class KIND_EXT_CLASS KGridContourData : public KGridData
{
public:
	KGridContourData(void);
	virtual ~KGridContourData(void);

public:
	// �����ֵ��
	virtual BOOL BuildContours();

public:
	// ȡ�õ�ֵ�߹�����
	KContourBuilder* GetContourBuilder() { return m_pContourBuilder; }

protected:
	// װ������ - ASC�ļ�
	virtual BOOL LoadData_txt(LPCTSTR lpszFile);

protected:
	// ��ֵ�߹�����
	KContourBuilder* m_pContourBuilder;
};


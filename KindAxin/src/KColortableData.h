/******************************************************************************
 ** KColortableData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KColortableData
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

#include "KAxinData.h"

class KIND_EXT_CLASS KColortableData : public KAxinData
{
public:
	KColortableData(void);
	virtual ~KColortableData(void);

public:
	// ���������Ϣ
	virtual void FillDataInfo(LPVOID lpvData);

protected:
	// װ������ - ASC�ļ�
	virtual BOOL LoadData_txt(LPCTSTR lpszFile);
};


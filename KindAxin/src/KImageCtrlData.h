/******************************************************************************
 ** KImageCtrlData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KImageCtrlData
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

class KIND_EXT_CLASS KImageCtrlData : public KAxinData
{
public:
	KImageCtrlData(void);
	virtual ~KImageCtrlData(void);

protected:
	// ����װ������
	virtual BOOL LoadData_txt(LPCTSTR lpszFile);
};


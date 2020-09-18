/******************************************************************************
 ** KWindVectorData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KWindVectorData - �糡ʸ������
 **  Version: 
 ** Function: ���ݷ糡���ݵ�U��V�������Ʒ糡���ߵ�
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

class KIND_EXT_CLASS KWindVectorData : public KGridData
{
public:
	KWindVectorData(void);
	virtual ~KWindVectorData(void);

protected:
	// ����װ������
	virtual BOOL LoadData_txt(LPCTSTR lpszFile);

private:
	// �������������ݼ�ֵ
	void LookupExtremum();
};


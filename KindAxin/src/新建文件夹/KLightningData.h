/******************************************************************************
 ** KLightningData.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KLightningData - ��������
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

#include "KSnycData.h"

class KIND_EXT_CLASS KLightningData : public KSnycData
{
public:
	KLightningData(void);
	virtual ~KLightningData(void);

public:
	// ������ݲ������ڴ�
	virtual void Empty();
	virtual BOOL IsEmpty();

public:
	KLightningCollection* GetLightningCollection() { return m_pLightningCollection; }

protected:
	virtual BOOL LoadData_txt(LPCTSTR lpszFile);

private:
	// ����󶨾���
	void ComputeBoundingBox();

private:
	KLightningCollection* m_pLightningCollection;
};

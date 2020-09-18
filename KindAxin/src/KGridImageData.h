/******************************************************************************
 ** KGridImageData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KGridImageData
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

class KIND_EXT_CLASS KGridImageData : public KGridData
{
public:
	KGridImageData(void);
	virtual ~KGridImageData(void);

public:
	// �ļ�ͷ
	PCHAR FileHeader() { return m_pFileHeader; }
	
public:
	// ȡ������ƫ�Ƶ�ַ
	DWORD GetDataOffset() { return m_dwDataOffset; }

protected:
	// װ������
	virtual BOOL LoadData_bin(LPCTSTR lpszFile);

private:
	// ���������Ϣ
	void FillDataInfo(LPVOID lpvData);

protected:
	// �ļ�ͷ
	PCHAR m_pFileHeader;

	// ����ƫ��
	DWORD m_dwDataOffset;

	// ��ɫ�弯��
	TPtrCollection<KPalette*>* m_pPaletteCollection;
};

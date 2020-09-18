/******************************************************************************
 ** KAxinData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KDiamondData
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

class KIND_EXT_CLASS KDiamondData : public KCustomDataObject
{
public:
	KDiamondData(void);
	virtual ~KDiamondData(void);

public:
	// ������ɫ��
	void BuildPalette(LPCTSTR lpszFile = NULL);

protected:
	// ���������Ϣ
	virtual void FillDataInfo(LPVOID lpvData) {}

public:
	static bool IsDiamondData(LPCTSTR lpszFile);
	static int GetFormatCode(LPCTSTR lpszFile);
};


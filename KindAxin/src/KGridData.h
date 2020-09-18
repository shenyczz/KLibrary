/******************************************************************************
 ** KGridData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KGridData
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

class KIND_EXT_CLASS KGridData : public KAxinData
{
public:
	KGridData(void);
	virtual ~KGridData(void);

public:
	// �������ݼ�Сֵ�ͼ���ֵ
	virtual void LookupExtremum();

public:
	// ��γ������ת��Ϊ�������
	BOOL LonLat2xy(double lon,double lat,int& x,int& y);
	// �������ת��Ϊ��γ������
	BOOL xy2LonLat(int x,int y,double& lon,double& lat);

	double GetGridValue(int row,int col);
	BOOL SetGridValue(int row,int col,double value);

	double GetGridValue(double lon,double lat);
	BOOL SetGridValue(double lon,double lat,double value);

public:
	// ���� ENVI �ļ�ͷ����
	BOOL ExportEnviHdr(LPCTSTR lpszFile);

	// ���� ArcGis bmp & bmpw
	BOOL ExportArcGis_bmp_bmpw(LPCTSTR lpszFile);
};


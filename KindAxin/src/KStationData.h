/******************************************************************************
 ** KStationData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KStationData
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

class KIND_EXT_CLASS KStationData : public KAxinData
{
public:
	KStationData(void);
	virtual ~KStationData(void);

public:
	// ������ݲ������ڴ�
	virtual void Empty();
	virtual BOOL IsEmpty();

protected:
	// װ������ - ASC�ļ�
	virtual BOOL LoadData_txt(LPCTSTR lpszFile);

public:
	// �����ֵ��
	BOOL BuildContours();

	// �Ƿ�׷�ٵ�ֵ��
	BOOL IsTarceContour();

	// ȡ��վ��Ҫ������
	int GetElementNumber();

public:
	// ��ֵ���Ƿ�׼����
	BOOL IsContourReady() { return m_bContourReady; }

	// ��ֵ���Ƿ����¹���
	BOOL IsRebuildContour() { return m_bRebuildContour; }
	void EnableRebuildContour(BOOL bRebuildContour = TRUE) { m_bRebuildContour = bRebuildContour; }

	// ��ǰҪ������
	int& CurrentElementIndex() { return m_iCurrentElementIndex; }

	// ȡ�õ�ֵ�߹�����
	KContourBuilder* GetContourBuilder() { return m_pContourBuilder; }

	// ȡ��վ����Ϣ����
	KObjectPtrCollection* GetStationInfoPtrCollection() { return m_pStationInfoPtrCollection; }

	// ��ֵ��¼���ֵ
	int& InterpolationRecordMax() { return m_InterpolationRecordMax; };

protected:
	// ����󶨾���
	void ComputeExtents();

	// ����SNYC���ݼ�Сֵ�ͼ���ֵ
	void LookupExtremum();

protected:
	// ��ֵ���Ƿ�׼����
	BOOL m_bContourReady;
	// ��ֵ���Ƿ�����׷��
	BOOL m_bRebuildContour;
	// ��ǰҪ������
	int m_iCurrentElementIndex;
	// ��ֵ�߹�����
	KContourBuilder* m_pContourBuilder;
	// ���󼯺�
	KObjectPtrCollection* m_pStationInfoPtrCollection;

	// ��ֵ��¼���ֵ
	int m_InterpolationRecordMax;

public:
	static BOOL SaveData(LPCTSTR pszFile,
		KDataInfo* pDataInfo,
		KObjectPtrCollection* pStationInfoPtrCollection,
		int iPrecision = 8,
		int iDecimal = 3);
};


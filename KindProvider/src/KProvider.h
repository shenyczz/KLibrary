/******************************************************************************
 ** KProvider.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KProvider
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

#include "KProgressObject.h"
#include "KDataObject.h"

enum ProviderType
{
	ProviderType_Unknown = 0,
	ProviderType_AgmrsData,
	ProviderType_Blackboard,
	ProviderType_DiamondData,
	ProviderType_AxinData,
	ProviderType_RadarData,
	ProviderType_ShapeFile,
};

class KIND_EXT_CLASS KProvider : public KProgressObject
{
public:
	KProvider(void);
	KProvider(const KProvider& provider);
	KProvider(LPCTSTR lpszConnectionString);
	virtual ~KProvider(void);

public:
	// ��
	virtual BOOL Open(LPCTSTR lpszConnectionString = NULL);
	// �ر�
	virtual void Close();

public:
	// �Ƿ��
	BOOL IsOpen() const										{ return m_bOpen; }

	// ���ݶ���
	KDataObject* GetDataObject()							{ return m_pDataObject; }
	void SetDataObject(KDataObject* pDataObject)			{ _delete(m_pDataObject); m_pDataObject = pDataObject; }

	// �����ַ���
	KString GetConnectionString() const						{ return m_ConnectionString; }
	void SetConnectionString(LPCTSTR lpszConnectionString)	{ m_ConnectionString = lpszConnectionString; }

	// ע��
	KString GetComment();

protected:
	// �Ƿ��
	BOOL m_bOpen;
	// �����ַ���
	KString m_ConnectionString;
	// ���ݶ���
	KDataObject* m_pDataObject;
};

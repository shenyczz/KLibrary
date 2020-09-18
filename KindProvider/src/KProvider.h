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
 **   Author: 申永辰.郑州 (shenyczz@163.com)
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
	// 打开
	virtual BOOL Open(LPCTSTR lpszConnectionString = NULL);
	// 关闭
	virtual void Close();

public:
	// 是否打开
	BOOL IsOpen() const										{ return m_bOpen; }

	// 数据对象
	KDataObject* GetDataObject()							{ return m_pDataObject; }
	void SetDataObject(KDataObject* pDataObject)			{ _delete(m_pDataObject); m_pDataObject = pDataObject; }

	// 连接字符串
	KString GetConnectionString() const						{ return m_ConnectionString; }
	void SetConnectionString(LPCTSTR lpszConnectionString)	{ m_ConnectionString = lpszConnectionString; }

	// 注释
	KString GetComment();

protected:
	// 是否打开
	BOOL m_bOpen;
	// 连接字符串
	KString m_ConnectionString;
	// 数据对象
	KDataObject* m_pDataObject;
};

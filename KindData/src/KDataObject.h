/******************************************************************************
 ** KDataObject.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KDataObject
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

#include "KDataProcessor.h"

// �ڴ����ö��
enum UVW
{
	eUvw_u = 1,		// U ����
	eUvw_v = 2,		// V ����
	eUvw_w = 4,		// W ����(DWORD)
	eUvw_uv = 3,	// UV
	eUvw_uw = 5,	// UW
	eUvw_vw = 6,	// VW
	eUvw_all = 7,	// UVW
};

class KIND_EXT_CLASS KDataObject : public KProgressObject
{
public:
	KDataObject(void);
	KDataObject(LPCTSTR lpszFile);
	virtual ~KDataObject(void);

public:
	// ������ݲ������ڴ�
	virtual void Empty();
	//���������Ƿ����(������Ҫ���������и�����Ҫȡ������)
	virtual BOOL IsEmpty();

	// װ������
	virtual BOOL LoadData(LPCTSTR lpszFile);

	// ��������
	virtual BOOL SaveData(LPCTSTR lpszFile);

protected:
	virtual BOOL LoadData_bin(LPCTSTR lpszFile);
	virtual BOOL LoadData_txt(LPCTSTR lpszFile);


public:
	// ����ͼ��
	virtual BOOL BuildImage(COLORREF clrBack,LPCTSTR lpszFile = NULL);

	// Ӧ�õ�ɫ��
	virtual BOOL UsePalette(KPalette* pPalette);

	// ��Ч����
	BOOL IsInvalidData(double value);
	// �ض�ֵ
	BOOL IsSpecialData(double value);

public:
	// ȡ��������Ϣ
	KDataInfo* GetDataInfo() { return m_pDataInfo; }
	// ���ݴ�����
	KDataProcessor* GetDataProcessor() { return m_pDataProcessor; }

	// �ļ���
	LPCTSTR GetFileName() { return m_strFileName; }
	void SetFileName(LPCTSTR lpszFile) { m_strFileName = lpszFile; }

	// Gdiplus Image
	Image* GetImage();

	// ��ɫ��
	KPalette* GetPalette() { return m_pPalette; }
	void SetPalette(KPalette* pPalette)	{ _delete(m_pPalette); m_pPalette = pPalette;	}

public:
	//ȡ��x����ָ��
	double* GetXData() { return m_px; }
	//ȡ��x��������
	double GetXData(int i) { return *(m_px+i); }

	//ȡ��y����ָ��
	double* GetYData() { return m_py; }
	//ȡ��y��������
	double GetYData(int i) { return *(m_py+i); }

	//ȡ��z����ָ��
	double* GetZData() { return m_pz; }
	//ȡ��z��������
	double GetZData(int i) { return *(m_pz+i); }

	//ȡ��U����ָ��
	double* GetUData() { return m_pu; }
	//ȡ��u��������
	double GetUData(int i) { return *(m_pu+i); }

	//ȡ��V����ָ��
	double* GetVData() { return m_pv; }
	//ȡ��v��������
	double GetVData(int i) { return *(m_pv+i); }

	//ȡ��W����ָ��
	double* GetWData() { return m_pw; }
	//ȡ��w��������
	double GetWData(int i) { return *(m_pw+i); }

	// ��Ϣ
	LPCTSTR GetMessage() { return m_pszMessage; }
	void SetMessage(LPCTSTR lpszMessage)  { KString::SetString(m_pszMessage,lpszMessage); }

	// ����Ԫ�ص�λ
	virtual LPCTSTR GetUnit() { return m_pszUint; }
	void SetUnit(LPCTSTR lpszUnit) { KString::SetString(m_pszUint,lpszUnit); }

public:
	// �������ݼ�Сֵ�ͼ���ֵ
	virtual void LookupExtremum() { }

protected:
	// Ϊ���ݷ����ڴ�
	void PrepareMemory(UVW eUvw);

protected:
	// ���������Ϣ
	virtual void FillDataInfo(LPVOID lpvData) = 0;

protected:
	// x����ָ��
	double* m_px;
	// y����ָ��
	double* m_py;
	// z����ָ��
	double* m_pz;

	// �糡U����ָ��
	double* m_pu;
	// �糡V����ָ��/����ָ��
	double* m_pv;
	// DWORD����ָ��
	double* m_pw;

	// ����Ԫ�ص�λ
	TCHAR* m_pszUint;
	// ������Ϣ
	TCHAR* m_pszMessage;

protected:
	// ��ɫ��
	KPalette* m_pPalette;
	// �����ļ�����
	KString m_strFileName;

protected:
	// ������Ϣ
	KDataInfo* m_pDataInfo;
	// ���ݴ�����
	KDataProcessor* m_pDataProcessor;
};

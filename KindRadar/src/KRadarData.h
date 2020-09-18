/******************************************************************************
 ** KRadarData.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.
              All rights reserved.

 **     Name: KRadarData - �״�������
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

#include "KRadarDataOperator.h"

class KIND_EXT_CLASS KRadarData : public KCustomDataObject
{
public:
	KRadarData(void);
	virtual ~KRadarData(void);

public:
	// �������
	virtual void Empty();

	// װ������
	virtual BOOL LoadData(LPCTSTR lpszFile,LPVOID lpvData = 0);

	// ����ͼ��
	virtual BOOL SaveImage(LPCTSTR lpszFile);

	// ��ʼ����ɫ��
	virtual void InitColorTable(int iProductCode);


public:
	// �Ƿ�����ݼ�
	BOOL IsEmpty() { return m_bEmpty; }

	// ȡ��λͼ���
	void SetImageHandle(HBITMAP hBitmap);
	HBITMAP GetImageHandle() { return m_hBitmap; }

	// ����/��ȡͼ�񱳾���ɫ
	void SetImageBkColor(COLORREF clr) { m_clrBack = clr; }
	COLORREF GetImageBkColor() { return m_clrBack; }

	// ����ͼ��ߴ�
	void SetImageSize(DWORD dwSize) { m_dwWidth = m_dwHeight = dwSize; }

	// ����/��ȡͼ����
	void SetImageWidth(DWORD dwWidth) { m_dwWidth = dwWidth; }
	DWORD GetImageWidth() { return m_dwWidth; }

	// ����/��ȡͼ��߶�
	void SetImageHeight(DWORD dwHeight) { m_dwHeight = dwHeight; }
	DWORD GetImageHeight() { return m_dwHeight; }

	// ȡ��ͼ��
	Legend* GetLegend() { return &m_Legend[0]; }
	Legend* GetLegend(int index) { return &m_Legend[index]; }

	// ����/��ȡ�״��ͺ�
	void SetRadarType(RadarType eRadarType) { m_eRadarType = eRadarType; }
	RadarType GetRadarType() { return m_eRadarType; }

	// ��ȡ�״���Ϣ
	RadarInfo* GetRadarInfo() { return &m_RadarInfo; }

public:
	// ����
	void SetDate(sfx_uint16 year,sfx_uint16 month,sfx_uint16 day);
	void GetDate(sfx_uint16 &year,sfx_uint16 &month,sfx_uint16 &day);

	// ʱ��
	void SetTime(sfx_uint16 hour,sfx_uint16 minute,sfx_uint16 second);
	void GetTime(sfx_uint16 &hour,sfx_uint16 &minute,sfx_uint16 &second);

	// ���� ʱ��
	void SetDateTime(sfx_uint16 year,sfx_uint16 month,sfx_uint16 day,sfx_uint16 hour,sfx_uint16 minute,sfx_uint16 second);
	void GetDateTime(sfx_uint16 &year,sfx_uint16 &month,sfx_uint16 &day,sfx_uint16 &hour,sfx_uint16 &minute,sfx_uint16 &second);

	// �״����ݲ�����
	void SetRadarDataOperator(KRadarDataOperator* pRadarDataOperator) { m_pRadarDataOperator = pRadarDataOperator; }
	KRadarDataOperator* GetRadarDataOperator() { return m_pRadarDataOperator; }

protected:
	// ��д�״���Ϣ
	virtual void FillRadarInfo(RadarInfo* pRadarInfo);

protected:
	// ����
	sfx_uint16 m_iYear;
	sfx_uint16 m_iMonth;
	sfx_uint16 m_iDay;

	// ʱ��
	sfx_uint16 m_iHour;
	sfx_uint16 m_iMinute;
	sfx_uint16 m_iSecond;

	// �״��ͺ�
	RadarType m_eRadarType;

	// �״���Ϣ
	RadarInfo m_RadarInfo;

	// ͼ��
	HBITMAP		m_hBitmap;		// λͼ���
	COLORREF	m_clrBack;		// ͼ��ı�����ɫ
	DWORD		m_dwWidth;		// ͼ����
	DWORD		m_dwHeight;		// ͼ��߶�

	// ��ɫ���ͼ��
	COLORREF m_ColorTable[256];	// 256ɫ��ɫ��
	Legend m_Legend[256];		// ͼ��

	// �Ƿ�����ݼ�
	BOOL m_bEmpty;

	// ������λ
	TCHAR m_szUnit[32];

	// ������
	KRadarDataOperator* m_pRadarDataOperator;

	public:
		static bool IsRadarData(LPCTSTR lpszFile);
};

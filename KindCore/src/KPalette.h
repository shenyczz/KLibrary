/******************************************************************************
 ** KPalette.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KPalette
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

#include "KObject.h"
#include "KPaletteItem.h"

enum PaletteType : int
{
	PaletteType_Unknown		= 0,	// δ֪
	PaletteType_Standard	= 1,	// ��׼��ɫ��
	PaletteType_Index		= 2,	// ������ɫ��
	PaletteType_Rainbow		= 3,	// �ʺ��ɫ��
	PaletteType_Segment		= 4,	// �ֶε�ɫ��
	PaletteType_Linear		= 5,	// ���Ե�ɫ��
};

class KIND_EXT_CLASS KPalette : public KObject, public TPtrCollection<KPaletteItem*>
{
public:
	KPalette(void);
	virtual ~KPalette(void);

public:
	// ��¡
	virtual KPalette* Clone();
	// ��ɫ
	virtual COLORREF GetColor(double dValue,COLORREF clrDefault = 0);

public:
	// Ĭ��ɫ
	COLORREF& DefaultColor() { return m_clrDefault; }
	// ͸��ɫ
	COLORREF& TransparentColor() { return m_clrTransparent; }
	// �Ƿ���ע��
	BOOL& HasComment() { return m_bHasComment; }
	// �Ƿ���͸��ɫ
	BOOL& HasTransparentColor() { return m_bHasTransparentColor; }
	// ȡ�õ�ɫ����ɫ�������
	int GetColorEntries() { return this->Count(); }
	// �ض���ɫ����
	const int& SpecificColorNumbers() { return this->m_iSpecificColorNumbers; }
	// �ض�������Сֵ
	DWORD& SpecificDataMin()	{ return this->m_dwSpecificDataMin; }
	// �ض��������ֵ
	DWORD& SpecificDataMax()	{ return this->m_dwSpecificDataMax; }

public:
	// �����ɫ
	virtual BOOL AddColor(double dValue, COLORREF color, LPCTSTR pszComment = _T(""));
	virtual BOOL AddColor(double dValue, BYTE r, BYTE g, BYTE b, LPCTSTR pszComment = _T(""));
	virtual BOOL AddColor(double dValue, BYTE a, BYTE r, BYTE g, BYTE b, LPCTSTR pszComment = _T(""));
	virtual BOOL AddColor(const VARGB& vargb, LPCTSTR pszComment);

public:
	// ָ��ֵ������
	int FindIndex(double dValue);
	// ָ��ֵ�����ٽ�����
	int FindNearestIndex(double dValue);
	// ��Ч��Ŀ����
	int NumbersOfValidItem(double spv_min = 0x8000);

public:
	// ���ݷ�Χ(���������ݶȵ�ɫ��)
	void SetValueRange(double vMin,double vMax);
	void GetValueRange(double& vMin,double& vMax);

protected:
	// ȡ��������ɫ
	COLORREF GetIdxColor(double dValue,COLORREF clrDefault);
	// ȡ�÷ֶ���ɫ
	COLORREF GetSegColor(double dValue,COLORREF clrDefault);
	// ȡ�ö�����ɫ
	COLORREF GetDitColor(double dValue,COLORREF clrDefault);

	// ���� value ��λ���ҳ�����ɫ���ж�Ӧ����ɫ,
	COLORREF LookupColor(double value,COLORREF clrDefault);

protected:
	// ��Ч��Ŀ����(�ֶε�ɫ������)
	int m_iValidItemNumbers;
	// Ĭ��ɫ
	COLORREF m_clrDefault;
	// ͸��ɫ
	COLORREF m_clrTransparent;
	// �Ƿ���ע��
	BOOL m_bHasComment;
	// �Ƿ���͸��ɫ
	BOOL m_bHasTransparentColor;
	// �ض���ɫ����
	int m_iSpecificColorNumbers;
	// �ض�������Сֵ
	DWORD m_dwSpecificDataMin;
	// �ض��������ֵ
	DWORD m_dwSpecificDataMax;

protected:
	// ���ڲʺ��ɫ��
	double m_dValueMin;
	double m_dValueMax;
};

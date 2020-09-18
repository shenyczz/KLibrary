/******************************************************************************
 ** KDataObjectProcessor.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KDataObjectProcessor
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

#include "KDataAlgorithm.h"

class KIND_EXT_CLASS KDataProcessor : public KProgressObject
{
public:
	KDataProcessor(void);
	virtual ~KDataProcessor(void);

public:
	// Gdiplus Image
	virtual Image* GetImage();

	// �����㷨
	KDataAlgorithm* GetDataAlgorithm() { return m_pDataAlgorithm; }

public:
	// DIB
	KDib* GetDib() { return m_pDib; }
	void SetDib(KDib* pDib) { _delete(m_pDib); m_pDib = pDib; }

	// ͼ��λ
	void RestoreDib();
	// ����
	void ReverseDib();
	// ת��Ϊ�Ҷ�ͼ��
	void GrayDib();
	// ͼ����ɫ���� - RGB
	void AdjustColor_Rgb(double dRScale,double dGScale,double dBScale);
	// ͼ����ɫ���� - CMY
	void AdjustColor_Cmy();
	// ͼ����ɫ���� - HSI
	void AdjustColor_Hsi();
	// ͼ����ɫ���� - HLS
	void AdjustColor_Hls();
	// ͼ��ƽ��
	void SmoothDib();
	// ��������
	void BrightDib(double dBright);
	// �����Աȶ�
	void ContrastDib(double dContrast);
	// ֱ��ͼ����
	void HistogramAverageDib(BOOL bRChannel = TRUE, BOOL bGChannel = TRUE, BOOL bBChannel = TRUE);

	BOOL& IsDibRestore() { return m_bDibRestore; }
	BOOL& IsDibChanged() { return m_bDibChanged; }

public:
	// ����ͼ��
	virtual BOOL BuildImage(COLORREF clrBack,LPCTSTR lpszFile = NULL);

protected:
	// �������ת��Ϊ��ɫ
	virtual COLORREF* Data2Colors(COLORREF clrBack);
	// �������ת��Ϊ��ɫ
	virtual COLORREF* Data2Colors_Gray(COLORREF clrBack);
	// �ı�DIB
	void ChangeDib();

protected:
	KDib* m_pDib;
	KDib* m_pDib_org;
	BOOL m_bDibChanged;
	BOOL m_bDibRestore;

	Image* m_pImage;

	// ������ݶ�Ӧ����ɫ
	COLORREF* m_pColors;

	// DIB ת�����
	DWORD m_dwDibTransform;

private:
	BOOL m_bRChannel;
	BOOL m_bGChannel;
	BOOL m_bBChannel;

	double m_dBright;		// ����		[-100,100]
	double m_dContrast;		// �Աȶ�	[-100,100]

	double m_dRScale;		// ��ɫͨ�����ű��� [-100,100]
	double m_dGScale;		// ��ɫͨ�����ű��� [-100,100]
	double m_dBScale;		// ��ɫͨ�����ű��� [-100,100]

protected:
	// �����㷨
	KDataAlgorithm* m_pDataAlgorithm;

};


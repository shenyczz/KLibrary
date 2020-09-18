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

#include "KDataAlgorithm.h"

class KIND_EXT_CLASS KDataProcessor : public KProgressObject
{
public:
	KDataProcessor(void);
	virtual ~KDataProcessor(void);

public:
	// Gdiplus Image
	virtual Image* GetImage();

	// 数据算法
	KDataAlgorithm* GetDataAlgorithm() { return m_pDataAlgorithm; }

public:
	// DIB
	KDib* GetDib() { return m_pDib; }
	void SetDib(KDib* pDib) { _delete(m_pDib); m_pDib = pDib; }

	// 图像复位
	void RestoreDib();
	// 反相
	void ReverseDib();
	// 转换为灰度图像
	void GrayDib();
	// 图像颜色调整 - RGB
	void AdjustColor_Rgb(double dRScale,double dGScale,double dBScale);
	// 图像颜色调整 - CMY
	void AdjustColor_Cmy();
	// 图像颜色调整 - HSI
	void AdjustColor_Hsi();
	// 图像颜色调整 - HLS
	void AdjustColor_Hls();
	// 图像平滑
	void SmoothDib();
	// 调整亮度
	void BrightDib(double dBright);
	// 调整对比度
	void ContrastDib(double dContrast);
	// 直方图均衡
	void HistogramAverageDib(BOOL bRChannel = TRUE, BOOL bGChannel = TRUE, BOOL bBChannel = TRUE);

	BOOL& IsDibRestore() { return m_bDibRestore; }
	BOOL& IsDibChanged() { return m_bDibChanged; }

public:
	// 生成图像
	virtual BOOL BuildImage(COLORREF clrBack,LPCTSTR lpszFile = NULL);

protected:
	// 格点数据转换为颜色
	virtual COLORREF* Data2Colors(COLORREF clrBack);
	// 格点数据转换为颜色
	virtual COLORREF* Data2Colors_Gray(COLORREF clrBack);
	// 改变DIB
	void ChangeDib();

protected:
	KDib* m_pDib;
	KDib* m_pDib_org;
	BOOL m_bDibChanged;
	BOOL m_bDibRestore;

	Image* m_pImage;

	// 格点数据对应的颜色
	COLORREF* m_pColors;

	// DIB 转换标记
	DWORD m_dwDibTransform;

private:
	BOOL m_bRChannel;
	BOOL m_bGChannel;
	BOOL m_bBChannel;

	double m_dBright;		// 亮度		[-100,100]
	double m_dContrast;		// 对比度	[-100,100]

	double m_dRScale;		// 红色通道缩放比例 [-100,100]
	double m_dGScale;		// 绿色通道缩放比例 [-100,100]
	double m_dBScale;		// 蓝色通道缩放比例 [-100,100]

protected:
	// 数据算法
	KDataAlgorithm* m_pDataAlgorithm;

};


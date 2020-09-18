#include "StdAfx.h"
#include "KDataProcessor.h"

#include "KDataObject.h"


KDataProcessor::KDataProcessor(void)
{
	m_pDib = NULL;
	m_pDib_org = NULL;
	m_bDibRestore = TRUE;
	m_bDibChanged = FALSE;

	m_pImage = NULL;
	m_pColors = NULL;

	m_dwDibTransform = 0;

	m_dBright = 0;
	m_dContrast = 0;

	m_dRScale = 0;
	m_dGScale = 0;
	m_dBScale = 0;

	m_pDataAlgorithm = NULL;
}

KDataProcessor::~KDataProcessor(void)
{
	_delete(m_pDib);
	_delete(m_pDib_org);
	_delete(m_pImage);

	_delete(m_pDataAlgorithm);

	_deletea(m_pColors);


}

// 生成图像
BOOL KDataProcessor::BuildImage(COLORREF clrBack,LPCTSTR lpszFile)
{
	// 数据对象
	KDataObject* pDataObject = (KDataObject*)this->GetOwner();
	if(!pDataObject)
		return FALSE;

	// 数据信息
	KDataInfo* pDataInfo = pDataObject->GetDataInfo();
	if(!pDataInfo)
		return FALSE;

	// 纬度升序
	BOOL bLatAsceding = pDataInfo->LatInterval() > 0;

	// 数据转换为颜色
	COLORREF* pColors = Data2Colors(clrBack);
	if(!pColors)
		return FALSE;

	// 数据尺寸
	int iWidth = pDataInfo->Width();
	int iHeight = pDataInfo->Height();

	// 24位 DIB 每行的字节
	DWORD dwBytesPerLine = DIBWIDTHBYTES(iWidth*8*3);
	// 数据尺寸(24位)
	DWORD dwSizeImage = dwBytesPerLine * iHeight;
	// 整个DIB 大小(无调色板数据)
	DWORD dwDIBSize = sizeof(BITMAPINFOHEADER) + 0 + dwSizeImage;

	// 一次分配 DIB 内存
	PBYTE pDib = new BYTE[dwDIBSize];
	memset(pDib, 0, sizeof(BYTE) * dwDIBSize);

	// 设置 BITMAPINFOHRADER
	PBITMAPINFO pBitmapInfo					= (PBITMAPINFO)pDib;
	pBitmapInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	pBitmapInfo->bmiHeader.biWidth			= iWidth;
	pBitmapInfo->bmiHeader.biHeight			= iHeight;
	pBitmapInfo->bmiHeader.biPlanes			= 1;
	pBitmapInfo->bmiHeader.biBitCount		= 24;
	pBitmapInfo->bmiHeader.biCompression	= BI_RGB;
	pBitmapInfo->bmiHeader.biSizeImage		= dwSizeImage;	// 用BI_RGB方式时设可为0
	pBitmapInfo->bmiHeader.biXPelsPerMeter	= 0;
	pBitmapInfo->bmiHeader.biYPelsPerMeter	= 0;
	pBitmapInfo->bmiHeader.biClrUsed		= 0;
	pBitmapInfo->bmiHeader.biClrImportant	= 0;

	// DIB 数据指针
	PBYTE pDIBits = pDib + sizeof(BITMAPINFOHEADER) + 0 ;

	// 数据转换为DIB
	ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
	if(pProgressParam) pProgressParam->SetInfo(_T("数据转换为DIB..."));
	for(int i=0;i<iHeight;i++)
	{
		for(int j=0;j<iWidth;j++)
		{
			COLORREF clr = *(pColors + i * iWidth + j);
			int ii = bLatAsceding ? i : (iHeight-1-i);

			*(pDIBits + ii*dwBytesPerLine+j*3 + 0) = GetBValue(clr);	// blue
			*(pDIBits + ii*dwBytesPerLine+j*3 + 1) = GetGValue(clr);	// green
			*(pDIBits + ii*dwBytesPerLine+j*3 + 2) = GetRValue(clr);	// red

		}// next j

		// 显示进度
		if(m_pCallbackFunc)
		{
			if(m_pCallbackParam)
			{
				ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
				pProgressParam->lTotalNums = iHeight;
				pProgressParam->lCurrentPos = i;
			}
			m_pCallbackFunc(m_pCallbackParam);
		}

	}// next i

	// 分配 Dib 对象
	_delete(m_pDib_org);
	m_pDib_org = new KDib();

	if(!m_pDib_org->AttachMemory(pDib,TRUE))	// TRUE - 由 KDib 释放内存
		return FALSE;

	m_pDib = m_pDib_org->Clone();

	m_bDibChanged = TRUE;

	if(lpszFile)
	{
		m_pDib->Save(lpszFile);
	}

	return TRUE;
}

// 格点数据转换为颜色
COLORREF* KDataProcessor::Data2Colors(COLORREF clrBack)
{
	int i,j;
	//-----------------------------------------------------
	// 数据对象
	KDataObject* pDataObject = (KDataObject*)this->GetOwner();
	if(!pDataObject)
		return NULL;

	// 数据信息
	KDataInfo* pDataInfo = pDataObject->GetDataInfo();
	if(!pDataInfo)
		return NULL;

	// 数据
	double* pData = pDataObject->GetVData();
	if(!pData)
		return NULL;

	// 调色板
	KPalette* pPalette = pDataObject->GetPalette();
	if(!pPalette)
		return Data2Colors_Gray(clrBack);
	//-----------------------------------------------------
	// 数据尺寸
	int iWidth = pDataInfo->Width();
	int iHeight = pDataInfo->Height();
	int iTotlaSize = iWidth*iHeight;

	// 颜色数据
	_deletea(m_pColors);
	m_pColors = new COLORREF[iWidth*iHeight];
	if(!m_pColors)
		return NULL;

	// 基本值和缩放系数
	double dBaseValue = pDataInfo->BaseValue();
	double dScale = pDataInfo->Scale();

	// 有效条目数量
	int iValidItemNum = pPalette->NumbersOfValidItem();

	// 最大有效值及其对应的颜色
	double dValidmax = 0;
	COLORREF clrValidmax = 0;
	if(iValidItemNum>0)
	{
		dValidmax = pPalette->at(iValidItemNum-1)->Value();
		clrValidmax = pPalette->at(iValidItemNum-1)->Rgb();
	}

	// 进度参数
	ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;

	double value(AXIN_INVALID_DATA);
	COLORREF clr = 0;
	if(pProgressParam) pProgressParam->SetInfo(_T("数据可视化转换..."));
	for(i=0;i<iHeight;i++)
	{
		for(j=0;j<iWidth;j++)
		{
			value = *(pData + i*iWidth+j);

			// 等于0颜色为黑色
			if(fabs(value)<EPS)
			{
				clr = 0;
			}
			// 无效值的颜色为背景色
			else if(pDataObject->IsInvalidData(value))
			{
				clr = clrBack;
			}
			// 特殊值水体,云雾等
			else if(pDataObject->IsSpecialData(value))
			{
				clr = pPalette->GetColor(value,clrBack);
			}
			else
			{
				value = value / dScale + dBaseValue;
				clr = pPalette->GetColor(value,clrBack);	// 调用函数太慢
			}

			// 颜色值
			*(m_pColors+i*iWidth+j) = clr;

		}// next j

		// 显示进度
		if(m_pCallbackFunc)
		{
			if(m_pCallbackParam)
			{
				ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
				pProgressParam->lTotalNums = iHeight;
				pProgressParam->lCurrentPos = i;
			}
			m_pCallbackFunc(m_pCallbackParam);
		}

	}// next i

	return m_pColors;
}

// 格点数据转换为颜色(没有调色板则转换为灰度值)
COLORREF* KDataProcessor::Data2Colors_Gray(COLORREF clrBack)
{
	int i,j;

	// 数据对象
	KDataObject* pDataObject = (KDataObject*)this->GetOwner();
	if(!pDataObject)
		return FALSE;

	// 数据信息
	KDataInfo* pDataInfo = pDataObject->GetDataInfo();
	if(!pDataInfo)
		return FALSE;

	// 数据
	double* pData = pDataObject->GetVData();
	if(!pData)
		return NULL;

	// 查找数据极小值和极大值,顺便计算平均值
	pDataObject->LookupExtremum();
	double vMin = pDataInfo->vExtremumMin;
	double vMax = pDataInfo->vExtremumMax;
	if(vMin<0) vMax -= vMin;

	// 数据尺寸
	int iWidth = pDataInfo->Width();
	int iHeight = pDataInfo->Height();
	int iTotlaSize = iWidth*iHeight;

	// 颜色数据
	_deletea(m_pColors);
	m_pColors = new COLORREF[iWidth*iHeight];
	if(!m_pColors)
		return NULL;

	// 基本值和缩放系数
	double dBaseValue = pDataInfo->BaseValue();
	double dScale = pDataInfo->Scale();

	// 进度参数
	ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;

	double value(AXIN_INVALID_DATA);
	BYTE r,g,b;
	COLORREF clr = 0;
	if(pProgressParam) pProgressParam->SetInfo(_T("数据可视化转换..."));
	for(i=0;i<iHeight;i++)
	{
		for(j=0;j<iWidth;j++)
		{
			value = *(pData + i*iWidth+j);

			// 等于0颜色为黑色
			if(fabs(value)<EPS)
			{
				clr = 0;
			}
			// 无效值的颜色为背景色
			else if(pDataObject->IsInvalidData(value))
			{
				clr = clrBack;
			}
			// 特殊值水体,云雾等
			else if(pDataObject->IsSpecialData(value))
			{
				clr = clrBack;
			}
			else
			{
				value = value / dScale + dBaseValue;
				//if(vMin<0) value = (value-vMin) / dScale + dBaseValue;
				int iTemp = (int)(255 * value / vMax);
				if(iTemp<0) iTemp += 255;
				r = g = b = (BYTE)iTemp;
				clr = RGB(r,g,b);
			}

			// 颜色值
			*(m_pColors+i*iWidth+j) = clr;

		}// next j

		// 显示进度
		if(m_pCallbackFunc)
		{
			if(m_pCallbackParam)
			{
				ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
				pProgressParam->lTotalNums = iHeight;
				pProgressParam->lCurrentPos = i;
			}
			m_pCallbackFunc(m_pCallbackParam);
		}

	}// next i

	return m_pColors;
}

// 取得图像
Image* KDataProcessor::GetImage()
{
	if(!m_pImage || m_bDibChanged)
	{
		_delete(m_pImage);
		m_pImage = Bitmap::FromBITMAPINFO((BITMAPINFO*)m_pDib->BitmapInfoHeader(),m_pDib->ImageData());
		if(Status::Ok != m_pImage->GetLastStatus())
			return NULL;

		m_bDibChanged = FALSE;
	}

	return m_pImage; 
}

// DIB转换
// 图像复位
void KDataProcessor::RestoreDib()
{
	_delete(m_pDib);
	m_pDib = m_pDib_org->Clone();

	m_bDibChanged = TRUE;
	m_bDibRestore = TRUE;
	m_dwDibTransform = 0;
}
// 反相
void KDataProcessor::ReverseDib()
{
	m_bDibChanged = TRUE;
	m_dwDibTransform |= KDib::DibTransform_Reverse;
	ChangeDib();
}
// 转换为灰度图像
void KDataProcessor::GrayDib()
{
	m_bDibChanged = TRUE;
	m_dwDibTransform |= KDib::DibTransform_ToGray;
	ChangeDib();
}
// 图像颜色调整
// dRScale = [-100,100]
// dGScale = [-100,100]
// dBScale = [-100,100]
void KDataProcessor::AdjustColor_Rgb(double dRScale,double dGScale,double dBScale)
{
	m_bDibChanged = TRUE;
	m_dRScale = dRScale;
	m_dGScale = dGScale;
	m_dBScale = dBScale;
	m_dwDibTransform |= KDib::DibTransform_Adjust_Color_Rgb;
	ChangeDib();
}
// 图像颜色调整 - CMY
void KDataProcessor::AdjustColor_Cmy()
{
	m_bDibChanged = TRUE;
	m_dwDibTransform |= KDib::DibTransform_Adjust_Color_Cmy;
	ChangeDib();
}
// 图像颜色调整 - HSI
void KDataProcessor::AdjustColor_Hsi()
{
	m_bDibChanged = TRUE;
	m_dwDibTransform |= KDib::DibTransform_Adjust_Color_Hsi;
	ChangeDib();
}
// 图像颜色调整 - HLS
void KDataProcessor::AdjustColor_Hls()
{
	m_bDibChanged = TRUE;
	m_dwDibTransform |= KDib::DibTransform_Adjust_Color_Hls;
	ChangeDib();
}
// 图像平滑
void KDataProcessor::SmoothDib()
{
	m_bDibChanged = TRUE;
	m_dwDibTransform |= KDib::DibTransform_Smooth;
	ChangeDib();
}
// 调整亮度
// dBright = [-100,100]
void KDataProcessor::BrightDib(double dBright)
{
	m_bDibChanged = TRUE;
	m_dBright = dBright;
	m_dwDibTransform |= KDib::DibTransform_Bright;
	ChangeDib();
}
// 调整对比度
// dContrast = [-100,100]
void KDataProcessor::ContrastDib(double dContrast)
{
	m_bDibChanged = TRUE;
	m_dContrast = dContrast;
	m_dwDibTransform |= KDib::DibTransform_Contrast;
	ChangeDib();
}
// 直方图均衡
void KDataProcessor::HistogramAverageDib(BOOL bRChannel, BOOL bGChannel, BOOL bBChannel)
{
	m_bDibChanged = TRUE;
	m_bRChannel = bRChannel;
	m_bGChannel = bGChannel;
	m_bBChannel = bBChannel;
	m_dwDibTransform |= KDib::DibTransform_HistogramAverage;
	ChangeDib();
}
// 改变DIB
void KDataProcessor::ChangeDib()
{
	_delete(m_pDib);
	m_pDib = m_pDib_org->Clone();

	// 平滑
	if(m_dwDibTransform & KDib::DibTransform_Smooth)
	{
		//pDib->Smooth_Neighbour(3);
		m_pDib->Smooth_Power(16);
		//pDib->Smooth_Auto();
		//pDib->Smooth_MedianFilter();
		//pDib->Sharp_Grad();
		//pDib->Sharp_LapTemplate();
	}
	// 转换为灰度图
	if(m_dwDibTransform & KDib::DibTransform_ToGray)
	{
		m_pDib->Color2Gray();
	}
	if(m_dwDibTransform & KDib::DibTransform_Adjust_Color_Rgb)
	{
		m_pDib->AdjustColorRgb(m_dRScale/100, m_dGScale/100, m_dBScale/100);
	}
	if(m_dwDibTransform & KDib::DibTransform_Adjust_Color_Cmy)
	{
		//m_pDib->AdjustColorCym();
	}
	if(m_dwDibTransform & KDib::DibTransform_Adjust_Color_Hsi)
	{
		//m_pDib->AdjustColorHsi();
	}
	if(m_dwDibTransform & KDib::DibTransform_Adjust_Color_Hls)
	{
		//m_pDib->AdjustColorHls();
	}
	// 亮度
	if(m_dwDibTransform & KDib::DibTransform_Bright)
	{
		float a = 1;
		float b = (float)(1.5 * m_dBright);
		m_pDib->LinerTransform(a, b);
	}
	// 对比度
	if(m_dwDibTransform & KDib::DibTransform_Contrast)
	{
		// 对比度
		double div = m_dContrast > 0 ? 35 : 101;
		float a = (float)(1.0 + m_dContrast / div);
		float b = 0;
		m_pDib->LinerTransform(a, b);
	}
	// 直方图均衡
	if(m_dwDibTransform & KDib::DibTransform_HistogramAverage)
	{
		m_pDib->HistogramAverage(m_bRChannel,m_bGChannel,m_bBChannel);
	}
	// 反相
	if(m_dwDibTransform & KDib::DibTransform_Reverse)
	{
		m_pDib->Reverse();
	}

	m_bDibRestore = FALSE;
	return;
}

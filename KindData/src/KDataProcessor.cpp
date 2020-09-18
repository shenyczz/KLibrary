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

// ����ͼ��
BOOL KDataProcessor::BuildImage(COLORREF clrBack,LPCTSTR lpszFile)
{
	// ���ݶ���
	KDataObject* pDataObject = (KDataObject*)this->GetOwner();
	if(!pDataObject)
		return FALSE;

	// ������Ϣ
	KDataInfo* pDataInfo = pDataObject->GetDataInfo();
	if(!pDataInfo)
		return FALSE;

	// γ������
	BOOL bLatAsceding = pDataInfo->LatInterval() > 0;

	// ����ת��Ϊ��ɫ
	COLORREF* pColors = Data2Colors(clrBack);
	if(!pColors)
		return FALSE;

	// ���ݳߴ�
	int iWidth = pDataInfo->Width();
	int iHeight = pDataInfo->Height();

	// 24λ DIB ÿ�е��ֽ�
	DWORD dwBytesPerLine = DIBWIDTHBYTES(iWidth*8*3);
	// ���ݳߴ�(24λ)
	DWORD dwSizeImage = dwBytesPerLine * iHeight;
	// ����DIB ��С(�޵�ɫ������)
	DWORD dwDIBSize = sizeof(BITMAPINFOHEADER) + 0 + dwSizeImage;

	// һ�η��� DIB �ڴ�
	PBYTE pDib = new BYTE[dwDIBSize];
	memset(pDib, 0, sizeof(BYTE) * dwDIBSize);

	// ���� BITMAPINFOHRADER
	PBITMAPINFO pBitmapInfo					= (PBITMAPINFO)pDib;
	pBitmapInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	pBitmapInfo->bmiHeader.biWidth			= iWidth;
	pBitmapInfo->bmiHeader.biHeight			= iHeight;
	pBitmapInfo->bmiHeader.biPlanes			= 1;
	pBitmapInfo->bmiHeader.biBitCount		= 24;
	pBitmapInfo->bmiHeader.biCompression	= BI_RGB;
	pBitmapInfo->bmiHeader.biSizeImage		= dwSizeImage;	// ��BI_RGB��ʽʱ���Ϊ0
	pBitmapInfo->bmiHeader.biXPelsPerMeter	= 0;
	pBitmapInfo->bmiHeader.biYPelsPerMeter	= 0;
	pBitmapInfo->bmiHeader.biClrUsed		= 0;
	pBitmapInfo->bmiHeader.biClrImportant	= 0;

	// DIB ����ָ��
	PBYTE pDIBits = pDib + sizeof(BITMAPINFOHEADER) + 0 ;

	// ����ת��ΪDIB
	ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
	if(pProgressParam) pProgressParam->SetInfo(_T("����ת��ΪDIB..."));
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

		// ��ʾ����
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

	// ���� Dib ����
	_delete(m_pDib_org);
	m_pDib_org = new KDib();

	if(!m_pDib_org->AttachMemory(pDib,TRUE))	// TRUE - �� KDib �ͷ��ڴ�
		return FALSE;

	m_pDib = m_pDib_org->Clone();

	m_bDibChanged = TRUE;

	if(lpszFile)
	{
		m_pDib->Save(lpszFile);
	}

	return TRUE;
}

// �������ת��Ϊ��ɫ
COLORREF* KDataProcessor::Data2Colors(COLORREF clrBack)
{
	int i,j;
	//-----------------------------------------------------
	// ���ݶ���
	KDataObject* pDataObject = (KDataObject*)this->GetOwner();
	if(!pDataObject)
		return NULL;

	// ������Ϣ
	KDataInfo* pDataInfo = pDataObject->GetDataInfo();
	if(!pDataInfo)
		return NULL;

	// ����
	double* pData = pDataObject->GetVData();
	if(!pData)
		return NULL;

	// ��ɫ��
	KPalette* pPalette = pDataObject->GetPalette();
	if(!pPalette)
		return Data2Colors_Gray(clrBack);
	//-----------------------------------------------------
	// ���ݳߴ�
	int iWidth = pDataInfo->Width();
	int iHeight = pDataInfo->Height();
	int iTotlaSize = iWidth*iHeight;

	// ��ɫ����
	_deletea(m_pColors);
	m_pColors = new COLORREF[iWidth*iHeight];
	if(!m_pColors)
		return NULL;

	// ����ֵ������ϵ��
	double dBaseValue = pDataInfo->BaseValue();
	double dScale = pDataInfo->Scale();

	// ��Ч��Ŀ����
	int iValidItemNum = pPalette->NumbersOfValidItem();

	// �����Чֵ�����Ӧ����ɫ
	double dValidmax = 0;
	COLORREF clrValidmax = 0;
	if(iValidItemNum>0)
	{
		dValidmax = pPalette->at(iValidItemNum-1)->Value();
		clrValidmax = pPalette->at(iValidItemNum-1)->Rgb();
	}

	// ���Ȳ���
	ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;

	double value(AXIN_INVALID_DATA);
	COLORREF clr = 0;
	if(pProgressParam) pProgressParam->SetInfo(_T("���ݿ��ӻ�ת��..."));
	for(i=0;i<iHeight;i++)
	{
		for(j=0;j<iWidth;j++)
		{
			value = *(pData + i*iWidth+j);

			// ����0��ɫΪ��ɫ
			if(fabs(value)<EPS)
			{
				clr = 0;
			}
			// ��Чֵ����ɫΪ����ɫ
			else if(pDataObject->IsInvalidData(value))
			{
				clr = clrBack;
			}
			// ����ֵˮ��,�����
			else if(pDataObject->IsSpecialData(value))
			{
				clr = pPalette->GetColor(value,clrBack);
			}
			else
			{
				value = value / dScale + dBaseValue;
				clr = pPalette->GetColor(value,clrBack);	// ���ú���̫��
			}

			// ��ɫֵ
			*(m_pColors+i*iWidth+j) = clr;

		}// next j

		// ��ʾ����
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

// �������ת��Ϊ��ɫ(û�е�ɫ����ת��Ϊ�Ҷ�ֵ)
COLORREF* KDataProcessor::Data2Colors_Gray(COLORREF clrBack)
{
	int i,j;

	// ���ݶ���
	KDataObject* pDataObject = (KDataObject*)this->GetOwner();
	if(!pDataObject)
		return FALSE;

	// ������Ϣ
	KDataInfo* pDataInfo = pDataObject->GetDataInfo();
	if(!pDataInfo)
		return FALSE;

	// ����
	double* pData = pDataObject->GetVData();
	if(!pData)
		return NULL;

	// �������ݼ�Сֵ�ͼ���ֵ,˳�����ƽ��ֵ
	pDataObject->LookupExtremum();
	double vMin = pDataInfo->vExtremumMin;
	double vMax = pDataInfo->vExtremumMax;
	if(vMin<0) vMax -= vMin;

	// ���ݳߴ�
	int iWidth = pDataInfo->Width();
	int iHeight = pDataInfo->Height();
	int iTotlaSize = iWidth*iHeight;

	// ��ɫ����
	_deletea(m_pColors);
	m_pColors = new COLORREF[iWidth*iHeight];
	if(!m_pColors)
		return NULL;

	// ����ֵ������ϵ��
	double dBaseValue = pDataInfo->BaseValue();
	double dScale = pDataInfo->Scale();

	// ���Ȳ���
	ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;

	double value(AXIN_INVALID_DATA);
	BYTE r,g,b;
	COLORREF clr = 0;
	if(pProgressParam) pProgressParam->SetInfo(_T("���ݿ��ӻ�ת��..."));
	for(i=0;i<iHeight;i++)
	{
		for(j=0;j<iWidth;j++)
		{
			value = *(pData + i*iWidth+j);

			// ����0��ɫΪ��ɫ
			if(fabs(value)<EPS)
			{
				clr = 0;
			}
			// ��Чֵ����ɫΪ����ɫ
			else if(pDataObject->IsInvalidData(value))
			{
				clr = clrBack;
			}
			// ����ֵˮ��,�����
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

			// ��ɫֵ
			*(m_pColors+i*iWidth+j) = clr;

		}// next j

		// ��ʾ����
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

// ȡ��ͼ��
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

// DIBת��
// ͼ��λ
void KDataProcessor::RestoreDib()
{
	_delete(m_pDib);
	m_pDib = m_pDib_org->Clone();

	m_bDibChanged = TRUE;
	m_bDibRestore = TRUE;
	m_dwDibTransform = 0;
}
// ����
void KDataProcessor::ReverseDib()
{
	m_bDibChanged = TRUE;
	m_dwDibTransform |= KDib::DibTransform_Reverse;
	ChangeDib();
}
// ת��Ϊ�Ҷ�ͼ��
void KDataProcessor::GrayDib()
{
	m_bDibChanged = TRUE;
	m_dwDibTransform |= KDib::DibTransform_ToGray;
	ChangeDib();
}
// ͼ����ɫ����
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
// ͼ����ɫ���� - CMY
void KDataProcessor::AdjustColor_Cmy()
{
	m_bDibChanged = TRUE;
	m_dwDibTransform |= KDib::DibTransform_Adjust_Color_Cmy;
	ChangeDib();
}
// ͼ����ɫ���� - HSI
void KDataProcessor::AdjustColor_Hsi()
{
	m_bDibChanged = TRUE;
	m_dwDibTransform |= KDib::DibTransform_Adjust_Color_Hsi;
	ChangeDib();
}
// ͼ����ɫ���� - HLS
void KDataProcessor::AdjustColor_Hls()
{
	m_bDibChanged = TRUE;
	m_dwDibTransform |= KDib::DibTransform_Adjust_Color_Hls;
	ChangeDib();
}
// ͼ��ƽ��
void KDataProcessor::SmoothDib()
{
	m_bDibChanged = TRUE;
	m_dwDibTransform |= KDib::DibTransform_Smooth;
	ChangeDib();
}
// ��������
// dBright = [-100,100]
void KDataProcessor::BrightDib(double dBright)
{
	m_bDibChanged = TRUE;
	m_dBright = dBright;
	m_dwDibTransform |= KDib::DibTransform_Bright;
	ChangeDib();
}
// �����Աȶ�
// dContrast = [-100,100]
void KDataProcessor::ContrastDib(double dContrast)
{
	m_bDibChanged = TRUE;
	m_dContrast = dContrast;
	m_dwDibTransform |= KDib::DibTransform_Contrast;
	ChangeDib();
}
// ֱ��ͼ����
void KDataProcessor::HistogramAverageDib(BOOL bRChannel, BOOL bGChannel, BOOL bBChannel)
{
	m_bDibChanged = TRUE;
	m_bRChannel = bRChannel;
	m_bGChannel = bGChannel;
	m_bBChannel = bBChannel;
	m_dwDibTransform |= KDib::DibTransform_HistogramAverage;
	ChangeDib();
}
// �ı�DIB
void KDataProcessor::ChangeDib()
{
	_delete(m_pDib);
	m_pDib = m_pDib_org->Clone();

	// ƽ��
	if(m_dwDibTransform & KDib::DibTransform_Smooth)
	{
		//pDib->Smooth_Neighbour(3);
		m_pDib->Smooth_Power(16);
		//pDib->Smooth_Auto();
		//pDib->Smooth_MedianFilter();
		//pDib->Sharp_Grad();
		//pDib->Sharp_LapTemplate();
	}
	// ת��Ϊ�Ҷ�ͼ
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
	// ����
	if(m_dwDibTransform & KDib::DibTransform_Bright)
	{
		float a = 1;
		float b = (float)(1.5 * m_dBright);
		m_pDib->LinerTransform(a, b);
	}
	// �Աȶ�
	if(m_dwDibTransform & KDib::DibTransform_Contrast)
	{
		// �Աȶ�
		double div = m_dContrast > 0 ? 35 : 101;
		float a = (float)(1.0 + m_dContrast / div);
		float b = 0;
		m_pDib->LinerTransform(a, b);
	}
	// ֱ��ͼ����
	if(m_dwDibTransform & KDib::DibTransform_HistogramAverage)
	{
		m_pDib->HistogramAverage(m_bRChannel,m_bGChannel,m_bBChannel);
	}
	// ����
	if(m_dwDibTransform & KDib::DibTransform_Reverse)
	{
		m_pDib->Reverse();
	}

	m_bDibRestore = FALSE;
	return;
}

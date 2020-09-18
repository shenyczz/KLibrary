#include "StdAfx.h"
#include "KGridData.h"

KGridData::KGridData(void)
{
}

KGridData::~KGridData(void)
{
}

// ��γ������ת��Ϊ�������
BOOL KGridData::LonLat2xy(double lon,double lat,int& x,int& y)
{
	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return FALSE;

	int iWidth = pDataInfo->Width();
	int iHeight = pDataInfo->Height();

	double dLonMin = pDataInfo->MinLon();
	double dLonMax = pDataInfo->MaxLon();

	double dLatMin = pDataInfo->MinLat();
	double dLatMax = pDataInfo->MaxLat();

	x = (LONG)((lon - dLonMin) / pDataInfo->LonInterval() + 0.1);
	y = (LONG)((lat - dLatMin) / pDataInfo->LatInterval() + 0.1);

	return (x>=0 && x<=iWidth-1 && y>=0 && y<=iHeight-1);
}

// �������ת��Ϊ��γ������
BOOL KGridData::xy2LonLat(int x,int y,double& lon,double& lat)
{
	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return FALSE;

	int iWidth = pDataInfo->Width();
	int iHeight = pDataInfo->Height();

	double dLonMin = pDataInfo->MinLon();
	double dLonMax = pDataInfo->MaxLon();

	double dLatMin = pDataInfo->MinLat();
	double dLatMax = pDataInfo->MaxLat();

	lon = dLonMin + pDataInfo->LonInterval() * x;
	lat = dLatMin + pDataInfo->LatInterval() * y;
	if(pDataInfo->LatInterval() < 0)
	{
		lat = dLatMin + fabs(pDataInfo->LatInterval()) * (iHeight - 1 - y);
	}

	return ((lon>dLonMin || fabs(lon-dLonMin)<EPS) && (lat>dLatMin || fabs(lat-dLatMin)<EPS));
}

// ȡ�ø������
double KGridData::GetGridValue(int row,int col)
{
	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return SFX_INVALID_DATA;


	int rows = pDataInfo->Height();
	int cols = pDataInfo->Width();

	if(row<0 || row>=rows || col<0 || col>=cols)
		return SFX_INVALID_DATA;


	double* pData = this->GetVData();
	double dValue = *(pData+row*cols+col);

	return dValue;
}
// ���ø������
BOOL KGridData::SetGridValue(int row,int col,double value)
{
	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return FALSE;


	int rows = pDataInfo->Height();
	int cols = pDataInfo->Width();

	if(row<0 || row>=rows || col<0 || col>=cols)
		return FALSE;


	double* pData = this->GetVData();
	*(pData+row*cols+col) = value;

	return TRUE;
}

// ȡ�ø������
double KGridData::GetGridValue(double lon,double lat)
{
	int x,y;
	if(!this->LonLat2xy(lon,lat,x,y))
		return SFX_INVALID_DATA_DOUBLE;

	return this->GetGridValue(y,x);
}
// ���ø������
BOOL KGridData::SetGridValue(double lon,double lat,double value)
{
	int x,y;
	if(!this->LonLat2xy(lon,lat,x,y))
		return FALSE;

	return this->SetGridValue(y,x,value);
}

// �������ݼ�Сֵ�ͼ���ֵ,˳�����ƽ��ֵ
void KGridData::LookupExtremum()
{
	if(!m_pv)
		return;

	KDataObject* pDataObject = (KDataObject*)this;
	KDataInfo* pDataInfo = this->GetDataInfo();

	// ����ֵ������ϵ��
	double dBaseValue = pDataInfo->BaseValue();
	double dScale = pDataInfo->Scale();

	//---������С�����ֵ
	int nx = pDataInfo->xNum;
	int ny = pDataInfo->yNum;
	double vAvg(0);
	double vmin = 1.0e+12, vmax = -1.0e+12;
	double value(0);
	for(int i=0; i<ny;i++)
	{
		for(int j=0;j<nx;j++)
		{
			value = *(m_pv + i*nx+j);

			// ��Чֵ����ɫΪ����ɫ
			if(pDataObject->IsInvalidData(value))
				continue;

			// ����ֵˮ��,�����
			if(pDataObject->IsSpecialData(value))
				continue;

			value = value / dScale + dBaseValue;
			vAvg += value;
			vmin=min(vmin,value);
			vmax=max(vmax,value); 
		}
	}

	pDataInfo->vAvg = vAvg / (nx * ny);
	pDataInfo->vExtremumMin = vmin;
	pDataInfo->vExtremumMax = vmax;

	return;
}

// ���� ENVI �ļ�ͷ����
BOOL KGridData::ExportEnviHdr(LPCTSTR lpszFile)
{
	return KAxinData::ExportEnviHdr(lpszFile,this->GetDataInfo());
}

// ���� ArcGis bmp & bmpw
BOOL KGridData::ExportArcGis_bmp_bmpw(LPCTSTR lpszFile)
{
	if(this->IsEmpty())
		return FALSE;

	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return FALSE;

	KDataProcessor* pDataProcessor = this->GetDataProcessor();
	if(!pDataProcessor)
		return FALSE;

	KDib* pDib = pDataProcessor->GetDib();
	if(!pDib)
		return FALSE;

	if(pDib->Save(lpszFile))
	{
		KString strBmp = lpszFile;
		if(strBmp.Find(_T(".bmp"))<0)
		{
			strBmp += _T(".bmp");
		}

		KString strBmpw = strBmp;
		if(strBmpw.Find(_T(".bmp"))>0)
		{
			strBmpw.Replace(_T(".bmp"),_T(".bmpw"));
		}

		FILE* fp = _tfopen((LPCTSTR)strBmpw,_T("wt"));
		if(fp == nullptr)
			return FALSE;

		_ftprintf(fp,_T("%f\n"),pDataInfo->XInterval());
		_ftprintf(fp,_T("0\n"));
		_ftprintf(fp,_T("0\n"));
		_ftprintf(fp,_T("%f\n"),-pDataInfo->YInterval());
		_ftprintf(fp,_T("%f\n"),pDataInfo->MinLon());
		_ftprintf(fp,_T("%f\n"),pDataInfo->MaxLat());
		fclose(fp);

		return TRUE;
	}

	return FALSE;
}

#include "StdAfx.h"
#include "KGridData.h"

KGridData::KGridData(void)
{
}

KGridData::~KGridData(void)
{
}

// 经纬度坐标转换为格点坐标
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

// 格点坐标转换为经纬度坐标
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

// 取得格点数据
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
// 设置格点数据
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

// 取得格点数据
double KGridData::GetGridValue(double lon,double lat)
{
	int x,y;
	if(!this->LonLat2xy(lon,lat,x,y))
		return SFX_INVALID_DATA_DOUBLE;

	return this->GetGridValue(y,x);
}
// 设置格点数据
BOOL KGridData::SetGridValue(double lon,double lat,double value)
{
	int x,y;
	if(!this->LonLat2xy(lon,lat,x,y))
		return FALSE;

	return this->SetGridValue(y,x,value);
}

// 查找数据极小值和极大值,顺便计算平均值
void KGridData::LookupExtremum()
{
	if(!m_pv)
		return;

	KDataObject* pDataObject = (KDataObject*)this;
	KDataInfo* pDataInfo = this->GetDataInfo();

	// 基本值和缩放系数
	double dBaseValue = pDataInfo->BaseValue();
	double dScale = pDataInfo->Scale();

	//---查找最小、最大值
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

			// 无效值的颜色为背景色
			if(pDataObject->IsInvalidData(value))
				continue;

			// 特殊值水体,云雾等
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

// 导出 ENVI 文件头数据
BOOL KGridData::ExportEnviHdr(LPCTSTR lpszFile)
{
	return KAxinData::ExportEnviHdr(lpszFile,this->GetDataInfo());
}

// 导出 ArcGis bmp & bmpw
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

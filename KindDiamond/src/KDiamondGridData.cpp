#include "StdAfx.h"
#include "KDiamondGridData.h"


KDiamondGridData::KDiamondGridData(void)
{
	m_pContourBuilder = NULL;
}


KDiamondGridData::~KDiamondGridData(void)
{
	_delete(m_pContourBuilder);
}

// 经纬度坐标转换为格点坐标
BOOL KDiamondGridData::LonLat2xy(double lon,double lat,int& x,int& y)
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
BOOL KDiamondGridData::xy2LonLat(int x,int y,double& lon,double& lat)
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
double KDiamondGridData::GetGridValue(int row,int col)
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
BOOL KDiamondGridData::SetGridValue(int row,int col,double value)
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
double KDiamondGridData::GetGridValue(double lon,double lat)
{
	int x,y;
	if(!this->LonLat2xy(lon,lat,x,y))
		return SFX_INVALID_DATA;

	return this->GetGridValue(y,x);
}
// 设置格点数据
BOOL KDiamondGridData::SetGridValue(double lon,double lat,double value)
{
	int x,y;
	if(!this->LonLat2xy(lon,lat,x,y))
		return FALSE;

	return this->SetGridValue(y,x,value);
}

// 查找数据极小值和极大值,顺便计算平均值
void KDiamondGridData::LookupExtremum()
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

// 装载数据 - ASC文件
BOOL KDiamondGridData::LoadData_txt(LPCTSTR lpszFile)
{
	int i,j;
	int iTemp = 0;
	double dTemp = 0;
	TCHAR szTemp[MAX_PATH] = _T("数据文件格式不合要求");

	// 清空数据
	if(!IsEmpty())
		Empty();

	// 数据信息
	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return FALSE;

	// 打开文件
	FILE* fp = _tfopen(lpszFile,_T("rt"));
	if(fp==NULL)
	{
		SetMessage(_T("打开文件错误! in KStationData::LoadData_txt"));
		return FALSE;
	}

	// 1.读文件标识
	int sizeFid = _tcslen(DIAM_FID_TXT_MARK); 
	_ftscanf(fp,_T("%s"),szTemp);		// 文件头标示,为字符串 _T("DIAMOND"),大小写均可
	if( szTemp[sizeFid] != _T('\0') )
	{
		SetMessage(_T("数据文件格式不合要求! - FILE_ID"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}
	//转换为大写
	KString::UpperString(szTemp);
	if( _tcscmp(szTemp,DIAM_FID_TXT_MARK) != 0 )
	{
		SetMessage(_T("数据文件格式不合要求! - FILE_ID"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}
	// 保存文件标识
	_tcscpy(pDataInfo->szFileID,szTemp);

	// 2.读产品数据格式代码
	_ftscanf(fp,_T("%d"),&iTemp);
	if(iTemp != DIAM_FMT_CODE_GRID)	// DIAM_FMT_CODE_GRID = 4
	{
		SetMessage(_T("SNYC 数据文件格式不合要求! - wFormatCode"));
		fclose(fp);
		return FALSE;
	}
	// 保存格式代码
	pDataInfo->FormatCode() = iTemp;

	// 3.读注释信息
	_ftscanf(fp,_T("%s"),szTemp);
	if( _tcslen(szTemp) > (DIAM_COMMENT_LENGTH) )	// DIAM_COMMENT_LENGTH = 64
	{
		// 注释信息太长则截断
		szTemp[AXIN_COMMENT_LENGTH] = _T('\0');
	}
	pDataInfo->SetComment(szTemp);

	// 4.读日期时间(yyyy,mm,dd,HH)
	int yyyy,mm,dd,HH;
	_ftscanf(fp,_T("%d%d%d %d"), &yyyy, &mm, &dd, &HH);
	pDataInfo->Year()			= yyyy;
	pDataInfo->Month()			= mm;
	pDataInfo->Day()			= dd;
	pDataInfo->Hour()			= HH;

	// 5.读时效、层次
	int tp,ly;
	_ftscanf(fp,_T("%d%d"),&tp,&ly);
	pDataInfo->TimePeriod()		= tp;
	pDataInfo->Layer()			= ly;

	pDataInfo->ProductCode()	= 0;

    // 6.读数据网格数据信息
	double xinterval,yinterval;
	_ftscanf(fp,_T("%lf %lf"), &xinterval, &yinterval);
	pDataInfo->XInterval()	= xinterval;	// X坐标间隔值
	pDataInfo->YInterval()	= yinterval;	// Y坐标间隔值

	double xmin,xmax;
	_ftscanf(fp,_T("%lf %lf"), &xmin, &xmax);
	pDataInfo->MinX() = xmin;
	pDataInfo->MaxX() = xmax;

	double ymin,ymax;
	_ftscanf(fp,_T("%lf %lf"), &ymin, &ymax);
	pDataInfo->MinY() = ymin;
	pDataInfo->MaxY() = ymax;

	int nx,ny;
	_ftscanf(fp,_T("%d %d"), &nx, &ny);
	pDataInfo->Width() = nx;
	pDataInfo->Height() = ny;

	// 7.读入等值线信息
	double cinterval,cmin,cmax;
	_ftscanf(fp,_T("%lf %lf %lf"),&cinterval,&cmin,&cmax);
	pDataInfo->ContourInterval() = cinterval;
	pDataInfo->ContourMin() = cmin;
	pDataInfo->ContourMax() = cmax;

	// 计算等值线数量
	//pDataInfo->ContourNumbers() = (int)((cmax - cmin) / cinterval) + 1;
	pDataInfo->ContourNumbers() = DIAM_FMT_CVID_CONTOUR;

	// 8.平滑系数、加粗线值
	double dSmoothCoefficient,dBoldContourValue;
	_ftscanf(fp,_T("%lf %lf"),&dSmoothCoefficient,&dBoldContourValue);
	pDataInfo->SmoothCoefficient() = dSmoothCoefficient;
	pDataInfo->BoldContourValue() = dBoldContourValue;

	// 9.为数据分配内存
	PrepareMemory(eUvw_v);

	// 10.读入数据
	nx = pDataInfo->Width();
	ny = pDataInfo->Height();
	// 进度参数
	ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
	if(pProgressParam) pProgressParam->SetInfo(_T("读取数据..."));
	for(i=0; i<ny;i++)
	{
		for(j=0;j<nx;j++)
		{
			_ftscanf(fp,_T("%lf"),m_pv+i*nx+j);
		}

		// 显示进度
		if(m_pCallbackFunc)
		{
			if(m_pCallbackParam)
			{
				ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
				pProgressParam->lTotalNums = ny;
				pProgressParam->lCurrentPos = i;
			}

			m_pCallbackFunc(m_pCallbackParam);
		}

	}// next i

	// 关闭文件
	fclose(fp);
	fp = NULL;

	// 设置绑定矩形
	this->SetExtents(KExtents(pDataInfo->MinX(),pDataInfo->MinY(),pDataInfo->MaxX(),pDataInfo->MaxY()));

	// 查找极小值和极大值
	// 顺便计算平均值
	this->LookupExtremum();

	// 构建默认调色板
	this->BuildPalette(NULL);

	return this->BuildContours();
}

// 构造等值线
BOOL KDiamondGridData::BuildContours()
{
	if(m_pContourBuilder==NULL)
		m_pContourBuilder = new KQContourBuilder();

	// 取得等值线构造器
	KContourBuilder* pContourBuilder = m_pContourBuilder;
	// 为等值线构造器注册回调函数
	pContourBuilder->RegiestCallbackFunc(this->CallbackFunc(),this->CallbackParam());
	// 1.设置数据
	pContourBuilder->SetData(this->GetVData());
	// 2.设置数据信息
	pContourBuilder->SetDataInfo(this->GetDataInfo());
	// 3.构造等值线
	BOOL bContoursReady = pContourBuilder->BuildContours();

	return bContoursReady;
}

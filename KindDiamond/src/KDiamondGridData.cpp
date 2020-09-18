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

// ��γ������ת��Ϊ�������
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

// �������ת��Ϊ��γ������
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

// ȡ�ø������
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
// ���ø������
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

// ȡ�ø������
double KDiamondGridData::GetGridValue(double lon,double lat)
{
	int x,y;
	if(!this->LonLat2xy(lon,lat,x,y))
		return SFX_INVALID_DATA;

	return this->GetGridValue(y,x);
}
// ���ø������
BOOL KDiamondGridData::SetGridValue(double lon,double lat,double value)
{
	int x,y;
	if(!this->LonLat2xy(lon,lat,x,y))
		return FALSE;

	return this->SetGridValue(y,x,value);
}

// �������ݼ�Сֵ�ͼ���ֵ,˳�����ƽ��ֵ
void KDiamondGridData::LookupExtremum()
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

// װ������ - ASC�ļ�
BOOL KDiamondGridData::LoadData_txt(LPCTSTR lpszFile)
{
	int i,j;
	int iTemp = 0;
	double dTemp = 0;
	TCHAR szTemp[MAX_PATH] = _T("�����ļ���ʽ����Ҫ��");

	// �������
	if(!IsEmpty())
		Empty();

	// ������Ϣ
	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return FALSE;

	// ���ļ�
	FILE* fp = _tfopen(lpszFile,_T("rt"));
	if(fp==NULL)
	{
		SetMessage(_T("���ļ�����! in KStationData::LoadData_txt"));
		return FALSE;
	}

	// 1.���ļ���ʶ
	int sizeFid = _tcslen(DIAM_FID_TXT_MARK); 
	_ftscanf(fp,_T("%s"),szTemp);		// �ļ�ͷ��ʾ,Ϊ�ַ��� _T("DIAMOND"),��Сд����
	if( szTemp[sizeFid] != _T('\0') )
	{
		SetMessage(_T("�����ļ���ʽ����Ҫ��! - FILE_ID"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}
	//ת��Ϊ��д
	KString::UpperString(szTemp);
	if( _tcscmp(szTemp,DIAM_FID_TXT_MARK) != 0 )
	{
		SetMessage(_T("�����ļ���ʽ����Ҫ��! - FILE_ID"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}
	// �����ļ���ʶ
	_tcscpy(pDataInfo->szFileID,szTemp);

	// 2.����Ʒ���ݸ�ʽ����
	_ftscanf(fp,_T("%d"),&iTemp);
	if(iTemp != DIAM_FMT_CODE_GRID)	// DIAM_FMT_CODE_GRID = 4
	{
		SetMessage(_T("SNYC �����ļ���ʽ����Ҫ��! - wFormatCode"));
		fclose(fp);
		return FALSE;
	}
	// �����ʽ����
	pDataInfo->FormatCode() = iTemp;

	// 3.��ע����Ϣ
	_ftscanf(fp,_T("%s"),szTemp);
	if( _tcslen(szTemp) > (DIAM_COMMENT_LENGTH) )	// DIAM_COMMENT_LENGTH = 64
	{
		// ע����Ϣ̫����ض�
		szTemp[AXIN_COMMENT_LENGTH] = _T('\0');
	}
	pDataInfo->SetComment(szTemp);

	// 4.������ʱ��(yyyy,mm,dd,HH)
	int yyyy,mm,dd,HH;
	_ftscanf(fp,_T("%d%d%d %d"), &yyyy, &mm, &dd, &HH);
	pDataInfo->Year()			= yyyy;
	pDataInfo->Month()			= mm;
	pDataInfo->Day()			= dd;
	pDataInfo->Hour()			= HH;

	// 5.��ʱЧ�����
	int tp,ly;
	_ftscanf(fp,_T("%d%d"),&tp,&ly);
	pDataInfo->TimePeriod()		= tp;
	pDataInfo->Layer()			= ly;

	pDataInfo->ProductCode()	= 0;

    // 6.����������������Ϣ
	double xinterval,yinterval;
	_ftscanf(fp,_T("%lf %lf"), &xinterval, &yinterval);
	pDataInfo->XInterval()	= xinterval;	// X������ֵ
	pDataInfo->YInterval()	= yinterval;	// Y������ֵ

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

	// 7.�����ֵ����Ϣ
	double cinterval,cmin,cmax;
	_ftscanf(fp,_T("%lf %lf %lf"),&cinterval,&cmin,&cmax);
	pDataInfo->ContourInterval() = cinterval;
	pDataInfo->ContourMin() = cmin;
	pDataInfo->ContourMax() = cmax;

	// �����ֵ������
	//pDataInfo->ContourNumbers() = (int)((cmax - cmin) / cinterval) + 1;
	pDataInfo->ContourNumbers() = DIAM_FMT_CVID_CONTOUR;

	// 8.ƽ��ϵ�����Ӵ���ֵ
	double dSmoothCoefficient,dBoldContourValue;
	_ftscanf(fp,_T("%lf %lf"),&dSmoothCoefficient,&dBoldContourValue);
	pDataInfo->SmoothCoefficient() = dSmoothCoefficient;
	pDataInfo->BoldContourValue() = dBoldContourValue;

	// 9.Ϊ���ݷ����ڴ�
	PrepareMemory(eUvw_v);

	// 10.��������
	nx = pDataInfo->Width();
	ny = pDataInfo->Height();
	// ���Ȳ���
	ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
	if(pProgressParam) pProgressParam->SetInfo(_T("��ȡ����..."));
	for(i=0; i<ny;i++)
	{
		for(j=0;j<nx;j++)
		{
			_ftscanf(fp,_T("%lf"),m_pv+i*nx+j);
		}

		// ��ʾ����
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

	// �ر��ļ�
	fclose(fp);
	fp = NULL;

	// ���ð󶨾���
	this->SetExtents(KExtents(pDataInfo->MinX(),pDataInfo->MinY(),pDataInfo->MaxX(),pDataInfo->MaxY()));

	// ���Ҽ�Сֵ�ͼ���ֵ
	// ˳�����ƽ��ֵ
	this->LookupExtremum();

	// ����Ĭ�ϵ�ɫ��
	this->BuildPalette(NULL);

	return this->BuildContours();
}

// �����ֵ��
BOOL KDiamondGridData::BuildContours()
{
	if(m_pContourBuilder==NULL)
		m_pContourBuilder = new KQContourBuilder();

	// ȡ�õ�ֵ�߹�����
	KContourBuilder* pContourBuilder = m_pContourBuilder;
	// Ϊ��ֵ�߹�����ע��ص�����
	pContourBuilder->RegiestCallbackFunc(this->CallbackFunc(),this->CallbackParam());
	// 1.��������
	pContourBuilder->SetData(this->GetVData());
	// 2.����������Ϣ
	pContourBuilder->SetDataInfo(this->GetDataInfo());
	// 3.�����ֵ��
	BOOL bContoursReady = pContourBuilder->BuildContours();

	return bContoursReady;
}

#include "StdAfx.h"
#include "KWindVectorData.h"


KWindVectorData::KWindVectorData(void)
{
}


KWindVectorData::~KWindVectorData(void)
{
}

// 重载装载数据
BOOL KWindVectorData::LoadData_txt(LPCTSTR lpszFile)
{
	int i,j;
	TCHAR szTemp[MAX_PATH] = _T("");

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
		SetMessage(_T("打开文件错误! in KWindVectorData::LoadData_txt"));
		return FALSE;
	}

	// 1.读文件标识
	int sizeFid = _tcslen(AXIN_FID_TXT_MARK);
	_ftscanf(fp,_T("%s"),szTemp);		//文件头标示,为字符串“AXIN”,大小写均可
	if( szTemp[sizeFid] != _T('\0') )
	{
		SetMessage(_T("数据文件格式不合要求! - AXIN_FID_TXT_MARK"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}
	//转换为大写
	KString::UpperString(szTemp);
	if( _tcscmp(szTemp,AXIN_FID_TXT_MARK) != 0 )
	{
		SetMessage(_T("数据文件格式不合要求! - AXIN_FID_TXT_MARK"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}
	// 保存文件标识
	_tcscpy(pDataInfo->szFileID,szTemp);

	// 2.读产品数据格式代码
	int iFormatCode = 0;
	_ftscanf(fp,_T("%d"),&iFormatCode);
	if(iFormatCode != AXIN_FMT_CODE_GRID_WIND_VECTOR)	// 49
	{
		SetMessage(_T("数据文件格式不合要求! - iFormatCode"));
		fclose(fp);
		return FALSE;
	}
	// 保存文件格式
	pDataInfo->FormatCode() = iFormatCode;

	// 3.读注释信息
	_ftscanf(fp,_T("%s"),szTemp);
	if( _tcslen(szTemp) > (AXIN_COMMENT_LENGTH) )	// AXIN_COMMENT_LENGTH = 64
	{
		szTemp[AXIN_COMMENT_LENGTH] = _T('\0');
	}
	// 保存注释信息
	pDataInfo->Comment() = szTemp;

	// 4.读日期时间(yyyy,mm,dd,HH,MM,SS,MS)
	int yyyy,mm,dd,HH,MM,SS,MS;
	_ftscanf(fp,_T("%d%d%d %d%d%d%s"), &yyyy, &mm, &dd, &HH, &MM, &SS, &MS);
	pDataInfo->Year()			= yyyy;
	pDataInfo->Month()			= mm;
	pDataInfo->Day()			= dd;
	pDataInfo->Hour()			= HH;
	pDataInfo->Minute()			= MM;
	pDataInfo->Second()			= SS;
	pDataInfo->Millisecond()	= MS;

	// 5.读时效、层次、产品代码、要素代码
	int tt,ll,pc,ec;
	_ftscanf(fp,_T("%d%d%d%d"),&tt,&ll,&pc,&ec);
	pDataInfo->TimePeriod()		= tt;
	pDataInfo->Layer()			= ll;
	pDataInfo->ProductCode()	= pc;
	pDataInfo->ElementCode()	= ec;

    // 6.读数据网格数据信息(nx,xmin,xinterval)(ny,ymin,yinterval)
	int nx,ny;
	double xmin,xinterval,ymin,yinterval;
	_ftscanf(fp,_T("%d %lf %lf"), &nx, &xmin, &xinterval);
	pDataInfo->Width()		= nx;			// X方向格点数
	pDataInfo->MinX()		= xmin;			// X坐标最小值
	pDataInfo->XInterval()	= xinterval;	// X坐标间隔值

	_ftscanf(fp,_T("%d %lf %lf"), &ny, &ymin, &yinterval);
	pDataInfo->Height()		= ny;			// Y方向格点数
	pDataInfo->MinY()		= ymin;			// Y坐标最小值
	pDataInfo->YInterval()	= yinterval;	// Y坐标间隔值

	// 计算 XY 坐标最大值
	pDataInfo->MaxX() = pDataInfo->MinX() + pDataInfo->XInterval() * (pDataInfo->Width()-1);
	pDataInfo->MaxY() = pDataInfo->MinY() + pDataInfo->YInterval() * (pDataInfo->Height()-1);

	// 7.分配内存
	PrepareMemory(eUvw_uv);

	// 8.读入数据
	nx = pDataInfo->XNum();
	ny = pDataInfo->YNum();
	// 进度参数
	ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;

	// 8.1 读网格数据 - U
	if(pProgressParam) pProgressParam->SetInfo(_T("读取数据..."));
	for(i=0; i<ny;i++)
	{
		for(j=0;j<nx;j++)
		{
			_ftscanf(fp,_T("%lf"),m_pu+i*nx+j);
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

	// 8.2 读网格数据 - V
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
	this->SetExtents(KExtents(pDataInfo->XMin(),pDataInfo->YMin(),pDataInfo->XMax(),pDataInfo->YMax()));

	//查找极小值和极大值
	this->LookupExtremum();

	return TRUE;
}

// 查找网格数据极小值、极大值
// pDataInfo->VExtremumMin - 保存极小值
// pDataInfo->VExtremumMax - 保存极大值
void KWindVectorData::LookupExtremum()
{
	int i,j;
	KDataInfo* pDataInfo = this->GetDataInfo();

	int nx = pDataInfo->XNum();
	int ny = pDataInfo->YNum();

	//查找U最小、最大值
	double umin = m_pu[0], umax = m_pu[0];
	for(i=0; i<ny;i++)
	{
		for(j=0;j<nx;j++)
		{
			umin = min(umin,m_pu[i*nx+j]);
			umax = max(umax,m_pu[i*nx+j]); 
		}
	}
	pDataInfo->UExtremumMin() = umin;
	pDataInfo->UExtremumMax() = umax;

	//查找V最小、最大值
	double vmin=m_pv[0],vmax=m_pv[0];
	for(i=0; i<ny;i++)
	{
		for(j=0;j<nx;j++)
		{
			vmin = min(vmin,m_pv[i*nx+j]);
			vmax = max(vmax,m_pv[i*nx+j]); 
		}
	}
	pDataInfo->VExtremumMin() = vmin;
	pDataInfo->VExtremumMax() = vmax;
}

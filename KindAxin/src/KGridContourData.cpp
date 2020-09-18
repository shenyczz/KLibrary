#include "StdAfx.h"
#include "KGridContourData.h"


KGridContourData::KGridContourData(void)
{
	SetType(KAxinData::eDataStyle_Grid_Contour);

	m_pContourBuilder = NULL;
}

KGridContourData::~KGridContourData(void)
{
	_delete(m_pContourBuilder);
}

// 装载数据 - ASC文件
BOOL KGridContourData::LoadData_txt(LPCTSTR lpszFile)
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
	int sizeFid = _tcslen(AXIN_FID_TXT_MARK); 
	_ftscanf(fp,_T("%s"),szTemp);		// 文件头标示,为字符串 _T("AXIN"),大小写均可
	if( szTemp[sizeFid] != _T('\0') )
	{
		SetMessage(_T("数据文件格式不合要求! - FILE_ID"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}
	//转换为大写
	KString::UpperString(szTemp);
	if( _tcscmp(szTemp,AXIN_FID_TXT_MARK) != 0 )
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
	if(iTemp != AXIN_FMT_CODE_GRID)	// LEAP_FMT_CODE_GRID = 40
	{
		SetMessage(_T("SNYC 数据文件格式不合要求! - wFormatCode"));
		fclose(fp);
		return FALSE;
	}
	// 保存格式代码
	pDataInfo->FormatCode() = iTemp;

	// 3.读注释信息
	_ftscanf(fp,_T("%s"),szTemp);
	if( _tcslen(szTemp) > (AXIN_COMMENT_LENGTH) )	// LEAP_COMMENT_LENGTH = 64
	{
		// 注释信息太长则截断
		szTemp[AXIN_COMMENT_LENGTH] = _T('\0');
	}
	pDataInfo->SetComment(szTemp);

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
	int tp,ly,pc,ec;
	_ftscanf(fp,_T("%d%d%d%d"),&tp,&ly,&pc,&ec);
	pDataInfo->TimePeriod()		= tp;
	pDataInfo->Layer()			= ly;
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

	// 7.读入等值线值数量
	int iContourNums;
	_ftscanf(fp,_T("%d"),&iContourNums);
	pDataInfo->ContourNumbers() = iContourNums;
	if(iContourNums>0)
	{
		// 等值线数量是否规定的值
		if(iContourNums<AXIN_FMT_CVID_MIN ||
			(iContourNums>AXIN_FMT_CVID_MAX && iContourNums!=AXIN_FMT_CVID_AUTO && iContourNums!=AXIN_FMT_CVID_CONTOUR))
		{
			SetMessage(_T("等值线数量不符合要求! - wContourNums"));
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		// 当 wContourNums=9999 时
		// 表示紧随其后的三个数是: 等值线间隔值,等值线最小值,等值线最大值
		else if(iContourNums == AXIN_FMT_CVID_CONTOUR)	// 9999
		{
			double cinterval,cmin,cmax;
			_ftscanf(fp,_T("%lf %lf %lf"), &cinterval, &cmin, &cmax);
			if(cmin > cmax || (cinterval < 0) || fabs(cinterval)<EPS)
			{
				SetMessage(_T("SNYC 数据文件格式不合要求!! - dContourInterval"));
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
			pDataInfo->ContourInterval()	= cinterval;
			pDataInfo->ContourMin()			= cmin;
			pDataInfo->ContourMax()			= cmax;
		}
		// 当 wContourNums=8888 时
		// 表示自动计算等值线的等值线间隔值、等值线最小值、 等值线最大值以及加粗显示的等值线值
		else if(iContourNums == AXIN_FMT_CVID_AUTO)	// 8888
		{
			// 自动计算等值线的等值线间隔值、等值线最小值、 等值线最大值以及加粗显示的等值线值
			// 在后面查找数据极值后处理
			int iContourPortion;
			_ftscanf(fp,_T("%d"), &iContourPortion);
			if(iContourPortion<10 || iContourPortion>50)	// 取之范围在[10,50]
			{
				iContourPortion = 10;
			}
			pDataInfo->ContourPortion() = iContourPortion;
		}
		// 当 wContourNums = [1,99] 时表示
		// 紧随其后的是 wContourNums 个要求追踪的等值线值数
		else if(iContourNums >=1 && iContourNums<=99)
		{
			for(i=0;i<iContourNums;i++)
			{
				_ftscanf(fp,_T("%lf"),&dTemp);
				pDataInfo->ContourValue(i) = dTemp;
			}
		}
		else
		{
			// 其他情况
		}

		// 读取加粗显示的等值线值
		if(iContourNums != 0 && iContourNums!=AXIN_FMT_CVID_AUTO)
		{
			_ftscanf(fp,_T("%lf"),&dTemp);
			pDataInfo->BoldContourValue() = dTemp;
		}

	}// if(nContours>0)

	// 8.为数据分配内存
	PrepareMemory(eUvw_v);

	// 9.读入数据
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

	// 自动计算等值线的等值线间隔值、等值线最小值、 等值线最大值以及加粗显示的等值线值
	WORD wContourNums = pDataInfo->ContourNumbers();
	if(wContourNums == AXIN_FMT_CVID_AUTO)
	{
		int iContourPortion = pDataInfo->ContourPortion();
		//if(iContourPortion<10 || iContourPortion>50) iContourPortion = 10;
		double vmin = pDataInfo->ExtremumValueMin();
		double vmax = pDataInfo->ExtremumValueMax();
		double vinterval = (vmax - vmin) / iContourPortion;			// 分成iContourPortion份
		// 间隔值大于1时使用整数间隔
		if(vinterval > 1.0)
		{
			vinterval	= (int)(vinterval+0.5);
			vmax		= (int)(vmax + 0.9);
			if(vmin>0)
			{
				vmin = (vmin-0.5)<0 ? 0 : (int)(vmin-0.5);
			}
			if(vmin<0)
			{
				vmin = (vmin+0.5)>0 ? 0 : (int)(vmin+0.5);
			}
		}
		double vbold = vmin + vinterval * iContourPortion / 2;

		pDataInfo->ContourInterval()	= vinterval;
		pDataInfo->ContourMin()			= vmin;
		pDataInfo->ContourMax()			= vmax;
		pDataInfo->BoldContourValue()	= vbold;
	}

	// 构建默认调色板
	this->BuildPalette(NULL);

	// 是否构造等值线
	if( (wContourNums>0 && wContourNums<100) || wContourNums == AXIN_FMT_CVID_AUTO || wContourNums == AXIN_FMT_CVID_CONTOUR )
	{
		// 构造等值线
		this->BuildContours();
	}

	return TRUE;
}

// 构造等值线
BOOL KGridContourData::BuildContours()
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

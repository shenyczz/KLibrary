#include "StdAfx.h"
#include "KStationData.h"

KStationData::KStationData(void)
{
	m_bContourReady = FALSE;
	m_bRebuildContour = TRUE;
	m_iCurrentElementIndex = 0;
	m_pContourBuilder = NULL;
	m_pStationInfoPtrCollection = NULL;
	m_InterpolationRecordMax = 5000;
}

KStationData::~KStationData(void)
{
	_delete(m_pContourBuilder);
	_delete(m_pStationInfoPtrCollection);
}

// 清除数据并回收内存
void KStationData::Empty()
{
	_delete(m_pContourBuilder);
	_delete(m_pStationInfoPtrCollection);
}
// 是否为空
BOOL KStationData::IsEmpty()
{
	return (m_pStationInfoPtrCollection == NULL)
		|| (m_pStationInfoPtrCollection != NULL) && (0 == m_pStationInfoPtrCollection->Count())
		   ;
}

// 装载数据 - ASC文件
BOOL KStationData::LoadData_txt(LPCTSTR lpszFile)
{
	int i,j;
	int iTemp = 0;
	TCHAR szTemp[MAX_PATH] = _T("");

	// 清空数据
	if(!IsEmpty())
		Empty();

	// 数据信息
	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return FALSE;

	// 站点信息集
	if(m_pStationInfoPtrCollection==NULL)
	{
		m_pStationInfoPtrCollection = new KObjectPtrCollection();
	}
	m_pStationInfoPtrCollection->Clear();
	KObjectPtrCollection* pStationInfoPtrCollection = m_pStationInfoPtrCollection;

	//打开文件
	FILE* fp = _tfopen(lpszFile,_T("rt"));
	if(fp==NULL)
	{
		SetMessage(_T("打开文件错误! in KStationData::LoadData_txt"));
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
	else
	{
		// 保存文件标识
		_tcscpy(pDataInfo->szFileID,szTemp);
	}

	// 2.读产品数据格式代码
	_ftscanf(fp,_T("%d"),&iTemp);
	if(iTemp != AXIN_FMT_CODE_TIN)	// AXIN_FMT_CODE_TIN = 30
	{
		SetMessage(_T("SNYC 数据文件格式不合要求! - wFmtCode"));
		fclose(fp);
		return FALSE;
	}
	// 保存格式代码
	pDataInfo->FormatCode() = iTemp;

	// 3.读注释信息
	_ftscanf(fp,_T("%s"),szTemp);
	if( _tcslen(szTemp) > (AXIN_COMMENT_LENGTH) )
	{
		szTemp[AXIN_COMMENT_LENGTH] = _T('\0');
	}
	// 保存注释信息
	pDataInfo->Comment() = szTemp;

	// 4.读日期时间(yyyy,mm,dd,HH,MM,SS,MS)
	int yyyy,mm,dd,HH,MM,SS,MS;
	_ftscanf(fp,_T("%d%d%d %d%d%d%d"), &yyyy, &mm, &dd, &HH, &MM, &SS, &MS);
	pDataInfo->Year()			= yyyy;
	pDataInfo->Month()			= mm;
	pDataInfo->Day()			= dd;
	pDataInfo->Hour()			= HH;
	pDataInfo->Minute()			= MM;
	pDataInfo->Second()			= SS;
	pDataInfo->Millisecond()	= MS;

	// 5.读时效、层次、产品代码、要素代码
	int tt,ll,pc,ec;
	_ftscanf(fp,_T("%d%d%d%d"),&tt, &ll, &pc, &ec);
	pDataInfo->TimePeriod()		= tt;
	pDataInfo->Layer()			= ll;
	pDataInfo->ProductCode()	= pc;
	pDataInfo->ElementCode()	= ec;

	// 6.读入站点数量
	_ftscanf(fp,_T("%d"),&pDataInfo->wTotalNum);
	if( ((int)pDataInfo->wTotalNum) < 0)
	{
		SetMessage(_T("SNYC 数据文件格式不合要求! - wTotalNum"));
		fclose(fp);
		return FALSE;
	}

	// 7.读入每个站点要素数量
	_ftscanf(fp,_T("%d"),&pDataInfo->wElementNum);
	if( ((int)pDataInfo->wElementNum) < 0 || pDataInfo->wElementNum>50) // <=50
	{
		SetMessage(_T("SNYC 数据文件格式不合要求! - wElementNum"));
		fclose(fp);
		return FALSE;
	}

	// 8.读取 FLAG
	_ftscanf(fp,_T("%d"),&pDataInfo->dwFlag);

	// 9.读等值线值数量
	int iContourNums;
	_ftscanf(fp,_T("%d"),&iContourNums);
	pDataInfo->ContourNumbers()	= iContourNums;
	if(iContourNums>0)
	{
		// 是否规定的值
		if(iContourNums < AXIN_FMT_CVID_MIN || 
			(iContourNums > AXIN_FMT_CVID_MAX) && (iContourNums != AXIN_FMT_CVID_AUTO) && (iContourNums != AXIN_FMT_CVID_CONTOUR))
		{
			SetMessage(_T("等值线数量不符合要求! - wContourNums"));
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		// 当 wContourNums=9999 时表示紧随其后的三个数表示
		//    等值线间隔值 等值线最小值 等值线最大值
		else if(iContourNums == AXIN_FMT_CVID_CONTOUR)	// 9999
		{
			_ftscanf(fp,_T("%lf %lf %lf")
				,&pDataInfo->dContourInterval
				,&pDataInfo->dContourMin
				,&pDataInfo->dContourMax
				);

			if(pDataInfo->dContourMin >= pDataInfo->dContourMax || (pDataInfo->dContourInterval < 0) || fabs(pDataInfo->dContourInterval)<EPS)
			{
				SetMessage(_T("SNYC 数据文件格式不合要求!! - dContourInterval"));
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

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
			for(i=0;i<pDataInfo->wContourNums;i++)
			{
				_ftscanf(fp,_T("%lf"),&pDataInfo->dContourValue[i]);
			}
		}
		else
		{
			// 其他情况
		}

		// 读取加粗显示的等值线值(自动等值线无法确定等值线的值)
		if(iContourNums != 0 && iContourNums!=AXIN_FMT_CVID_AUTO)
		{
			_ftscanf(fp,_T("%lf"),&pDataInfo->dBoldContourValue);
		}

	}// if(iContourNums>0)

	// 10.剪切区
	int iClipArea = 0;
	_ftscanf(fp,_T("%d"),&iClipArea);
	pDataInfo->ClipArea() = iClipArea;
	if( (iClipArea<4 && iClipArea!=0) || (iClipArea>99 && iClipArea!=AXIN_FMT_CLIP_AREA_BOX))
	{
		// 最少4个点
		SetMessage(_T("SNYC 数据文件格式不合要求!! - wClipPointNums"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}
	else if(iClipArea>3 && iClipArea<100)
	{
		// 11.读入剪切边界点坐标
		for(i=0;i<iClipArea;i++)
		{
			double x,y;
			_ftscanf(fp,_T("%lf%lf"),&x,&y);
			pDataInfo->xClip[i] = x;
			pDataInfo->yClip[i] = y;
		}
	}
	else if(iClipArea==AXIN_FMT_CLIP_AREA_BOX)	// AXIN_FMT_CLIP_AREA_BOX = 9999
	{
		// 11.读入剪切边界范围
		double xmin,ymin,xmax,ymax;
		_ftscanf(fp,_T("%lf%lf%lf%lf"),&xmin,&ymin,&xmax,&ymax);
		pDataInfo->XClipMin() = min(xmin,xmax);
		pDataInfo->YClipMin() = min(ymin,ymax);
		pDataInfo->XClipMax() = max(xmin,xmax);
		pDataInfo->YClipMax() = max(ymin,ymax);
	}
	else
	{
		// no body
	}

	// 12.读入数据
	int iStationNum = pDataInfo->TotalNum();
	KStationInfo* pStationInfo = NULL;
	pStationInfoPtrCollection->Clear();
	// 进度参数
	ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
	if(pProgressParam) pProgressParam->SetInfo(_T("读取数据..."));
	for(i=0;i<iStationNum;i++)
	{
		pStationInfo = new KStationInfo();

		double lon,lat,alt;
		int lvl;

		_ftscanf(fp,_T("%s"),szTemp);
		pStationInfo->StationID() = szTemp;
		int iret = _ftscanf(fp,_T("%lf%lf%lf %d")
			,&lon		// 经度
			,&lat		// 维度
			,&alt		// 海拔
			,&lvl		// 级别
			);
		pStationInfo->Lon()		= lon;
		pStationInfo->Lat()		= lat;
		pStationInfo->Alt()		= alt;
		pStationInfo->Level()	= lvl;

		// 要素数量
		pStationInfo->ElementNum() = pDataInfo->ElementNumbers();
		for(j=0;j<pStationInfo->ElementNum();j++)
		{// 站点要素
			double ev;
			_ftscanf(fp,_T("%lf"),&ev);
			pStationInfo->ElementValue(j) = ev;
		}

		if(pDataInfo->Flag() !=0)
		{
			_ftscanf(fp,_T("%s"),szTemp);	// 站点名称
			pStationInfo->StationName() = szTemp;
		}

		// 显示进度
		if(m_pCallbackFunc)
		{
			if(m_pCallbackParam)
			{
				ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
				pProgressParam->lTotalNums = iStationNum;
				pProgressParam->lCurrentPos = i;
			}

			m_pCallbackFunc(m_pCallbackParam);
		}

		if(feof(fp))
		{
			_delete(pStationInfo);
			break;
		}

		pStationInfoPtrCollection->Add(pStationInfo);

	}// next i;

	// 关闭文件
	fclose(fp);
	fp = NULL;

	// 修正站点数量
	pDataInfo->TotalNum() = pStationInfoPtrCollection->Count();

	// 计算绑定矩形
	this->ComputeExtents();

	// 查找极小值和极大值
	// 顺便计算平均值
	this->LookupExtremum();

	WORD wContourNums = pDataInfo->ContourNumbers();
	// 自动计算等值线的等值线间隔值、等值线最小值、 等值线最大值以及加粗显示的等值线值
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

	// 构造默认调色板
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
BOOL KStationData::BuildContours()
{
	if(IsEmpty())
		return FALSE;

	if(!this->IsRebuildContour())
		return IsContourReady();
	else
		this->EnableRebuildContour(FALSE);

	// 计算绑定矩形
	this->ComputeExtents();

	// 查找极小值和极大值
	// 顺便计算平均值
	this->LookupExtremum();

	// 数据信息
	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return FALSE;

	KObjectPtrCollection* pStationInfoPtrCollection = this->GetStationInfoPtrCollection();
	if(!pStationInfoPtrCollection)
		return FALSE;

	// 不追踪等值线因此不需要调色板
	if(!this->IsTarceContour())
		return FALSE;

	// 构造默认调色板
	this->BuildPalette(NULL);

	// 包围盒
	KExtents box = this->GetExtents();

	//
	// 站点数据插值到格点数据
	//

	// 记录数量对插值有很大影响,
	// 因此要对记录数量有限制
	int nRecord = pStationInfoPtrCollection->Count();	// 记录数量
	if(nRecord>m_InterpolationRecordMax)	// 插值记录最大值
		return FALSE;

	double xInterval = 0.05;
	double yInterval = 0.05;

	double xmin = box.Xmin;
	double ymin = box.Ymin;
	double w = box.Width();
	double h = box.Height();
	double xmax = xmin + w + xInterval;
	double ymax = ymin + h + yInterval;

	int ni = pStationInfoPtrCollection->Count();
	double* pxi = new double[ni];
	double* pyi = new double[ni];
	double* pvi = new double[ni];

	for(int i=0;i<ni;i++)
	{
		KStationInfo* pStationInfo = (KStationInfo*)pStationInfoPtrCollection->GetAt(i);
		*(pxi+i) = (double)pStationInfo->Lon();
		*(pyi+i) = (double)pStationInfo->Lat();
		*(pvi+i) = (double)pStationInfo->ElementValue(m_iCurrentElementIndex);
	}

	//-----------------------------------------------------
	//1. 声明类对象
	KV2GInterpolate	v2g;
	//2. 设置源数据
	v2g.SetInput( pxi, pyi, pvi, ni);
	//3. 设置网格属性参数
	v2g.SetGridParam( xmin, ymin, xmax, ymax, xInterval, yInterval);
	//4. 设置扫描半径(km)
	//   如果扫描半径 =0,则自动计算扫描半径,,使扫描半径内至少有5个点
	//   如果扫描半径!=0,则按扫描半径计算,nVertexMin 参数无效
	v2g.SetRadius(0,5);
	//5. 实施插值
	v2g.Transact(0);
	//-----------------------------------------------------
	_deletea(pxi);
	_deletea(pyi);
	_deletea(pvi);
	//-----------------------------------------------------
	//取得结果
	int nx,ny;
	double* pData = v2g.GetOutput(&nx, &ny);

	KDataInfo dataInfo = *pDataInfo;
	dataInfo.XNum() = nx;
	dataInfo.YNum() = ny;
	dataInfo.MinX() = xmin;
	dataInfo.MinY() = ymin;
	dataInfo.XInterval() = xInterval;
	dataInfo.YInterval() = yInterval;

	// 计算 XY 坐标最大值
	dataInfo.XMax() = dataInfo.XMin() + dataInfo.XInterval() * (dataInfo.XNum()-1);
	dataInfo.YMax() = dataInfo.YMin() + dataInfo.YInterval() * (dataInfo.YNum()-1);

	// 等值线是否准备好
	BOOL bContoursReady = FALSE;

	if(m_pContourBuilder==NULL)
	{
		m_pContourBuilder = new KQContourBuilder();
	}
	KContourBuilder* pContourBuilder = m_pContourBuilder;

	if(!pContourBuilder->CallbackFunc())
		pContourBuilder->RegiestCallbackFunc(this->CallbackFunc(),this->CallbackParam());

	// 1.设置数据
	pContourBuilder->SetData(pData);
	// 2.设置数据信息
	pContourBuilder->SetDataInfo(&dataInfo);
	// 3.构造等值线
	bContoursReady = pContourBuilder->BuildContours();

	return bContoursReady;
}

// 计算绑定矩形
void KStationData::ComputeExtents()
{
	// 范围
	this->SetExtents(KExtents(0,0,0,0));
	
	if(IsEmpty())
		return;

	// 数据信息
	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return;

	double xmin = 361.0;
	double xmax = -361.0;
	double ymin = 91.0;
	double ymax = -91.0;

	KObjectPtrCollection* pStationInfoPtrCollection = this->GetStationInfoPtrCollection();
	int iStationCount = pStationInfoPtrCollection->Count();
	if(iStationCount<=0)
		return;

	// 剪切区标记
	int iClipArea = pDataInfo->ClipArea();
	if(iClipArea<=0)	// 自动计算剪切区
	{
		for(int i=0;i<iStationCount;i++)
		{
			KStationInfo* pStationInfo = (KStationInfo*)pStationInfoPtrCollection->GetAt(i);
			xmin = min(xmin,pStationInfo->Lon());
			xmax = max(xmax,pStationInfo->Lon());
			ymin = min(ymin,pStationInfo->Lat());
			ymax = max(ymax,pStationInfo->Lat());
		}
	}
	else if(iClipArea>3 && iClipArea<100)
	{
		for(int i=0;i<iClipArea;i++)
		{
			xmin = min(xmin,pDataInfo->XClip(i));
			xmax = max(xmax,pDataInfo->XClip(i));
			ymin = min(ymin,pDataInfo->YClip(i));
			ymax = max(ymax,pDataInfo->YClip(i));
		}
	}
	else if(iClipArea == AXIN_FMT_CLIP_AREA_BOX)
	{
		xmin = pDataInfo->XClipMin();
		ymin = pDataInfo->YClipMin();
		ymax = pDataInfo->YClipMax();
		xmax = pDataInfo->XClipMax();
	}
	else
	{
		// no body
	}

	// 范围
	this->SetExtents(KExtents(xmin,ymin,xmax,ymax));

	return;
}

// 查找SNYC数据极小值和极大值
void KStationData::LookupExtremum()
{
	if(IsEmpty())
		return;

	// 数据信息
	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return;

	KObjectPtrCollection* pStationInfoPtrCollection = GetStationInfoPtrCollection();
	int iStationCount = pStationInfoPtrCollection->Count();
	if(iStationCount<=0)
		return;

	double vAvg = 0;
	double vmin = ((KStationInfo*)pStationInfoPtrCollection->GetAt(0))->ElementValue(m_iCurrentElementIndex);
	double vmax = ((KStationInfo*)pStationInfoPtrCollection->GetAt(0))->ElementValue(m_iCurrentElementIndex);

	for(int i=0;i<iStationCount;i++)
	{
		KStationInfo* pStationInfo = (KStationInfo*)pStationInfoPtrCollection->GetAt(i);
		vAvg += pStationInfo->ElementValue(m_iCurrentElementIndex);
		vmin = min(vmin,pStationInfo->ElementValue(m_iCurrentElementIndex));
		vmax = max(vmax,pStationInfo->ElementValue(m_iCurrentElementIndex));
	}

	pDataInfo->vAvg = vAvg / iStationCount;
	pDataInfo->vExtremumMin = vmin;
	pDataInfo->vExtremumMax = vmax;

	return;
}

// 是否追踪等值线
BOOL KStationData::IsTarceContour()
{
	WORD wContourNums = this->GetDataInfo()->ContourNumbers();
	return ( (wContourNums>0 && wContourNums<100) || wContourNums == AXIN_FMT_CVID_AUTO || wContourNums == AXIN_FMT_CVID_CONTOUR );
}

// 取得站点要素数量
int KStationData::GetElementNumber()
{
	int nums = 0;

	KDataInfo* pDataInfo = this->GetDataInfo();
	if(pDataInfo)
		nums = pDataInfo->ElementNumbers();

	return nums;
}


//static
BOOL KStationData::SaveData(LPCTSTR lpszFile, KDataInfo* pDataInfo, KObjectPtrCollection* pStationInfoPtrCollection, int iPrecision/* = 8*/, int iDecimal/* = 3*/)
{
	// 数据信息
	if(!pDataInfo || !pStationInfoPtrCollection)
		return FALSE;

	// 数据集
	int iCount = pStationInfoPtrCollection->Count();
	if(iCount==0)
		return FALSE;

	//打开文件
	FILE* fp = _tfopen(lpszFile,_T("wt"));
	if(fp==NULL)
		return FALSE;

	// 1.写文件标识、格式代码、注释
	_ftprintf(fp,_T("%s %d %s\n"),AXIN_FID_TXT_MARK,AXIN_FMT_CODE_TIN,(LPCTSTR)pDataInfo->Comment());

	// 2.年、月、日、时、分、秒、毫秒
	_ftprintf(fp,_T("%4d %02d %02d %02d %02d %02d %d\n")
		,pDataInfo->Year()
		,pDataInfo->Month()
		,pDataInfo->Day()
		,pDataInfo->Hour()
		,pDataInfo->Minute()
		,pDataInfo->Second()
		,pDataInfo->Millisecond()
		);

	// 3.时效、层次、产品代码、要素代码
	_ftprintf(fp,_T("%02d %03d %d %d\n")
		,pDataInfo->TimePeriod()
		,pDataInfo->Layer()
		,pDataInfo->ProductCode()
		,pDataInfo->ElementCode()
		);

	// 4.站点记录数量、要素数量、站点名称标记
	_ftprintf(fp,_T("%d %d %d\n")
		,iCount
		,pDataInfo->ElementNumbers()
		,pDataInfo->Flag()
		);

	// 5.等值线
	int iContourNums = pDataInfo->ContourNumbers();
	_ftprintf(fp,_T("%d "),iContourNums);
	if(iContourNums == AXIN_FMT_CVID_CONTOUR)	// 9999
	{
		// 等值线间隔、最小值、最大值、加粗值
		_ftprintf(fp,_T("%8.3f %8.3f %8.3f %8.3f")
			,pDataInfo->ContourInterval()
			,pDataInfo->ContourMin()
			,pDataInfo->ContourMax()
			,pDataInfo->BoldContourValue()
			);
	}
	else if(iContourNums == AXIN_FMT_CVID_AUTO)
	{
		_ftprintf(fp,_T("%d"),pDataInfo->ContourPortion());
	}
	else if(iContourNums>0 && iContourNums<100)
	{
		for(int i=0;i<iContourNums;i++)
		{
			_ftprintf(fp,_T("%8.3f "),pDataInfo->ContourValue(i));
		}
		_ftprintf(fp,_T("%8.3f"),pDataInfo->BoldContourValue());
	}
	_ftprintf(fp,_T("\n"));

	// 6.剪切区边界
	int iClipArea = pDataInfo->ClipArea();
	_ftprintf(fp,_T("%d "),iClipArea);
	if(iClipArea>0 && iClipArea!=AXIN_FMT_CLIP_AREA_BOX)
	{
		for(int i=0;i<iClipArea;i++)
		{
			_ftprintf(fp,_T("%8.3f %8.3f "),pDataInfo->XClip(i),pDataInfo->YClip(i));
		}
	}
	else if(iClipArea == AXIN_FMT_CLIP_AREA_BOX)
	{
		_ftprintf(fp,_T("%8.3f %8.3f %8.3f %8.3f")
			,pDataInfo->XClipMin()
			,pDataInfo->YClipMin()
			,pDataInfo->XClipMax()
			,pDataInfo->YClipMax()
			);
	}
	else
	{
		// no body
	}
	_ftprintf(fp,_T("\n"));
	_ftprintf(fp,_T("\n"));

	// 7.数据
	for(int i=0;i<iCount;i++)
	{
		KStationInfo* pStationInfo = (KStationInfo*)pStationInfoPtrCollection->GetAt(i);

		// StationID Lon Lat Alt Level
		_ftprintf(fp,_T("%s %8.3f %8.3f %8.3f %3d ")
			,(LPCTSTR)pStationInfo->StationID()
			,pStationInfo->Lon()
			,pStationInfo->Lat()
			,pStationInfo->Alt()
			,pStationInfo->Level()
			);

		TCHAR szFormat[MAX_PATH] = _T("");
		_stprintf(szFormat,_T("%%%d.%df "),iPrecision,iDecimal);
		// 物理量要素
		for(int j=0;j<pDataInfo->ElementNumbers();j++)
		{
			_ftprintf(fp,szFormat,pStationInfo->ElementValue(j));
		}

		// 站点名称标记
		if(pDataInfo->Flag()!=0)
		{
			_ftprintf(fp,_T("  %s"),(LPCTSTR)pStationInfo->StationName());
		}

		// 下一行
		_ftprintf(fp,_T("\n"));
	}

	// 关闭文件
	fclose(fp);
	fp = NULL;

	return TRUE;
}

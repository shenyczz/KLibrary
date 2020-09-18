#include "StdAfx.h"
#include "KLightningData.h"

KLightningData::KLightningData(void)
{
	m_pLightningCollection = NULL;
}

KLightningData::~KLightningData(void)
{
	Empty();
}

// 清除数据并回收内存
void KLightningData::Empty()
{
	_delete(m_pLightningCollection);
}
// 是否为空
BOOL KLightningData::IsEmpty()
{
	return (m_pLightningCollection == NULL) ||
		   (m_pLightningCollection != NULL) && (0 == m_pLightningCollection->Count())
		   ;
}


BOOL KLightningData::LoadData_txt(LPCTSTR lpszFile)
{
	int i;
	int iTemp;
	TCHAR szTemp[MAX_PATH] = _T("");

	// 清空数据
	if(!IsEmpty())
		Empty();

	// 数据信息
	DataInfo* pDataInfo = this->GetDataInfo();

	// 站点信息集
	if(m_pLightningCollection==NULL)
	{
		m_pLightningCollection = new KLightningCollection();
	}
	KLightningCollection* pLightningCollection = m_pLightningCollection;

	//打开文件
	FILE* fp = _tfopen(lpszFile,_T("rt"));
	if(fp==NULL)
	{
		SetMessage(_T("打开文件错误! in KStationData::LoadData_txt"));
		return FALSE;
	}

	// 1.读文件标识
	int sizeFid = _tcslen(SNYC_FID);
	_ftscanf(fp,_T("%s"),szTemp);		//文件头标示,为字符串“SNYC”,大小写均可
	if( szTemp[sizeFid] != _T('\0') )
	{
		SetMessage(_T("SNYC 数据文件格式不合要求! - SNYC_FID"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}

	//转换为大写
	KBase::UpperString(szTemp);

	if( _tcscmp(szTemp,SNYC_FID) != 0 )
	{
		SetMessage(_T("SNYC 数据文件格式不合要求! - SNYC_FID"));
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
	_ftscanf(fp,_T("%d"),&pDataInfo->wFormatCode);
	if(pDataInfo->wFormatCode != SNYC_FMT_CODE_LIGHTNING)	// 130
	{
		SetMessage(_T("SNYC 数据文件格式不合要求! - wFmtCode"));
		fclose(fp);
		return FALSE;
	}

	// 3.读注释信息
	_ftscanf(fp,_T("%s"),szTemp);
	if( _tcslen(szTemp) > (SNYC_COMMENT_LENGTH) )
	{
		szTemp[SNYC_COMMENT_LENGTH] = _T('\0');
	}
	_tcscpy(pDataInfo->szComment,szTemp);

	// 4.读日期时间
	_ftscanf(fp,_T("%d%d%d %d%d%d")
		,&pDataInfo->wYear
		,&pDataInfo->wMonth
		,&pDataInfo->wDay

		,&pDataInfo->wHour
		,&pDataInfo->wMinute
		,&pDataInfo->wSecond
		);

	// 5.读时效、层次、产品代码
	_ftscanf(fp,_T("%d%d%d")
		,&pDataInfo->wTimePeriod
		,&pDataInfo->wLayer
		,&pDataInfo->wProductCode
		);

	// 6.读入闪电数量
	_ftscanf(fp,_T("%d"),&pDataInfo->wTotalNum);
	if( ((int)pDataInfo->wTotalNum) < 0)
	{
		SetMessage(_T("SNYC 数据文件格式不合要求! - wTotalNum"));
		fclose(fp);
		return FALSE;
	}

	// 7.读入数据
	int iLightningNum = pDataInfo->wTotalNum;
	KLightning* pLightning = NULL;
	pLightningCollection->Clear();
	for(i=0;i<iLightningNum;i++)
	{
		pLightning = new KLightning();

		KString kstring(_T(""));
		KStringArray astring;

		// YYYY-MM-DD
		_ftscanf(fp,_T("%s"),szTemp);
		kstring  = szTemp;
		astring = kstring.Split(_T('-'));
		if(astring.Count()!=3)
		{
			_delete(pLightning);
			break;
		}
		pLightning->wYear = _tstoi(astring[0]);
		pLightning->wMonth = _tstoi(astring[1]);
		pLightning->wDay = _tstoi(astring[2]);

		// hh:mm:ss
		_ftscanf(fp,_T("%s"),szTemp);
		kstring  = szTemp;
		astring = kstring.Split(_T(':'));
		if(astring.Count()!=3)
		{
			_delete(pLightning);
			break;
		}
		pLightning->wHour = _tstoi(astring[0]);
		pLightning->wMinute = _tstoi(astring[1]);
		pLightning->wSecond = _tstoi(astring[2]);

		_ftscanf(fp,_T("%u"),&pLightning->dwMillisecond);	// 毫秒

		_ftscanf(fp,_T("%d"),&pLightning->wType);	// 闪电类型

		_ftscanf(fp,_T("%f%f")
			,&pLightning->fLon			// 经度
			,&pLightning->fLat			// 纬度
			);

		_ftscanf(fp,_T("%f%f%f%f%f")
			,&pLightning->fIntensity	// 电流强度(KA)
			,&pLightning->fGradient		// 陡度(KA/us)
			,&pLightning->fEnergy		// 能量(兆瓦)
			,&pLightning->fCharge		// 电荷(C)
			,&pLightning->fLocateError	// 定位误差
			);

		_ftscanf(fp,_T("%d"),&iTemp);// 定位方法
		pLightning->wLocateMethod = (WORD)iTemp;

		if(feof(fp))
		{
			_delete(pLightning);
			break;
		}

		// 添加闪电数据
		pLightningCollection->Add(pLightning);

		// 显示进度
		if(m_pCallbackFunc)
		{
			if(m_pCallbackParam)
			{
				ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
				pProgressParam->lTotalNums = iLightningNum;
				pProgressParam->lCurrentPos = i;
			}

			m_pCallbackFunc(m_pCallbackParam);
		}
	}
	
	// 关闭文件
	fclose(fp);
	fp = NULL;

	// 修正闪电数量
	pDataInfo->wTotalNum = pLightningCollection->Count();

	// 计算绑定矩形
	this->ComputeBoundingBox();

	//pLightningCollection->Clear();

	return TRUE;
}

// 计算绑定矩形
void KLightningData::ComputeBoundingBox()
{
	this->SetBoundingBox(KBox(0,0,0,0));
}


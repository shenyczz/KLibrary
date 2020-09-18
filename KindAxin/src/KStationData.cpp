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

// ������ݲ������ڴ�
void KStationData::Empty()
{
	_delete(m_pContourBuilder);
	_delete(m_pStationInfoPtrCollection);
}
// �Ƿ�Ϊ��
BOOL KStationData::IsEmpty()
{
	return (m_pStationInfoPtrCollection == NULL)
		|| (m_pStationInfoPtrCollection != NULL) && (0 == m_pStationInfoPtrCollection->Count())
		   ;
}

// װ������ - ASC�ļ�
BOOL KStationData::LoadData_txt(LPCTSTR lpszFile)
{
	int i,j;
	int iTemp = 0;
	TCHAR szTemp[MAX_PATH] = _T("");

	// �������
	if(!IsEmpty())
		Empty();

	// ������Ϣ
	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return FALSE;

	// վ����Ϣ��
	if(m_pStationInfoPtrCollection==NULL)
	{
		m_pStationInfoPtrCollection = new KObjectPtrCollection();
	}
	m_pStationInfoPtrCollection->Clear();
	KObjectPtrCollection* pStationInfoPtrCollection = m_pStationInfoPtrCollection;

	//���ļ�
	FILE* fp = _tfopen(lpszFile,_T("rt"));
	if(fp==NULL)
	{
		SetMessage(_T("���ļ�����! in KStationData::LoadData_txt"));
		return FALSE;
	}

	// 1.���ļ���ʶ
	int sizeFid = _tcslen(AXIN_FID_TXT_MARK);
	_ftscanf(fp,_T("%s"),szTemp);		//�ļ�ͷ��ʾ,Ϊ�ַ�����AXIN��,��Сд����
	if( szTemp[sizeFid] != _T('\0') )
	{
		SetMessage(_T("�����ļ���ʽ����Ҫ��! - AXIN_FID_TXT_MARK"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}
	//ת��Ϊ��д
	KString::UpperString(szTemp);
	if( _tcscmp(szTemp,AXIN_FID_TXT_MARK) != 0 )
	{
		SetMessage(_T("�����ļ���ʽ����Ҫ��! - AXIN_FID_TXT_MARK"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}
	else
	{
		// �����ļ���ʶ
		_tcscpy(pDataInfo->szFileID,szTemp);
	}

	// 2.����Ʒ���ݸ�ʽ����
	_ftscanf(fp,_T("%d"),&iTemp);
	if(iTemp != AXIN_FMT_CODE_TIN)	// AXIN_FMT_CODE_TIN = 30
	{
		SetMessage(_T("SNYC �����ļ���ʽ����Ҫ��! - wFmtCode"));
		fclose(fp);
		return FALSE;
	}
	// �����ʽ����
	pDataInfo->FormatCode() = iTemp;

	// 3.��ע����Ϣ
	_ftscanf(fp,_T("%s"),szTemp);
	if( _tcslen(szTemp) > (AXIN_COMMENT_LENGTH) )
	{
		szTemp[AXIN_COMMENT_LENGTH] = _T('\0');
	}
	// ����ע����Ϣ
	pDataInfo->Comment() = szTemp;

	// 4.������ʱ��(yyyy,mm,dd,HH,MM,SS,MS)
	int yyyy,mm,dd,HH,MM,SS,MS;
	_ftscanf(fp,_T("%d%d%d %d%d%d%d"), &yyyy, &mm, &dd, &HH, &MM, &SS, &MS);
	pDataInfo->Year()			= yyyy;
	pDataInfo->Month()			= mm;
	pDataInfo->Day()			= dd;
	pDataInfo->Hour()			= HH;
	pDataInfo->Minute()			= MM;
	pDataInfo->Second()			= SS;
	pDataInfo->Millisecond()	= MS;

	// 5.��ʱЧ����Ρ���Ʒ���롢Ҫ�ش���
	int tt,ll,pc,ec;
	_ftscanf(fp,_T("%d%d%d%d"),&tt, &ll, &pc, &ec);
	pDataInfo->TimePeriod()		= tt;
	pDataInfo->Layer()			= ll;
	pDataInfo->ProductCode()	= pc;
	pDataInfo->ElementCode()	= ec;

	// 6.����վ������
	_ftscanf(fp,_T("%d"),&pDataInfo->wTotalNum);
	if( ((int)pDataInfo->wTotalNum) < 0)
	{
		SetMessage(_T("SNYC �����ļ���ʽ����Ҫ��! - wTotalNum"));
		fclose(fp);
		return FALSE;
	}

	// 7.����ÿ��վ��Ҫ������
	_ftscanf(fp,_T("%d"),&pDataInfo->wElementNum);
	if( ((int)pDataInfo->wElementNum) < 0 || pDataInfo->wElementNum>50) // <=50
	{
		SetMessage(_T("SNYC �����ļ���ʽ����Ҫ��! - wElementNum"));
		fclose(fp);
		return FALSE;
	}

	// 8.��ȡ FLAG
	_ftscanf(fp,_T("%d"),&pDataInfo->dwFlag);

	// 9.����ֵ��ֵ����
	int iContourNums;
	_ftscanf(fp,_T("%d"),&iContourNums);
	pDataInfo->ContourNumbers()	= iContourNums;
	if(iContourNums>0)
	{
		// �Ƿ�涨��ֵ
		if(iContourNums < AXIN_FMT_CVID_MIN || 
			(iContourNums > AXIN_FMT_CVID_MAX) && (iContourNums != AXIN_FMT_CVID_AUTO) && (iContourNums != AXIN_FMT_CVID_CONTOUR))
		{
			SetMessage(_T("��ֵ������������Ҫ��! - wContourNums"));
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		// �� wContourNums=9999 ʱ��ʾ����������������ʾ
		//    ��ֵ�߼��ֵ ��ֵ����Сֵ ��ֵ�����ֵ
		else if(iContourNums == AXIN_FMT_CVID_CONTOUR)	// 9999
		{
			_ftscanf(fp,_T("%lf %lf %lf")
				,&pDataInfo->dContourInterval
				,&pDataInfo->dContourMin
				,&pDataInfo->dContourMax
				);

			if(pDataInfo->dContourMin >= pDataInfo->dContourMax || (pDataInfo->dContourInterval < 0) || fabs(pDataInfo->dContourInterval)<EPS)
			{
				SetMessage(_T("SNYC �����ļ���ʽ����Ҫ��!! - dContourInterval"));
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

		}
		// �� wContourNums=8888 ʱ
		// ��ʾ�Զ������ֵ�ߵĵ�ֵ�߼��ֵ����ֵ����Сֵ�� ��ֵ�����ֵ�Լ��Ӵ���ʾ�ĵ�ֵ��ֵ
		else if(iContourNums == AXIN_FMT_CVID_AUTO)	// 8888
		{
			// �Զ������ֵ�ߵĵ�ֵ�߼��ֵ����ֵ����Сֵ�� ��ֵ�����ֵ�Լ��Ӵ���ʾ�ĵ�ֵ��ֵ
			// �ں���������ݼ�ֵ����
			int iContourPortion;
			_ftscanf(fp,_T("%d"), &iContourPortion);
			if(iContourPortion<10 || iContourPortion>50)	// ȡ֮��Χ��[10,50]
			{
				iContourPortion = 10;
			}
			pDataInfo->ContourPortion() = iContourPortion;
		}
		// �� wContourNums = [1,99] ʱ��ʾ
		// ���������� wContourNums ��Ҫ��׷�ٵĵ�ֵ��ֵ��
		else if(iContourNums >=1 && iContourNums<=99)
		{
			for(i=0;i<pDataInfo->wContourNums;i++)
			{
				_ftscanf(fp,_T("%lf"),&pDataInfo->dContourValue[i]);
			}
		}
		else
		{
			// �������
		}

		// ��ȡ�Ӵ���ʾ�ĵ�ֵ��ֵ(�Զ���ֵ���޷�ȷ����ֵ�ߵ�ֵ)
		if(iContourNums != 0 && iContourNums!=AXIN_FMT_CVID_AUTO)
		{
			_ftscanf(fp,_T("%lf"),&pDataInfo->dBoldContourValue);
		}

	}// if(iContourNums>0)

	// 10.������
	int iClipArea = 0;
	_ftscanf(fp,_T("%d"),&iClipArea);
	pDataInfo->ClipArea() = iClipArea;
	if( (iClipArea<4 && iClipArea!=0) || (iClipArea>99 && iClipArea!=AXIN_FMT_CLIP_AREA_BOX))
	{
		// ����4����
		SetMessage(_T("SNYC �����ļ���ʽ����Ҫ��!! - wClipPointNums"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}
	else if(iClipArea>3 && iClipArea<100)
	{
		// 11.������б߽������
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
		// 11.������б߽緶Χ
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

	// 12.��������
	int iStationNum = pDataInfo->TotalNum();
	KStationInfo* pStationInfo = NULL;
	pStationInfoPtrCollection->Clear();
	// ���Ȳ���
	ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
	if(pProgressParam) pProgressParam->SetInfo(_T("��ȡ����..."));
	for(i=0;i<iStationNum;i++)
	{
		pStationInfo = new KStationInfo();

		double lon,lat,alt;
		int lvl;

		_ftscanf(fp,_T("%s"),szTemp);
		pStationInfo->StationID() = szTemp;
		int iret = _ftscanf(fp,_T("%lf%lf%lf %d")
			,&lon		// ����
			,&lat		// ά��
			,&alt		// ����
			,&lvl		// ����
			);
		pStationInfo->Lon()		= lon;
		pStationInfo->Lat()		= lat;
		pStationInfo->Alt()		= alt;
		pStationInfo->Level()	= lvl;

		// Ҫ������
		pStationInfo->ElementNum() = pDataInfo->ElementNumbers();
		for(j=0;j<pStationInfo->ElementNum();j++)
		{// վ��Ҫ��
			double ev;
			_ftscanf(fp,_T("%lf"),&ev);
			pStationInfo->ElementValue(j) = ev;
		}

		if(pDataInfo->Flag() !=0)
		{
			_ftscanf(fp,_T("%s"),szTemp);	// վ������
			pStationInfo->StationName() = szTemp;
		}

		// ��ʾ����
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

	// �ر��ļ�
	fclose(fp);
	fp = NULL;

	// ����վ������
	pDataInfo->TotalNum() = pStationInfoPtrCollection->Count();

	// ����󶨾���
	this->ComputeExtents();

	// ���Ҽ�Сֵ�ͼ���ֵ
	// ˳�����ƽ��ֵ
	this->LookupExtremum();

	WORD wContourNums = pDataInfo->ContourNumbers();
	// �Զ������ֵ�ߵĵ�ֵ�߼��ֵ����ֵ����Сֵ�� ��ֵ�����ֵ�Լ��Ӵ���ʾ�ĵ�ֵ��ֵ
	if(wContourNums == AXIN_FMT_CVID_AUTO)
	{
		int iContourPortion = pDataInfo->ContourPortion();
		//if(iContourPortion<10 || iContourPortion>50) iContourPortion = 10;
		double vmin = pDataInfo->ExtremumValueMin();
		double vmax = pDataInfo->ExtremumValueMax();
		double vinterval = (vmax - vmin) / iContourPortion;			// �ֳ�iContourPortion��
		// ���ֵ����1ʱʹ���������
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

	// ����Ĭ�ϵ�ɫ��
	this->BuildPalette(NULL);

	// �Ƿ����ֵ��
	if( (wContourNums>0 && wContourNums<100) || wContourNums == AXIN_FMT_CVID_AUTO || wContourNums == AXIN_FMT_CVID_CONTOUR )
	{
		// �����ֵ��
		this->BuildContours();
	}

	return TRUE;
}

// �����ֵ��
BOOL KStationData::BuildContours()
{
	if(IsEmpty())
		return FALSE;

	if(!this->IsRebuildContour())
		return IsContourReady();
	else
		this->EnableRebuildContour(FALSE);

	// ����󶨾���
	this->ComputeExtents();

	// ���Ҽ�Сֵ�ͼ���ֵ
	// ˳�����ƽ��ֵ
	this->LookupExtremum();

	// ������Ϣ
	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return FALSE;

	KObjectPtrCollection* pStationInfoPtrCollection = this->GetStationInfoPtrCollection();
	if(!pStationInfoPtrCollection)
		return FALSE;

	// ��׷�ٵ�ֵ����˲���Ҫ��ɫ��
	if(!this->IsTarceContour())
		return FALSE;

	// ����Ĭ�ϵ�ɫ��
	this->BuildPalette(NULL);

	// ��Χ��
	KExtents box = this->GetExtents();

	//
	// վ�����ݲ�ֵ���������
	//

	// ��¼�����Բ�ֵ�кܴ�Ӱ��,
	// ���Ҫ�Լ�¼����������
	int nRecord = pStationInfoPtrCollection->Count();	// ��¼����
	if(nRecord>m_InterpolationRecordMax)	// ��ֵ��¼���ֵ
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
	//1. ���������
	KV2GInterpolate	v2g;
	//2. ����Դ����
	v2g.SetInput( pxi, pyi, pvi, ni);
	//3. �����������Բ���
	v2g.SetGridParam( xmin, ymin, xmax, ymax, xInterval, yInterval);
	//4. ����ɨ��뾶(km)
	//   ���ɨ��뾶 =0,���Զ�����ɨ��뾶,,ʹɨ��뾶��������5����
	//   ���ɨ��뾶!=0,��ɨ��뾶����,nVertexMin ������Ч
	v2g.SetRadius(0,5);
	//5. ʵʩ��ֵ
	v2g.Transact(0);
	//-----------------------------------------------------
	_deletea(pxi);
	_deletea(pyi);
	_deletea(pvi);
	//-----------------------------------------------------
	//ȡ�ý��
	int nx,ny;
	double* pData = v2g.GetOutput(&nx, &ny);

	KDataInfo dataInfo = *pDataInfo;
	dataInfo.XNum() = nx;
	dataInfo.YNum() = ny;
	dataInfo.MinX() = xmin;
	dataInfo.MinY() = ymin;
	dataInfo.XInterval() = xInterval;
	dataInfo.YInterval() = yInterval;

	// ���� XY �������ֵ
	dataInfo.XMax() = dataInfo.XMin() + dataInfo.XInterval() * (dataInfo.XNum()-1);
	dataInfo.YMax() = dataInfo.YMin() + dataInfo.YInterval() * (dataInfo.YNum()-1);

	// ��ֵ���Ƿ�׼����
	BOOL bContoursReady = FALSE;

	if(m_pContourBuilder==NULL)
	{
		m_pContourBuilder = new KQContourBuilder();
	}
	KContourBuilder* pContourBuilder = m_pContourBuilder;

	if(!pContourBuilder->CallbackFunc())
		pContourBuilder->RegiestCallbackFunc(this->CallbackFunc(),this->CallbackParam());

	// 1.��������
	pContourBuilder->SetData(pData);
	// 2.����������Ϣ
	pContourBuilder->SetDataInfo(&dataInfo);
	// 3.�����ֵ��
	bContoursReady = pContourBuilder->BuildContours();

	return bContoursReady;
}

// ����󶨾���
void KStationData::ComputeExtents()
{
	// ��Χ
	this->SetExtents(KExtents(0,0,0,0));
	
	if(IsEmpty())
		return;

	// ������Ϣ
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

	// ���������
	int iClipArea = pDataInfo->ClipArea();
	if(iClipArea<=0)	// �Զ����������
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

	// ��Χ
	this->SetExtents(KExtents(xmin,ymin,xmax,ymax));

	return;
}

// ����SNYC���ݼ�Сֵ�ͼ���ֵ
void KStationData::LookupExtremum()
{
	if(IsEmpty())
		return;

	// ������Ϣ
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

// �Ƿ�׷�ٵ�ֵ��
BOOL KStationData::IsTarceContour()
{
	WORD wContourNums = this->GetDataInfo()->ContourNumbers();
	return ( (wContourNums>0 && wContourNums<100) || wContourNums == AXIN_FMT_CVID_AUTO || wContourNums == AXIN_FMT_CVID_CONTOUR );
}

// ȡ��վ��Ҫ������
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
	// ������Ϣ
	if(!pDataInfo || !pStationInfoPtrCollection)
		return FALSE;

	// ���ݼ�
	int iCount = pStationInfoPtrCollection->Count();
	if(iCount==0)
		return FALSE;

	//���ļ�
	FILE* fp = _tfopen(lpszFile,_T("wt"));
	if(fp==NULL)
		return FALSE;

	// 1.д�ļ���ʶ����ʽ���롢ע��
	_ftprintf(fp,_T("%s %d %s\n"),AXIN_FID_TXT_MARK,AXIN_FMT_CODE_TIN,(LPCTSTR)pDataInfo->Comment());

	// 2.�ꡢ�¡��ա�ʱ���֡��롢����
	_ftprintf(fp,_T("%4d %02d %02d %02d %02d %02d %d\n")
		,pDataInfo->Year()
		,pDataInfo->Month()
		,pDataInfo->Day()
		,pDataInfo->Hour()
		,pDataInfo->Minute()
		,pDataInfo->Second()
		,pDataInfo->Millisecond()
		);

	// 3.ʱЧ����Ρ���Ʒ���롢Ҫ�ش���
	_ftprintf(fp,_T("%02d %03d %d %d\n")
		,pDataInfo->TimePeriod()
		,pDataInfo->Layer()
		,pDataInfo->ProductCode()
		,pDataInfo->ElementCode()
		);

	// 4.վ���¼������Ҫ��������վ�����Ʊ��
	_ftprintf(fp,_T("%d %d %d\n")
		,iCount
		,pDataInfo->ElementNumbers()
		,pDataInfo->Flag()
		);

	// 5.��ֵ��
	int iContourNums = pDataInfo->ContourNumbers();
	_ftprintf(fp,_T("%d "),iContourNums);
	if(iContourNums == AXIN_FMT_CVID_CONTOUR)	// 9999
	{
		// ��ֵ�߼������Сֵ�����ֵ���Ӵ�ֵ
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

	// 6.�������߽�
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

	// 7.����
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
		// ������Ҫ��
		for(int j=0;j<pDataInfo->ElementNumbers();j++)
		{
			_ftprintf(fp,szFormat,pStationInfo->ElementValue(j));
		}

		// վ�����Ʊ��
		if(pDataInfo->Flag()!=0)
		{
			_ftprintf(fp,_T("  %s"),(LPCTSTR)pStationInfo->StationName());
		}

		// ��һ��
		_ftprintf(fp,_T("\n"));
	}

	// �ر��ļ�
	fclose(fp);
	fp = NULL;

	return TRUE;
}

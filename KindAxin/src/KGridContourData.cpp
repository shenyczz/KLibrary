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

// װ������ - ASC�ļ�
BOOL KGridContourData::LoadData_txt(LPCTSTR lpszFile)
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
	int sizeFid = _tcslen(AXIN_FID_TXT_MARK); 
	_ftscanf(fp,_T("%s"),szTemp);		// �ļ�ͷ��ʾ,Ϊ�ַ��� _T("AXIN"),��Сд����
	if( szTemp[sizeFid] != _T('\0') )
	{
		SetMessage(_T("�����ļ���ʽ����Ҫ��! - FILE_ID"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}
	//ת��Ϊ��д
	KString::UpperString(szTemp);
	if( _tcscmp(szTemp,AXIN_FID_TXT_MARK) != 0 )
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
	if(iTemp != AXIN_FMT_CODE_GRID)	// LEAP_FMT_CODE_GRID = 40
	{
		SetMessage(_T("SNYC �����ļ���ʽ����Ҫ��! - wFormatCode"));
		fclose(fp);
		return FALSE;
	}
	// �����ʽ����
	pDataInfo->FormatCode() = iTemp;

	// 3.��ע����Ϣ
	_ftscanf(fp,_T("%s"),szTemp);
	if( _tcslen(szTemp) > (AXIN_COMMENT_LENGTH) )	// LEAP_COMMENT_LENGTH = 64
	{
		// ע����Ϣ̫����ض�
		szTemp[AXIN_COMMENT_LENGTH] = _T('\0');
	}
	pDataInfo->SetComment(szTemp);

	// 4.������ʱ��(yyyy,mm,dd,HH,MM,SS,MS)
	int yyyy,mm,dd,HH,MM,SS,MS;
	_ftscanf(fp,_T("%d%d%d %d%d%d%s"), &yyyy, &mm, &dd, &HH, &MM, &SS, &MS);
	pDataInfo->Year()			= yyyy;
	pDataInfo->Month()			= mm;
	pDataInfo->Day()			= dd;
	pDataInfo->Hour()			= HH;
	pDataInfo->Minute()			= MM;
	pDataInfo->Second()			= SS;
	pDataInfo->Millisecond()	= MS;

	// 5.��ʱЧ����Ρ���Ʒ���롢Ҫ�ش���
	int tp,ly,pc,ec;
	_ftscanf(fp,_T("%d%d%d%d"),&tp,&ly,&pc,&ec);
	pDataInfo->TimePeriod()		= tp;
	pDataInfo->Layer()			= ly;
	pDataInfo->ProductCode()	= pc;
	pDataInfo->ElementCode()	= ec;

    // 6.����������������Ϣ(nx,xmin,xinterval)(ny,ymin,yinterval)
	int nx,ny;
	double xmin,xinterval,ymin,yinterval;
	_ftscanf(fp,_T("%d %lf %lf"), &nx, &xmin, &xinterval);
	pDataInfo->Width()		= nx;			// X��������
	pDataInfo->MinX()		= xmin;			// X������Сֵ
	pDataInfo->XInterval()	= xinterval;	// X������ֵ

	_ftscanf(fp,_T("%d %lf %lf"), &ny, &ymin, &yinterval);
	pDataInfo->Height()		= ny;			// Y��������
	pDataInfo->MinY()		= ymin;			// Y������Сֵ
	pDataInfo->YInterval()	= yinterval;	// Y������ֵ

	// ���� XY �������ֵ
	pDataInfo->MaxX() = pDataInfo->MinX() + pDataInfo->XInterval() * (pDataInfo->Width()-1);
	pDataInfo->MaxY() = pDataInfo->MinY() + pDataInfo->YInterval() * (pDataInfo->Height()-1);

	// 7.�����ֵ��ֵ����
	int iContourNums;
	_ftscanf(fp,_T("%d"),&iContourNums);
	pDataInfo->ContourNumbers() = iContourNums;
	if(iContourNums>0)
	{
		// ��ֵ�������Ƿ�涨��ֵ
		if(iContourNums<AXIN_FMT_CVID_MIN ||
			(iContourNums>AXIN_FMT_CVID_MAX && iContourNums!=AXIN_FMT_CVID_AUTO && iContourNums!=AXIN_FMT_CVID_CONTOUR))
		{
			SetMessage(_T("��ֵ������������Ҫ��! - wContourNums"));
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		// �� wContourNums=9999 ʱ
		// ��ʾ����������������: ��ֵ�߼��ֵ,��ֵ����Сֵ,��ֵ�����ֵ
		else if(iContourNums == AXIN_FMT_CVID_CONTOUR)	// 9999
		{
			double cinterval,cmin,cmax;
			_ftscanf(fp,_T("%lf %lf %lf"), &cinterval, &cmin, &cmax);
			if(cmin > cmax || (cinterval < 0) || fabs(cinterval)<EPS)
			{
				SetMessage(_T("SNYC �����ļ���ʽ����Ҫ��!! - dContourInterval"));
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
			pDataInfo->ContourInterval()	= cinterval;
			pDataInfo->ContourMin()			= cmin;
			pDataInfo->ContourMax()			= cmax;
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
			for(i=0;i<iContourNums;i++)
			{
				_ftscanf(fp,_T("%lf"),&dTemp);
				pDataInfo->ContourValue(i) = dTemp;
			}
		}
		else
		{
			// �������
		}

		// ��ȡ�Ӵ���ʾ�ĵ�ֵ��ֵ
		if(iContourNums != 0 && iContourNums!=AXIN_FMT_CVID_AUTO)
		{
			_ftscanf(fp,_T("%lf"),&dTemp);
			pDataInfo->BoldContourValue() = dTemp;
		}

	}// if(nContours>0)

	// 8.Ϊ���ݷ����ڴ�
	PrepareMemory(eUvw_v);

	// 9.��������
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

	// �Զ������ֵ�ߵĵ�ֵ�߼��ֵ����ֵ����Сֵ�� ��ֵ�����ֵ�Լ��Ӵ���ʾ�ĵ�ֵ��ֵ
	WORD wContourNums = pDataInfo->ContourNumbers();
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
BOOL KGridContourData::BuildContours()
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

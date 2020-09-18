#include "StdAfx.h"
#include "KWindVectorData.h"


KWindVectorData::KWindVectorData(void)
{
}


KWindVectorData::~KWindVectorData(void)
{
}

// ����װ������
BOOL KWindVectorData::LoadData_txt(LPCTSTR lpszFile)
{
	int i,j;
	TCHAR szTemp[MAX_PATH] = _T("");

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
		SetMessage(_T("���ļ�����! in KWindVectorData::LoadData_txt"));
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
	// �����ļ���ʶ
	_tcscpy(pDataInfo->szFileID,szTemp);

	// 2.����Ʒ���ݸ�ʽ����
	int iFormatCode = 0;
	_ftscanf(fp,_T("%d"),&iFormatCode);
	if(iFormatCode != AXIN_FMT_CODE_GRID_WIND_VECTOR)	// 49
	{
		SetMessage(_T("�����ļ���ʽ����Ҫ��! - iFormatCode"));
		fclose(fp);
		return FALSE;
	}
	// �����ļ���ʽ
	pDataInfo->FormatCode() = iFormatCode;

	// 3.��ע����Ϣ
	_ftscanf(fp,_T("%s"),szTemp);
	if( _tcslen(szTemp) > (AXIN_COMMENT_LENGTH) )	// AXIN_COMMENT_LENGTH = 64
	{
		szTemp[AXIN_COMMENT_LENGTH] = _T('\0');
	}
	// ����ע����Ϣ
	pDataInfo->Comment() = szTemp;

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
	int tt,ll,pc,ec;
	_ftscanf(fp,_T("%d%d%d%d"),&tt,&ll,&pc,&ec);
	pDataInfo->TimePeriod()		= tt;
	pDataInfo->Layer()			= ll;
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

	// 7.�����ڴ�
	PrepareMemory(eUvw_uv);

	// 8.��������
	nx = pDataInfo->XNum();
	ny = pDataInfo->YNum();
	// ���Ȳ���
	ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;

	// 8.1 ���������� - U
	if(pProgressParam) pProgressParam->SetInfo(_T("��ȡ����..."));
	for(i=0; i<ny;i++)
	{
		for(j=0;j<nx;j++)
		{
			_ftscanf(fp,_T("%lf"),m_pu+i*nx+j);
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

	// 8.2 ���������� - V
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
	this->SetExtents(KExtents(pDataInfo->XMin(),pDataInfo->YMin(),pDataInfo->XMax(),pDataInfo->YMax()));

	//���Ҽ�Сֵ�ͼ���ֵ
	this->LookupExtremum();

	return TRUE;
}

// �����������ݼ�Сֵ������ֵ
// pDataInfo->VExtremumMin - ���漫Сֵ
// pDataInfo->VExtremumMax - ���漫��ֵ
void KWindVectorData::LookupExtremum()
{
	int i,j;
	KDataInfo* pDataInfo = this->GetDataInfo();

	int nx = pDataInfo->XNum();
	int ny = pDataInfo->YNum();

	//����U��С�����ֵ
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

	//����V��С�����ֵ
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

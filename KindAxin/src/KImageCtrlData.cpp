#include "StdAfx.h"
#include "KImageCtrlData.h"

#include "KAxinDataProcessor.h"


KImageCtrlData::KImageCtrlData(void)
{
	m_pDataProcessor = new KAxinDataProcessor();
}


KImageCtrlData::~KImageCtrlData(void)
{
}

// ����װ������
BOOL KImageCtrlData::LoadData_txt(LPCTSTR lpszFile)
{
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
		SetMessage(_T("���ļ�����! in KImageCtrlData::LoadData_txt"));
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
	if(iFormatCode != AXIN_FMT_CODE_IMAGE_CTRL_INFO)	// 70
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

	// 5.��ʱЧ����Ρ�ͶӰ���͡�Ҫ�ش���
	int tt,ll,prj,ec;
	_ftscanf(fp,_T("%d%d%d%d"),&tt,&ll,&prj,&ec);
	pDataInfo->TimePeriod()		= tt;
	pDataInfo->Layer()			= ll;
	pDataInfo->ProjectType()	= prj;
	pDataInfo->ElementCode()	= ec;

	// 6.���󶨾���
	double xmin,ymin,xmax,ymax;
	_ftscanf(fp,_T("%lf%lf%lf%lf"),&xmin,&ymin,&xmax,&ymax);
	pDataInfo->XMin() = xmin;
	pDataInfo->YMin() = ymin;
	pDataInfo->XMax() = xmax;
	pDataInfo->YMax() = ymax;

	// 7.ͼ���ļ�
	TCHAR szImageFile[MAX_PATH];
	_ftscanf(fp,_T("%s"),szImageFile);

	// �ر��ļ�
	fclose(fp);
	fp = NULL;

	// ���ð󶨾���
	this->SetExtents(KExtents(pDataInfo->XMin(),pDataInfo->YMin(),pDataInfo->XMax(),pDataInfo->YMax()));

	// ͼ���ļ�
	KFile imgFile(szImageFile);
	KString imgPath = imgFile.GetPath();
	if(imgPath.IsEmpty())
	{
		// �����ļ�
		KFile ctlFile(this->GetFileName());
		KString ctlPath = ctlFile.GetPath();

		_stprintf(szImageFile,_T("%s%s"),(LPCTSTR)ctlPath,(LPCTSTR)imgFile.GetFileName(true));
	}

	if(!KFile::Exists(szImageFile))
	{
		SetMessage(_T("�����ļ�������! - KImageCtrlData::LoadData_txt(...)"));
		return FALSE;
	}

	Bitmap* pImage = Bitmap::FromFile(KString::ToWChar(szImageFile));
	if(Status::Ok != pImage->GetLastStatus())
	{
		_delete(pImage);
		SetMessage(_T("����ͼ�����! - KImageCtrlData::LoadData_txt(...)"));
		return FALSE;
	}
	else
	{
		KDib* pDib = NULL;
		HBITMAP hBitmap = NULL;
		pImage->GetHBITMAP(Color::Black,&hBitmap);
		if(hBitmap)
		{
			pDib = new KDib();
			pDib->AttachHBITMAP(hBitmap);
			this->GetDataProcessor()->SetDib(pDib);
		}

		_delete(pImage);
		//pDib->Save(_T("e:\\temp\\1.bmp"));
	}

	return TRUE;
}

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

// 重载装载数据
BOOL KImageCtrlData::LoadData_txt(LPCTSTR lpszFile)
{
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
		SetMessage(_T("打开文件错误! in KImageCtrlData::LoadData_txt"));
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
	if(iFormatCode != AXIN_FMT_CODE_IMAGE_CTRL_INFO)	// 70
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

	// 5.读时效、层次、投影类型、要素代码
	int tt,ll,prj,ec;
	_ftscanf(fp,_T("%d%d%d%d"),&tt,&ll,&prj,&ec);
	pDataInfo->TimePeriod()		= tt;
	pDataInfo->Layer()			= ll;
	pDataInfo->ProjectType()	= prj;
	pDataInfo->ElementCode()	= ec;

	// 6.读绑定矩形
	double xmin,ymin,xmax,ymax;
	_ftscanf(fp,_T("%lf%lf%lf%lf"),&xmin,&ymin,&xmax,&ymax);
	pDataInfo->XMin() = xmin;
	pDataInfo->YMin() = ymin;
	pDataInfo->XMax() = xmax;
	pDataInfo->YMax() = ymax;

	// 7.图像文件
	TCHAR szImageFile[MAX_PATH];
	_ftscanf(fp,_T("%s"),szImageFile);

	// 关闭文件
	fclose(fp);
	fp = NULL;

	// 设置绑定矩形
	this->SetExtents(KExtents(pDataInfo->XMin(),pDataInfo->YMin(),pDataInfo->XMax(),pDataInfo->YMax()));

	// 图像文件
	KFile imgFile(szImageFile);
	KString imgPath = imgFile.GetPath();
	if(imgPath.IsEmpty())
	{
		// 控制文件
		KFile ctlFile(this->GetFileName());
		KString ctlPath = ctlFile.GetPath();

		_stprintf(szImageFile,_T("%s%s"),(LPCTSTR)ctlPath,(LPCTSTR)imgFile.GetFileName(true));
	}

	if(!KFile::Exists(szImageFile))
	{
		SetMessage(_T("数据文件不存在! - KImageCtrlData::LoadData_txt(...)"));
		return FALSE;
	}

	Bitmap* pImage = Bitmap::FromFile(KString::ToWChar(szImageFile));
	if(Status::Ok != pImage->GetLastStatus())
	{
		_delete(pImage);
		SetMessage(_T("生成图像错误! - KImageCtrlData::LoadData_txt(...)"));
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

#include "StdAfx.h"
#include "KColortableData.h"


KColortableData::KColortableData(void)
{
}


KColortableData::~KColortableData(void)
{
}

// 装载数据 - ASC文件
BOOL KColortableData::LoadData_txt(LPCTSTR lpszFile)
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
	int iFormatCode;
	_ftscanf(fp,_T("%d"),&iFormatCode);
	if(iFormatCode != AXIN_FMT_CODE_COLOR_TABLE)	// AXIN_FMT_CODE_COLOR_TABLE = 60
	{
		SetMessage(_T("SNYC 数据文件格式不合要求! - wFormatCode"));
		fclose(fp);
		return FALSE;
	}
	// 保存格式代码
	pDataInfo->FormatCode() = iFormatCode;

	// 3.读注释信息
	_ftscanf(fp,_T("%s"),szTemp);
	if( _tcslen(szTemp) > (AXIN_COMMENT_LENGTH) )	// LEAP_COMMENT_LENGTH = 64
	{
		// 注释信息太长则截断
		szTemp[AXIN_COMMENT_LENGTH] = _T('\0');
	}
	pDataInfo->SetComment(szTemp);

	// 4.调色板类型(生成不同的调色板)
	int iPaletteType;
	_ftscanf(fp,_T("%d"),&iPaletteType);
	PaletteType ePaletteType = (PaletteType)iPaletteType;
	KPalette* pPalette = NULL;
	switch(ePaletteType)
	{
	case PaletteType_Standard:			// 1
		pPalette = new KStdPalette();
		break;
	case PaletteType_Index:				// 2
		pPalette = new KIdxPalette();
		break;
	case PaletteType_Rainbow:			// 3
		pPalette = new KRbwPalette();
		break;
	case PaletteType_Segment:			// 4
		pPalette = new KSegPalette();
		break;
	case PaletteType_Linear:			// 5
		pPalette = new KLinearPalette();
		break;
	}
	if(!pPalette)
		return FALSE;

	// 5.调色板条目数量、注释标记
	int flagComment,iPaletteNums;
	_ftscanf(fp,_T("%d%d"),&flagComment,&iPaletteNums);

	// 是否有注视标记
	pPalette->HasComment() = (flagComment != 0);

	// 6.读取数据
	double value(0);
	int r,g,b,a(255);
	BOOL bHasComment = flagComment != 0;
	TCHAR szComment[MAX_PATH] = _T("");
	for(int i=0;i<iPaletteNums;i++)
	{
		//_ftscanf(fp,_T("%lf %d%d%d%d"),&value,&r,&g,&b,&a);	// RGBA
		_ftscanf(fp,_T("%lf %d%d%d"),&value,&r,&g,&b);			// RGB
		if(bHasComment)	_ftscanf(fp,_T("%s"),szComment);
		else			_stprintf(szComment,_T(""));

		if(feof(fp)) break;

		pPalette->AddColor(value,255,r,g,b,szComment);
	}

	// 关闭文件
	fclose(fp);
	fp = NULL;

	this->SetPalette(pPalette);

	return TRUE;
}

// 填充数据信息
void KColortableData::FillDataInfo(LPVOID lpvData)
{
}

#include "StdAfx.h"
#include "KColortableData.h"


KColortableData::KColortableData(void)
{
}


KColortableData::~KColortableData(void)
{
}

// װ������ - ASC�ļ�
BOOL KColortableData::LoadData_txt(LPCTSTR lpszFile)
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
	int iFormatCode;
	_ftscanf(fp,_T("%d"),&iFormatCode);
	if(iFormatCode != AXIN_FMT_CODE_COLOR_TABLE)	// AXIN_FMT_CODE_COLOR_TABLE = 60
	{
		SetMessage(_T("SNYC �����ļ���ʽ����Ҫ��! - wFormatCode"));
		fclose(fp);
		return FALSE;
	}
	// �����ʽ����
	pDataInfo->FormatCode() = iFormatCode;

	// 3.��ע����Ϣ
	_ftscanf(fp,_T("%s"),szTemp);
	if( _tcslen(szTemp) > (AXIN_COMMENT_LENGTH) )	// LEAP_COMMENT_LENGTH = 64
	{
		// ע����Ϣ̫����ض�
		szTemp[AXIN_COMMENT_LENGTH] = _T('\0');
	}
	pDataInfo->SetComment(szTemp);

	// 4.��ɫ������(���ɲ�ͬ�ĵ�ɫ��)
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

	// 5.��ɫ����Ŀ������ע�ͱ��
	int flagComment,iPaletteNums;
	_ftscanf(fp,_T("%d%d"),&flagComment,&iPaletteNums);

	// �Ƿ���ע�ӱ��
	pPalette->HasComment() = (flagComment != 0);

	// 6.��ȡ����
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

	// �ر��ļ�
	fclose(fp);
	fp = NULL;

	this->SetPalette(pPalette);

	return TRUE;
}

// ���������Ϣ
void KColortableData::FillDataInfo(LPVOID lpvData)
{
}

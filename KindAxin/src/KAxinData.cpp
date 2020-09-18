#include "StdAfx.h"
#include "KAxinData.h"

#include "KColortableData.h"

KAxinData::KAxinData(void)
	: KCustomDataObject(),m_pProductDataInfo(NULL)
{
	SetType(KAxinData::eDataStyle_Marker);

	m_pProductDataInfo = new KDataInfo();
}


KAxinData::~KAxinData(void)
{
	_delete(m_pProductDataInfo);
}

// ������ɫ��
// �ȹ���ʺ��ɫ��
// Ȼ����ݲ�Ʒ����ѡ���ɫ���ļ�
// �����ɫ���ļ�������ʹ�������ݹ����ɫ��
void KAxinData::BuildPalette(LPCTSTR lpszFile /* = NULL */)
{
	if(IsEmpty())
		return;

	// ������Ϣ
	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return;

	// ɾ��ԭ�е�ɫ��
	_delete(m_pPalette);

	// ��Ʒ����
	// 0 - ��ʾʹ�òʺ��ɫ��
	// x - ʹ�÷ֶε�ɫ��(���ⲿ����)
	int iProductCode = pDataInfo->ProductCode();

	// �ʺ��ɫ��
	KSegPalette* pPalette = new KSegPalette();
	m_pPalette = pPalette;

	// 1.���òʺ���ɫ����ɫ��
	// �ʺ��ɫ��
	//KRbwPalette rbwPalette(KRbwPalette::InverseRainbow);
	KRbwPalette rbwPalette(KRbwPalette::GreenToRed);
	//KRbwPalette rbwPalette;
	rbwPalette.SetValueRange(pDataInfo->ExtremumValueMin(),pDataInfo->ExtremumValueMax());

	// ��ֵ������
	int iContourNums = pDataInfo->ContourNumbers();
	if(iContourNums>=1 && iContourNums<=99)
	{
		for(int i=0;i<pDataInfo->wContourNums;i++)
		{
			double value = pDataInfo->dContourValue[i];
			COLORREF clr = rbwPalette.GetColor(value);
			pPalette->AddColor(value,clr);
		}
	}
	else if(iContourNums == AXIN_FMT_CVID_AUTO || iContourNums == AXIN_FMT_CVID_CONTOUR)
	{
		double vInterval = pDataInfo->ContourInterval();	// ��ֵ�߼��
		double vmin = pDataInfo->ContourMin();				// ��ֵ����Сֵ
		double vmax = pDataInfo->ContourMax();				// ��ֵ�����ֵ
		for(double value = vmin; value < vmax+0.000001; value += vInterval)
		{
			if(value<pDataInfo->vExtremumMin || value>pDataInfo->vExtremumMax)
				continue;

			COLORREF clr = rbwPalette.GetColor(value);
			pPalette->AddColor(value,clr);
		}
	}
	else
	{
		// 
	}

	// 2.�������ɫ���ļ������ļ�����
	if(!lpszFile && KFile::Exists(lpszFile))
	{
		KColortableData clrtbl;
		if(!clrtbl.LoadData(lpszFile))
			return;

		// ���õ�ɫ��Ϊ��ɫ��ĵ�ɫ�屸��
		this->SetPalette(clrtbl.GetPalette()->Clone());
	}

	return;
}

// ȡ�ò�Ʒ��ɫ��
KPalette* KAxinData::GetProductPalette(int iProductCode)
{
	return NULL;
}

//=========================================================
// ��̬���� - beg
//---------------
BOOL KAxinData::IsAxinData(LPCTSTR lpszFile)
{
	int iFormarCode = KAxinData::GetFormatCode(lpszFile);
	return iFormarCode>0 && iFormarCode<99;	// 1-100
}
int KAxinData::GetFormatCode(LPCTSTR lpszFile)
{
	int iFormatCode = 0;
	TCHAR szTemp[MAX_PATH] = _T("");

	BOOL bIsBinary = KFile::IsBinary(lpszFile);

	// 
	if(bIsBinary)	// bin file
	{
		char cBytes[256];
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(fp==NULL)
			return 0;

		fread(cBytes,256,1,fp);

		fclose(fp);
		fp = NULL;

		if(AXIN_FID_BIN_MARK == ((AxinGridImageHeader*)cBytes)->wFileID)
		{
			iFormatCode = ((AxinGridImageHeader*)cBytes)->wFormatCode;
		}
	}
	else	// txt file
	{
		FILE* fp = _tfopen(lpszFile,_T("rt"));
		if(fp==NULL)
			return 0;

		int sizeFid = _tcslen(AXIN_FID_TXT_MARK); 
		_ftscanf(fp,_T("%s"),szTemp);		// �ļ�ͷ��ʾ,Ϊ�ַ��� _T("AXIN"),��Сд����
		if( szTemp[sizeFid] != _T('\0') )
		{
			szTemp[sizeFid] =  _T('\0');
			fclose(fp);
			fp = NULL;
			return 0;
		}
		//ת��Ϊ��д
		KString::UpperString(szTemp);
		if( _tcscmp(szTemp,AXIN_FID_TXT_MARK) != 0 )
		{
			fclose(fp);
			fp = NULL;
			return 0;
		}

		// 2.����Ʒ���ݸ�ʽ����
		_ftscanf(fp,_T("%d"),&iFormatCode);

		fclose(fp);
		fp = NULL;
	}

	return iFormatCode;
}
BOOL KAxinData::GetDateTime(LPCTSTR lpszFile, DATETIME* pDatm)
{
	int iFormatCode = 0;
	TCHAR szTemp[MAX_PATH] = _T("");

	BOOL bIsBinary = KFile::IsBinary(lpszFile);
	if(bIsBinary)	// bin file
	{
		char cBytes[256];
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(fp==NULL)
			return FALSE;

		fread(cBytes,256,1,fp);

		fclose(fp);
		fp = NULL;

		// AXIN
		if(AXIN_FID_BIN_MARK == ((AxinGridImageHeader*)cBytes)->wFileID)
		{
			AxinGridImageHeader* pAxinGridImageHeader = (AxinGridImageHeader*)cBytes;
			pDatm->wYear			= pAxinGridImageHeader->wYear;
			pDatm->wMonth			= pAxinGridImageHeader->wMonth;
			pDatm->wDay				= pAxinGridImageHeader->wDay;
			pDatm->wHour			= pAxinGridImageHeader->wHour;
			pDatm->wMinute			= pAxinGridImageHeader->wMinute;
			pDatm->wSecond			= pAxinGridImageHeader->wSecond;
			pDatm->dwMillisecond	= pAxinGridImageHeader->dwMillisecond;
		}

		// LLocal File - LD2 LD3
		if(AGMRS_LOCAL_DATA_MARKER == ((LD2Header*)cBytes)->wFileID)
		{
			LD2Header* pLD2Header = (LD2Header*)cBytes;
			pDatm->wYear			= pLD2Header->wYear;
			pDatm->wMonth			= pLD2Header->wMonth;
			pDatm->wDay				= pLD2Header->wDay;
			pDatm->wHour			= pLD2Header->wHour;
			pDatm->wMinute			= pLD2Header->wMinute;
			pDatm->wSecond			= 0;
			pDatm->dwMillisecond	= 0;
		}
	}
	else
	{
		FILE* fp = _tfopen(lpszFile,_T("rt"));
		if(fp==NULL)
			return FALSE;

		int sizeFid = _tcslen(AXIN_FID_TXT_MARK); 
		_ftscanf(fp,_T("%s"),szTemp);		// �ļ�ͷ��ʾ,Ϊ�ַ��� _T("AXIN"),��Сд����
		if( szTemp[sizeFid] != _T('\0') )
		{
			fclose(fp);
			fp = NULL;
			return 0;
		}
		//ת��Ϊ��д
		KString::UpperString(szTemp);
		if( _tcscmp(szTemp,AXIN_FID_TXT_MARK) != 0 )
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// 2.����Ʒ���ݸ�ʽ����
		_ftscanf(fp,_T("%d"),&iFormatCode);

		// 3.��ע����Ϣ
		_ftscanf(fp,_T("%s"),szTemp);
		if( _tcslen(szTemp) > (AXIN_COMMENT_LENGTH) )	// LEAP_COMMENT_LENGTH = 64
		{
			// ע����Ϣ̫����ض�
			szTemp[AXIN_COMMENT_LENGTH] = _T('\0');
		}

		// 4.������ʱ��(yyyy,mm,dd,HH,MM,SS,MS)
		int yyyy,mm,dd,HH,MM,SS,MS;
		_ftscanf(fp,_T("%d%d%d %d%d%d%s"), &yyyy, &mm, &dd, &HH, &MM, &SS, &MS);
		pDatm->wYear			= yyyy;
		pDatm->wMonth			= mm;
		pDatm->wDay				= dd;
		pDatm->wHour			= HH;
		pDatm->wMinute			= MM;
		pDatm->wSecond			= SS;
		pDatm->dwMillisecond	= MS;

		fclose(fp);
		fp = NULL;
	}

	return TRUE;
}

BOOL KAxinData::GetFileDataInfo(LPCTSTR lpszFile, KDataInfo* pDataInfo)
{
	BOOL bReturn = FALSE;

	// �Ƿ�������ļ�
	BOOL bIsBinary = KFile::IsBinary(lpszFile);

	if(bIsBinary)
		bReturn = KAxinData::GetFileDataInfo_bin(lpszFile,pDataInfo);
	else
		bReturn = KAxinData::GetFileDataInfo_txt(lpszFile,pDataInfo);

	return bReturn;
}

BOOL KAxinData::GetFileDataInfo_bin(LPCTSTR lpszFile, KDataInfo* pDataInfo)
{
	if(!lpszFile || !pDataInfo)
		return FALSE;
	if(!KFile::Exists(lpszFile))
		return FALSE;

	char cBytes[256];
	FILE* fp = _tfopen(lpszFile,_T("rb"));
	if(fp==NULL)					return FALSE;
	if(fread(cBytes,256,1,fp)<1)	return FALSE;
	fclose(fp);
	fp = NULL;

	// AXIN
	if(AXIN_FID_BIN_MARK == ((AxinGridImageHeader*)cBytes)->wFileID)
	{
		AxinGridImageHeader* pAxinGridImageHeader = (AxinGridImageHeader*)cBytes;

		pDataInfo->FileID()			= pAxinGridImageHeader->wFileID;				// �ļ� ID
		pDataInfo->FormatCode()		= pAxinGridImageHeader->wFormatCode;			// ��ʽ����
		pDataInfo->ProductCode()	= pAxinGridImageHeader->wProductCode;			// ��Ʒ����
		pDataInfo->DataSource()		= pAxinGridImageHeader->wDataSource;			// ������Դ

		pDataInfo->DataType()		= pAxinGridImageHeader->cDataType;				// ��������
		pDataInfo->LayerNumbers()	= pAxinGridImageHeader->cLayerNums;				// ����
		pDataInfo->HasColorTable()	= pAxinGridImageHeader->cColorTableFlag != 0;	// ��ɫ����
		pDataInfo->ProjectType()	= pAxinGridImageHeader->cProjectType;			// ͶӰ����
		pDataInfo->ElementCode()	= pAxinGridImageHeader->wElementCode;			// Ҫ�ش���
		pDataInfo->TimePeriod()		= pAxinGridImageHeader->wTimePeriod;			// ʱЧ

		pDataInfo->Year()			= pAxinGridImageHeader->wYear;				// ��
		pDataInfo->Month()			= pAxinGridImageHeader->wMonth;				// ��
		pDataInfo->Day()			= pAxinGridImageHeader->wDay;				// ��
		pDataInfo->Hour()			= pAxinGridImageHeader->wHour;				// ʱ
		pDataInfo->Minute()			= pAxinGridImageHeader->wMinute;			// ��
		pDataInfo->Second()			= pAxinGridImageHeader->wSecond;			// ��
		pDataInfo->Millisecond()	= pAxinGridImageHeader->dwMillisecond;		// ����

		pDataInfo->Width()			= pAxinGridImageHeader->wWidth;				// ����
		pDataInfo->Height()			= pAxinGridImageHeader->wHeight;			// ����

		pDataInfo->MinX()			= pAxinGridImageHeader->xMin;				// ��С����
		pDataInfo->MinY()			= pAxinGridImageHeader->yMin;				// ��Сγ��

		pDataInfo->MaxX()			= pAxinGridImageHeader->xMax;				// ��󾭶�
		pDataInfo->MaxY()			= pAxinGridImageHeader->yMax;				// ���γ��

		pDataInfo->XInterval()		= pAxinGridImageHeader->xInterval;			// ���ȷֱ���
		pDataInfo->YInterval()		= pAxinGridImageHeader->yInterval;			// γ�ȷֱ���

		pDataInfo->StandardLon()	= pAxinGridImageHeader->fStandardLon;		// 

		pDataInfo->StandardLat1()	= pAxinGridImageHeader->fStandardLat1;		// 
		pDataInfo->StandardLat2()	= pAxinGridImageHeader->fStandardLat2;		// 

		pDataInfo->CenterLon()		= pAxinGridImageHeader->fCenterLon;			// 
		pDataInfo->CenterLat()		= pAxinGridImageHeader->fCenterLat;			// 

		pDataInfo->InvalidData()	= pAxinGridImageHeader->dwInvalidData;		// 
		pDataInfo->BaseValue()		= pAxinGridImageHeader->fBaseValue;			// 
		pDataInfo->Scale()			= pAxinGridImageHeader->fScale;				// 

		for(int i=0;i<16;i++)
			pDataInfo->LayerCode(i) = pAxinGridImageHeader->wLayerCode[i];		// 

		pDataInfo->Year2()			= pAxinGridImageHeader->wYear2;				// ��
		pDataInfo->Month2()			= pAxinGridImageHeader->wMonth2;			// ��
		pDataInfo->Day2()			= pAxinGridImageHeader->wDay2;				// ��
		pDataInfo->Hour2()			= pAxinGridImageHeader->wHour2;				// ʱ
		pDataInfo->Minute2()		= pAxinGridImageHeader->wMinute2;			// ��
		pDataInfo->Second2()		= pAxinGridImageHeader->wSecond2;			// ��
		pDataInfo->Millisecond2()	= pAxinGridImageHeader->dwMillisecond2;		// ����

		pDataInfo->SetComment(KString::ToTChar((char*)pAxinGridImageHeader->cComment));
	}
	//-----------------------------------------------------
	// AGMRS
	if(AGMRS_LOCAL_DATA_MARKER == ((LD2Header*)cBytes)->wFileID)
	{
		LD2Header* pLD2Header = (LD2Header*)cBytes;

		pDataInfo->FileID()			= pLD2Header->wFileID;				// �ļ� ID
		//pDataInfo->FormatCode()		= pLD2Header->wFormatCode;			// ��ʽ����
		//pDataInfo->ProductCode()	= pLD2Header->wProductCode;			// ��Ʒ����
		//pDataInfo->DataSource()		= pLD2Header->wDataSource;			// ������Դ

		//pDataInfo->DataType()		= pLD2Header->cDataType;				// ��������
		pDataInfo->LayerNumbers()	= pLD2Header->wChannelNums;				// ����
		//pDataInfo->HasColorTable()	= pLD2Header->cColorTableFlag != 0;	// ��ɫ����
		pDataInfo->ProjectType()	= pLD2Header->wProjectType;			// ͶӰ����
		//pDataInfo->ElementCode()	= pLD2Header->wElementCode;			// Ҫ�ش���
		//pDataInfo->TimePeriod()		= pLD2Header->wTimePeriod;			// ʱЧ

		pDataInfo->Year()			= pLD2Header->wYear;				// ��
		pDataInfo->Month()			= pLD2Header->wMonth;				// ��
		pDataInfo->Day()			= pLD2Header->wDay;				// ��
		pDataInfo->Hour()			= pLD2Header->wHour;				// ʱ
		pDataInfo->Minute()			= pLD2Header->wMinute;			// ��
		//pDataInfo->Second()			= pLD2Header->wSecond;			// ��
		//pDataInfo->Millisecond()	= pLD2Header->dwMillisecond;		// ����

		pDataInfo->Width()			= pLD2Header->wWidth;				// ����
		pDataInfo->Height()			= pLD2Header->wHeight;			// ����

		pDataInfo->MinX()			= pLD2Header->fLonMin;				// ��С����
		pDataInfo->MinY()			= pLD2Header->fLatMin;				// ��Сγ��

		pDataInfo->MaxX()			= pLD2Header->fLonMax;				// ��󾭶�
		pDataInfo->MaxY()			= pLD2Header->fLatMax;				// ���γ��

		pDataInfo->XInterval()		= pLD2Header->fLonResolution;		// ���ȷֱ���
		pDataInfo->YInterval()		= pLD2Header->fLatResolution;			// γ�ȷֱ���

		pDataInfo->StandardLon()	= pLD2Header->fStandardLon;		// 

		pDataInfo->StandardLat1()	= pLD2Header->fStandardLat1;		// 
		pDataInfo->StandardLat2()	= pLD2Header->fStandardLat2;		// 

		pDataInfo->CenterLon()		= pLD2Header->fCenterLon;			// 
		pDataInfo->CenterLat()		= pLD2Header->fCenterLat;			// 

		//pDataInfo->InvalidData()	= pLD2Header->dwInvalidData;		// 
		pDataInfo->BaseValue()		= 0;			// 
		pDataInfo->Scale()			= 1;				// 

		for(int i=0;i<pLD2Header->wChannelNums;i++)
			pDataInfo->LayerCode(i) = pLD2Header->byChannelIndex[i];

		pDataInfo->SetComment(_T("LDF"));
	}

	return TRUE;
}
BOOL KAxinData::GetFileDataInfo_txt(LPCTSTR lpszFile, KDataInfo* pDataInfo)
{
	if(!lpszFile || !pDataInfo)
		return FALSE;
	if(!KFile::Exists(lpszFile))
		return FALSE;

	return FALSE;
}

BOOL KAxinData::CreateFile(LPCTSTR lpszFile, BYTE* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// ������������
	pDataInfo->DataType() = DataType_Byte;
	return KAxinData::CreateFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KAxinData::CreateFile(LPCTSTR lpszFile, char* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// ������������
	pDataInfo->DataType() = DataType_Char;
	return KAxinData::CreateFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KAxinData::CreateFile(LPCTSTR lpszFile, WORD* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// ������������
	pDataInfo->DataType() = DataType_Word;
	return KAxinData::CreateFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KAxinData::CreateFile(LPCTSTR lpszFile, SHORT* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// ������������
	pDataInfo->DataType() = DataType_Short;
	return KAxinData::CreateFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KAxinData::CreateFile(LPCTSTR lpszFile, DWORD* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// ������������
	pDataInfo->DataType() = DataType_Dword;
	return KAxinData::CreateFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KAxinData::CreateFile(LPCTSTR lpszFile, LONG* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// ������������
	pDataInfo->DataType() = DataType_Long;
	return KAxinData::CreateFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KAxinData::CreateFile(LPCTSTR lpszFile, float* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// ������������
	pDataInfo->DataType() = DataType_Float;
	return KAxinData::CreateFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KAxinData::CreateFile(LPCTSTR lpszFile, double* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// ������������
	pDataInfo->DataType() = DataType_Double;
	return KAxinData::CreateFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}

BOOL KAxinData::CreateFile(LPCTSTR lpszFile, void* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	int i;

	if(!pData || !pDataInfo)
		return FALSE;

	KFile file(lpszFile);
	if(!KDirectory::Exist(file.GetDirectory()))
	{
		KDirectory::Create(file.GetDirectory());
	}

	// ���ļ�
	FILE* fp = _tfopen(lpszFile,_T("wb"));
	if(fp==NULL)
		return FALSE;

	// �ļ�ͷ256�ֽ�
	BYTE fileHeader[256];
	memset(fileHeader,0,256);
	BYTE* pFileHeader = &fileHeader[0];

	// 1.�ļ�ͷ��Ϣ
	AxinGridImageHeader* pGridFileHeader = (AxinGridImageHeader*)pFileHeader;

	pGridFileHeader->wFileID			= AXIN_FID_BIN_MARK;					// 1		�ļ���ʶ 'AX'
	pGridFileHeader->wFormatCode		= pDataInfo->FormatCode();				// 2		��ʽ����
	pGridFileHeader->wProductCode		= pDataInfo->ProductCode();				// 3		��Ʒ����
	pGridFileHeader->wDataSource		= pDataInfo->DataSource();				// 4		������Դ

	pGridFileHeader->cDataType			= (BYTE)pDataInfo->DataType();			// 5.1		�������� 1-BYTE  2-SHORT/WORD  4-LONG/DWORD 40-FLOAT 80-DOUBLE
	pGridFileHeader->cLayerNums			= (BYTE)pDataInfo->LayerNumbers();		// 5.2		����(���16��)
	pGridFileHeader->cColorTableFlag	= (BYTE)(NULL != pPalette);				// 6.1		��ɫ����
	pGridFileHeader->cProjectType		= (BYTE)pDataInfo->ProjectType();		// 6.2		ͶӰ����: 0-��ͶӰ,1-�Ƚ�ͶӰ,2-����ͶӰ,3-������ͶӰ,4-�������ͶӰ,5-������˹ͶӰ
	pGridFileHeader->wElementCode		= pDataInfo->ElementCode();				// 7		Ҫ�ش���
	pGridFileHeader->wTimePeriod		= pDataInfo->TimePeriod();				// 8		ʱЧ

	pGridFileHeader->wYear				= pDataInfo->Year();					// 9		��
	pGridFileHeader->wMonth				= pDataInfo->Month();					// 10		��
	pGridFileHeader->wDay				= pDataInfo->Day();						// 11		��
	pGridFileHeader->wHour				= pDataInfo->Hour();					// 12		ʱ
	pGridFileHeader->wMinute			= pDataInfo->Minute();					// 13		��
	pGridFileHeader->wSecond			= pDataInfo->Second();					// 14		��
	pGridFileHeader->dwMillisecond		= pDataInfo->Millisecond();				// 15-16	����

	pGridFileHeader->wWidth				= pDataInfo->Width();					// 17		����
	pGridFileHeader->wHeight			= pDataInfo->Height();					// 18		����

	pGridFileHeader->xMin			= (float)pDataInfo->MinX();					// 19-20	��С����
	pGridFileHeader->yMin			= (float)pDataInfo->MinY();					// 21-22	��Сγ��

	pGridFileHeader->xInterval		= (float)pDataInfo->XInterval();			// 27-28	���ȷֱ��� -- �Ƚ�ͶӰ		x�ֱ��� -- ���С������С��������ͶӰ 
	pGridFileHeader->yInterval		= (float)pDataInfo->YInterval();			// 29-30	γ�ȷֱ��� -- �Ƚ�ͶӰ		y�ֱ��� -- ���С������С��������ͶӰ

	pGridFileHeader->xMax			= (float)pDataInfo->MaxX();					// 23-24	��󾭶�
	pGridFileHeader->yMax			= (float)pDataInfo->MaxY();					// 25-26	���γ��

	pGridFileHeader->fStandardLon	= (float)pDataInfo->StandardLon();			// 31-32	Բ׶ͶӰ���ľ��� (LAMBERT,POLAR,ALBERS)

	pGridFileHeader->fStandardLat1	= (float)pDataInfo->StandardLat1();			// 33-34	��׼γ��1 -- ���С������С��������ͶӰ��Ч
	pGridFileHeader->fStandardLat2	= (float)pDataInfo->StandardLat2();			// 35-36	��׼γ��2 -- ������ͶӰ��Ч

	pGridFileHeader->fCenterLon		= (float)pDataInfo->CenterLon();			// 37-38	���ĵ�ľ���
	pGridFileHeader->fCenterLat		= (float)pDataInfo->CenterLat();			// 39-40	���ĵ��γ��

	pGridFileHeader->dwInvalidData	= pDataInfo->InvalidData();					// 41-42	��Ч����ֵ
	pGridFileHeader->fBaseValue		= (float)pDataInfo->BaseValue();			// 43-44	��׼ֵ
	pGridFileHeader->fScale			= (float)pDataInfo->Scale();				// 45-46	��������

	//pGridFileHeader->dwVersion		= 0;										// 47-48	�汾

	for(int index=0;index<16;index++)
	{
		pGridFileHeader->wLayerCode[index] = pDataInfo->LayerCode(index);		// 49-64	�����
	}
	//-----------------------------------------------------
	pGridFileHeader->wYear2				= pDataInfo->Year2();					// 9		��
	pGridFileHeader->wMonth2			= pDataInfo->Month2();					// 10		��
	pGridFileHeader->wDay2				= pDataInfo->Day2();					// 11		��
	pGridFileHeader->wHour2				= pDataInfo->Hour2();					// 12		ʱ
	pGridFileHeader->wMinute2			= pDataInfo->Minute2();					// 13		��
	pGridFileHeader->wSecond2			= pDataInfo->Second2();					// 14		��
	pGridFileHeader->dwMillisecond2		= pDataInfo->Millisecond2();			// 15-16	����
	//-----------------------------------------------------
	for(i=0;i<64;i++)
	{
		pGridFileHeader->cComment[i] = 0;
	}
	strncpy((char*)&pGridFileHeader->cComment[0],KString::ToMByte(pDataInfo->GetComment()),64);
	//-----------------------------------------------------
	// д���ļ�ͷ
	fwrite(pFileHeader,sizeof(BYTE),256,fp);

	// 2.��ɫ��
	if(pPalette)
	{
		ColorTablePacketHeader tColorTablePacketHeader;

		tColorTablePacketHeader.wPacketCode				= Const_Color_Table_Packet_Code;	// 01		������ = 0x80
		tColorTablePacketHeader.dwPacketLength			= 0;								// 02-03	������
		tColorTablePacketHeader.wType					= pPalette->GetType();				// 04		����
		tColorTablePacketHeader.wColorEntries			= pPalette->GetColorEntries();		// 05		��ɫ�����
		tColorTablePacketHeader.wSpecificColorNumbers	= pPalette->SpecificColorNumbers();	// 06		�ض���ɫ����
		tColorTablePacketHeader.wHasComment				= pPalette->HasComment();			// 07		�Ƿ��ж�Ӧע��
		tColorTablePacketHeader.wHasTransparentColor	= pPalette->HasTransparentColor();	// 08		ʹ��͸��ɫ

		tColorTablePacketHeader.dwTransparentColor		= pPalette->TransparentColor();		// 09-10	͸����ɫ
		tColorTablePacketHeader.dwDefaultColor			= pPalette->DefaultColor();			// 11-12	Ĭ����ɫ

		tColorTablePacketHeader.dwSpecialDataMin		= pPalette->SpecificDataMin();		// 13-14	�ض���С����
		tColorTablePacketHeader.dwSpecialDataMax		= pPalette->SpecificDataMax();		// 15-16	�ض��������

		// ���������
		WORD wPacketLength = sizeof(ColorTablePacketHeader);
		wPacketLength += 8 * tColorTablePacketHeader.wColorEntries;			// rgbv 8���ֽ�
		if(pPalette->HasComment())
			wPacketLength += 16 * tColorTablePacketHeader.wColorEntries;	// ע��Ϊ16�ֽ�

		tColorTablePacketHeader.dwPacketLength			= wPacketLength;
		tColorTablePacketHeader.TA						= pPalette->HasTransparentColor() ? 0 : 255;

		// д����ɫ���ͷ
		fwrite(&tColorTablePacketHeader,sizeof(ColorTablePacketHeader),1,fp);

		// д����ɫ��
		for(i=0;i<tColorTablePacketHeader.wColorEntries;i++)
		{
			KPaletteItem item = *pPalette->GetAt(i);
			VARGB vargb(item.Value(),item.Rgb());
			fwrite(&vargb,sizeof(VARGB),1,fp);
		}

		// ��ɫ�����д��ɫ��ע��
		if(tColorTablePacketHeader.wHasComment)
		{
			char szTemp[17];
			for(i=0;i<tColorTablePacketHeader.wColorEntries;i++)
			{
				memset(szTemp,0,17);
				LPCTSTR ptsz = pPalette->GetAt(i)->GetComment();
				if(ptsz)
				{
					strncpy(szTemp,KString::ToMByte(ptsz),16);
				}
				fwrite(szTemp,sizeof(char),16,fp);
			}
		}

	}// if(pPalette)

	// д��������
	int totalNum = pGridFileHeader->xNum * pGridFileHeader->yNum;
	int writeSize=0;

	DataType eDataType = (DataType)pDataInfo->DataType();
	switch(eDataType)
	{
	case 1:
		writeSize = fwrite(pData,sizeof(BYTE),totalNum,fp);
		break;

	case 2:
		writeSize = fwrite(pData,sizeof(SHORT),totalNum,fp);
		break;

	case 4:
		writeSize = fwrite(pData,sizeof(LONG),totalNum,fp);
		break;

	case 40:
		writeSize = fwrite(pData,sizeof(float),totalNum,fp);
		break;

	case 80:
		writeSize = fwrite(pData,sizeof(double),totalNum,fp);
		break;

	default:
		break;
	}

	// �ر��ļ�
	fflush(fp);
	fclose(fp);
	fp = NULL;

	return TRUE;
}

BOOL KAxinData::ExportEnviHdr(LPCTSTR lpszFile,KDataInfo* pDataInfo)
{
	if(!pDataInfo)
		return FALSE;

	int dt=pDataInfo->GetDataStoreType();
	if(pDataInfo->DataStoreType != 2)
		return FALSE;

	FILE* fp = _tfopen(lpszFile,_T("wt"));
	if(!fp)
		return FALSE;

	// �ļ�����
	_ftprintf(fp,_T("ENVI\n"));
	_ftprintf(fp,_T("description = {File Imported into ENVI.}\n"));
	_ftprintf(fp,_T("samples = %d\n"),pDataInfo->XNum());
	_ftprintf(fp,_T("lines = %d\n"),pDataInfo->YNum());
	_ftprintf(fp,_T("bands = %d\n"),pDataInfo->LayerNumbers());
	_ftprintf(fp,_T("header offset = %d\n"),pDataInfo->DataOffset);
	_ftprintf(fp,_T("file type = ENVI Standard\n"));
	_ftprintf(fp,_T("data type = 4\n"));
	_ftprintf(fp,_T("interleave = BSQ\n"));
	_ftprintf(fp,_T("sensor type = Unknown\n"));
	_ftprintf(fp,_T("byte order = 0\n"));
	_ftprintf(fp,_T("map info = {Geographic Lat/Lon, %d, %d, %f, %f, %f, %f, WGS-84, units=Degrees}\n")
		,pDataInfo->XNum() / 2
		,pDataInfo->YNum() / 2
		,pDataInfo->MinLon() + pDataInfo->IntervalX() * pDataInfo->XNum() / 2
		,pDataInfo->MinLat() + pDataInfo->IntervalY() * pDataInfo->YNum() / 2
		,pDataInfo->IntervalX()
		,pDataInfo->IntervalY()
		);
	_ftprintf(fp,_T("geo points = {\n%d,%d,%f,%f,\n%d,%d,%f,%f,\n%d,%d,%f,%f,\n%d,%d,%f,%f}\n")
		,1,1,pDataInfo->MinLat(),pDataInfo->MinLon()
		,pDataInfo->XNum(),1,pDataInfo->MinLat(),pDataInfo->MaxLon()
		,1,pDataInfo->YNum(),pDataInfo->MaxLat(),pDataInfo->MinLon()
		,pDataInfo->XNum(),pDataInfo->YNum(),pDataInfo->MaxLat(),pDataInfo->MaxLon()
		);
	_ftprintf(fp,_T("band names = { %s }\n"),pDataInfo->Comment());

	// �ر��ļ�
	fclose(fp);
	fp = nullptr;

	return TRUE;
}

//---------------
// ��̬���� - end
//=========================================================

#include "StdAfx.h"
#include "KGridImageData.h"

KGridImageData::KGridImageData(void)
{
	SetType(KAxinData::eDataStyle_Grid_Image);

	m_pFileHeader = NULL;
	m_pPaletteCollection = NULL;

	m_dwDataOffset = 0;

	m_pDataProcessor = new KDataProcessor();
	m_pDataProcessor->SetOwner(this);
}

KGridImageData::~KGridImageData(void)
{
	_deletea(m_pFileHeader);
}

// װ������
BOOL KGridImageData::LoadData_bin(LPCTSTR lpszFile)
{
	int i;

	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return FALSE;

	if(!this->IsEmpty())
		Empty();

	// ���ļ�
	FILE* fp = _tfopen(lpszFile,_T("rb"));
	if(fp==NULL)
		return FALSE;

	// 1.��ȡ�ļ�ͷ
	if(!m_pFileHeader)
		m_pFileHeader = new char[256];

	if(fread(m_pFileHeader,1,256,fp)<256)
	{
		fclose(fp);
		fp = NULL;
		return FALSE;
	}

	// ��� DataInfo
	this->FillDataInfo(m_pFileHeader);
	if(AXIN_FID_BIN_MARK != pDataInfo->FileID())
		return FALSE;
	if(!(pDataInfo->FormatCode()>0 && pDataInfo->FormatCode()<100))
		return FALSE;

	// �ļ�ͷ
	AxinGridImageHeader* pAxinGridImageHeader = (AxinGridImageHeader*)m_pFileHeader;

	m_dwDataOffset = 256;

	// 2.��ȡ��ɫ��
	if(pAxinGridImageHeader->cColorTableFlag != 0)
	{
		// ��ȡ��ɫ�����ݰ�ͷ�ṹ
		ColorTablePacketHeader tColorTablePacketHeader;
		if(fread(&tColorTablePacketHeader,sizeof(ColorTablePacketHeader),1,fp)<1)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// ��ɫ�������
		int iColorEntries = tColorTablePacketHeader.wColorEntries;
		vector<VARGB> vargbs;
		for(i=0;i<iColorEntries;i++)
		{
			VARGB vargb;
			fread(&vargb,sizeof(VARGB),1,fp);
			vargbs.push_back(vargb);
		}

		// ������ڵ�ɫ��ע��
		// ��ȡ��ɫ��ע�͵���ʱ�ַ�������
		vector<KString> comments;
		BOOL bHasComment = tColorTablePacketHeader.wHasComment != 0;
		if(bHasComment)
		{
			char szTemp[17];	// ÿ��ע��16��ASCII�ַ�
			for(i=0;i<iColorEntries;i++)
			{
				fread(szTemp,sizeof(char),16,fp);	// ÿ��ע��16���ֽ�
				szTemp[16] = '\0';

				KString comment = KString::ToTChar(szTemp);
				comments.push_back(comment);
			}
		}

		// ����ɫ���ע�ͺϲ�����ɫ��
		KPalette* pPalette = NULL;
		PaletteType ePaletteType = (PaletteType)tColorTablePacketHeader.wType;
		switch(ePaletteType)
		{
		case PaletteType_Standard:
			pPalette = new KStdPalette();
			break;
		case PaletteType_Index:
			pPalette = new KIdxPalette();
			break;
		case PaletteType_Rainbow:
			pPalette = new KRbwPalette();
			break;
		case PaletteType_Segment:
			pPalette = new KSegPalette();
			break;

		default:
			pPalette = new KSegPalette();
			break;
		}

		for(i=0;i<iColorEntries;i++)
		{
			VARGB vargb = vargbs.at(i);
			TCHAR szComment[24] = _T("");
			_tcscpy(szComment,bHasComment ? (LPCTSTR)comments.at(i) : _T(""));
			pPalette->AddColor(vargb.V,vargb.R,vargb.G,vargb.B,szComment);
		}

		// �Ƿ���͸����ɫ
		BOOL bHasTransparentColor = tColorTablePacketHeader.wHasTransparentColor != 0;
		COLORREF clrTransparent = RGB(tColorTablePacketHeader.TR,tColorTablePacketHeader.TG,tColorTablePacketHeader.TB);
		pPalette->HasTransparentColor() = bHasTransparentColor;
		pPalette->TransparentColor() = clrTransparent;

		this->SetPalette(pPalette);

		this->GetDataInfo()->HasTransparentColor() = pPalette->HasTransparentColor();
		this->GetDataInfo()->TransparentColor() = pPalette->TransparentColor();

		m_dwDataOffset += tColorTablePacketHeader.dwPacketLength;

	}//if(pAxinGridImageHeader->cColorTableFlag != 0)

	// ����ƫ�Ƶ�ַ
	this->GetDataInfo()->DataOffset = m_dwDataOffset;

	// ��ȡ����
	void* pVoid = NULL;
	int xNum = pAxinGridImageHeader->xNum;
	int yNum = pAxinGridImageHeader->yNum;

	// �����������Ͷ�ȡ����
	DataType eDataType = (DataType)pAxinGridImageHeader->cDataType;
	if(eDataType == DataType_Byte)
	{
		pVoid = new BYTE[xNum * yNum];
		fread(pVoid,sizeof(BYTE),xNum * yNum,fp);
	}
	else if(eDataType == DataType_Short)
	{
		pVoid = new SHORT[xNum * yNum];
		fread(pVoid,sizeof(SHORT),xNum * yNum,fp);
	}
	else if(eDataType == DataType_Long)
	{
		pVoid = new LONG[xNum * yNum];
		fread(pVoid,sizeof(LONG),xNum * yNum,fp);
	}
	else if(eDataType == DataType_Float)
	{
		pVoid = new float[xNum * yNum];
		fread(pVoid,sizeof(FLOAT),xNum * yNum,fp);
	}
	else if(eDataType == DataType_Double)
	{
		pVoid = new double[xNum * yNum];
		fread(pVoid,sizeof(double),xNum * yNum,fp);
	}
	else
	{
		fclose(fp);
		fp = NULL;
		return FALSE;
	}

	// �ر��ļ�
	fclose(fp);
	fp = NULL;

	if(!pVoid)
		return FALSE;

	// �����ڴ�
	this->PrepareMemory(eUvw_v);

	// ȡ������ָ��
	double* pdv = this->GetVData();
	if(!pdv)
	{
		_deletea(pVoid);
		return FALSE;
	}

	// ת������
	int totalSize = pDataInfo->Width() * pDataInfo->Height();	// ���ݳߴ�
	for(int i=0;i<totalSize;i++)
	{
		if(eDataType == DataType_Byte)
			pdv[i]	= (BYTE)(((BYTE*)pVoid)[i]);
		else if(eDataType == DataType_Short)
			pdv[i]	= (SHORT)(((SHORT*)pVoid)[i]);
		else if(eDataType == DataType_Long)
			pdv[i]	= (LONG)(((LONG*)pVoid)[i]);
		else if(eDataType == DataType_Float)
			pdv[i]	= (float)(((float*)pVoid)[i]);
		else if(eDataType == DataType_Double)
			pdv[i]	= (double)(((double*)pVoid)[i]);
	}

	// ���շ�����ڴ�
	_deletea(pVoid);

	// ���ð󶨺�
	this->SetExtents(KExtents(pDataInfo->xMin,pDataInfo->yMin,pDataInfo->xMax,pDataInfo->yMax));

	return KGridData::LoadData_bin(lpszFile);
}

// ���������Ϣ
void KGridImageData::FillDataInfo(LPVOID lpvData)
{
	// �ļ�ͷ
	AxinGridImageHeader* pAxinGridImageHeader = (AxinGridImageHeader*)lpvData;
	// ���浽 DataInfo
	KDataInfo* pDataInfo = this->GetDataInfo();

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

	//KPalette* pPalette = this->GetPalette();
	//pDataInfo->HasColorTable()			= pPalette ? TRUE : FALSE;
	//pDataInfo->bHasComment				= pPalette->HasComment();
	//pDataInfo->HasTransparentColor()	= pPalette->HasTransparentColor();
}

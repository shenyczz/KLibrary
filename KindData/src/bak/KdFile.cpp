#include "StdAfx.h"
#include "KdFile.h"

KdFile::KdFile(void)
	:m_pFileHeader(NULL)
	,m_pColors(NULL)
{
}

KdFile::~KdFile(void)
{
	Empty();
}

void KdFile::Empty()
{
	_deletea(m_pFileHeader); 
	_deletea(m_pColors);

	KCustomDataObject::Empty();
}

//���������Ƿ����(������Ҫ���������и�����Ҫȡ������)
BOOL KdFile::IsEmpty()
{
	return TRUE
		&& KCustomDataObject::IsEmpty()
		&& m_pFileHeader == NULL
		&& m_pColors == NULL
		;
}

//---------------------------------------------------------
// ��̬���� - Beg
//---------------------------------------------------------

// �Ƿ� KdFile �ļ�
BOOL KdFile::IsKdFile(LPCTSTR lpszFile)
{
	BOOL bKdFile = FALSE;

	if(KFile::IsBinary(lpszFile))
		bKdFile = IsKdFile_bin(lpszFile);
	else
		bKdFile = IsKdFile_txt(lpszFile);

	return bKdFile;
}

BOOL KdFile::IsKdFile_bin(LPCTSTR lpszFile)
{
	// ���ļ�
	FILE* fp = _tfopen(lpszFile,_T("rb"));
	if(fp==NULL)
		return FALSE;

	char bytes[256];
	if(fread(bytes,sizeof(char),256,fp)<256)
		return FALSE;

	// �ر��ļ�
	fclose(fp);
	fp = NULL;

	return FALSE
		|| ((DMB*)bytes)->wFileID == KD_FILE_ID
		|| ((KdGridFileHeader*)bytes)->wFileID == KD_FILE_ID
		;
}
BOOL KdFile::IsKdFile_txt(LPCTSTR lpszFile)
{
	TCHAR szTemp[MAX_PATH] = _T("");

	// ���ļ�
	FILE* fp = _tfopen(lpszFile,_T("rt"));
	if(fp==NULL) return FALSE;

	// 1.���ļ�Logo
	memset(szTemp,0,MAX_PATH);
	int sizeLogo = _tcslen(KD_FILE_ID_TEXT);
	_ftscanf(fp,_T("%s"),szTemp);		//�ļ�ͷ��ʾ,Ϊ�ַ�����KD��,��Сд����
	if( szTemp[sizeLogo] != _T('\0') )
	{
		fclose(fp);
		fp = NULL;
		return FALSE;
	}

	// ת��Ϊ��д
	KString::UpperString(szTemp);

	// Logo ����
	if( _tcscmp(szTemp,KD_FILE_ID_TEXT) !=0 )
	{
		fclose(fp);
		fp = NULL;
		return FALSE;
	}

	// 2.����Ʒ���ݸ�ʽ����
	int iFormatCode = -1;
	_ftscanf(fp,_T("%d"),&iFormatCode);

	// �ر��ļ�
	fclose(fp);
	fp = NULL;

	return iFormatCode > 0 && iFormatCode < 300;
}

BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, BYTE* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// ������������
	pDataInfo->DataType() = DataType_Byte;
	return CreateKdFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, char* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// ������������
	pDataInfo->DataType() = DataType_Char;
	return CreateKdFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, WORD* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// ������������
	pDataInfo->DataType() = DataType_Word;
	return CreateKdFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, SHORT* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// ������������
	pDataInfo->DataType() = DataType_Short;
	return CreateKdFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, DWORD* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// ������������
	pDataInfo->DataType() = DataType_Dword;
	return CreateKdFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, LONG* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// ������������
	pDataInfo->DataType() = DataType_Long;
	return CreateKdFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, float* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// ������������
	pDataInfo->DataType() = DataType_Float;
	return CreateKdFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, double* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// ������������
	pDataInfo->DataType() = DataType_Double;
	return CreateKdFile(lpszFile,(void*)pData,pDataInfo,pPalette);
}

BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, void* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	return CreateKdFile_easy(lpszFile, pData, pDataInfo, pPalette);
}
BOOL KdFile::CreateKdFile_easy(LPCTSTR lpszFile, void* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	int i;

	if(!pData || !pDataInfo)
		return FALSE;

	// ���ļ�
	FILE* fp = _tfopen(lpszFile,_T("wb"));
	if(fp==NULL)
		return FALSE;

	// �ļ�ͷ256�ֽ�
	BYTE fileHeader[256];
	memset(fileHeader,0,256);
	BYTE* pFileHeader = &fileHeader[0];

	// 1.�ļ�ͷ��Ϣ
	KdGridFileHeader* pGridFileHeader = (KdGridFileHeader*)pFileHeader;

	pGridFileHeader->wFileID			= KD_FILE_ID;							// 1		�ļ���ʶ 0x444b
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

	pGridFileHeader->xMax			= (float)pDataInfo->MaxX();					// 23-24	��󾭶�
	pGridFileHeader->yMax			= (float)pDataInfo->MaxY();					// 25-26	���γ��

	pGridFileHeader->xInterval		= (float)pDataInfo->XInterval();			// 27-28	���ȷֱ��� -- �Ƚ�ͶӰ		x�ֱ��� -- ���С������С��������ͶӰ 
	pGridFileHeader->yInterval		= (float)pDataInfo->YInterval();			// 29-30	γ�ȷֱ��� -- �Ƚ�ͶӰ		y�ֱ��� -- ���С������С��������ͶӰ

	pGridFileHeader->fStandardLon	= (float)pDataInfo->StandardLon();			// 31-32	Բ׶ͶӰ���ľ��� (LAMBERT,POLAR,ALBERS)

	pGridFileHeader->fStandardLat1	= (float)pDataInfo->StandardLat1();			// 33-34	��׼γ��1 -- ���С������С��������ͶӰ��Ч
	pGridFileHeader->fStandardLat2	= (float)pDataInfo->StandardLat2();			// 35-36	��׼γ��2 -- ������ͶӰ��Ч

	pGridFileHeader->fCenterLon		= (float)pDataInfo->CenterLon();			// 37-38	���ĵ�ľ���
	pGridFileHeader->fCenterLat		= (float)pDataInfo->CenterLat();			// 39-40	���ĵ��γ��

	pGridFileHeader->dwInvalidData	= pDataInfo->InvalidData();					// 41-42	��Ч����ֵ
	pGridFileHeader->fBaseValue		= (float)pDataInfo->BaseValue();			// 43-44	��׼ֵ
	pGridFileHeader->fScale			= (float)pDataInfo->Scale();				// 45-46	��������

	pGridFileHeader->dwReserved		= 0;										// 47-48	����

	for(int index=0;index<16;index++)
		pGridFileHeader->wLayerCode[index] = pDataInfo->LayerCode(index);		// 49-64	�����
	//-----------------------------------------------------
	//WORD	wReserved2[24];		// 

	pGridFileHeader->wYear2				= pDataInfo->Year2();					// 9		��
	pGridFileHeader->wMonth2			= pDataInfo->Month2();					// 10		��
	pGridFileHeader->wDay2				= pDataInfo->Day2();					// 11		��
	pGridFileHeader->wHour2				= pDataInfo->Hour2();					// 12		ʱ
	pGridFileHeader->wMinute2			= pDataInfo->Minute2();					// 13		��
	pGridFileHeader->wSecond2			= pDataInfo->Second2();					// 14		��
	pGridFileHeader->dwMillisecond2		= pDataInfo->Millisecond2();			// 15-16	����
	//-----------------------------------------------------
	for(i=0;i<64;i++)
		pGridFileHeader->cComment[i] = 0;

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
		wPacketLength += 8 * tColorTablePacketHeader.wColorEntries;
		if(pPalette->HasComment())
			wPacketLength += 16 * tColorTablePacketHeader.wColorEntries;

		tColorTablePacketHeader.dwPacketLength			= wPacketLength;
		tColorTablePacketHeader.TA						= pPalette->HasTransparentColor() ? 0 : 255;

		fwrite(&tColorTablePacketHeader,sizeof(ColorTablePacketHeader),1,fp);

		// д����ɫ��
		for(i=0;i<tColorTablePacketHeader.wColorEntries;i++)
		{
			KPaletteItem item = *pPalette->GetAt(i);
			VARGB vargb(item.Value(),item.Rgb());
			fwrite(&vargb,sizeof(VARGB),1,fp);
		}

		// д��ɫ��ע��
		if(tColorTablePacketHeader.wHasComment)
		{
			char szTemp[17];
			for(i=0;i<tColorTablePacketHeader.wColorEntries;i++)
			{
				memset(szTemp,0,17);
				LPCTSTR ptsz = pPalette->GetAt(i)->GetComment();
				LPCSTR psz = KString::ToMByte(ptsz);
				strncpy(szTemp,psz,16);
				fwrite(szTemp,sizeof(char),16,fp);
			}
		}

	}// if(pPalette)

	// д��������
	int totalNum = pGridFileHeader->xNum * pGridFileHeader->yNum;

	DataType eDataType = (DataType)pDataInfo->DataType();
	switch(eDataType)
	{
	case 1:
		fwrite(pData,sizeof(BYTE),totalNum,fp);
		break;

	case 2:
		fwrite(pData,sizeof(SHORT),totalNum,fp);
		break;

	case 4:
		fwrite(pData,sizeof(LONG),totalNum,fp);
		break;

	case 40:
		fwrite(pData,sizeof(float),totalNum,fp);
		break;

	case 80:
		fwrite(pData,sizeof(double),totalNum,fp);
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
BOOL KdFile::CreateKdFile_hard(LPCTSTR lpszFile, void* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	int i;

	if(!pData || !pDataInfo)
		return FALSE;

	// ���ļ�
	FILE* fp = _tfopen(lpszFile,_T("wb"));
	if(fp==NULL)
		return FALSE;

	long lOffsetSave = 0;

	// �ļ�ͷһ�� 256 �ֽ�
	BYTE fileHeader[256];
	memset(fileHeader,0,256);
	BYTE* pFileHeader = &fileHeader[0];

	// 1. ������Ϣ�� DMB
	DMB dmb;
	dmb.wDataSource		= pDataInfo->DataSource();
	dmb.wDataCategory	= pDataInfo->DataCategory();	// KD_CATEGORY_GRID
	dmb.wBytesOrder		= pDataInfo->BytesOrder();

	DMB* pDmb = (DMB*)((char*)pFileHeader + KD_BLOCK_OFFSET_DMB);
	*pDmb = dmb;


	// 2. ��չ��Ϣ�� EMB
	EMB emb;
	emb.dwInvalidData		= pDataInfo->InvalidData();
	emb.dwSpecificDataMin	= pDataInfo->SpecificDataMin();
	emb.dwSpecificDataMax	= pDataInfo->SpecificDataMax();

	EMB* pEmb = (EMB*)((char*)pFileHeader + KD_BLOCK_OFFSET_EMB);
	*pEmb = emb;

	// 3. ע�����ݿ� CDB
	CDB cdb;
	cdb.SetComment( KString::ToMByte(pDataInfo->Comment()) );

	CDB* pCdb = (CDB*)((char*)pFileHeader + KD_BLOCK_OFFSET_CDB);
	*pCdb = cdb;

	// 4. ���������� DDB
	DDB ddb;
	ddb.wProductCode	= pDataInfo->ProductCode();
	ddb.wElementCode	= pDataInfo->ElementCode();
	ddb.wDataType		= pDataInfo->DataType();
	ddb.wTimePeriod		= pDataInfo->TimePeriod();

	ddb.datm_beg		= pDataInfo->DateTimeBegin();
	ddb.datm_end		= pDataInfo->DateTimeEnd();

	ddb.fBaseValue		= (FLOAT)pDataInfo->BaseValue();
	ddb.fScale			= (FLOAT)pDataInfo->Scale();

	ddb.dwOffsetOfSdb	= 256;
	ddb.dwOffsetOfLdb	= 0;		// ͨ��������
	ddb.dwOffsetOfTdb	= 0;		// ͨ��������

	DDB* pDdb = (DDB*)((char*)pFileHeader + KD_BLOCK_OFFSET_DDB);
	*pDdb = ddb;

	// д���ļ�ͷ
	fwrite(pFileHeader,1,256,fp);

	// 5. �������ݿ� SDB
	//
	// 5.1 �������ݿ�ͷ
	long lOffsetOfSDBH = ftell(fp);

	SDBH sdbh;
	sdbh.dwBlockLength	= sizeof(SDBH);
	sdbh.wLayerNumbers	= 1;	// 1 ������

	// д��������ݿ�ͷ
	fwrite(&sdbh,sizeof(SDBH),1,fp);

	// 5.2 ���ݲ�ͷ
	long lOffsetOfLayerHeader = ftell(fp);

	LayerHeader layh;
	layh.wLayerID		= 1;
	layh.dwLayerLength	= sizeof(LayerHeader);	// ͨ��������

	// д�����ݲ�ͷ
	fwrite(&layh,sizeof(LayerHeader),1,fp);

	// 5.2.1 ��ɫ��
	ColorTablePacketHeader tColorTablePacketHeader;
	if(pPalette)
	{
		int iColorEntries	= pPalette->GetColorEntries();
		BOOL bHasComment	= pPalette->HasComment();

		BOOL bHasTransColor = pPalette->HasTransparentColor();
		COLORREF clrTrans	= pPalette->TransparentColor();

		// ���������
		DWORD dwPacketLength = sizeof(ColorTablePacketHeader);
		dwPacketLength += 8 * iColorEntries;
		if(bHasComment)
			dwPacketLength += 16 * iColorEntries;

		tColorTablePacketHeader.wPacketCode				= Const_Color_Table_Packet_Code;
		tColorTablePacketHeader.dwPacketLength			= dwPacketLength;
		tColorTablePacketHeader.wType					= pPalette->GetType();
		tColorTablePacketHeader.wColorEntries			= pPalette->GetColorEntries();
		tColorTablePacketHeader.wSpecificColorNumbers	= pPalette->SpecificColorNumbers();
		tColorTablePacketHeader.wHasComment				= pPalette->HasComment();
		tColorTablePacketHeader.wHasTransparentColor	= pPalette->HasTransparentColor();

		tColorTablePacketHeader.TA				= bHasTransColor ? 0 : 255;
		tColorTablePacketHeader.TR				= GetRValue(clrTrans);
		tColorTablePacketHeader.TG				= GetGValue(clrTrans);
		tColorTablePacketHeader.TB				= GetBValue(clrTrans);

		tColorTablePacketHeader.DA				= 255;
		tColorTablePacketHeader.DR				= 0;
		tColorTablePacketHeader.DG				= 0;
		tColorTablePacketHeader.DB				= 0;
	}

	// д����ɫ��ͷ
	fwrite(&tColorTablePacketHeader,sizeof(ColorTablePacketHeader),1,fp);

	// д����ɫ��
	if(pPalette)
	{
		int iColorEntries	= pPalette->GetColorEntries();
		for(i=0;i<iColorEntries;i++)
		{
			KPaletteItem item = *pPalette->GetAt(i);
			VARGB vargb = VARGB(item.V,item.A,item.R,item.G,item.B);
			fwrite(&vargb,sizeof(VARGB),1,fp);
		}

		// д��ɫ��ע��
		if(pPalette->HasComment())
		{
			char szTemp[17];
			for(i=0;i<iColorEntries;i++)
			{
				memset(szTemp,0,17);
				LPCTSTR ptsz = pPalette->GetAt(i)->GetComment();
				LPCSTR psz = KString::ToMByte(ptsz);
				strncpy(szTemp,psz,16);
				fwrite(szTemp,sizeof(char),16,fp);
			}
		}

	}

	// 5.2.2 �������
	GridDataPacketHeader tGridDataPacketHeader;
	tGridDataPacketHeader.wPacketCode		= Const_Grid_Data_Packet_Code;
	tGridDataPacketHeader.dwPacketLength	= sizeof(GridDataPacketHeader);

	tGridDataPacketHeader.dwWidth			= pDataInfo->Width();
	tGridDataPacketHeader.dwHeight			= pDataInfo->Height();

	tGridDataPacketHeader.xMin				= (float)pDataInfo->MinX();
	tGridDataPacketHeader.yMin				= (float)pDataInfo->MinY();

	tGridDataPacketHeader.xMax				= (float)pDataInfo->MaxX();
	tGridDataPacketHeader.yMax				= (float)pDataInfo->MaxY();

	tGridDataPacketHeader.xInterval			= (float)pDataInfo->XInterval();
	tGridDataPacketHeader.yInterval			= (float)pDataInfo->YInterval();

	// ���¼������ݰ�����
	int dtsize = ddb.wDataType;		// ��������
	if(dtsize>10) dtsize /= 10;		// float = 40 double = 80
	tGridDataPacketHeader.dwPacketLength	= sizeof(GridDataPacketHeader)
		+ tGridDataPacketHeader.dwWidth * tGridDataPacketHeader.dwHeight * dtsize
		;

	// д�������ݰ�ͷ
	fwrite(&tGridDataPacketHeader,sizeof(GridDataPacketHeader),1,fp);

	// д��������
	int totalNum = tGridDataPacketHeader.dwWidth * tGridDataPacketHeader.dwHeight;
	DataType eDataType = (DataType)pDataInfo->DataType();
	switch(eDataType)
	{
	case DataType_Byte:
		fwrite(pData,sizeof(BYTE),totalNum,fp);
		break;

	case DataType_Short:
		fwrite(pData,sizeof(SHORT),totalNum,fp);
		break;

	case DataType_Long:
		fwrite(pData,sizeof(LONG),totalNum,fp);
		break;

	case DataType_Float:
		fwrite(pData,sizeof(float),totalNum,fp);
		break;

	case DataType_Double:
		fwrite(pData,sizeof(double),totalNum,fp);
		break;

	default:
		break;
	}

	lOffsetSave = ftell(fp);
	
	//-----------------------------------------------------
	// ���㱾�����ݿ鳤��
	layh.dwLayerLength	= sizeof(LayerHeader)
		+ tColorTablePacketHeader.dwPacketLength	// ��ɫ�����ݰ�����
		+ tGridDataPacketHeader.dwPacketLength		// ������ݰ�����
		;
	fseek(fp,lOffsetOfLayerHeader,SEEK_SET);
	fwrite(&layh,sizeof(LayerHeader),1,fp);
	fseek(fp,lOffsetSave,SEEK_SET);


	// ����������ݿ鳤��
	sdbh.dwBlockLength	= sizeof(SDBH)
		+ layh.dwLayerLength		// �����ݿ鳤��
		;
	fseek(fp,lOffsetOfSDBH,SEEK_SET);
	fwrite(&sdbh,sizeof(SDBH),1,fp);
	fseek(fp,lOffsetSave,SEEK_SET);
	//-----------------------------------------------------

	// �ر��ļ�
	fflush(fp);
	fclose(fp);
	fp = NULL;

	return TRUE;
}

//---------------------------------------------------------
// ��̬���� - End
//---------------------------------------------------------

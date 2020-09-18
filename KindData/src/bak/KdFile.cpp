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

//本类数据是否清空(根据需要在派生类中根据需要取舍条件)
BOOL KdFile::IsEmpty()
{
	return TRUE
		&& KCustomDataObject::IsEmpty()
		&& m_pFileHeader == NULL
		&& m_pColors == NULL
		;
}

//---------------------------------------------------------
// 静态函数 - Beg
//---------------------------------------------------------

// 是否 KdFile 文件
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
	// 打开文件
	FILE* fp = _tfopen(lpszFile,_T("rb"));
	if(fp==NULL)
		return FALSE;

	char bytes[256];
	if(fread(bytes,sizeof(char),256,fp)<256)
		return FALSE;

	// 关闭文件
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

	// 打开文件
	FILE* fp = _tfopen(lpszFile,_T("rt"));
	if(fp==NULL) return FALSE;

	// 1.读文件Logo
	memset(szTemp,0,MAX_PATH);
	int sizeLogo = _tcslen(KD_FILE_ID_TEXT);
	_ftscanf(fp,_T("%s"),szTemp);		//文件头标示,为字符串“KD”,大小写均可
	if( szTemp[sizeLogo] != _T('\0') )
	{
		fclose(fp);
		fp = NULL;
		return FALSE;
	}

	// 转换为大写
	KString::UpperString(szTemp);

	// Logo 出错
	if( _tcscmp(szTemp,KD_FILE_ID_TEXT) !=0 )
	{
		fclose(fp);
		fp = NULL;
		return FALSE;
	}

	// 2.读产品数据格式代码
	int iFormatCode = -1;
	_ftscanf(fp,_T("%d"),&iFormatCode);

	// 关闭文件
	fclose(fp);
	fp = NULL;

	return iFormatCode > 0 && iFormatCode < 300;
}

BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, BYTE* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// 设置数据类型
	pDataInfo->DataType() = DataType_Byte;
	return CreateKdFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, char* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// 设置数据类型
	pDataInfo->DataType() = DataType_Char;
	return CreateKdFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, WORD* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// 设置数据类型
	pDataInfo->DataType() = DataType_Word;
	return CreateKdFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, SHORT* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// 设置数据类型
	pDataInfo->DataType() = DataType_Short;
	return CreateKdFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, DWORD* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// 设置数据类型
	pDataInfo->DataType() = DataType_Dword;
	return CreateKdFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, LONG* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// 设置数据类型
	pDataInfo->DataType() = DataType_Long;
	return CreateKdFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, float* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// 设置数据类型
	pDataInfo->DataType() = DataType_Float;
	return CreateKdFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KdFile::CreateKdFile(LPCTSTR lpszFile, double* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// 设置数据类型
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

	// 打开文件
	FILE* fp = _tfopen(lpszFile,_T("wb"));
	if(fp==NULL)
		return FALSE;

	// 文件头256字节
	BYTE fileHeader[256];
	memset(fileHeader,0,256);
	BYTE* pFileHeader = &fileHeader[0];

	// 1.文件头信息
	KdGridFileHeader* pGridFileHeader = (KdGridFileHeader*)pFileHeader;

	pGridFileHeader->wFileID			= KD_FILE_ID;							// 1		文件标识 0x444b
	pGridFileHeader->wFormatCode		= pDataInfo->FormatCode();				// 2		格式代码
	pGridFileHeader->wProductCode		= pDataInfo->ProductCode();				// 3		产品代码
	pGridFileHeader->wDataSource		= pDataInfo->DataSource();				// 4		数据来源

	pGridFileHeader->cDataType			= (BYTE)pDataInfo->DataType();			// 5.1		数据类型 1-BYTE  2-SHORT/WORD  4-LONG/DWORD 40-FLOAT 80-DOUBLE
	pGridFileHeader->cLayerNums			= (BYTE)pDataInfo->LayerNumbers();		// 5.2		层数(最多16层)
	pGridFileHeader->cColorTableFlag	= (BYTE)(NULL != pPalette);				// 6.1		颜色表标记
	pGridFileHeader->cProjectType		= (BYTE)pDataInfo->ProjectType();		// 6.2		投影类型: 0-不投影,1-等角投影,2-麦卡托投影,3-兰布托投影,4-极射赤面投影,5-艾尔伯斯投影
	pGridFileHeader->wElementCode		= pDataInfo->ElementCode();				// 7		要素代码
	pGridFileHeader->wTimePeriod		= pDataInfo->TimePeriod();				// 8		时效

	pGridFileHeader->wYear				= pDataInfo->Year();					// 9		年
	pGridFileHeader->wMonth				= pDataInfo->Month();					// 10		月
	pGridFileHeader->wDay				= pDataInfo->Day();						// 11		日
	pGridFileHeader->wHour				= pDataInfo->Hour();					// 12		时
	pGridFileHeader->wMinute			= pDataInfo->Minute();					// 13		分
	pGridFileHeader->wSecond			= pDataInfo->Second();					// 14		秒
	pGridFileHeader->dwMillisecond		= pDataInfo->Millisecond();				// 15-16	毫秒

	pGridFileHeader->wWidth				= pDataInfo->Width();					// 17		列数
	pGridFileHeader->wHeight			= pDataInfo->Height();					// 18		行数

	pGridFileHeader->xMin			= (float)pDataInfo->MinX();					// 19-20	最小经度
	pGridFileHeader->yMin			= (float)pDataInfo->MinY();					// 21-22	最小纬度

	pGridFileHeader->xMax			= (float)pDataInfo->MaxX();					// 23-24	最大经度
	pGridFileHeader->yMax			= (float)pDataInfo->MaxY();					// 25-26	最大纬度

	pGridFileHeader->xInterval		= (float)pDataInfo->XInterval();			// 27-28	经度分辨率 -- 等角投影		x分辨率 -- 麦卡托、兰布托、极射赤面投影 
	pGridFileHeader->yInterval		= (float)pDataInfo->YInterval();			// 29-30	纬度分辨率 -- 等角投影		y分辨率 -- 麦卡托、兰布托、极射赤面投影

	pGridFileHeader->fStandardLon	= (float)pDataInfo->StandardLon();			// 31-32	圆锥投影中心经线 (LAMBERT,POLAR,ALBERS)

	pGridFileHeader->fStandardLat1	= (float)pDataInfo->StandardLat1();			// 33-34	标准纬度1 -- 麦卡托、兰布托、极射赤面投影有效
	pGridFileHeader->fStandardLat2	= (float)pDataInfo->StandardLat2();			// 35-36	标准纬度2 -- 兰布托投影有效

	pGridFileHeader->fCenterLon		= (float)pDataInfo->CenterLon();			// 37-38	中心点的经度
	pGridFileHeader->fCenterLat		= (float)pDataInfo->CenterLat();			// 39-40	中心点的纬度

	pGridFileHeader->dwInvalidData	= pDataInfo->InvalidData();					// 41-42	无效数据值
	pGridFileHeader->fBaseValue		= (float)pDataInfo->BaseValue();			// 43-44	基准值
	pGridFileHeader->fScale			= (float)pDataInfo->Scale();				// 45-46	比例因子

	pGridFileHeader->dwReserved		= 0;										// 47-48	保留

	for(int index=0;index<16;index++)
		pGridFileHeader->wLayerCode[index] = pDataInfo->LayerCode(index);		// 49-64	层代码
	//-----------------------------------------------------
	//WORD	wReserved2[24];		// 

	pGridFileHeader->wYear2				= pDataInfo->Year2();					// 9		年
	pGridFileHeader->wMonth2			= pDataInfo->Month2();					// 10		月
	pGridFileHeader->wDay2				= pDataInfo->Day2();					// 11		日
	pGridFileHeader->wHour2				= pDataInfo->Hour2();					// 12		时
	pGridFileHeader->wMinute2			= pDataInfo->Minute2();					// 13		分
	pGridFileHeader->wSecond2			= pDataInfo->Second2();					// 14		秒
	pGridFileHeader->dwMillisecond2		= pDataInfo->Millisecond2();			// 15-16	毫秒
	//-----------------------------------------------------
	for(i=0;i<64;i++)
		pGridFileHeader->cComment[i] = 0;

	strncpy((char*)&pGridFileHeader->cComment[0],KString::ToMByte(pDataInfo->GetComment()),64);
	//-----------------------------------------------------
	// 写入文件头
	fwrite(pFileHeader,sizeof(BYTE),256,fp);

	// 2.颜色表
	if(pPalette)
	{
		 ColorTablePacketHeader tColorTablePacketHeader;

		tColorTablePacketHeader.wPacketCode				= Const_Color_Table_Packet_Code;	// 01		包代码 = 0x80
		tColorTablePacketHeader.dwPacketLength			= 0;								// 02-03	包长度
		tColorTablePacketHeader.wType					= pPalette->GetType();				// 04		类型
		tColorTablePacketHeader.wColorEntries			= pPalette->GetColorEntries();		// 05		颜色入口数
		tColorTablePacketHeader.wSpecificColorNumbers	= pPalette->SpecificColorNumbers();	// 06		特定颜色数量
		tColorTablePacketHeader.wHasComment				= pPalette->HasComment();			// 07		是否有对应注释
		tColorTablePacketHeader.wHasTransparentColor	= pPalette->HasTransparentColor();	// 08		使用透明色

		tColorTablePacketHeader.dwTransparentColor		= pPalette->TransparentColor();		// 09-10	透明颜色
		tColorTablePacketHeader.dwDefaultColor			= pPalette->DefaultColor();			// 11-12	默认颜色

		tColorTablePacketHeader.dwSpecialDataMin		= pPalette->SpecificDataMin();		// 13-14	特定最小数据
		tColorTablePacketHeader.dwSpecialDataMax		= pPalette->SpecificDataMax();		// 15-16	特定最大数据

		// 计算包长度
		WORD wPacketLength = sizeof(ColorTablePacketHeader);
		wPacketLength += 8 * tColorTablePacketHeader.wColorEntries;
		if(pPalette->HasComment())
			wPacketLength += 16 * tColorTablePacketHeader.wColorEntries;

		tColorTablePacketHeader.dwPacketLength			= wPacketLength;
		tColorTablePacketHeader.TA						= pPalette->HasTransparentColor() ? 0 : 255;

		fwrite(&tColorTablePacketHeader,sizeof(ColorTablePacketHeader),1,fp);

		// 写入颜色表
		for(i=0;i<tColorTablePacketHeader.wColorEntries;i++)
		{
			KPaletteItem item = *pPalette->GetAt(i);
			VARGB vargb(item.Value(),item.Rgb());
			fwrite(&vargb,sizeof(VARGB),1,fp);
		}

		// 写颜色表注释
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

	// 写入格点数据
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

	// 关闭文件
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

	// 打开文件
	FILE* fp = _tfopen(lpszFile,_T("wb"));
	if(fp==NULL)
		return FALSE;

	long lOffsetSave = 0;

	// 文件头一共 256 字节
	BYTE fileHeader[256];
	memset(fileHeader,0,256);
	BYTE* pFileHeader = &fileHeader[0];

	// 1. 数据信息块 DMB
	DMB dmb;
	dmb.wDataSource		= pDataInfo->DataSource();
	dmb.wDataCategory	= pDataInfo->DataCategory();	// KD_CATEGORY_GRID
	dmb.wBytesOrder		= pDataInfo->BytesOrder();

	DMB* pDmb = (DMB*)((char*)pFileHeader + KD_BLOCK_OFFSET_DMB);
	*pDmb = dmb;


	// 2. 扩展信息块 EMB
	EMB emb;
	emb.dwInvalidData		= pDataInfo->InvalidData();
	emb.dwSpecificDataMin	= pDataInfo->SpecificDataMin();
	emb.dwSpecificDataMax	= pDataInfo->SpecificDataMax();

	EMB* pEmb = (EMB*)((char*)pFileHeader + KD_BLOCK_OFFSET_EMB);
	*pEmb = emb;

	// 3. 注释数据块 CDB
	CDB cdb;
	cdb.SetComment( KString::ToMByte(pDataInfo->Comment()) );

	CDB* pCdb = (CDB*)((char*)pFileHeader + KD_BLOCK_OFFSET_CDB);
	*pCdb = cdb;

	// 4. 数据描述块 DDB
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
	ddb.dwOffsetOfLdb	= 0;		// 通过计算获得
	ddb.dwOffsetOfTdb	= 0;		// 通过计算获得

	DDB* pDdb = (DDB*)((char*)pFileHeader + KD_BLOCK_OFFSET_DDB);
	*pDdb = ddb;

	// 写入文件头
	fwrite(pFileHeader,1,256,fp);

	// 5. 表征数据块 SDB
	//
	// 5.1 表征数据块头
	long lOffsetOfSDBH = ftell(fp);

	SDBH sdbh;
	sdbh.dwBlockLength	= sizeof(SDBH);
	sdbh.wLayerNumbers	= 1;	// 1 层数据

	// 写入表征数据块头
	fwrite(&sdbh,sizeof(SDBH),1,fp);

	// 5.2 数据层头
	long lOffsetOfLayerHeader = ftell(fp);

	LayerHeader layh;
	layh.wLayerID		= 1;
	layh.dwLayerLength	= sizeof(LayerHeader);	// 通过计算获得

	// 写入数据层头
	fwrite(&layh,sizeof(LayerHeader),1,fp);

	// 5.2.1 颜色表
	ColorTablePacketHeader tColorTablePacketHeader;
	if(pPalette)
	{
		int iColorEntries	= pPalette->GetColorEntries();
		BOOL bHasComment	= pPalette->HasComment();

		BOOL bHasTransColor = pPalette->HasTransparentColor();
		COLORREF clrTrans	= pPalette->TransparentColor();

		// 计算包长度
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

	// 写入颜色表头
	fwrite(&tColorTablePacketHeader,sizeof(ColorTablePacketHeader),1,fp);

	// 写入颜色表
	if(pPalette)
	{
		int iColorEntries	= pPalette->GetColorEntries();
		for(i=0;i<iColorEntries;i++)
		{
			KPaletteItem item = *pPalette->GetAt(i);
			VARGB vargb = VARGB(item.V,item.A,item.R,item.G,item.B);
			fwrite(&vargb,sizeof(VARGB),1,fp);
		}

		// 写颜色表注释
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

	// 5.2.2 格点数据
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

	// 重新计算数据包长度
	int dtsize = ddb.wDataType;		// 数据类型
	if(dtsize>10) dtsize /= 10;		// float = 40 double = 80
	tGridDataPacketHeader.dwPacketLength	= sizeof(GridDataPacketHeader)
		+ tGridDataPacketHeader.dwWidth * tGridDataPacketHeader.dwHeight * dtsize
		;

	// 写入格点数据包头
	fwrite(&tGridDataPacketHeader,sizeof(GridDataPacketHeader),1,fp);

	// 写入格点数据
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
	// 计算本层数据块长度
	layh.dwLayerLength	= sizeof(LayerHeader)
		+ tColorTablePacketHeader.dwPacketLength	// 颜色表数据包长度
		+ tGridDataPacketHeader.dwPacketLength		// 格点数据包长度
		;
	fseek(fp,lOffsetOfLayerHeader,SEEK_SET);
	fwrite(&layh,sizeof(LayerHeader),1,fp);
	fseek(fp,lOffsetSave,SEEK_SET);


	// 计算表征数据块长度
	sdbh.dwBlockLength	= sizeof(SDBH)
		+ layh.dwLayerLength		// 层数据块长度
		;
	fseek(fp,lOffsetOfSDBH,SEEK_SET);
	fwrite(&sdbh,sizeof(SDBH),1,fp);
	fseek(fp,lOffsetSave,SEEK_SET);
	//-----------------------------------------------------

	// 关闭文件
	fflush(fp);
	fclose(fp);
	fp = NULL;

	return TRUE;
}

//---------------------------------------------------------
// 静态函数 - End
//---------------------------------------------------------

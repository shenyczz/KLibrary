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

// 构建调色板
// 先构造彩虹调色板
// 然后根据产品代码选择调色板文件
// 如果调色板文件存在则使用其数据构造调色板
void KAxinData::BuildPalette(LPCTSTR lpszFile /* = NULL */)
{
	if(IsEmpty())
		return;

	// 数据信息
	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return;

	// 删除原有调色板
	_delete(m_pPalette);

	// 产品代码
	// 0 - 表示使用彩虹调色板
	// x - 使用分段调色板(由外部设置)
	int iProductCode = pDataInfo->ProductCode();

	// 彩虹调色板
	KSegPalette* pPalette = new KSegPalette();
	m_pPalette = pPalette;

	// 1.先用彩虹颜色填充调色板
	// 彩虹调色板
	//KRbwPalette rbwPalette(KRbwPalette::InverseRainbow);
	KRbwPalette rbwPalette(KRbwPalette::GreenToRed);
	//KRbwPalette rbwPalette;
	rbwPalette.SetValueRange(pDataInfo->ExtremumValueMin(),pDataInfo->ExtremumValueMax());

	// 等值线数量
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
		double vInterval = pDataInfo->ContourInterval();	// 等值线间隔
		double vmin = pDataInfo->ContourMin();				// 等值线最小值
		double vmax = pDataInfo->ContourMax();				// 等值线最大值
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

	// 2.如果有颜色表文件并且文件存在
	if(!lpszFile && KFile::Exists(lpszFile))
	{
		KColortableData clrtbl;
		if(!clrtbl.LoadData(lpszFile))
			return;

		// 设置调色板为颜色表的调色板备份
		this->SetPalette(clrtbl.GetPalette()->Clone());
	}

	return;
}

// 取得产品调色板
KPalette* KAxinData::GetProductPalette(int iProductCode)
{
	return NULL;
}

//=========================================================
// 静态函数 - beg
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
		_ftscanf(fp,_T("%s"),szTemp);		// 文件头标示,为字符串 _T("AXIN"),大小写均可
		if( szTemp[sizeFid] != _T('\0') )
		{
			szTemp[sizeFid] =  _T('\0');
			fclose(fp);
			fp = NULL;
			return 0;
		}
		//转换为大写
		KString::UpperString(szTemp);
		if( _tcscmp(szTemp,AXIN_FID_TXT_MARK) != 0 )
		{
			fclose(fp);
			fp = NULL;
			return 0;
		}

		// 2.读产品数据格式代码
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
		_ftscanf(fp,_T("%s"),szTemp);		// 文件头标示,为字符串 _T("AXIN"),大小写均可
		if( szTemp[sizeFid] != _T('\0') )
		{
			fclose(fp);
			fp = NULL;
			return 0;
		}
		//转换为大写
		KString::UpperString(szTemp);
		if( _tcscmp(szTemp,AXIN_FID_TXT_MARK) != 0 )
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// 2.读产品数据格式代码
		_ftscanf(fp,_T("%d"),&iFormatCode);

		// 3.读注释信息
		_ftscanf(fp,_T("%s"),szTemp);
		if( _tcslen(szTemp) > (AXIN_COMMENT_LENGTH) )	// LEAP_COMMENT_LENGTH = 64
		{
			// 注释信息太长则截断
			szTemp[AXIN_COMMENT_LENGTH] = _T('\0');
		}

		// 4.读日期时间(yyyy,mm,dd,HH,MM,SS,MS)
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

	// 是否二进制文件
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

		pDataInfo->FileID()			= pAxinGridImageHeader->wFileID;				// 文件 ID
		pDataInfo->FormatCode()		= pAxinGridImageHeader->wFormatCode;			// 格式代码
		pDataInfo->ProductCode()	= pAxinGridImageHeader->wProductCode;			// 产品代码
		pDataInfo->DataSource()		= pAxinGridImageHeader->wDataSource;			// 数据来源

		pDataInfo->DataType()		= pAxinGridImageHeader->cDataType;				// 数据类型
		pDataInfo->LayerNumbers()	= pAxinGridImageHeader->cLayerNums;				// 层数
		pDataInfo->HasColorTable()	= pAxinGridImageHeader->cColorTableFlag != 0;	// 颜色表标记
		pDataInfo->ProjectType()	= pAxinGridImageHeader->cProjectType;			// 投影类型
		pDataInfo->ElementCode()	= pAxinGridImageHeader->wElementCode;			// 要素代码
		pDataInfo->TimePeriod()		= pAxinGridImageHeader->wTimePeriod;			// 时效

		pDataInfo->Year()			= pAxinGridImageHeader->wYear;				// 年
		pDataInfo->Month()			= pAxinGridImageHeader->wMonth;				// 月
		pDataInfo->Day()			= pAxinGridImageHeader->wDay;				// 日
		pDataInfo->Hour()			= pAxinGridImageHeader->wHour;				// 时
		pDataInfo->Minute()			= pAxinGridImageHeader->wMinute;			// 分
		pDataInfo->Second()			= pAxinGridImageHeader->wSecond;			// 秒
		pDataInfo->Millisecond()	= pAxinGridImageHeader->dwMillisecond;		// 毫秒

		pDataInfo->Width()			= pAxinGridImageHeader->wWidth;				// 列数
		pDataInfo->Height()			= pAxinGridImageHeader->wHeight;			// 行数

		pDataInfo->MinX()			= pAxinGridImageHeader->xMin;				// 最小经度
		pDataInfo->MinY()			= pAxinGridImageHeader->yMin;				// 最小纬度

		pDataInfo->MaxX()			= pAxinGridImageHeader->xMax;				// 最大经度
		pDataInfo->MaxY()			= pAxinGridImageHeader->yMax;				// 最大纬度

		pDataInfo->XInterval()		= pAxinGridImageHeader->xInterval;			// 经度分辨率
		pDataInfo->YInterval()		= pAxinGridImageHeader->yInterval;			// 纬度分辨率

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

		pDataInfo->Year2()			= pAxinGridImageHeader->wYear2;				// 年
		pDataInfo->Month2()			= pAxinGridImageHeader->wMonth2;			// 月
		pDataInfo->Day2()			= pAxinGridImageHeader->wDay2;				// 日
		pDataInfo->Hour2()			= pAxinGridImageHeader->wHour2;				// 时
		pDataInfo->Minute2()		= pAxinGridImageHeader->wMinute2;			// 分
		pDataInfo->Second2()		= pAxinGridImageHeader->wSecond2;			// 秒
		pDataInfo->Millisecond2()	= pAxinGridImageHeader->dwMillisecond2;		// 毫秒

		pDataInfo->SetComment(KString::ToTChar((char*)pAxinGridImageHeader->cComment));
	}
	//-----------------------------------------------------
	// AGMRS
	if(AGMRS_LOCAL_DATA_MARKER == ((LD2Header*)cBytes)->wFileID)
	{
		LD2Header* pLD2Header = (LD2Header*)cBytes;

		pDataInfo->FileID()			= pLD2Header->wFileID;				// 文件 ID
		//pDataInfo->FormatCode()		= pLD2Header->wFormatCode;			// 格式代码
		//pDataInfo->ProductCode()	= pLD2Header->wProductCode;			// 产品代码
		//pDataInfo->DataSource()		= pLD2Header->wDataSource;			// 数据来源

		//pDataInfo->DataType()		= pLD2Header->cDataType;				// 数据类型
		pDataInfo->LayerNumbers()	= pLD2Header->wChannelNums;				// 层数
		//pDataInfo->HasColorTable()	= pLD2Header->cColorTableFlag != 0;	// 颜色表标记
		pDataInfo->ProjectType()	= pLD2Header->wProjectType;			// 投影类型
		//pDataInfo->ElementCode()	= pLD2Header->wElementCode;			// 要素代码
		//pDataInfo->TimePeriod()		= pLD2Header->wTimePeriod;			// 时效

		pDataInfo->Year()			= pLD2Header->wYear;				// 年
		pDataInfo->Month()			= pLD2Header->wMonth;				// 月
		pDataInfo->Day()			= pLD2Header->wDay;				// 日
		pDataInfo->Hour()			= pLD2Header->wHour;				// 时
		pDataInfo->Minute()			= pLD2Header->wMinute;			// 分
		//pDataInfo->Second()			= pLD2Header->wSecond;			// 秒
		//pDataInfo->Millisecond()	= pLD2Header->dwMillisecond;		// 毫秒

		pDataInfo->Width()			= pLD2Header->wWidth;				// 列数
		pDataInfo->Height()			= pLD2Header->wHeight;			// 行数

		pDataInfo->MinX()			= pLD2Header->fLonMin;				// 最小经度
		pDataInfo->MinY()			= pLD2Header->fLatMin;				// 最小纬度

		pDataInfo->MaxX()			= pLD2Header->fLonMax;				// 最大经度
		pDataInfo->MaxY()			= pLD2Header->fLatMax;				// 最大纬度

		pDataInfo->XInterval()		= pLD2Header->fLonResolution;		// 经度分辨率
		pDataInfo->YInterval()		= pLD2Header->fLatResolution;			// 纬度分辨率

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
	// 设置数据类型
	pDataInfo->DataType() = DataType_Byte;
	return KAxinData::CreateFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KAxinData::CreateFile(LPCTSTR lpszFile, char* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// 设置数据类型
	pDataInfo->DataType() = DataType_Char;
	return KAxinData::CreateFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KAxinData::CreateFile(LPCTSTR lpszFile, WORD* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// 设置数据类型
	pDataInfo->DataType() = DataType_Word;
	return KAxinData::CreateFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KAxinData::CreateFile(LPCTSTR lpszFile, SHORT* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// 设置数据类型
	pDataInfo->DataType() = DataType_Short;
	return KAxinData::CreateFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KAxinData::CreateFile(LPCTSTR lpszFile, DWORD* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// 设置数据类型
	pDataInfo->DataType() = DataType_Dword;
	return KAxinData::CreateFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KAxinData::CreateFile(LPCTSTR lpszFile, LONG* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// 设置数据类型
	pDataInfo->DataType() = DataType_Long;
	return KAxinData::CreateFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KAxinData::CreateFile(LPCTSTR lpszFile, float* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// 设置数据类型
	pDataInfo->DataType() = DataType_Float;
	return KAxinData::CreateFile(lpszFile,(void*)pData,pDataInfo,pPalette);;
}
BOOL KAxinData::CreateFile(LPCTSTR lpszFile, double* pData, KDataInfo* pDataInfo, KPalette* pPalette)
{
	// 设置数据类型
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

	// 打开文件
	FILE* fp = _tfopen(lpszFile,_T("wb"));
	if(fp==NULL)
		return FALSE;

	// 文件头256字节
	BYTE fileHeader[256];
	memset(fileHeader,0,256);
	BYTE* pFileHeader = &fileHeader[0];

	// 1.文件头信息
	AxinGridImageHeader* pGridFileHeader = (AxinGridImageHeader*)pFileHeader;

	pGridFileHeader->wFileID			= AXIN_FID_BIN_MARK;					// 1		文件标识 'AX'
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

	pGridFileHeader->xInterval		= (float)pDataInfo->XInterval();			// 27-28	经度分辨率 -- 等角投影		x分辨率 -- 麦卡托、兰布托、极射赤面投影 
	pGridFileHeader->yInterval		= (float)pDataInfo->YInterval();			// 29-30	纬度分辨率 -- 等角投影		y分辨率 -- 麦卡托、兰布托、极射赤面投影

	pGridFileHeader->xMax			= (float)pDataInfo->MaxX();					// 23-24	最大经度
	pGridFileHeader->yMax			= (float)pDataInfo->MaxY();					// 25-26	最大纬度

	pGridFileHeader->fStandardLon	= (float)pDataInfo->StandardLon();			// 31-32	圆锥投影中心经线 (LAMBERT,POLAR,ALBERS)

	pGridFileHeader->fStandardLat1	= (float)pDataInfo->StandardLat1();			// 33-34	标准纬度1 -- 麦卡托、兰布托、极射赤面投影有效
	pGridFileHeader->fStandardLat2	= (float)pDataInfo->StandardLat2();			// 35-36	标准纬度2 -- 兰布托投影有效

	pGridFileHeader->fCenterLon		= (float)pDataInfo->CenterLon();			// 37-38	中心点的经度
	pGridFileHeader->fCenterLat		= (float)pDataInfo->CenterLat();			// 39-40	中心点的纬度

	pGridFileHeader->dwInvalidData	= pDataInfo->InvalidData();					// 41-42	无效数据值
	pGridFileHeader->fBaseValue		= (float)pDataInfo->BaseValue();			// 43-44	基准值
	pGridFileHeader->fScale			= (float)pDataInfo->Scale();				// 45-46	比例因子

	//pGridFileHeader->dwVersion		= 0;										// 47-48	版本

	for(int index=0;index<16;index++)
	{
		pGridFileHeader->wLayerCode[index] = pDataInfo->LayerCode(index);		// 49-64	层代码
	}
	//-----------------------------------------------------
	pGridFileHeader->wYear2				= pDataInfo->Year2();					// 9		年
	pGridFileHeader->wMonth2			= pDataInfo->Month2();					// 10		月
	pGridFileHeader->wDay2				= pDataInfo->Day2();					// 11		日
	pGridFileHeader->wHour2				= pDataInfo->Hour2();					// 12		时
	pGridFileHeader->wMinute2			= pDataInfo->Minute2();					// 13		分
	pGridFileHeader->wSecond2			= pDataInfo->Second2();					// 14		秒
	pGridFileHeader->dwMillisecond2		= pDataInfo->Millisecond2();			// 15-16	毫秒
	//-----------------------------------------------------
	for(i=0;i<64;i++)
	{
		pGridFileHeader->cComment[i] = 0;
	}
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
		wPacketLength += 8 * tColorTablePacketHeader.wColorEntries;			// rgbv 8个字节
		if(pPalette->HasComment())
			wPacketLength += 16 * tColorTablePacketHeader.wColorEntries;	// 注释为16字节

		tColorTablePacketHeader.dwPacketLength			= wPacketLength;
		tColorTablePacketHeader.TA						= pPalette->HasTransparentColor() ? 0 : 255;

		// 写入颜色表包头
		fwrite(&tColorTablePacketHeader,sizeof(ColorTablePacketHeader),1,fp);

		// 写入颜色表
		for(i=0;i<tColorTablePacketHeader.wColorEntries;i++)
		{
			KPaletteItem item = *pPalette->GetAt(i);
			VARGB vargb(item.Value(),item.Rgb());
			fwrite(&vargb,sizeof(VARGB),1,fp);
		}

		// 颜色表后面写颜色表注释
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

	// 写入格点数据
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

	// 关闭文件
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

	// 文件内容
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

	// 关闭文件
	fclose(fp);
	fp = nullptr;

	return TRUE;
}

//---------------
// 静态函数 - end
//=========================================================

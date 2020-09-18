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

// 装载数据
BOOL KGridImageData::LoadData_bin(LPCTSTR lpszFile)
{
	int i;

	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return FALSE;

	if(!this->IsEmpty())
		Empty();

	// 打开文件
	FILE* fp = _tfopen(lpszFile,_T("rb"));
	if(fp==NULL)
		return FALSE;

	// 1.读取文件头
	if(!m_pFileHeader)
		m_pFileHeader = new char[256];

	if(fread(m_pFileHeader,1,256,fp)<256)
	{
		fclose(fp);
		fp = NULL;
		return FALSE;
	}

	// 填充 DataInfo
	this->FillDataInfo(m_pFileHeader);
	if(AXIN_FID_BIN_MARK != pDataInfo->FileID())
		return FALSE;
	if(!(pDataInfo->FormatCode()>0 && pDataInfo->FormatCode()<100))
		return FALSE;

	// 文件头
	AxinGridImageHeader* pAxinGridImageHeader = (AxinGridImageHeader*)m_pFileHeader;

	m_dwDataOffset = 256;

	// 2.读取颜色表
	if(pAxinGridImageHeader->cColorTableFlag != 0)
	{
		// 读取颜色标数据包头结构
		ColorTablePacketHeader tColorTablePacketHeader;
		if(fread(&tColorTablePacketHeader,sizeof(ColorTablePacketHeader),1,fp)<1)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// 颜色表入口数
		int iColorEntries = tColorTablePacketHeader.wColorEntries;
		vector<VARGB> vargbs;
		for(i=0;i<iColorEntries;i++)
		{
			VARGB vargb;
			fread(&vargb,sizeof(VARGB),1,fp);
			vargbs.push_back(vargb);
		}

		// 如果存在调色板注释
		// 读取调色板注释到临时字符串数组
		vector<KString> comments;
		BOOL bHasComment = tColorTablePacketHeader.wHasComment != 0;
		if(bHasComment)
		{
			char szTemp[17];	// 每条注释16个ASCII字符
			for(i=0;i<iColorEntries;i++)
			{
				fread(szTemp,sizeof(char),16,fp);	// 每条注释16个字节
				szTemp[16] = '\0';

				KString comment = KString::ToTChar(szTemp);
				comments.push_back(comment);
			}
		}

		// 把颜色表和注释合并到调色板
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

		// 是否有透明颜色
		BOOL bHasTransparentColor = tColorTablePacketHeader.wHasTransparentColor != 0;
		COLORREF clrTransparent = RGB(tColorTablePacketHeader.TR,tColorTablePacketHeader.TG,tColorTablePacketHeader.TB);
		pPalette->HasTransparentColor() = bHasTransparentColor;
		pPalette->TransparentColor() = clrTransparent;

		this->SetPalette(pPalette);

		this->GetDataInfo()->HasTransparentColor() = pPalette->HasTransparentColor();
		this->GetDataInfo()->TransparentColor() = pPalette->TransparentColor();

		m_dwDataOffset += tColorTablePacketHeader.dwPacketLength;

	}//if(pAxinGridImageHeader->cColorTableFlag != 0)

	// 数据偏移地址
	this->GetDataInfo()->DataOffset = m_dwDataOffset;

	// 读取数据
	void* pVoid = NULL;
	int xNum = pAxinGridImageHeader->xNum;
	int yNum = pAxinGridImageHeader->yNum;

	// 根据数据类型读取数据
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

	// 关闭文件
	fclose(fp);
	fp = NULL;

	if(!pVoid)
		return FALSE;

	// 分配内存
	this->PrepareMemory(eUvw_v);

	// 取得数据指针
	double* pdv = this->GetVData();
	if(!pdv)
	{
		_deletea(pVoid);
		return FALSE;
	}

	// 转存数据
	int totalSize = pDataInfo->Width() * pDataInfo->Height();	// 数据尺寸
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

	// 回收分配的内存
	_deletea(pVoid);

	// 设置绑定盒
	this->SetExtents(KExtents(pDataInfo->xMin,pDataInfo->yMin,pDataInfo->xMax,pDataInfo->yMax));

	return KGridData::LoadData_bin(lpszFile);
}

// 填充数据信息
void KGridImageData::FillDataInfo(LPVOID lpvData)
{
	// 文件头
	AxinGridImageHeader* pAxinGridImageHeader = (AxinGridImageHeader*)lpvData;
	// 保存到 DataInfo
	KDataInfo* pDataInfo = this->GetDataInfo();

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

	//KPalette* pPalette = this->GetPalette();
	//pDataInfo->HasColorTable()			= pPalette ? TRUE : FALSE;
	//pDataInfo->bHasComment				= pPalette->HasComment();
	//pDataInfo->HasTransparentColor()	= pPalette->HasTransparentColor();
}

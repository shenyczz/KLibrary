#include "StdAfx.h"
#include "KTest.h"

#include "MainFrm.h"
#include "KindMapDoc.h"
#include "KindMapView.h"

//static
int KTest::ProductCode = 0;


KTest::KTest(void)
{
}


KTest::~KTest(void)
{
}


BOOL KTest::Asc2Axd(IN LPCTSTR lpszFileAsc, OUT LPCTSTR lpszFileAxd, LPCTSTR lpszComment)
{
	int i,j;
	//-----------------------------------------------------
	TCHAR szTemp[MAX_PATH] = _T("");

	int iWidth = 0;
	int iHeight = 0;

	float dMinLon = 0;
	float dMinLat = 0;
	float dInterval = 0;

	int iNoDataValue = -9999;
	int iTemp = 0;
	short *pwData = NULL;

	// rastert_市界_po1.asc
	FILE* fpAsc = _tfopen(lpszFileAsc,_T("rt"));
	if(fpAsc==nullptr)
		return FALSE;

	_ftscanf(fpAsc,_T("%s %d"),szTemp,&iWidth);
	_ftscanf(fpAsc,_T("%s %d"),szTemp,&iHeight);
	_ftscanf(fpAsc,_T("%s %f"),szTemp,&dMinLon);
	_ftscanf(fpAsc,_T("%s %f"),szTemp,&dMinLat);
	_ftscanf(fpAsc,_T("%s %f"),szTemp,&dInterval);

	_ftscanf(fpAsc,_T("%s %d"),szTemp,&iNoDataValue);

	pwData = new short[iWidth * iHeight];
	memset(pwData,0,sizeof(short)*iWidth * iHeight);

	int iii = 0;
	for(i=0;i<iHeight;i++)
	{
		int ii = iHeight-1-i;
		for(j=0;j<iWidth;j++)
		{
			_ftscanf(fpAsc,_T("%d"),&iTemp);
			*(pwData + ii*iWidth+j) = iTemp;
		}
	}

	fclose(fpAsc);
	fpAsc=nullptr;
	//-----------------------------------------------------
	KDataInfo di;
	KDataInfo* pProductDataInfo = &di;

	KPalette* pPalette = NULL;
	//pPalette = KAgmrsData::GetAgmrsProductPalette(0xF000,PaletteType_Index);	// 地市格点
	pPalette = KAgmrsData::GetAgmrsProductPalette(KTest::ProductCode,PaletteType_Segment);	// 地市格点
	//pPalette = KAgmrsData::GetAgmrsProductPalette(0xF002,PaletteType_Segment);	// 县市格点

	// 设置调色板透明色
	pPalette->HasTransparentColor() = TRUE;
	pPalette->TransparentColor() = RGB(0,0,0);

	// 1.格式代码
	pProductDataInfo->wFormatCode	= AXIN_FMT_CODE_GRID_IMAGE;			// important

	// 2.日期
	pProductDataInfo->wYear			= 2012;
	pProductDataInfo->wMonth		= 3;
	pProductDataInfo->wDay			= 8;
	pProductDataInfo->wHour			= 11;
	pProductDataInfo->wMinute		= 0;
	pProductDataInfo->wSecond		= 0;
	pProductDataInfo->dwMillisecond	= 0;

	// 3.数据描述
	pProductDataInfo->wElementCode	= 0;
	pProductDataInfo->wProductCode	= KTest::ProductCode;
	pProductDataInfo->wDataType		= DataType_Short;
	pProductDataInfo->InvalidData()	= AXIN_INVALID_DATA;
	pProductDataInfo->dBaseValue	= 0;
	pProductDataInfo->dScale		= 1.0;

	// 4.格点参数
	pProductDataInfo->xNum			= iWidth;
	pProductDataInfo->xMin			= dMinLon;
	pProductDataInfo->xMax			= dMinLon + dInterval*iWidth;
	pProductDataInfo->xInterval		= dInterval;

	pProductDataInfo->yNum			= iHeight;
	pProductDataInfo->yMin			= dMinLat;
	pProductDataInfo->yMax			= dMinLat + dInterval * iHeight;
	pProductDataInfo->yInterval		= dInterval;

	// 注释信息
	//_stprintf(szTemp,_T("%s"),_T("河南省县市级行政区划格点数据_1:25_0.0025"));
	_stprintf(szTemp,_T("%s"),lpszComment);
	pProductDataInfo->SetComment(szTemp);

	KAxinData::CreateFile(lpszFileAxd,pwData,&di,pPalette);
	//-----------------------------------------------------
	_deletea(pwData);
	pwData = NULL;

	return TRUE;
}

void KTest::Fire()
{

}

void KTest::EnviHdr(KGridData* pGridData)
{
	CString strFileName = pGridData->GetFileName();
	strFileName.Replace(_T("axd"),_T("hdr"));
	pGridData->ExportEnviHdr(strFileName);
	return;
}

BOOL KTest::Layer2Bmp(LPCTSTR lpszFile)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	KLayerDockablePane* pLayerWnd = (KLayerDockablePane*)pMainFrame->GetLayerWnd();

	KDib* pDib = NULL;
	KLayer* pLayer = NULL;
	KProvider* pProvider = NULL;			// 数据提供者
	KDataInfo* pDataInfo = NULL;			// 数据信息
	KDataObject* pDataObject = NULL;		// 数据对象
	KDataProcessor* pDataProcessor = NULL;	// 数据处理器

	pLayer = pLayerWnd->GetSelectedLayer();
	if(!pLayer)
		return FALSE;

	pProvider = pLayer->GetProvider();
	if(!pProvider)
		return FALSE;

	pDataObject = pProvider->GetDataObject();
	if(!pDataObject)
		return FALSE;

	pDataInfo = pDataObject->GetDataInfo();
	if(!pDataInfo)
		return FALSE;

	pDataProcessor = pDataObject->GetDataProcessor();
	if(!pDataProcessor)
		return FALSE;

	pDib = pDataProcessor->GetDib();
	if(!pDib)
		return FALSE;

	if(pDib->Save(lpszFile))
	{
		CString strBmpw = lpszFile;
		if(strBmpw.Find(_T(".bmp"))>0)
		{
			//strBmpw += _T(".bmpw");
			strBmpw.Replace(_T(".bmp"),_T(".bmpw"));
		}

		FILE* fp = _tfopen(strBmpw,_T("wt"));
		if(fp)
		{
			_ftprintf(fp,_T("%f\n"),pDataInfo->XInterval());
			_ftprintf(fp,_T("0\n"));
			_ftprintf(fp,_T("0\n"));
			_ftprintf(fp,_T("%f\n"),-pDataInfo->YInterval());
			_ftprintf(fp,_T("%f\n"),pDataInfo->MinLon());
			_ftprintf(fp,_T("%f\n"),pDataInfo->MaxLat());
			fclose(fp);

			return TRUE;
		}
	}

	return FALSE;
}

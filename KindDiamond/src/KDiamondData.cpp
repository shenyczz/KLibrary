#include "StdAfx.h"
#include "KDiamondData.h"


KDiamondData::KDiamondData(void)
{
}


KDiamondData::~KDiamondData(void)
{
}

// 构建调色板
// 先构造彩虹调色板
// 然后根据产品代码选择调色板文件
// 如果调色板文件存在则使用其数据构造调色板
void KDiamondData::BuildPalette(LPCTSTR lpszFile /* = NULL */)
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
	// 彩虹调色板 RainbowStyle
	//KRbwPalette rbwPalette;
	//KRbwPalette rbwPalette(KRbwPalette::RainbowStyle::InverseRainbow);
	KRbwPalette rbwPalette(KRbwPalette::GreenToRed);
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
	else if(iContourNums == DIAM_FMT_CVID_CONTOUR)
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

	// 2.如果有颜色表文件并且文件存在
	if(lpszFile)
	{
		KColortableData clrtbl;
		if(!clrtbl.LoadData(lpszFile))
			return;

		// 设置调色板为颜色表的调色板备份
		this->SetPalette(clrtbl.GetPalette()->Clone());
		this->GetPalette()->SetValueRange(pDataInfo->ExtremumValueMin(),pDataInfo->ExtremumValueMax());
	}

	return;
}

//=========================================================
// 静态函数 - beg
//---------------
bool KDiamondData::IsDiamondData(LPCTSTR lpszFile)
{
	int iFormarCode = KDiamondData::GetFormatCode(lpszFile);
	return iFormarCode>0 && iFormarCode<99;	// 1-100
	return false;
}
int KDiamondData::GetFormatCode(LPCTSTR lpszFile)
{
	int iFormatCode = 0;
	TCHAR szTemp[MAX_PATH] = _T("");

	BOOL bIsBinary = KFile::IsBinary(lpszFile);
	if(bIsBinary)	// bin file
	{

	}
	else			// txt file
	{
		FILE* fp = _tfopen(lpszFile,_T("rt"));
		if(fp==NULL)
			return 0;

		int sizeFid = _tcslen(_T("DIAMOND")); 
		_ftscanf(fp,_T("%s"),szTemp);		// 文件头标示,为字符串 _T("DIAMOND"),大小写均可
		if( szTemp[sizeFid] != _T('\0') )
		{
			fclose(fp);
			fp = NULL;
			return 0;
		}
		//转换为大写
		KString::UpperString(szTemp);
		if( _tcscmp(szTemp,_T("DIAMOND")) != 0 )
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

//---------------
// 静态函数 - end
//=========================================================

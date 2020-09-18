#include "StdAfx.h"
#include "KIdxPalette.h"


KIdxPalette::KIdxPalette(void)
{
	SetType(PaletteType_Index);
	this->SetName(_T("索引调色板"));
}


KIdxPalette::~KIdxPalette(void)
{
}

// 克隆
KPalette* KIdxPalette::Clone()
{
	// 重新牛一个调色板
	KPalette* pPalette = new KIdxPalette();
	int iEntries = this->GetColorEntries();
	for(int i=0;i<iEntries;i++)
	{
		pPalette->Add(this->GetAt(i)->Clone());
	}
	return pPalette;
}
// 颜色
COLORREF KIdxPalette::GetColor(double dValue,COLORREF clrDefault)
{
	COLORREF clrReturn = clrDefault;

	// 条目数量
	int iCount = this->Count();
	for(int i=0;i<iCount;i++)
	{
		KPaletteItem* pItem = this->GetAt(i);
		double vtmp = pItem->Value();
		if(fabs(vtmp - dValue)<EPS)
		{
			clrReturn = pItem->Rgb();
			break;
		}
	}

	return clrReturn;
}

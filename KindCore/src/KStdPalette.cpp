#include "StdAfx.h"
#include "KStdPalette.h"


KStdPalette::KStdPalette(void)
{
	SetType(PaletteType_Standard);
	this->SetName(_T("标准调色板"));
}


KStdPalette::~KStdPalette(void)
{
}

// 克隆
KPalette* KStdPalette::Clone()
{
	// 重新牛一个调色板
	KPalette* pPalette = new KStdPalette();
	int iEntries = this->GetColorEntries();
	for(int i=0;i<iEntries;i++)
	{
		pPalette->Add(this->GetAt(i)->Clone());
	}
	return pPalette;
}

// 颜色
COLORREF KStdPalette::GetColor(double dValue,COLORREF clrDefault)
{
	COLORREF clrReturn = clrDefault;

	// 有效条目数量
	int iCount = this->NumbersOfValidItem();
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

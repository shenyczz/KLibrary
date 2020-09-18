#include "StdAfx.h"
#include "KIdxPalette.h"


KIdxPalette::KIdxPalette(void)
{
	SetType(PaletteType_Index);
	this->SetName(_T("������ɫ��"));
}


KIdxPalette::~KIdxPalette(void)
{
}

// ��¡
KPalette* KIdxPalette::Clone()
{
	// ����ţһ����ɫ��
	KPalette* pPalette = new KIdxPalette();
	int iEntries = this->GetColorEntries();
	for(int i=0;i<iEntries;i++)
	{
		pPalette->Add(this->GetAt(i)->Clone());
	}
	return pPalette;
}
// ��ɫ
COLORREF KIdxPalette::GetColor(double dValue,COLORREF clrDefault)
{
	COLORREF clrReturn = clrDefault;

	// ��Ŀ����
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

#include "StdAfx.h"
#include "KStdPalette.h"


KStdPalette::KStdPalette(void)
{
	SetType(PaletteType_Standard);
	this->SetName(_T("��׼��ɫ��"));
}


KStdPalette::~KStdPalette(void)
{
}

// ��¡
KPalette* KStdPalette::Clone()
{
	// ����ţһ����ɫ��
	KPalette* pPalette = new KStdPalette();
	int iEntries = this->GetColorEntries();
	for(int i=0;i<iEntries;i++)
	{
		pPalette->Add(this->GetAt(i)->Clone());
	}
	return pPalette;
}

// ��ɫ
COLORREF KStdPalette::GetColor(double dValue,COLORREF clrDefault)
{
	COLORREF clrReturn = clrDefault;

	// ��Ч��Ŀ����
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

#include "StdAfx.h"
#include "KLinearPalette.h"


KLinearPalette::KLinearPalette(void)
{
	SetType(PaletteType_Linear);
	this->SetName(_T("���Ե�ɫ��"));
}


KLinearPalette::~KLinearPalette(void)
{
}

// ��¡
KLinearPalette* KLinearPalette::Clone()
{
	// ����ţһ����ɫ��
	KLinearPalette* pPalette = new KLinearPalette();
	int iEntries = this->GetColorEntries();
	for(int i=0;i<iEntries;i++)
	{
		pPalette->Add(this->GetAt(i)->Clone());
	}
	return pPalette;
}

COLORREF KLinearPalette::GetColor(double dValue,COLORREF clrDefault)
{
	COLORREF clrRet = clrDefault;

	// ��Ч�ĸ���ֵ(-9999)
	if(fabs(dValue-KIND_INVALID_FLOAT_VALUE)<EPS)
	{
		return clrDefault;
	}

	// ��Ч��Ŀ����
	int numValidItem = this->NumbersOfValidItem(0x8000);

	double dValidmin = 0;		// ��С��Чֵ
	double dValidmax = 0;		// �����Чֵ
	double dValidDif = 1;
	COLORREF clrValidmin = 0;	// ��С��Ч��ɫֵ
	COLORREF clrValidmax = 0;	// �����Ч��ɫֵ
	if(numValidItem>0)
	{
		dValidmin = this->GetAt(0)->Value();
		clrValidmin = this->GetAt(0)->Rgb();

		dValidmax = this->GetAt(numValidItem-1)->Value();
		clrValidmax = this->GetAt(numValidItem-1)->Rgb();

		dValidDif = fabs(dValidmax - dValidmin);
	}

	int index = this->FindIndex(dValue);
	// ������ֵ
	if(index>=numValidItem)
	{
		return this->GetAt(index)->Rgb();
	}

	// ��������ֵ
	for(int i=0;i<numValidItem;i++)
	{
		KPaletteItem item = *this->GetAt(i);
		double vv = item.V;

		if(vv > dValidmax)	// ���������Чֵ
		{
			clrRet = clrValidmax;
			break;
		}
		// <= vv
		if(dValue<vv || fabs(dValue-vv)<10.e-12)
		{
			// �ȸ�ֵĬ��
			clrRet = item.Rgb();
			// != vv ȡ��ɫƽ��ֵ
			if(fabs(dValue-vv)>10.e-12 && i>0)
			{
				KPaletteItem itemPrv = *this->GetAt(i-1);
				clrRet = RGB
				(
					(item.R + itemPrv.R) / 2,
					(item.G + itemPrv.G) / 2,
					(item.B + itemPrv.B) / 2
				);
			}

			break;

		}

	}

	return clrRet;
}

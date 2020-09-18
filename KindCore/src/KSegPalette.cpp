#include "StdAfx.h"
#include "KSegPalette.h"


KSegPalette::KSegPalette(void)
{
	SetType(PaletteType_Segment);
	this->SetName(_T("�ֶε�ɫ��"));
}


KSegPalette::~KSegPalette(void)
{
}

// ��¡
KPalette* KSegPalette::Clone()
{
	// ����ţһ����ɫ��
	KPalette* pPalette = new KSegPalette();
	int iEntries = this->GetColorEntries();
	for(int i=0;i<iEntries;i++)
	{
		pPalette->Add(this->GetAt(i)->Clone());
	}
	return pPalette;
}

// ȡ�÷ֶ���ɫ
COLORREF KSegPalette::GetColor(double dValue,COLORREF clrDefault)
{
	COLORREF clrRet = clrDefault;

	// ��Ч�ĸ���ֵ(-9999)
	if(fabs(dValue-KIND_INVALID_FLOAT_VALUE)<EPS)
	{
		return clrDefault;
	}

	// ��Ч��Ŀ����
	int numValidItem = this->NumbersOfValidItem(0x8000);

	double dValidmax = 0;
	COLORREF clrValidmax = 0;
	if(numValidItem>0)
	{
		dValidmax = this->GetAt(numValidItem-1)->Value();
		clrValidmax = this->GetAt(numValidItem-1)->Rgb();
	}

	// ����ָ��ֵ����(-1��ʾû���ҵ�)
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

		// �������ֵȡ���ֵ��ɫ
		if(vv > dValidmax)
		{
			clrRet = clrValidmax;
			break;
		}

		// <= vv
		if(dValue<vv || fabs(dValue-vv)<10.e-12)
		{
			clrRet = item.Rgb();
			//// != vv ȡ��ɫƽ��ֵ
			//if(fabs(dValue-vv)>10.e-12 && i>0)
			//{
			//	KPaletteItem itemPrv = *this->GetAt(i-1);
			//	clrRet = RGB
			//	(
			//		(item.R + itemPrv.R) / 2,
			//		(item.G + itemPrv.G) / 2,
			//		(item.B + itemPrv.B) / 2
			//	);
			//}
			break;
		}
	}

	return clrRet;
}

#include "StdAfx.h"
#include "KLinearPalette.h"


KLinearPalette::KLinearPalette(void)
{
	SetType(PaletteType_Linear);
	this->SetName(_T("线性调色板"));
}


KLinearPalette::~KLinearPalette(void)
{
}

// 克隆
KLinearPalette* KLinearPalette::Clone()
{
	// 重新牛一个调色板
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

	// 无效的浮点值(-9999)
	if(fabs(dValue-KIND_INVALID_FLOAT_VALUE)<EPS)
	{
		return clrDefault;
	}

	// 有效条目数量
	int numValidItem = this->NumbersOfValidItem(0x8000);

	double dValidmin = 0;		// 最小有效值
	double dValidmax = 0;		// 最大有效值
	double dValidDif = 1;
	COLORREF clrValidmin = 0;	// 最小有效颜色值
	COLORREF clrValidmax = 0;	// 最大有效颜色值
	if(numValidItem>0)
	{
		dValidmin = this->GetAt(0)->Value();
		clrValidmin = this->GetAt(0)->Rgb();

		dValidmax = this->GetAt(numValidItem-1)->Value();
		clrValidmax = this->GetAt(numValidItem-1)->Rgb();

		dValidDif = fabs(dValidmax - dValidmin);
	}

	int index = this->FindIndex(dValue);
	// 是特殊值
	if(index>=numValidItem)
	{
		return this->GetAt(index)->Rgb();
	}

	// 不是特殊值
	for(int i=0;i<numValidItem;i++)
	{
		KPaletteItem item = *this->GetAt(i);
		double vv = item.V;

		if(vv > dValidmax)	// 大于最大有效值
		{
			clrRet = clrValidmax;
			break;
		}
		// <= vv
		if(dValue<vv || fabs(dValue-vv)<10.e-12)
		{
			// 先赋值默认
			clrRet = item.Rgb();
			// != vv 取颜色平均值
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

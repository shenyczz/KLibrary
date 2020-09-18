#include "StdAfx.h"
#include "KSegPalette.h"


KSegPalette::KSegPalette(void)
{
	SetType(PaletteType_Segment);
	this->SetName(_T("分段调色板"));
}


KSegPalette::~KSegPalette(void)
{
}

// 克隆
KPalette* KSegPalette::Clone()
{
	// 重新牛一个调色板
	KPalette* pPalette = new KSegPalette();
	int iEntries = this->GetColorEntries();
	for(int i=0;i<iEntries;i++)
	{
		pPalette->Add(this->GetAt(i)->Clone());
	}
	return pPalette;
}

// 取得分段颜色
COLORREF KSegPalette::GetColor(double dValue,COLORREF clrDefault)
{
	COLORREF clrRet = clrDefault;

	// 无效的浮点值(-9999)
	if(fabs(dValue-KIND_INVALID_FLOAT_VALUE)<EPS)
	{
		return clrDefault;
	}

	// 有效条目数量
	int numValidItem = this->NumbersOfValidItem(0x8000);

	double dValidmax = 0;
	COLORREF clrValidmax = 0;
	if(numValidItem>0)
	{
		dValidmax = this->GetAt(numValidItem-1)->Value();
		clrValidmax = this->GetAt(numValidItem-1)->Rgb();
	}

	// 查找指定值索引(-1表示没有找到)
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

		// 大于最大值取最大值颜色
		if(vv > dValidmax)
		{
			clrRet = clrValidmax;
			break;
		}

		// <= vv
		if(dValue<vv || fabs(dValue-vv)<10.e-12)
		{
			clrRet = item.Rgb();
			//// != vv 取颜色平均值
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

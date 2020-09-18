#include "StdAfx.h"
#include "KPalette.h"


KPalette::KPalette(void) : TPtrCollection<KPaletteItem*>()
{
	SetType(PaletteType_Unknown);
	this->SetName(_T("未知调色板"));

	m_bHasComment = FALSE;
	m_bHasTransparentColor = FALSE;
	m_clrTransparent = RGB(0,0,0);
	m_iSpecificColorNumbers = 0;

	m_dValueMin = 0;
	m_dValueMax = 1;
}

KPalette::~KPalette(void)
{
	this->Clear();
}

// 克隆
KPalette* KPalette::Clone()
{
	// 重新牛一个调色板
	KPalette* pPalette = new KPalette();
	pPalette->SetType(this->GetType());
	int iEntries = this->GetColorEntries();
	for(int i=0;i<iEntries;i++)
	{
		pPalette->Add(this->GetAt(i)->Clone());
	}
	return pPalette;
}

// 添加颜色
BOOL KPalette::AddColor(double dValue, COLORREF color, LPCTSTR pszComment)
{
	return AddColor(VARGB((float)dValue,color), pszComment);
}
// 添加颜色
BOOL KPalette::AddColor(double dValue, BYTE r, BYTE g, BYTE b,LPCTSTR pszComment)
{
	return AddColor(VARGB((float)dValue,r,g,b), pszComment);
}
// 添加颜色
BOOL KPalette::AddColor(double dValue, BYTE a, BYTE r, BYTE g, BYTE b, LPCTSTR pszComment)
{
	return AddColor(VARGB((float)dValue,a,r,g,b), pszComment);
}
// 添加颜色
BOOL KPalette::AddColor(const VARGB& vargb, LPCTSTR pszComment)
{
	TCHAR szComment[64] = _T("");
	for(int i=0;i<64;i++)
	{
		szComment[i] = _T('\0');
	}

	// 值
	double value = vargb.V;

	// 是否有注释
	m_bHasComment = _tcslen(pszComment) > 0 ? TRUE : FALSE;

	if(m_bHasComment)
	{
		_tcsncpy(szComment,pszComment,63);
	}
	else
	{
		m_bHasComment = TRUE;
		_stprintf(szComment,_T("%.2f"),value);
	}

	// 存在则返回
	if(this->FindIndex(value)>=0)
		return FALSE;

	KPaletteItem* pItem = new KPaletteItem(vargb, szComment);
	this->Add(pItem);

	return TRUE;
}

// 查找指定值的索引
int KPalette::FindIndex(double dValue)
{
	int index = 0;

	iterator itr;
	iterator itrbeg = this->begin();
	iterator itrend = this->end();
	for(itr=itrbeg; itr!=itrend; itr++)
	{
		double v = (*itr)->Value();
		if( fabs(dValue - v) < 1.0e-12 )	// 等于
		{
			return index;
		}

		index++;
	}

	return -1;
}
// 指定值的最临近索引
int KPalette::FindNearestIndex(double dValue)
{
	int index = 0;

	iterator itr;
	iterator itrbeg = this->begin();
	iterator itrend = this->end();
	for(itr=itrbeg; itr!=itrend; itr++)
	{
		double v = (*itr)->Value();
		if( dValue < v ||  fabs(dValue - v) < 1.0e-12 )	// 小于等于
		{
			return index;
		}

		index++;
	}

	return -1;
}
// 有效条目数量
// spv_min 是特殊值的最小值, 默认为 0x8000
int KPalette::NumbersOfValidItem(double spv_min)
{
	// 条目总数量
	int iCount = this->Count();

	// 在特殊值前的有效颜色数量
	int numValid = iCount;

	// 计算有效颜色数量
	for(int i=0;i<iCount;i++)
	{
		double value = this->GetAt(i)->Value();
		if(value>spv_min || fabs(value-spv_min)<EPS)	// >=
		{
			numValid = i;
			break;
		}
	}

	return numValid;
}
// 设置数据范围
void KPalette::SetValueRange(double vMin,double vMax)
{
	if(vMin > vMax)
	{
		double temp = vMin;
		vMin = vMax;
		vMax = temp;
	}

	m_dValueMin = vMin;
	m_dValueMax = vMax;
}
// 取得数据范围
void KPalette::GetValueRange(double& vMin,double& vMax)
{
	vMin = m_dValueMin;
	vMax = m_dValueMax;
}

// 颜色
COLORREF KPalette::GetColor(double dValue,COLORREF clrDefault)
{
	COLORREF clrReturn = clrDefault;
	PaletteType ePaletteType = (PaletteType)this->GetType();

	switch(ePaletteType)
	{
	case PaletteType_Standard:
	case PaletteType_Index:
		clrReturn = this->GetIdxColor(dValue,clrDefault);
		break;

	case PaletteType_Rainbow:
		clrReturn = this->GetDitColor(dValue,clrDefault);
		break;

	case PaletteType_Segment:
	case PaletteType_Linear:
		clrReturn = this->GetSegColor(dValue,clrDefault);
		break;
	}

	return clrReturn;
}

	// 颜色
COLORREF KPalette::GetIdxColor(double dValue,COLORREF clrDefault)
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
// 取得分段颜色
COLORREF KPalette::GetSegColor(double dValue,COLORREF clrDefault)
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
		if(vv > dValidmax)
		{
			clrRet = clrValidmax;
			break;
		}
		// <= vv
		if(dValue<vv || fabs(dValue-vv)<1.0e-12)
		{
			//clrRet = item.Rgb();
			clrRet = RGB(item.R,item.G,item.B);
			// != vv 取颜色平均值
			if(fabs(dValue-vv)>1.0e-12 && i>0)
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
// 取得抖动颜色(彩虹颜色)
// 需要预先设置最大、最小值
COLORREF KPalette::GetDitColor(double dValue,COLORREF clrDefault)
{
	if(dValue<m_dValueMin)
		dValue = m_dValueMin;
	if(dValue>m_dValueMax)
		dValue = m_dValueMax;

	return LookupColor( (dValue-m_dValueMin)/(m_dValueMax-m_dValueMin), clrDefault );
}
// 根据pos的位置找出在颜色表中对应的颜色,
// pos为0～1的值
// 由GerColor() 调用
COLORREF KPalette::LookupColor(double value, COLORREF clrDefault)
{
	COLORREF retColor = clrDefault;

	//int iCount = this->Count();
	int iCount = this->NumbersOfValidItem();
	for(int i=0;i<iCount;i++)
	{
		KPaletteItem item_cur =  *(this->GetAt(i));
		double valueCurrent = item_cur.Value();

		// 大于 >
		if(value>valueCurrent)
			continue;

		// 等于 ==
		if(fabs(value-valueCurrent)<1.0e-12)
		{
			retColor = item_cur.Rgb();
			break; 
		}

		// 小于
		if(i==0)
		{
			retColor = item_cur.Rgb();
			break; 
		}
		else
		{
			// i>0
			KPaletteItem item_prv =  *(this->GetAt(i-1));
			double ratio = (value - item_prv.Value()) / (item_cur.Value() - item_prv.Value());

			BYTE rBeg = item_prv.R;
			BYTE gBeg = item_prv.G;
			BYTE bBeg = item_prv.B;

			BYTE rEnd = item_cur.R;
			BYTE gEnd = item_cur.G;
			BYTE bEnd = item_cur.B;

			BYTE r = (BYTE)((rBeg + (rEnd - rBeg) * ratio) + 0.5);
			BYTE g = (BYTE)((gBeg + (gEnd - gBeg) * ratio) + 0.5);
			BYTE b = (BYTE)((bBeg + (bEnd - bBeg) * ratio) + 0.5);

			retColor = RGB(r,g,b);
			break; 
		}

	}//for(i)

	// 返回
	return retColor;
}

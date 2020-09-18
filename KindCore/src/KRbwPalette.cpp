#include "StdAfx.h"
#include "KRbwPalette.h"

KRbwPalette::KRbwPalette(void) : KPalette()
{
	m_dValueMin = 0;
	m_dValueMax = 1;
	this->SetType(PaletteType_Rainbow);
	this->SetName(_T("彩虹调色板"));

	// 默认是彩虹
	this->SetRainbowStyle(RainbowStyle::Default);
}

KRbwPalette::KRbwPalette(RainbowStyle eRainbowStyle) : KPalette()
{
	m_dValueMin = 0;
	m_dValueMax = 1;
	this->SetType(PaletteType_Rainbow);
	this->SetName(_T("彩虹调色板"));		// 线性梯度调色板

	// 默认是彩虹
	this->SetRainbowStyle(eRainbowStyle);
}

KRbwPalette::~KRbwPalette(void)
{
}

// 克隆
KPalette* KRbwPalette::Clone()
{
	// 重新牛一个调色板
	KPalette* pPalette = new KRbwPalette();
	int iEntries = this->GetColorEntries();
	for(int i=0;i<iEntries;i++)
	{
		pPalette->Add(this->GetAt(i)->Clone());
	}
	return pPalette;
}

// 根据数据值取得颜色
COLORREF KRbwPalette::GetColor(double dValue,COLORREF clrDefault)
{
	if(dValue<m_dValueMin) dValue = m_dValueMin;
	if(dValue>m_dValueMax) dValue = m_dValueMax;

	return LookupColor( (dValue-m_dValueMin)/(m_dValueMax-m_dValueMin), clrDefault );
}

// 彩虹类型
void KRbwPalette::SetRainbowStyle(RainbowStyle eStyle)
{
	m_eRainbowStyle = eStyle;
	this->Clear();

	switch(m_eRainbowStyle)
	{
	case Rainbow:
		// 采用 Rey G.Biv 的彩虹
		AddColor(0.00,RGB(255,  0,  0));
		AddColor(0.17,RGB(255,165,  0));
		AddColor(0.33,RGB(255,255,  0));
		AddColor(0.50,RGB(  0,255,  0));
		AddColor(0.67,RGB(  0,  0,255));
		AddColor(0.84,RGB( 75,  0,130));
		AddColor(1.00,RGB(238,130,238));
		break;

	case InverseRainbow:
		// 采用 Rey G.Biv 的彩虹(逆)
		AddColor(0.00,RGB(238,130,238));
		AddColor(0.17,RGB( 75,  0,130));
		AddColor(0.33,RGB(  0,  0,255));
		AddColor(0.50,RGB(  0,255,  0));
		AddColor(0.67,RGB(255,255,  0));
		AddColor(0.84,RGB(255,165,  0));
		AddColor(1.00,RGB(255,  0,  0));
		break;

	case GrADS:
		// 类似 GrADS 颜色
		AddColor(0.00,RGB(150, 90,250));
		AddColor(0.20,RGB(  0,200,200));
		AddColor(0.40,RGB(180,220,100));
		AddColor(0.60,RGB(230,120, 50));
		AddColor(0.80,RGB(200, 60, 90));
		AddColor(1.00,RGB(253,  0,  0));

	case GreenToRed:
		// 绿色过度到红色
		AddColor(0.00,RGB(  0,166,  0));
		AddColor(0.20,RGB(  0,255,  0));
		AddColor(0.40,RGB(150,255,  0));
		AddColor(0.60,RGB(255,238,  0));
		AddColor(0.80,RGB(255, 97,  0));
		AddColor(1.00,RGB(255,  0,  0));
		//AddColor(1.00,RGB(206,  0,  0));
		break; 

	default:
		// 采用 Rey G.Biv 的彩虹
		AddColor(0.00,RGB(255,  0,  0));
		AddColor(0.17,RGB(255,165,  0));
		AddColor(0.33,RGB(255,255,  0));
		AddColor(0.50,RGB(  0,255,  0));
		AddColor(0.67,RGB(  0,  0,255));
		AddColor(0.84,RGB( 75,  0,130));
		AddColor(1.00,RGB(238,130,238));
		break;
	}
}

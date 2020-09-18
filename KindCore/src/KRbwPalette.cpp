#include "StdAfx.h"
#include "KRbwPalette.h"

KRbwPalette::KRbwPalette(void) : KPalette()
{
	m_dValueMin = 0;
	m_dValueMax = 1;
	this->SetType(PaletteType_Rainbow);
	this->SetName(_T("�ʺ��ɫ��"));

	// Ĭ���ǲʺ�
	this->SetRainbowStyle(RainbowStyle::Default);
}

KRbwPalette::KRbwPalette(RainbowStyle eRainbowStyle) : KPalette()
{
	m_dValueMin = 0;
	m_dValueMax = 1;
	this->SetType(PaletteType_Rainbow);
	this->SetName(_T("�ʺ��ɫ��"));		// �����ݶȵ�ɫ��

	// Ĭ���ǲʺ�
	this->SetRainbowStyle(eRainbowStyle);
}

KRbwPalette::~KRbwPalette(void)
{
}

// ��¡
KPalette* KRbwPalette::Clone()
{
	// ����ţһ����ɫ��
	KPalette* pPalette = new KRbwPalette();
	int iEntries = this->GetColorEntries();
	for(int i=0;i<iEntries;i++)
	{
		pPalette->Add(this->GetAt(i)->Clone());
	}
	return pPalette;
}

// ��������ֵȡ����ɫ
COLORREF KRbwPalette::GetColor(double dValue,COLORREF clrDefault)
{
	if(dValue<m_dValueMin) dValue = m_dValueMin;
	if(dValue>m_dValueMax) dValue = m_dValueMax;

	return LookupColor( (dValue-m_dValueMin)/(m_dValueMax-m_dValueMin), clrDefault );
}

// �ʺ�����
void KRbwPalette::SetRainbowStyle(RainbowStyle eStyle)
{
	m_eRainbowStyle = eStyle;
	this->Clear();

	switch(m_eRainbowStyle)
	{
	case Rainbow:
		// ���� Rey G.Biv �Ĳʺ�
		AddColor(0.00,RGB(255,  0,  0));
		AddColor(0.17,RGB(255,165,  0));
		AddColor(0.33,RGB(255,255,  0));
		AddColor(0.50,RGB(  0,255,  0));
		AddColor(0.67,RGB(  0,  0,255));
		AddColor(0.84,RGB( 75,  0,130));
		AddColor(1.00,RGB(238,130,238));
		break;

	case InverseRainbow:
		// ���� Rey G.Biv �Ĳʺ�(��)
		AddColor(0.00,RGB(238,130,238));
		AddColor(0.17,RGB( 75,  0,130));
		AddColor(0.33,RGB(  0,  0,255));
		AddColor(0.50,RGB(  0,255,  0));
		AddColor(0.67,RGB(255,255,  0));
		AddColor(0.84,RGB(255,165,  0));
		AddColor(1.00,RGB(255,  0,  0));
		break;

	case GrADS:
		// ���� GrADS ��ɫ
		AddColor(0.00,RGB(150, 90,250));
		AddColor(0.20,RGB(  0,200,200));
		AddColor(0.40,RGB(180,220,100));
		AddColor(0.60,RGB(230,120, 50));
		AddColor(0.80,RGB(200, 60, 90));
		AddColor(1.00,RGB(253,  0,  0));

	case GreenToRed:
		// ��ɫ���ȵ���ɫ
		AddColor(0.00,RGB(  0,166,  0));
		AddColor(0.20,RGB(  0,255,  0));
		AddColor(0.40,RGB(150,255,  0));
		AddColor(0.60,RGB(255,238,  0));
		AddColor(0.80,RGB(255, 97,  0));
		AddColor(1.00,RGB(255,  0,  0));
		//AddColor(1.00,RGB(206,  0,  0));
		break; 

	default:
		// ���� Rey G.Biv �Ĳʺ�
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

#include "StdAfx.h"
#include "KDiamondData.h"


KDiamondData::KDiamondData(void)
{
}


KDiamondData::~KDiamondData(void)
{
}

// ������ɫ��
// �ȹ���ʺ��ɫ��
// Ȼ����ݲ�Ʒ����ѡ���ɫ���ļ�
// �����ɫ���ļ�������ʹ�������ݹ����ɫ��
void KDiamondData::BuildPalette(LPCTSTR lpszFile /* = NULL */)
{
	if(IsEmpty())
		return;

	// ������Ϣ
	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return;

	// ɾ��ԭ�е�ɫ��
	_delete(m_pPalette);

	// ��Ʒ����
	// 0 - ��ʾʹ�òʺ��ɫ��
	// x - ʹ�÷ֶε�ɫ��(���ⲿ����)
	int iProductCode = pDataInfo->ProductCode();

	// �ʺ��ɫ��
	KSegPalette* pPalette = new KSegPalette();
	m_pPalette = pPalette;

	// 1.���òʺ���ɫ����ɫ��
	// �ʺ��ɫ�� RainbowStyle
	//KRbwPalette rbwPalette;
	//KRbwPalette rbwPalette(KRbwPalette::RainbowStyle::InverseRainbow);
	KRbwPalette rbwPalette(KRbwPalette::GreenToRed);
	rbwPalette.SetValueRange(pDataInfo->ExtremumValueMin(),pDataInfo->ExtremumValueMax());

	// ��ֵ������
	int iContourNums = pDataInfo->ContourNumbers();
	if(iContourNums>=1 && iContourNums<=99)
	{
		for(int i=0;i<pDataInfo->wContourNums;i++)
		{
			double value = pDataInfo->dContourValue[i];
			COLORREF clr = rbwPalette.GetColor(value);
			pPalette->AddColor(value,clr);
		}
	}
	else if(iContourNums == DIAM_FMT_CVID_CONTOUR)
	{
		double vInterval = pDataInfo->ContourInterval();	// ��ֵ�߼��
		double vmin = pDataInfo->ContourMin();				// ��ֵ����Сֵ
		double vmax = pDataInfo->ContourMax();				// ��ֵ�����ֵ
		for(double value = vmin; value < vmax+0.000001; value += vInterval)
		{
			if(value<pDataInfo->vExtremumMin || value>pDataInfo->vExtremumMax)
				continue;

			COLORREF clr = rbwPalette.GetColor(value);
			pPalette->AddColor(value,clr);
		}
	}

	// 2.�������ɫ���ļ������ļ�����
	if(lpszFile)
	{
		KColortableData clrtbl;
		if(!clrtbl.LoadData(lpszFile))
			return;

		// ���õ�ɫ��Ϊ��ɫ��ĵ�ɫ�屸��
		this->SetPalette(clrtbl.GetPalette()->Clone());
		this->GetPalette()->SetValueRange(pDataInfo->ExtremumValueMin(),pDataInfo->ExtremumValueMax());
	}

	return;
}

//=========================================================
// ��̬���� - beg
//---------------
bool KDiamondData::IsDiamondData(LPCTSTR lpszFile)
{
	int iFormarCode = KDiamondData::GetFormatCode(lpszFile);
	return iFormarCode>0 && iFormarCode<99;	// 1-100
	return false;
}
int KDiamondData::GetFormatCode(LPCTSTR lpszFile)
{
	int iFormatCode = 0;
	TCHAR szTemp[MAX_PATH] = _T("");

	BOOL bIsBinary = KFile::IsBinary(lpszFile);
	if(bIsBinary)	// bin file
	{

	}
	else			// txt file
	{
		FILE* fp = _tfopen(lpszFile,_T("rt"));
		if(fp==NULL)
			return 0;

		int sizeFid = _tcslen(_T("DIAMOND")); 
		_ftscanf(fp,_T("%s"),szTemp);		// �ļ�ͷ��ʾ,Ϊ�ַ��� _T("DIAMOND"),��Сд����
		if( szTemp[sizeFid] != _T('\0') )
		{
			fclose(fp);
			fp = NULL;
			return 0;
		}
		//ת��Ϊ��д
		KString::UpperString(szTemp);
		if( _tcscmp(szTemp,_T("DIAMOND")) != 0 )
		{
			fclose(fp);
			fp = NULL;
			return 0;
		}

		// 2.����Ʒ���ݸ�ʽ����
		_ftscanf(fp,_T("%d"),&iFormatCode);

		fclose(fp);
		fp = NULL;
	}

	return iFormatCode;
}

//---------------
// ��̬���� - end
//=========================================================

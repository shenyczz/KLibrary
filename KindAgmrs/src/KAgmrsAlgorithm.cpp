#include "StdAfx.h"
#include "KAgmrsAlgorithm.h"

namespace Agmrs
{

	KAgmrsAlgorithm::KAgmrsAlgorithm(void)
	{
		m_pData = NULL;
	}

	KAgmrsAlgorithm::~KAgmrsAlgorithm(void)
	{
	}

	// ��֤ LAI ���ã�LID��VIX��EQU��
	// ȡ��VIX���ʽ�Ͷ�Ӧ�ķ��̱��ʽ
	bool KAgmrsAlgorithm::ConfigLai()
	{
		DATETIME* pDateTime= (DATETIME*)(&this->DateTime);
		int iMonth = pDateTime->wMonth;
		int iDay = pDateTime->wDay;

		//iMonth = 5;
		//iDay = 9;

		int iTendays = 1;
		if(iDay<11)			iTendays = 1;
		else if(iDay<21)	iTendays = 2;
		else				iTendays = 3;

		KString s;
		s.Format(_T("%02d%02d"),iMonth,iTendays);
		int i = m_LaiConfig.IndexOf(s);
		if(i<0) 
			return false;

		m_strVix = m_LaiConfig[i+1];
		m_strEqu = m_LaiConfig[i+2];

		return true;
	}

//---------------------------------------------------------
}// namespace Agmrs - end
//---------------------------------------------------------

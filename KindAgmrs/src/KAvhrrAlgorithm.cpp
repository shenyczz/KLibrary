#include "StdAfx.h"
#include "KAvhrrAlgorithm.h"

namespace Agmrs { namespace Avhrr
{

	KAvhrrAlgorithm::KAvhrrAlgorithm(void)
	{
	}

	KAvhrrAlgorithm::~KAvhrrAlgorithm(void)
	{
	}

	/*
	// 归一化植被指数[0,1]
	double KAvhrrAlgorithm::Ndvi()
	{ 
		double dValue = 0;
		//-------------------------------------------------
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;


		WORD wDataCh1 = pwData[1] & 0x0FFF;
		WORD wDataCh2 = pwData[2] & 0x0FFF;

		// 屏蔽无效数据
		if( wDataCh2==0x0FFF || wDataCh1==0x0FFF )
		{
			return 0;
		}

		// 分母不能为 0
		if( 0 == (wDataCh2+wDataCh1) )
		{
			return 0;
		}

		// 计算 NDVI
		dValue = (double)(wDataCh2-wDataCh1) / (wDataCh2+wDataCh1);
		// 把归一化植被指数限制在[0,1]
		dValue = dValue<0 ? 0 : dValue;
		//-------------------------------------------------
		return dValue;
	}

	// 比值植被指数
	double KAvhrrAlgorithm::Rvi()
	{
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		double dValue = 0;

		WORD wDataCh1 = pwData[1] & 0x0FFF;
		WORD wDataCh2 = pwData[2] & 0x0FFF;

		if( wDataCh2== 0x0FFF || wDataCh1== 0x0FFF )
		{// 屏蔽无效数据
			return 0;
		}

		if( wDataCh2 == 0 )
		{// 分母不能为 0
			return 0;
		}

		dValue = (double)wDataCh1 / wDataCh2;
		dValue = dValue<0 ? 0 : dValue;

		return dValue;
	}

	// 差值植被指数
	double KAvhrrAlgorithm::Dvi()
	{ 
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		double dValue = 0;

		WORD wDataCh1 = pwData[1] & 0x0FFF;		wDataCh1 = wDataCh1>=0x0FFE ? 0 : wDataCh1;
		WORD wDataCh2 = pwData[2] & 0x0FFF;		wDataCh2 = wDataCh2>=0x0FFE ? 0 : wDataCh2; 

		if( wDataCh2== 0x0FFF || wDataCh1== 0x0FFF )
		{// 屏蔽无效数据
			return 0;
		}

		if( wDataCh2 == 0 )
		{// 分母不能为 0
			return 0;
		}

		dValue = ((double)wDataCh2 - (double)wDataCh1) / 1000;
		dValue = dValue<0 ? 0 : dValue;

		return dValue;
	}
	*/

//---------------------------------------------------------
}}// namespace Agmrs::Avhrr - end
//---------------------------------------------------------

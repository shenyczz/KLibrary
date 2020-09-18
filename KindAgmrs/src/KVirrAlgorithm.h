/******************************************************************************
 ** KVirrAlgorithm.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KVirrAlgorithm
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: …Í”¿≥Ω.÷£÷› (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

#include "KLDAlgorithm.h"

namespace Agmrs { namespace Virr
{

	class KIND_EXT_CLASS KVirrAlgorithm : public KLDAlgorithm
	{
	public:
		KVirrAlgorithm(void);
		virtual ~KVirrAlgorithm(void);

	public:
		virtual double Lst(int iMethod = 0);
	};

//---------------------------------------------------------
}}// namespace Agmrs::Virr - end
//---------------------------------------------------------


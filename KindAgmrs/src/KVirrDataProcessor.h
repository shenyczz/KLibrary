/******************************************************************************
 ** KVirrDataProcessor.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KVirrDataProcessor
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: ������.֣�� (shenyczz@163.com)
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

#include "KLDFileProcessor.h"

namespace Agmrs { namespace Virr
{

	class KIND_EXT_CLASS KVirrDataProcessor : public KLDFileProcessor
	{
	public:
		KVirrDataProcessor(void);
		virtual ~KVirrDataProcessor(void);

	public:
		// ȡ����Ԫ����
		virtual int GetPixelType(int i,int j);
		// ��Ʒ���� - �ر��¶�
		virtual BOOL Lst(int iMethod);
	};

//---------------------------------------------------------
}}// namespace Agmrs::Virr - end
//---------------------------------------------------------


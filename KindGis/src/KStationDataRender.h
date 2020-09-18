/******************************************************************************
 ** KStationDataRender.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KStationDataRender
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

#include "KRender.h"

namespace Renders
{
	struct RectInfo
	{
		CRect rect;
		TCHAR szStationID[32];
	};

	class KIND_EXT_CLASS KStationDataRender : public KRender
	{

	public:
		KStationDataRender(void);
		virtual ~KStationDataRender(void);

	public:
		virtual void Rendering(HDC hDC, LPVOID lpvData);

	public:
		TPtrCollection<RectInfo*>* GetRectInfoPtrCollection() { return m_pRectInfoPtrCollection; }

	private:
		// ’æµ„√˚≥∆æÿ–Œ
		TPtrCollection<RectInfo*>* m_pRectInfoPtrCollection;
	};

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------

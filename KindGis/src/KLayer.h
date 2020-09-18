/******************************************************************************
 ** KLayer.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KLayer
 **  Version: 
 ** Function: 
 **  Explain: Layer[Render[LayerProperty[Provider[DataObject]]]]
 **           Í¼²ã<-äÖÈ¾Æ÷<-Í¼²ãÊôÐÔ<-Êý¾ÝÌá¹©Õß<-Êý¾Ý¶ÔÏó
 **
 **   Author: ÉêÓÀ³½.Ö£ÖÝ (shenyczz@163.com)
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

//---------------------------------------------------------
// Ìí¼Ó¡¢ÒÆ³ýÍ¼²ã OnAddLayer
#define WM_USER_LAYER_ADD					(WM_USER+1003)
// ÒÆ³ýÍ¼²ã OnRemoveLayer
#define WM_USER_LAYER_REMOVE				(WM_USER+1004)

// Ë¢ÐÂÍ¼²ã´°¿Ú OnRefreshLayerWnd
#define WM_USER_REFRESH_LAYER_WND			(WM_USER+1100)
// Ë¢ÐÂÊôÐÔ´°¿Ú
#define WM_USER_REFRESH_PROPERTY_WND		(WM_USER+1101)
//---------------------------------------------------------

namespace Layers
{
	enum LayerType : DWORD
	{
		LayerType_Unknown			= 0,

		LayerType_Axin				= 0x1000,
		LayerType_Blackboard,
		LayerType_ImageCtrl,
		LayerType_Lightning,
		LayerType_Grid_Contour,
		LayerType_Grid_Image,
		LayerType_Station,
		LayerType_Wind_Vector,

		LayerType_Diamond_Grid = LayerType_Grid_Contour,

		LayerType_Diamond			= 0x1500,

		LayerType_Agmrs				= 0x2000,
		LayerType_Avhrr,
		LayerType_Modis,
		LayerType_Mersi,
		LayerType_Virr,

		LayerType_Radar				= 0x8000,
		LayerType_Rda,
		LayerType_Rpg,

		LayerType_Shape				= 0x10000,
		LayerType_Misce				= 0x80000000,
	};

	class KIND_EXT_CLASS KLayer : public KObject
	{
	public:
		KLayer(void);
		virtual ~KLayer(void);

	public:
		virtual void Rendering(HDC hDC, LPVOID lpvData);

	public:
		// äÖÈ¾Æ÷
		virtual KRender* GetRender() { return m_pRender; }
		virtual void SetRender(KRender* pRender) { _delete(m_pRender); m_pRender = pRender; }

	public:
		virtual KLayerProperty* GetProperty();
		virtual KProvider* GetProvider();

	protected:
		LPVOID m_lpvData;
		KRender* m_pRender;
	};

//---------------------------------------------------------
}// namespace Layers - end
//---------------------------------------------------------

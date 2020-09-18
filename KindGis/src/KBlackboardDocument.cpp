// KBlackboardDocument.cpp : 实现文件
//

#include "stdafx.h"
#include "KBlackboardDocument.h"

#include "KLayer.h"
#include "KLayerProperty.h"
#include "KBlackboardRender.h"

namespace Framework
{


	// KBlackboardDocument

	IMPLEMENT_DYNCREATE(KBlackboardDocument, KAgmrsDataDocument)

	KBlackboardDocument::KBlackboardDocument()
	{
	}

	BOOL KBlackboardDocument::OnNewDocument()
	{
		if (!KAgmrsDataDocument::OnNewDocument())
			return FALSE;

		return TRUE;
	}

	KBlackboardDocument::~KBlackboardDocument()
	{
	}


	BEGIN_MESSAGE_MAP(KBlackboardDocument, KAgmrsDataDocument)
	END_MESSAGE_MAP()


	// KBlackboardDocument 诊断

	#ifdef _DEBUG
	void KBlackboardDocument::AssertValid() const
	{
		KAgmrsDataDocument::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KBlackboardDocument::Dump(CDumpContext& dc) const
	{
		KAgmrsDataDocument::Dump(dc);
	}
	#endif
	#endif //_DEBUG

	#ifndef _WIN32_WCE
	// KBlackboardDocument 序列化

	void KBlackboardDocument::Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{
		}
		else
		{
			// TODO: 在此添加加载代码
		}
	}
	#endif


	// KBlackboardDocument 命令

	// 添加空黑板图层
	BOOL KBlackboardDocument::AddEmptyBlackboardLayer(KLayerProperty* pProperty)
	{
		// 1. 数据提供者
		KProvider* pProvider = NULL;
		try
		{
			pProvider = new KBlackboardProvider(NULL);
		}
		catch (...)
		{
			_delete(pProvider);
			return false;
		}

		// 2.图层属性(伴随数据提供者)
		KLayerProperty* pLayerProperty = new KLayerProperty();
		pLayerProperty->AllowDelete(false);
		pLayerProperty->AllowMove(false);
		if(pProperty)
		{
			*pLayerProperty = *(KLayerProperty*)pProperty;
		}
		// 设置图层名称
		if(pLayerProperty->GetName().IsEmpty())
		{
			pLayerProperty->SetName(pProvider->GetComment());
		}
		// 设置图层数据提供者
		pLayerProperty->SetProvider(pProvider);

		// 3.渲染器(伴随图层属性)
		KRender* pRender = new KBlackboardRender();
		pRender->SetLayerProperty(pLayerProperty);

		// 4.图层(伴随渲染器)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_Blackboard);

		// 添加图层对象
		return this->AddLayer(pLayer);
	}

	///============================================================================
	///黑板操作 - Beg
	///---------------
	// 添加点
	BOOL KBlackboardDocument::AddPoint(LPVOID lpvData)
	{
		// 地图控件
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// 默认黑板图层
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// 数据提供者
		KProvider* pProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// 数据对象
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		// 定位
		KMouseListener* pMouseListener = pMapEngine->GetCurrentMouseListener();
		if(!pMouseListener)
			return false;

		POINT point = pMouseListener->GetLastPoint();
		double x = point.x;
		double y = point.y;
		pMapEngine->ClientToWorld(x,y);

		// 添加对象
		KPoint* pPoint = new KPoint(x,y);
		pPoint->SetExtents(KExtents(x,y,x,y));
		pBlackboardData->AddObject(pPoint);

		return true;
	}
	// 添加线段
	BOOL KBlackboardDocument::AddLine(LPVOID lpvData)
	{
		// 地图控件
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// 黑板图层
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// 数据提供者
		KProvider* pProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// 数据对象
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		// 定位
		KMouseListener* pMouseListener = pMapEngine->GetCurrentMouseListener();

		POINT point1 = pMouseListener->GetLButtonPoint();
		POINT point2 = pMouseListener->GetLastPoint();

		double x1 = point1.x;
		double y1 = point1.y;

		double x2 = point2.x;
		double y2 = point2.y;

		int dx = (int)fabs(x2-x1);
		int dy = (int)fabs(y2-y1);
		if(dx<5 && dy<5)
		{
			::MessageBox(pMapEngine->GetHwnd(),_T("线条太短!"),_T("信息"),MB_OK|MB_ICONINFORMATION);
			return false;
		}

		pMapEngine->ClientToWorld(x1,y1);
		pMapEngine->ClientToWorld(x2,y2);


		KLine* pLine = new KLine(x1,y1,x2,y2);
		pLine->SetExtents(KExtents(min(x1,x2),min(y1,y2),max(x1,x2),max(y1,y2)));

		pBlackboardData->AddObject(pLine);

		return true;
	}
	// 添加矩形
	BOOL KBlackboardDocument::AddRect(LPVOID lpvData)
	{
		// 地图控件
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// 黑板图层
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// 数据提供者
		KProvider* pProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// 数据对象
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		// 定位
		KMouseListener* pMouseListener = pMapEngine->GetCurrentMouseListener();

		POINT point1 = pMouseListener->GetLButtonPoint();
		POINT point2 = pMouseListener->GetLastPoint();

		double x1 = point1.x;
		double y1 = point1.y;

		double x2 = point2.x;
		double y2 = point2.y;

		int dx = (int)fabs(x2-x1);
		int dy = (int)fabs(y2-y1);
		if(dx<5 && dy<5)
		{
			::MessageBox(pMapEngine->GetHwnd(),_T("矩形太小!"),_T("信息"),MB_OK|MB_ICONINFORMATION);
			return false;
		}

		double l = min(x1,x2);
		double t = min(y1,y2);
		double r = max(x1,x2);
		double b = max(y1,y2);

		pMapEngine->ClientToWorld(l,t);
		pMapEngine->ClientToWorld(r,b);

		KRect* pRect = new KRect(l,t,fabs(r-l),fabs(b-t));
		pRect->SetExtents(KExtents(l,b,r,t));

		pBlackboardData->AddObject(pRect);

		return true;
	}
	// 添加椭圆
	BOOL KBlackboardDocument::AddEllipse(LPVOID lpvData)
	{
		// 地图控件
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// 黑板图层
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// 数据提供者
		KProvider* pProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// 数据对象
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		// 定位
		KMouseListener* pMouseListener = pMapEngine->GetCurrentMouseListener();

		POINT point1 = pMouseListener->GetLButtonPoint();
		POINT point2 = pMouseListener->GetLastPoint();

		double x1 = point1.x;
		double y1 = point1.y;

		double x2 = point2.x;
		double y2 = point2.y;

		int dx = (int)fabs(x2-x1);
		int dy = (int)fabs(y2-y1);
		if(dx<5 && dy<5)
		{
			::MessageBox(pMapEngine->GetHwnd(),_T("椭圆太小!"),_T("信息"),MB_OK|MB_ICONINFORMATION);
			return false;
		}

		double l = min(x1,x2);
		double t = min(y1,y2);
		double r = max(x1,x2);
		double b = max(y1,y2);

		pMapEngine->ClientToWorld(l,t);
		pMapEngine->ClientToWorld(r,b);

		KEllipse* pEllipse = new KEllipse(l,t,fabs(r-l),fabs(b-t));
		pEllipse->SetExtents(KExtents(l,b,r,t));

		pBlackboardData->AddObject(pEllipse);

		return true;
	}
	// 添加折线
	BOOL KBlackboardDocument::AddPolyline(LPVOID lpvData)
	{
		int i;

		// 地图控件
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// 黑板图层
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// 数据提供者
		KProvider* pProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// 数据对象
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		// 定位
		KMouseListener* pMouseListener = pMapEngine->GetCurrentMouseListener();

		vector<POINT>* pPointSequence = pMouseListener->GetPointSequence();
		int iPointCount = pPointSequence->size();
		if(iPointCount<2)
			return false;

		// 取得最大最小坐标
		double xmin(65535),xmax(-1),ymin(65535),ymax(-1);
		for(i=0;i<iPointCount;i++)
		{
			POINT point = pPointSequence->at(i);
			xmin = min(xmin,point.x);
			xmax = max(xmax,point.x);
			ymin = min(ymin,point.y);
			ymax = max(ymax,point.y);
		}

		int dx = (int)fabs(xmax-xmin);
		int dy = (int)fabs(ymax-ymin);
		if(dx<5 && dy<5 && pMouseListener->IsFinishedGeometry())
		{
			::MessageBox(pMapEngine->GetHwnd(),_T("折线跨度太小!"),_T("信息"),MB_OK|MB_ICONINFORMATION);
			return false;
		}

		double l = min(xmin,xmax);
		double t = min(ymin,ymax);
		double r = max(xmin,xmax);
		double b = max(ymin,ymax);

		pMapEngine->ClientToWorld(l,t);
		pMapEngine->ClientToWorld(r,b);

		KPolyline* pPolyline = new KPolyline();
		pPolyline->SetExtents(KExtents(l,b,r,t));

		pPolyline->Clear();
		for(i=0;i<iPointCount;i++)
		{
			POINT point = pPointSequence->at(i);
			double x = point.x;
			double y = point.y;
			pMapEngine->ClientToWorld(x,y);
			pPolyline->Add(new KPoint(x,y));
		}

		// 设置一个标志,在没有绘制完图形之前
		// 要先删除老图形
		DWORD dwFlag = 0x8001;
		pBlackboardData->RemoveObject(pBlackboardData->FindObject(dwFlag));
		pPolyline->SetFlag(dwFlag);

		if(pMouseListener->IsFinishedGeometry())
		{
			pPolyline->SetFlag(0);
			pPointSequence->clear();	// 清除旧坐标点
		}

		pBlackboardData->AddObject(pPolyline);

		return true;
	}
	// 添加多边形
	BOOL KBlackboardDocument::AddPolygon(LPVOID lpvData)
	{
		int i;

		// 地图控件
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// 黑板图层
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// 数据提供者
		KProvider* pProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// 数据对象
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		// 定位
		KMouseListener* pMouseListener = pMapEngine->GetCurrentMouseListener();

		vector<POINT>* pPointSequence = pMouseListener->GetPointSequence();
		int iPointCount = pPointSequence->size();
		if(iPointCount<2)
			return false;

		// 取得最大最小坐标
		double xmin(65535),xmax(-1),ymin(65535),ymax(-1);
		for(i=0;i<iPointCount;i++)
		{
			POINT point = pPointSequence->at(i);
			xmin = min(xmin,point.x);
			xmax = max(xmax,point.x);
			ymin = min(ymin,point.y);
			ymax = max(ymax,point.y);
		}

		int dx = (int)fabs(xmax-xmin);
		int dy = (int)fabs(ymax-ymin);
		if(dx<5 && dy<5 && pMouseListener->IsFinishedGeometry())
		{
			::MessageBox(pMapEngine->GetHwnd(),_T("多边形跨度太小!"),_T("信息"),MB_OK|MB_ICONINFORMATION);
			return false;
		}

		double l = min(xmin,xmax);
		double t = min(ymin,ymax);
		double r = max(xmin,xmax);
		double b = max(ymin,ymax);

		pMapEngine->ClientToWorld(l,t);
		pMapEngine->ClientToWorld(r,b);

		KPolygon* pPolygon = new KPolygon();
		pPolygon->SetExtents(KExtents(l,b,r,t));

		pPolygon->Clear();
		for(i=0;i<iPointCount;i++)
		{
			POINT point = pPointSequence->at(i);
			double x = point.x;
			double y = point.y;
			pMapEngine->ClientToWorld(x,y);
			pPolygon->Add(new KPoint(x,y));
		}

		// 设置一个标志,在没有绘制万图形之前
		// 要先删除老图形
		DWORD dwFlag = 0x8001;
		pBlackboardData->RemoveObject(pBlackboardData->FindObject(dwFlag));
		pPolygon->SetFlag(dwFlag);

		if(pMouseListener->IsFinishedGeometry())
		{
			pPolygon->SetFlag(0);
			pPointSequence->clear();	// 清除旧坐标点
		}

		pBlackboardData->AddObject(pPolygon);

		return true;
	}
	// 添加曲线
	BOOL KBlackboardDocument::AddCurve(LPVOID lpvData)
	{
		int i;

		// 地图控件
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// 黑板图层
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// 数据提供者
		KProvider* pProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// 数据对象
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		// 定位
		KMouseListener* pMouseListener = pMapEngine->GetCurrentMouseListener();

		vector<POINT>* pPointSequence = pMouseListener->GetPointSequence();
		int iPointCount = pPointSequence->size();
		if(iPointCount<2)
			return false;

		// 取得最大最小坐标
		double xmin(65535),xmax(-1),ymin(65535),ymax(-1);
		for(i=0;i<iPointCount;i++)
		{
			POINT point = pPointSequence->at(i);
			xmin = min(xmin,point.x);
			xmax = max(xmax,point.x);
			ymin = min(ymin,point.y);
			ymax = max(ymax,point.y);
		}

		int dx = (int)fabs(xmax-xmin);
		int dy = (int)fabs(ymax-ymin);
		if(dx<5 && dy<5 && pMouseListener->IsFinishedGeometry())
		{
			::MessageBox(pMapEngine->GetHwnd(),_T("折线跨度太小!"),_T("信息"),MB_OK|MB_ICONINFORMATION);
			return false;
		}

		double l = min(xmin,xmax);
		double t = min(ymin,ymax);
		double r = max(xmin,xmax);
		double b = max(ymin,ymax);

		pMapEngine->ClientToWorld(l,t);
		pMapEngine->ClientToWorld(r,b);

		KCurve* pCurve = new KCurve();
		pCurve->SetExtents(KExtents(l,b,r,t));

		pCurve->Clear();
		for(i=0;i<iPointCount;i++)
		{
			POINT point = pPointSequence->at(i);
			double x = point.x;
			double y = point.y;
			pMapEngine->ClientToWorld(x,y);
			pCurve->Add(new KPoint(x,y));
		}

		// 设置一个标志,在没有绘制万图形之前
		// 要先删除老图形
		DWORD dwFlag = 0x8001;
		pBlackboardData->RemoveObject(pBlackboardData->FindObject(dwFlag));
		pCurve->SetFlag(dwFlag);

		if(pMouseListener->IsFinishedGeometry())
		{
			pCurve->SetFlag(0);
			pPointSequence->clear();	// 清除旧坐标点
		}

		pBlackboardData->AddObject(pCurve);

		return true;
	}
	// 添加闭合曲线
	BOOL KBlackboardDocument::AddCloseCurve(LPVOID lpvData)
	{
		return AddCurve(lpvData);
	}
	// 添加标注
	BOOL KBlackboardDocument::AddLabel(LPVOID lpvData)
	{
		// 地图控件
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// 黑板图层
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// 数据提供者
		KProvider* pProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// 数据对象
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		// 定位
		KMouseListener* pMouseListener = pMapEngine->GetCurrentMouseListener();
		POINT point = pMouseListener->GetLastPoint();
		double x = point.x;
		double y = point.y;
		pMapEngine->ClientToWorld(x,y);

		// 默认文本
		TCHAR szLabel[MAX_PATH] = _T("TODO: 修改标注文本");
		//int labelCount = pBlackboardData->GetLabelCount();
		//labelCount = labelCount<=0 ? 0 : labelCount;
		//_stprintf(szLabel,_T("%s-%d"),_T("TODO: 修改标注文本"),labelCount+1);

		// 默认字体
		LOGFONT lfLabel;
		HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
		::GetObject(hFont,sizeof(LOGFONT),&lfLabel);
		lfLabel.lfHeight = -(LONG)(1.0 * 12.0 / 9 * 20);
		_tcscpy(lfLabel.lfFaceName,_T("宋体"));

		// 默认颜色
		COLORREF clrLabel = RGB(0,0,0);

		// 传递过来的标注
		KLabel* pLabel = (KLabel*)lpvData;

		// 新建标注对象
		KLabel* pLabel_new = new KLabel();
		pLabel_new->SetExtents(KExtents(x,y,x,y));
		pLabel_new->SetText(pLabel ? pLabel->GetText() : szLabel);
		pLabel_new->SetLogFont(pLabel ? pLabel->GetLogFont() : lfLabel);
		pLabel_new->SetColor(pLabel ? pLabel->GetColor() : clrLabel);

		pBlackboardData->AddObject(pLabel_new);

		return true;
	}
	// 添加图层图例
	BOOL KBlackboardDocument::AddLayerLegend(KLayer* pLayerCurrent)
	{
		if(!pLayerCurrent)
			return false;

		// 地图引擎
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// 数据提供者
		KProvider* pProvider = pLayerCurrent->GetProvider();
		if(!pProvider)
			return false;

		// 数据对象
		KDataObject* pDataObjectCurrent = pProvider->GetDataObject();
		if(!pDataObjectCurrent)
			return false;

		// 调色板
		KPalette* pPalette = pDataObjectCurrent->GetPalette();
		if(!pPalette)
		{
			::MessageBox(pMapEngine->GetHwnd(),_T("当前选中图层没有图例!"),_T("信息"),MB_OK|MB_ICONINFORMATION);
			return false;
		}

		// 黑板图层
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// 黑板图层数据提供者
		KProvider* pBlackboardProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// 黑板数据对象
		KBlackboardData* pBlackboardData = (KBlackboardData*)pBlackboardProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		KMouseListener* pMouseListener = pMapEngine->GetCurrentMouseListener();
		POINT point = pMouseListener->GetLastPoint();
		double x = point.x;
		double y = point.y;
		pMapEngine->ClientToWorld(x,y);

		// 添加图例
		KLegend* pLegend = new KLegend();
		pLegend->SetOwner(pDataObjectCurrent);
		pLegend->SetExtents(KExtents(x,y,x,y));
		pLegend->SetUnit(((KCustomDataObject*)pDataObjectCurrent)->GetUnit());
		KPalette* p=pPalette->Clone();
		pLegend->SetPalette(p);	// 设置调色板

		pBlackboardData->AddObject(pLegend);

		return TRUE;
	}
	///---------------
	///黑板操作 - End
	///============================================================================

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

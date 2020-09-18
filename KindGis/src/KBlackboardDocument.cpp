// KBlackboardDocument.cpp : ʵ���ļ�
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


	// KBlackboardDocument ���

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
	// KBlackboardDocument ���л�

	void KBlackboardDocument::Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{
		}
		else
		{
			// TODO: �ڴ���Ӽ��ش���
		}
	}
	#endif


	// KBlackboardDocument ����

	// ��ӿպڰ�ͼ��
	BOOL KBlackboardDocument::AddEmptyBlackboardLayer(KLayerProperty* pProperty)
	{
		// 1. �����ṩ��
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

		// 2.ͼ������(���������ṩ��)
		KLayerProperty* pLayerProperty = new KLayerProperty();
		pLayerProperty->AllowDelete(false);
		pLayerProperty->AllowMove(false);
		if(pProperty)
		{
			*pLayerProperty = *(KLayerProperty*)pProperty;
		}
		// ����ͼ������
		if(pLayerProperty->GetName().IsEmpty())
		{
			pLayerProperty->SetName(pProvider->GetComment());
		}
		// ����ͼ�������ṩ��
		pLayerProperty->SetProvider(pProvider);

		// 3.��Ⱦ��(����ͼ������)
		KRender* pRender = new KBlackboardRender();
		pRender->SetLayerProperty(pLayerProperty);

		// 4.ͼ��(������Ⱦ��)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_Blackboard);

		// ���ͼ�����
		return this->AddLayer(pLayer);
	}

	///============================================================================
	///�ڰ���� - Beg
	///---------------
	// ��ӵ�
	BOOL KBlackboardDocument::AddPoint(LPVOID lpvData)
	{
		// ��ͼ�ؼ�
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// Ĭ�Ϻڰ�ͼ��
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// �����ṩ��
		KProvider* pProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// ���ݶ���
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		// ��λ
		KMouseListener* pMouseListener = pMapEngine->GetCurrentMouseListener();
		if(!pMouseListener)
			return false;

		POINT point = pMouseListener->GetLastPoint();
		double x = point.x;
		double y = point.y;
		pMapEngine->ClientToWorld(x,y);

		// ��Ӷ���
		KPoint* pPoint = new KPoint(x,y);
		pPoint->SetExtents(KExtents(x,y,x,y));
		pBlackboardData->AddObject(pPoint);

		return true;
	}
	// ����߶�
	BOOL KBlackboardDocument::AddLine(LPVOID lpvData)
	{
		// ��ͼ�ؼ�
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// �ڰ�ͼ��
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// �����ṩ��
		KProvider* pProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// ���ݶ���
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		// ��λ
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
			::MessageBox(pMapEngine->GetHwnd(),_T("����̫��!"),_T("��Ϣ"),MB_OK|MB_ICONINFORMATION);
			return false;
		}

		pMapEngine->ClientToWorld(x1,y1);
		pMapEngine->ClientToWorld(x2,y2);


		KLine* pLine = new KLine(x1,y1,x2,y2);
		pLine->SetExtents(KExtents(min(x1,x2),min(y1,y2),max(x1,x2),max(y1,y2)));

		pBlackboardData->AddObject(pLine);

		return true;
	}
	// ��Ӿ���
	BOOL KBlackboardDocument::AddRect(LPVOID lpvData)
	{
		// ��ͼ�ؼ�
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// �ڰ�ͼ��
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// �����ṩ��
		KProvider* pProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// ���ݶ���
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		// ��λ
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
			::MessageBox(pMapEngine->GetHwnd(),_T("����̫С!"),_T("��Ϣ"),MB_OK|MB_ICONINFORMATION);
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
	// �����Բ
	BOOL KBlackboardDocument::AddEllipse(LPVOID lpvData)
	{
		// ��ͼ�ؼ�
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// �ڰ�ͼ��
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// �����ṩ��
		KProvider* pProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// ���ݶ���
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		// ��λ
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
			::MessageBox(pMapEngine->GetHwnd(),_T("��Բ̫С!"),_T("��Ϣ"),MB_OK|MB_ICONINFORMATION);
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
	// �������
	BOOL KBlackboardDocument::AddPolyline(LPVOID lpvData)
	{
		int i;

		// ��ͼ�ؼ�
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// �ڰ�ͼ��
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// �����ṩ��
		KProvider* pProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// ���ݶ���
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		// ��λ
		KMouseListener* pMouseListener = pMapEngine->GetCurrentMouseListener();

		vector<POINT>* pPointSequence = pMouseListener->GetPointSequence();
		int iPointCount = pPointSequence->size();
		if(iPointCount<2)
			return false;

		// ȡ�������С����
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
			::MessageBox(pMapEngine->GetHwnd(),_T("���߿��̫С!"),_T("��Ϣ"),MB_OK|MB_ICONINFORMATION);
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

		// ����һ����־,��û�л�����ͼ��֮ǰ
		// Ҫ��ɾ����ͼ��
		DWORD dwFlag = 0x8001;
		pBlackboardData->RemoveObject(pBlackboardData->FindObject(dwFlag));
		pPolyline->SetFlag(dwFlag);

		if(pMouseListener->IsFinishedGeometry())
		{
			pPolyline->SetFlag(0);
			pPointSequence->clear();	// ����������
		}

		pBlackboardData->AddObject(pPolyline);

		return true;
	}
	// ��Ӷ����
	BOOL KBlackboardDocument::AddPolygon(LPVOID lpvData)
	{
		int i;

		// ��ͼ�ؼ�
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// �ڰ�ͼ��
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// �����ṩ��
		KProvider* pProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// ���ݶ���
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		// ��λ
		KMouseListener* pMouseListener = pMapEngine->GetCurrentMouseListener();

		vector<POINT>* pPointSequence = pMouseListener->GetPointSequence();
		int iPointCount = pPointSequence->size();
		if(iPointCount<2)
			return false;

		// ȡ�������С����
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
			::MessageBox(pMapEngine->GetHwnd(),_T("����ο��̫С!"),_T("��Ϣ"),MB_OK|MB_ICONINFORMATION);
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

		// ����һ����־,��û�л�����ͼ��֮ǰ
		// Ҫ��ɾ����ͼ��
		DWORD dwFlag = 0x8001;
		pBlackboardData->RemoveObject(pBlackboardData->FindObject(dwFlag));
		pPolygon->SetFlag(dwFlag);

		if(pMouseListener->IsFinishedGeometry())
		{
			pPolygon->SetFlag(0);
			pPointSequence->clear();	// ����������
		}

		pBlackboardData->AddObject(pPolygon);

		return true;
	}
	// �������
	BOOL KBlackboardDocument::AddCurve(LPVOID lpvData)
	{
		int i;

		// ��ͼ�ؼ�
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// �ڰ�ͼ��
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// �����ṩ��
		KProvider* pProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// ���ݶ���
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		// ��λ
		KMouseListener* pMouseListener = pMapEngine->GetCurrentMouseListener();

		vector<POINT>* pPointSequence = pMouseListener->GetPointSequence();
		int iPointCount = pPointSequence->size();
		if(iPointCount<2)
			return false;

		// ȡ�������С����
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
			::MessageBox(pMapEngine->GetHwnd(),_T("���߿��̫С!"),_T("��Ϣ"),MB_OK|MB_ICONINFORMATION);
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

		// ����һ����־,��û�л�����ͼ��֮ǰ
		// Ҫ��ɾ����ͼ��
		DWORD dwFlag = 0x8001;
		pBlackboardData->RemoveObject(pBlackboardData->FindObject(dwFlag));
		pCurve->SetFlag(dwFlag);

		if(pMouseListener->IsFinishedGeometry())
		{
			pCurve->SetFlag(0);
			pPointSequence->clear();	// ����������
		}

		pBlackboardData->AddObject(pCurve);

		return true;
	}
	// ��ӱպ�����
	BOOL KBlackboardDocument::AddCloseCurve(LPVOID lpvData)
	{
		return AddCurve(lpvData);
	}
	// ��ӱ�ע
	BOOL KBlackboardDocument::AddLabel(LPVOID lpvData)
	{
		// ��ͼ�ؼ�
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// �ڰ�ͼ��
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// �����ṩ��
		KProvider* pProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// ���ݶ���
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		// ��λ
		KMouseListener* pMouseListener = pMapEngine->GetCurrentMouseListener();
		POINT point = pMouseListener->GetLastPoint();
		double x = point.x;
		double y = point.y;
		pMapEngine->ClientToWorld(x,y);

		// Ĭ���ı�
		TCHAR szLabel[MAX_PATH] = _T("TODO: �޸ı�ע�ı�");
		//int labelCount = pBlackboardData->GetLabelCount();
		//labelCount = labelCount<=0 ? 0 : labelCount;
		//_stprintf(szLabel,_T("%s-%d"),_T("TODO: �޸ı�ע�ı�"),labelCount+1);

		// Ĭ������
		LOGFONT lfLabel;
		HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
		::GetObject(hFont,sizeof(LOGFONT),&lfLabel);
		lfLabel.lfHeight = -(LONG)(1.0 * 12.0 / 9 * 20);
		_tcscpy(lfLabel.lfFaceName,_T("����"));

		// Ĭ����ɫ
		COLORREF clrLabel = RGB(0,0,0);

		// ���ݹ����ı�ע
		KLabel* pLabel = (KLabel*)lpvData;

		// �½���ע����
		KLabel* pLabel_new = new KLabel();
		pLabel_new->SetExtents(KExtents(x,y,x,y));
		pLabel_new->SetText(pLabel ? pLabel->GetText() : szLabel);
		pLabel_new->SetLogFont(pLabel ? pLabel->GetLogFont() : lfLabel);
		pLabel_new->SetColor(pLabel ? pLabel->GetColor() : clrLabel);

		pBlackboardData->AddObject(pLabel_new);

		return true;
	}
	// ���ͼ��ͼ��
	BOOL KBlackboardDocument::AddLayerLegend(KLayer* pLayerCurrent)
	{
		if(!pLayerCurrent)
			return false;

		// ��ͼ����
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return false;

		// �����ṩ��
		KProvider* pProvider = pLayerCurrent->GetProvider();
		if(!pProvider)
			return false;

		// ���ݶ���
		KDataObject* pDataObjectCurrent = pProvider->GetDataObject();
		if(!pDataObjectCurrent)
			return false;

		// ��ɫ��
		KPalette* pPalette = pDataObjectCurrent->GetPalette();
		if(!pPalette)
		{
			::MessageBox(pMapEngine->GetHwnd(),_T("��ǰѡ��ͼ��û��ͼ��!"),_T("��Ϣ"),MB_OK|MB_ICONINFORMATION);
			return false;
		}

		// �ڰ�ͼ��
		KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pBlackboardLayer)
			return false;

		// �ڰ�ͼ�������ṩ��
		KProvider* pBlackboardProvider = pBlackboardLayer->GetProvider();
		if(!pProvider)
			return false;

		// �ڰ����ݶ���
		KBlackboardData* pBlackboardData = (KBlackboardData*)pBlackboardProvider->GetDataObject();
		if(!pBlackboardData)
			return false;

		KMouseListener* pMouseListener = pMapEngine->GetCurrentMouseListener();
		POINT point = pMouseListener->GetLastPoint();
		double x = point.x;
		double y = point.y;
		pMapEngine->ClientToWorld(x,y);

		// ���ͼ��
		KLegend* pLegend = new KLegend();
		pLegend->SetOwner(pDataObjectCurrent);
		pLegend->SetExtents(KExtents(x,y,x,y));
		pLegend->SetUnit(((KCustomDataObject*)pDataObjectCurrent)->GetUnit());
		KPalette* p=pPalette->Clone();
		pLegend->SetPalette(p);	// ���õ�ɫ��

		pBlackboardData->AddObject(pLegend);

		return TRUE;
	}
	///---------------
	///�ڰ���� - End
	///============================================================================

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

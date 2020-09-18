// KPropertyDockablePane.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KPropertyDockablePane.h"

#include "KLayer.h"
#include "KGisDocumentBase.h"
#include "KGisViewBase.h"

#include "KCallbackFunc.h"

namespace DockablePanes
{

	// KPropertyDockablePane

	IMPLEMENT_DYNAMIC(KPropertyDockablePane, KDockablePane)

	KPropertyDockablePane::KPropertyDockablePane()
	{
		m_lpvPropertyOwner = NULL;
		m_pMfcPropertyGridCtrl = new CMFCPropertyGridCtrl();
	}

	KPropertyDockablePane::~KPropertyDockablePane()
	{
		_delete(m_pMfcPropertyGridCtrl);
	}


	BEGIN_MESSAGE_MAP(KPropertyDockablePane, KDockablePane)
		// ���Ըı�(ע����Ϣ)
		ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
		// ���� LayerWnd WM_USER_REFRESH_PROPERTY_WND ��Ϣ
		ON_MESSAGE(WM_USER_REFRESH_PROPERTY_WND,OnRefreshPropertyWnd)
	END_MESSAGE_MAP()


	// KPropertyDockablePane ��Ϣ�������

	// ˢ�»��ͼ
	void KPropertyDockablePane::RefreshActiveView()
	{
		// ���
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		CView* pView = pFrame->GetActiveView();
		if(!pView->IsKindOf(RUNTIME_CLASS(KGisViewBase)))
			return;

		// �ĵ�
		KGisDocumentBase* pDoc = (KGisDocumentBase*)pView->GetDocument();
		if(!pDoc)
			return;

		// ��ͼ�ؼ�
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if(!pMapEngine)
			return;

		pMapEngine->EnableRedraw();

   		pFrame->SetActiveView(pView);
		pView->Invalidate(false);

		return;
	}

	// ע����Ϣ AFX_WM_PROPERTY_CHANGED
	afx_msg LRESULT KPropertyDockablePane::OnPropertyChanged(WPARAM wParam,LPARAM lParam)
	{
		CMFCPropertyGridProperty* pMFCPropertyGridProperty = (CMFCPropertyGridProperty*)lParam;
		this->PropertyChanged(pMFCPropertyGridProperty);
		return 0L;
	}
	// ���� LayerWnd WM_USER_REFRESH_PROPERTY_WND ��Ϣ
	sfx_msg LRESULT KPropertyDockablePane::OnRefreshPropertyWnd(WPARAM wParam,LPARAM lParam)
	{
		Refresh((KLayer*)lParam);
		return 0L;
	}

	// ����ͣ�������ڲ��ؼ�
	BOOL KPropertyDockablePane::CreateCtrl()
	{
		CRect rectDummy;
		rectDummy.SetRectEmpty();

		// ������������
		if (m_pMfcPropertyGridCtrl && m_pMfcPropertyGridCtrl->Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
		{
			m_pMfcPropertyGridCtrl->EnableHeaderCtrl(FALSE);
			m_pMfcPropertyGridCtrl->EnableDescriptionArea();
			m_pMfcPropertyGridCtrl->SetVSDotNetLook();
			m_pMfcPropertyGridCtrl->MarkModifiedProperties();
		}
		else
		{
			TRACE0("δ�ܴ�����������\n");
			return FALSE;      // δ�ܴ���
		}

		/*
		// ˢ����������ؼ�
		// NULL - �����Ϣ
		RefreshPropertyWnd(NULL);
		*/

		return TRUE;
	}
	// �������ؼ�����
	void KPropertyDockablePane::SetCtrlFont()
	{
		NONCLIENTMETRICS info;
		info.cbSize = sizeof(info);
		afxGlobalData.GetNonClientMetrics(info);

		LOGFONT lf;
		afxGlobalData.fontRegular.GetLogFont(&lf);

		lf.lfHeight = info.lfMenuFont.lfHeight;
		lf.lfWeight = info.lfMenuFont.lfWeight;
		lf.lfItalic = info.lfMenuFont.lfItalic;

		m_Font.DeleteObject();
		m_Font.CreateFontIndirect(&lf);
		if(m_pMfcPropertyGridCtrl)
		{
			m_pMfcPropertyGridCtrl->SetFont(&m_Font);
		}
	}
	// ��������
	void KPropertyDockablePane::AdjustLayout()
	{
		KDockablePane::AdjustLayout();

		if (this->GetSafeHwnd() == NULL)
			return;

		CRect rectClient;
		this->GetClientRect(rectClient);

		m_pMfcPropertyGridCtrl->SetWindowPos(
			NULL,
			rectClient.left, 
			rectClient.top, 
			rectClient.Width(),
			rectClient.Height(), 
			SWP_NOACTIVATE | SWP_NOZORDER);
	}
	// ˢ��ͣ������(��Ҫʱ������������)
	void KPropertyDockablePane::Refresh(LPVOID lpvData, BOOL bForceRefresh)
	{
		// �������ͬ�����Ҳ�ǿ��ˢ��
		if(m_lpvPropertyOwner == lpvData && !bForceRefresh)
			return;

		// ����Ϊ��ʱ�������������Ϣ
		if(lpvData==NULL)
		{
			// �����������
			lpvData = NULL;
			m_pMfcPropertyGridCtrl->RemoveAll();
			m_pMfcPropertyGridCtrl->Invalidate(TRUE);
			return;
		}

		// ��ͬ�������Ի���ǿ��ˢ��
		if(m_lpvPropertyOwner != lpvData || bForceRefresh)
		{
			// �����¶���ָ��,������о�����
			m_lpvPropertyOwner = lpvData;
			m_pMfcPropertyGridCtrl->RemoveAll();
			m_pMfcPropertyGridCtrl->Invalidate(TRUE);
		}

		// ���������
		this->AddPropertyGroup();
	}

	//=====================================================
	// ��������� - beg
	//-----------------
	void KPropertyDockablePane::AddPropertyGroup()
	{
		// ���������_����
		addPropertyGroup_Misce();
		// �����_����
		addPropertyGroup_Lon();
		// ���������_γ��
		addPropertyGroup_Lat();
		// ���������_���ݳߴ�
		addPropertyGroup_DataSize();
		// ���������_վ����Ϣ
		addPropertyGroup_Station();
		// ���������_������Ϣ
		addPropertyGroup_Font();
		// ���������_ͨ��
		addPropertyGroup_Channel();
		// ���������_������Ϣ
		addPropertyGroup_GeoInfo();
		// ���������_����
		addPropertyGroup_Control();
		// ���������_ͼ��
		addPropertyGroup_Legend();
	}
	// ���������_����
	void KPropertyDockablePane::addPropertyGroup_Misce()
	{
		//-------------------------------------------------
		// ͼ��
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// ͼ������
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;
		//-------------------------------------------------
		CMFCPropertyGridProperty* pGroup_Misce = new CMFCPropertyGridProperty(_T("����"),(DWORD)eMisce);
		{// pGroup_Misce - beg

			CString strName = _T("");				// ����
			CString strProp = _T("");				// ����
			CString strDescr = _T("");				// ����
			CMFCPropertyGridProperty* pProp = NULL;	// ���Զ���

			// 1.����
			strName = _T("����");
			strDescr = _T("ͼ������");
			strProp.Format(_T("%s"),(LPCTSTR)pProvider->GetComment());
			pProp = new CMFCPropertyGridProperty(strName, (_variant_t)strProp, strDescr,(DWORD)eMisce_Name);
			pProp->AllowEdit(FALSE);
			pGroup_Misce->AddSubItem(pProp);
			// 2.����
			strName = _T("����");
			strDescr = _T("��������");
			DATETIME datm = pDataInfo->DateTime();
			COleDateTime dt = COleDateTime(
				datm.wYear,
				datm.wMonth,
				datm.wDay,
				datm.wHour,
				datm.wMinute,
				datm.wSecond
				);
			dt += COleDateTimeSpan(0,8,0,0);	//ת��Ϊ����ʱ
			strProp = (COleDateTime::invalid == dt.GetStatus()) ? _T("---") : dt.Format(_T("%Y-%m-%d %H:%M:%S"));
			pProp = new CMFCPropertyGridProperty(strName, (_variant_t)strProp, strDescr,(DWORD)eMisce_DateTime);
			pProp->AllowEdit(FALSE);
			pGroup_Misce->AddSubItem(pProp);
			// 3.��Χ
			strName = _T("��Χ");
			strDescr = _T("ͼ�㷶Χ");
			KExtents box = pProvider->GetExtents();
			strProp.Format(_T("(%.2f,%.2f) - (%.2f,%.2f)"),box.MinX(),box.MinY(),box.MaxX(),box.MaxY());
			pProp = new CMFCPropertyGridProperty(strName, (_variant_t)strProp, strDescr,(DWORD)eMisce_Extents);
			pProp->AllowEdit(FALSE);
			pGroup_Misce->AddSubItem(pProp);
			//-------------------------------------------------
			// StationLayer - beg
			if(eLayerType == LayerType_Station)
			{
				// ����������
				strName = _T("����������");
				strDescr = _T("ÿ��վ���¼����������������");
				//pProp = new CMFCPropertyGridProperty(_T("����������"), (_variant_t)pDataInfo->wFactorNum, _T("ÿ��վ���¼����������������"));
				pProp = new CMFCPropertyGridProperty(strName, (_variant_t)pDataInfo->ElementNumbers(), strDescr,(DWORD)eMisce_ElementNumber);
				pProp->AllowEdit(FALSE);
				pGroup_Misce->AddSubItem(pProp);
			}
			// StationLayer - end
			//-------------------------------------------------
		}// pGroup_Misce - end
		pGroup_Misce->Expand(TRUE);
		m_pMfcPropertyGridCtrl->AddProperty(pGroup_Misce);
		//-------------------------------------------------

		return;

		/*
		///----------------------------------------------------
		///������(eMisce) - Beg
		///--------------------
		CMFCPropertyGridProperty* pGroup_Misce = new CMFCPropertyGridProperty(_T("����"),(DWORD)eMisce);
		{
			// LightningLayer - beg
			if(eLayerType==KLayer::LayerType::LightningLayer)
			{
				// ��������
				pProp = new CMFCPropertyGridProperty(_T("��������"), (_variant_t)pDataInfo->wTotalNum, _T("��������"));
				pProp->AllowEdit(FALSE);
				pGroup_Misce->AddSubItem(pProp);
			}
			// LightningLayer - end
			//-------------------------------------------------
		}
		pGroup_Misce->Expand(TRUE);
		m_pMfcPropertyGridCtrl->AddProperty(pGroup_Misce);
		///--------------------
		///������(eMisce) - End
		///----------------------------------------------------

		return;
		*/
	}
	// �����_����
	void KPropertyDockablePane::addPropertyGroup_Lon()
	{

	}
	// ���������_γ��
	void KPropertyDockablePane::addPropertyGroup_Lat()
	{

	}
	// ���������_���ݳߴ�
	void KPropertyDockablePane::addPropertyGroup_DataSize()
	{

	}
	// ���������_վ����Ϣ
	void KPropertyDockablePane::addPropertyGroup_Station()
	{

	}
	// ���������_������Ϣ
	void KPropertyDockablePane::addPropertyGroup_Font()
	{
		return;
		//-------------------------------------------------
		// ͼ��
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// ͼ������
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;
		//-------------------------------------------------
		// Ҫ��Shapeͼ�� + ������
		if(eLayerType != LayerType_Shape)
			return;

		KShapeFile* pShapeFile = (KShapeFile*)pDataObject;
		ShapeType eShapeType = (ShapeType)pShapeFile->GetType();
		if(eShapeType != ShapeType_Point)
			return;
		//-------------------------------------------------
		CMFCPropertyGridProperty* pGroup_Font = new CMFCPropertyGridProperty(_T("��ע�ı�"), (DWORD)eControl);
		{// pGroup_Font - beg

			CString strName = _T("");				// ����
			CString strProp = _T("");				// ����
			CString strDescr = _T("");				// ����
			CMFCPropertyGridProperty* pProp = NULL;
			CMFCPropertyGridColorProperty* pColorProp = NULL;
			CMFCPropertyGridFontProperty* pFontProp = NULL;

			// �߼�����
			LOGFONT& lf = pLayerProperty->LogFont();
			COLORREF clrFont = pLayerProperty->FontColor();

			strName = _T("����");
			strDescr = _T("ָ������");
			pFontProp = new CMFCPropertyGridFontProperty(strName,lf,CF_EFFECTS | CF_SCREENFONTS,strDescr,(DWORD)eControl_LogFont,clrFont);
			pGroup_Font->AddSubItem(pFontProp);

		}// pGroup_Font - end
		pGroup_Font->Expand(TRUE);
		m_pMfcPropertyGridCtrl->AddProperty(pGroup_Font);

		return;
	}
	// ���������_ͨ��
	void KPropertyDockablePane::addPropertyGroup_Channel()
	{
		//-------------------------------------------------
		// ͼ��
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// ͼ������
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;

		if(    eLayerType != LayerType_Avhrr
			&& eLayerType != LayerType_Modis
			&& eLayerType != LayerType_Mersi
			&& eLayerType != LayerType_Virr
			)
			return;
		//-------------------------------------------------
		// ����ͨ����(eChannel)
		CMFCPropertyGridProperty* pGroup_Channel = new CMFCPropertyGridProperty(_T("����ͨ��"), (DWORD)eChannel);
		{// pGroup_Channel - beg

			CString strName = _T("");				// ����
			CString strProp = _T("");				// ����
			CString strDescr = _T("");				// ����
			CMFCPropertyGridProperty* pProp = NULL;	// ���Զ���

			KAgmrsData* pAgmrsData = (KAgmrsData*)pDataObject;

			// ȡ����ͨ����
			int chr,chg,chb;
			pAgmrsData->GetDataChannel(&chr,&chg,&chb);
			int chr38 = pAgmrsData->Channel2ChannelCode(chr);
			int chg38 = pAgmrsData->Channel2ChannelCode(chg);
			int chb38 = pAgmrsData->Channel2ChannelCode(chb);

			// ȡ��ͨ��ӳ������
			AgmrsDataChannelMap*  pAgmrsDataChannelMap = pAgmrsData->GetAgmrsDataChannelMap();
			AgmrsDataChannelMap::iterator itr;
			AgmrsDataChannelMap::iterator itrbeg = pAgmrsDataChannelMap->begin();
			AgmrsDataChannelMap::iterator itrend = pAgmrsDataChannelMap->end();

			// ��ɫͨ��
			strName = _T("��ɫͨ��");
			strDescr = _T("ͨ���ں�֮��ɫͨ��");
			strProp = pAgmrsData->ChannelCode2String(chr38);
			pProp = new CMFCPropertyGridProperty(strName, (_variant_t)strProp, strDescr,(DWORD)eRChannel);
			for(itr=itrbeg; itr!=itrend; itr++)
			{
				pProp->AddOption( (*itr).second.szName );
			}
			pProp->AllowEdit(FALSE);
			pGroup_Channel->AddSubItem(pProp);

			// ��ɫͨ��
			strName = _T("��ɫͨ��");
			strDescr = _T("ͨ���ں�֮��ɫͨ��");
			strProp = pAgmrsData->ChannelCode2String(chg38);
			pProp = new CMFCPropertyGridProperty(strName, (_variant_t)strProp, strDescr,(DWORD)eGChannel);
			for(itr=itrbeg; itr!=itrend; itr++)
			{
				pProp->AddOption( (*itr).second.szName );
			}
			pProp->AllowEdit(FALSE);
			pGroup_Channel->AddSubItem(pProp);

			// ��ɫͨ��
			strName = _T("��ɫͨ��");
			strDescr = _T("ͨ���ں�֮��ɫͨ��");
			strProp = pAgmrsData->ChannelCode2String(chb38);
			pProp = new CMFCPropertyGridProperty(strName, (_variant_t)strProp, strDescr,(DWORD)eBChannel);
			for(itr=itrbeg; itr!=itrend; itr++)
			{
				pProp->AddOption( (*itr).second.szName );
			}

			pProp->AllowEdit(FALSE);
			pGroup_Channel->AddSubItem(pProp);

		}// pGroup_Channel - end
		pGroup_Channel->Expand(TRUE);
		m_pMfcPropertyGridCtrl->AddProperty(pGroup_Channel);

		return;
	}
	// ���������_������Ϣ
	void KPropertyDockablePane::addPropertyGroup_GeoInfo()
	{
		// 
	}
	// ���������_����
	void KPropertyDockablePane::addPropertyGroup_Control()
	{
		//-------------------------------------------------
		// ͼ��
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// ͼ������
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;
		//-------------------------------------------------
		CMFCPropertyGridProperty* pGroup_Control = new CMFCPropertyGridProperty(_T("����"),(DWORD)eControl);
		{// pGroup_Control - beg
			//---------------------------------------------
			// ��ӿ��� - ��������
			this->addPropertyGroup_Control_Common(pGroup_Control,pLayer);
			// ��ӿ��� - ShapeLayer ����
			this->addPropertyGroup_Control_ShapeLayer(pGroup_Control,pLayer);
			// ��ӿ��� - StationLayer ����
			this->addPropertyGroup_Control_StationLayer(pGroup_Control,pLayer);
			// ��ӿ��� - GridContourLayer ����
			this->addPropertyGroup_Control_GridContourLayer(pGroup_Control,pLayer);
			// ��ӿ��� - WindVectorLayer ����
			this->addPropertyGroup_Control_WindVectorLayer(pGroup_Control,pLayer);
			// ��ӿ��� - RpgLayer ����
			this->addPropertyGroup_Control_RpgLayer(pGroup_Control,pLayer);
			// ��ӿ��� - BlackboardLayer ����
			this->addPropertyGroup_Control_BlackboardLayer(pGroup_Control,pLayer);
			//---------------------------------------------
		}// pGroup_Control - end
		pGroup_Control->Expand(TRUE);
		m_pMfcPropertyGridCtrl->AddProperty(pGroup_Control);

		return;
	}
	// ���������_ͼ��(��ɫ��)
	void KPropertyDockablePane::addPropertyGroup_Legend()
	{
		//-------------------------------------------------
		// ͼ��
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// ��ɫ��
		KPalette* pPalette = pDataObject->GetPalette();
		//if(!pPalette)
		//	return;

		// ͼ������
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;
		if(eLayerType == LayerType_Blackboard)	// �ڰ�ͼ��
			return;
		if(eLayerType == LayerType_Shape)		// ��ͼͼ��
			return;
		if(eLayerType == LayerType_Modis)		// ң��MODISͼ��
			return;
		// վ��ͼ��(��׷�ٵ�ֵ��)
		if(eLayerType == LayerType_Station && !((KStationData*)pDataObject)->IsTarceContour())
			return;
		//-------------------------------------------------
		CMFCPropertyGridProperty* pGroup_Legend = new CMFCPropertyGridProperty(_T("ͼ��"),(DWORD)eLegend);
		{// pGroup_Legend - beg

			CString strName = _T("");				// ����
			CString strProp = _T("");				// ����
			CString strDescr = _T("");				// ����
			CMFCPropertyGridProperty* pProp = NULL;
			CMFCPropertyGridColorProperty* pColorProp = NULL;
			//---------------------------------------------
			// ����е�ɫ��ֱ��ʹ�õ�ɫ������
			if(pPalette)
			{
				int iColorEntries = pPalette->GetColorEntries();
				for(int i=0;i<iColorEntries;i++)
				{
					// ����
					if(pPalette->HasComment())
						strName.Format(_T("%s"), pPalette->GetAt(i)->GetComment()==NULL ? _T("") : pPalette->GetAt(i)->GetComment());
					else
						strName.Format(_T("%.2f"),pPalette->GetAt(i)->Value());

					strName.Trim();
					if(strName.IsEmpty())
					{
						strName.Format(_T("%.2f"),pPalette->GetAt(i)->Value());
					}

					// ����
					strDescr.Format(_T("%s����ɫ"),strName);
					// ��ɫ
					COLORREF clr = pPalette->GetAt(i)->Rgb();
					// ��ɫ
					pColorProp =  new CMFCPropertyGridColorProperty(strName, clr, (CPalette*)NULL, strDescr, eLegend + i);
					pColorProp->EnableOtherButton(_T("����..."));
					// �ſ���ɫ��
					//pColorProp->Enable(FALSE);
					pColorProp->Enable(TRUE);
					pGroup_Legend->AddSubItem(pColorProp);

				}// next i

			}// if(pPalette)
			else // û�е�ɫ��
			{
				//-----------------------------------------
				// LayerType_Rpg - beg
				if(eLayerType == LayerType_Rpg)
				{
					KRpgData* pRpgData = (KRpgData*)pDataObject;

					int iProductCode = pRpgData->GetProductCode();
					int iDataLevel = pRpgData->GetProductDataLevel(iProductCode);

					RadarInfo* pRadarInfo = pRpgData->GetRadarInfo();
					Legend* pLegend = pRpgData->GetLegend();

					strProp.Format(_T("��λ(%s)"),pRpgData->GetProductUnit(iProductCode));
					pProp =  new CMFCPropertyGridProperty(strProp, (_variant_t)_T("��ɫ"), _T(""),(DWORD)(eLayerProperty_None));
					pProp->AllowEdit(FALSE);
					pGroup_Legend->AddSubItem(pProp);

					for(int i=1;i<iDataLevel;i++)
					{
						// ��ɫ
						strProp.Format(_T("%.0f"),pLegend[i].dValue);
						pColorProp =  new CMFCPropertyGridColorProperty(strProp, pLegend[i].dwColor, (CPalette*)NULL, _T("ָ��Ĭ�ϵ���ɫ"));
						pColorProp->Enable(FALSE);
						pGroup_Legend->AddSubItem(pColorProp);
					}

				}
				// LayerType_Rpg - end
				//-----------------------------------------
				// LayerType_Wind_Vector - beg
				if(eLayerType==LayerType_Wind_Vector)
				{
					// �繳
					strName = _T("�繳��ɫ");		// ����
					strProp = _T("ָ���繳����ɫ");	// ����
					pColorProp =  new CMFCPropertyGridColorProperty(strName, pLayerProperty->BarbColor(), NULL, strProp,(DWORD)(eLegend_WindBarbColor));
					pColorProp->EnableOtherButton(_T("����..."));
					pGroup_Legend->AddSubItem(pColorProp);
					// ��ʸ
					strName = _T("��ʸ��ɫ");		// ����
					strProp = _T("ָ����ʸ����ɫ");	// ����
					pColorProp =  new CMFCPropertyGridColorProperty(strName, pLayerProperty->ArrowColor(), NULL, strProp,(DWORD)(eLegend_WindArrowColor));
					pColorProp->EnableOtherButton(_T("����..."));
					pGroup_Legend->AddSubItem(pColorProp);
					// ��ʸ
					strName = _T("������ɫ");		// ����
					strProp = _T("ָ�����ߵ���ɫ");	// ����
					pColorProp =  new CMFCPropertyGridColorProperty(strName, pLayerProperty->StreamLineColor(), NULL, strProp,(DWORD)(eLegend_WindStreanLineColor));
					pColorProp->EnableOtherButton(_T("����..."));
					pGroup_Legend->AddSubItem(pColorProp);
				}
				// LayerType_Wind_Vector - end
				//-----------------------------------------

			}// û�е�ɫ���ͼ��
		}// pGroup_Legend - end
		pGroup_Legend->Expand(TRUE);
		m_pMfcPropertyGridCtrl->AddProperty(pGroup_Legend);
		//-------------------------------------------------

		/*
		static TCHAR BASED_CODE szFilter[] = _T("Icon Files(*.ico)|*.ico|All Files(*.*)|*.*||");
		apGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Icon"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Specifies the dialog icon")));
		apGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Folder"), _T("c:\\")));
		*/
		return;
	}

	// ���������_���� - ����
	void KPropertyDockablePane::addPropertyGroup_Control_Common(CMFCPropertyGridProperty* pGroupProperty,void* lpvData)
	{
		//-------------------------------------------------
		// ͼ��
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// ͼ������
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		// ����
		if(eLayerType != LayerType_Shape)
		{
			strName = _T("����");
			strDescr = _T("ͼ�������ڼ���������ʾ");
			pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->IsClip()),strDescr,(DWORD)eControl_Clip);
			pGroupProperty->AddSubItem(pProp);
		}
		// ͸����
		if(eLayerType != LayerType_Blackboard)	// �༭ͼ��͸�����ֲ�ͬ�������
		{
			strName = _T("͸����(%)");
			strDescr = _T("����ͼ��͸���ȡ�");
			pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->Transparency()),strDescr,(DWORD)eControl_Transparency);
			{
				pProp->AddOption(_T("0"));
				pProp->AddOption(_T("10"));
				pProp->AddOption(_T("20"));
				pProp->AddOption(_T("30"));
				pProp->AddOption(_T("40"));
				pProp->AddOption(_T("50"));
				pProp->AddOption(_T("60"));
				pProp->AddOption(_T("70"));
				pProp->AddOption(_T("80"));
				pProp->AddOption(_T("90"));
				pProp->AddOption(_T("100"));
			}
			pGroupProperty->AddSubItem(pProp);
		}
		//-------------------------------------------------
		return;
	}
	// ��ӿ��� - ShapeLayer ����
	void KPropertyDockablePane::addPropertyGroup_Control_ShapeLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData)
	{
		//-------------------------------------------------
		// ͼ��
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// ͼ������
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType != LayerType_Shape)
			return;
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		strName = _T("����");
		strDescr = _T("ͼ�������ڼ���������ʾ");
		pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->IsClip()),strDescr,(DWORD)eControl_Clip);
		pGroupProperty->AddSubItem(pProp);

		KShapeFile* pShapeFile = (KShapeFile*)pDataObject;
		ShapeType eShapeType = (ShapeType)pShapeFile->GetType();
		if(eShapeType == ShapeType_Point)	// �� Shape
		{
			// �߼�����
			LOGFONT& lf = pLayerProperty->LogFont();
			COLORREF clrFont = pLayerProperty->FontColor();

			strName = _T("����");
			strDescr = _T("ָ������");
			pProp = new CMFCPropertyGridFontProperty(strName,lf,CF_EFFECTS | CF_SCREENFONTS,strDescr,(DWORD)eControl_LogFont,clrFont);
			pGroupProperty->AddSubItem(pProp);
		}
		else
		{
			// ��ɫ
			strName = _T("��ɫ");
			strDescr = _T("ָ��Ĭ�ϵ���ɫ");
			pColorProp =  new CMFCPropertyGridColorProperty(strName, pLayerProperty->GetColor(), (CPalette*)NULL, strDescr, (DWORD)(eControl_Color));
			pColorProp->EnableOtherButton(_T("����..."));
			pGroupProperty->AddSubItem(pColorProp);

			// �߿�
			strName = _T("�߿�");
			strDescr = _T("���ü���ͼ�ε��߿�");
			pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLayerProperty->LineWidth(), strDescr, (DWORD)(eControl_LineWidth));
			pProp->EnableSpinControl(TRUE,1,50);
			pGroupProperty->AddSubItem(pProp);

			// ����
			strName = _T("����");
			strDescr = _T("���ü���ͼ�ε����͡�");
			pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLayerProperty->LineStyle(), strDescr, (DWORD)(eControl_LineStyle));
			pProp->AllowEdit(FALSE);
			pProp->AddOption(_T("0 _______"));
			pProp->AddOption(_T("1 ------------"));
			pProp->AddOption(_T("2 ............"));
			pProp->AddOption(_T("3 _._._._._"));
			pProp->AddOption(_T("4 _.._.._.._"));
			pGroupProperty->AddSubItem(pProp);
		}
		//-------------------------------------------------
		return;
	}
	// ��ӿ��� - StationLayer ����
	void KPropertyDockablePane::addPropertyGroup_Control_StationLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData)
	{
		//-------------------------------------------------
		// ͼ��
		KLayer* pLayer = (KLayer*)lpvData;
		if(!pLayer)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// ͼ������
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType != LayerType_Station)
			return;
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		// ���ݶ���
		KStationData* pStationData = (KStationData*)pDataObject;

		// ������ɫ
		strName = _T("��ɫ");
		strDescr = _T("ָ��Ĭ�ϵ���ɫ");
		pColorProp =  new CMFCPropertyGridColorProperty(strName, pLayerProperty->GetColor(), NULL, strDescr,(DWORD)(eControl_Color));
		pColorProp->EnableOtherButton(_T("����..."));
		pGroupProperty->AddSubItem(pColorProp);

		// ��ǰ����������
		strName = _T("��ǰ������");
		strDescr = _T("��ʾ��ǰ����������");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)(pStationData->CurrentElementIndex() + 1), strDescr, (DWORD)(eControl_CurrentElementIndex));
		//for(int i=0;i<pStationData->GetElementNumber();i++)
		//{
		//	strProp.Format(_T("%d"),i+1);
		//	pProp->AddOption(strProp);
		//}
		//pProp->AllowEdit(FALSE);
		pProp->EnableSpinControl(TRUE,1,pStationData->GetElementNumber());
		pProp->AllowEdit(FALSE);
		pGroupProperty->AddSubItem(pProp);

		// ��ʾվ������
		strName = _T("��ʾվ������");
		strDescr = _T("��ʾվ������");
		pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->IsShowStationName()),strDescr,(DWORD)eControl_ShowStationName);
		pGroupProperty->AddSubItem(pProp);

		// ��ʾվ������
		strName = _T("��ʾվ������");
		strDescr = _T("��ʾվ������");
		pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->IsShowData() ? true : false),strDescr,(DWORD)eControl_ShowData);
		pGroupProperty->AddSubItem(pProp);

		// �߼�����
		LOGFONT& lf = pLayerProperty->LogFont();
		COLORREF clrFont = pLayerProperty->FontColor();

		strName = _T("����");
		strDescr = _T("ָ������");
		pProp = new CMFCPropertyGridFontProperty(strName,lf,CF_EFFECTS | CF_SCREENFONTS,strDescr,(DWORD)eControl_LogFont,clrFont);
		pGroupProperty->AddSubItem(pProp);

		// ��ֵ��ѡ��
		if(((KStationData*)pDataObject)->IsTarceContour())
		{
			// ���
			strName = _T("ɫ��ͼ");
			strDescr = _T("��ʾɫ��ͼ");
			pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->IsFill() ? true : false),strDescr,(DWORD)eControl_Fill);
			pGroupProperty->AddSubItem(pProp);

			// �ʺ���
			strName = _T("��ʵ�ֵ��");
			strDescr = _T("��ʾ��ʵ�ֵ��");
			pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->IsRainbow() ? true : false),strDescr,(DWORD)eControl_Rainbow);
			pGroupProperty->AddSubItem(pProp);

			// ��ע
			strName = _T("��ע��ֵ��");
			strDescr = _T("��ע��ֵ��ֵ");
			pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->IsLabel() ? true : false),strDescr,(DWORD)eControl_Label);
			pGroupProperty->AddSubItem(pProp);
		}
		//-------------------------------------------------
		return;
	}
	// ��ӿ��� - GridContourLayer ����
	void KPropertyDockablePane::addPropertyGroup_Control_GridContourLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData)
	{
		//-------------------------------------------------
		// ͼ��
		KLayer* pLayer = (KLayer*)lpvData;
		if(!pLayer)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// ͼ������
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType != LayerType_Grid_Contour)
			return;
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		// ͼ�㻭����ɫ
		pColorProp =  new CMFCPropertyGridColorProperty(_T("��ɫ"), pLayerProperty->GetColor(), NULL, _T("ָ��Ĭ�ϵ���ɫ"),(DWORD)(eControl_Color));
		//pColorProp->EnableAutomaticButton(_T("Ĭ��"), RGB(255,255,255));
		pColorProp->EnableOtherButton(_T("����..."));
		pGroupProperty->AddSubItem(pColorProp);

		// �Ƿ����
		pProp = new CMFCPropertyGridProperty(_T("ɫ��ͼ"),(_variant_t)(pLayerProperty->IsFill() ? true : false),_T("��ʾɫ��ͼ"),(DWORD)eControl_Fill);
		pGroupProperty->AddSubItem(pProp);

		// �Ƿ�ʺ���
		pProp = new CMFCPropertyGridProperty(_T("��ʵ�ֵ��"),(_variant_t)(pLayerProperty->IsRainbow() ? true : false),_T("��ʾ��ʵ�ֵ��"),(DWORD)eControl_Rainbow);
		pGroupProperty->AddSubItem(pProp);

		// �Ƿ��ע
		pProp = new CMFCPropertyGridProperty(_T("��ע��ֵ��"),(_variant_t)(pLayerProperty->IsLabel() ? true : false),_T("��ע��ֵ��ֵ"),(DWORD)eControl_Label);
		pGroupProperty->AddSubItem(pProp);

		// �Ƿ���ʾ�������
		pProp = new CMFCPropertyGridProperty(_T("��ʾ�������"),(_variant_t)(pLayerProperty->IsShowData() ? true : false),_T("��ʾ�������"),(DWORD)eControl_ShowData);
		pGroupProperty->AddSubItem(pProp);

		// ��������ϵ��
		strName = _T("����ϵ��");
		strDescr = _T("���ü���ͼ�ε���������ϵ����");
		pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->Tension()),strDescr,(DWORD)eControl_Tension);
		pGroupProperty->AddSubItem(pProp);
		//-------------------------------------------------
		return;
	}
	// ��ӿ��� - WindVectorLayer ����
	void KPropertyDockablePane::addPropertyGroup_Control_WindVectorLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData)
	{
		//-------------------------------------------------
		// ͼ��
		KLayer* pLayer = (KLayer*)lpvData;
		if(!pLayer)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// ͼ������
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType != LayerType_Wind_Vector)
			return;
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		// �繳
		strName = _T("��ʾ�繳");
		strDescr = _T("��ʾ�繳");
		pProp = new CMFCPropertyGridProperty(strName, (_variant_t)(pLayerProperty->IsShowBarb() ? true : false), strDescr,(DWORD)eControl_WindBarb);
		pGroupProperty->AddSubItem(pProp);

		// ��ʸ
		strName = _T("��ʾ��ʸ");
		strDescr = _T("��ʾ��ʸ");
		pProp = new CMFCPropertyGridProperty(strName, (_variant_t)(pLayerProperty->IsShowBarb() ? true : false), strDescr,(DWORD)eControl_WindArrow);
		pGroupProperty->AddSubItem(pProp);

		// ����
		strName = _T("��ʾ����");
		strDescr = _T("��ʾ����");
		pProp = new CMFCPropertyGridProperty(strName, (_variant_t)(pLayerProperty->IsShowBarb() ? true : false), strDescr,(DWORD)eControl_WindStreamLine);
		pGroupProperty->AddSubItem(pProp);

		// �繳����
		strName = _T("�繳����");
		strDescr = _T("�����繳����");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLayerProperty->BarbStep(), strDescr, (DWORD)(eControl_WindBarbStep));
		pProp->AllowEdit(FALSE);
		pProp->EnableSpinControl(TRUE,1,9);
		pGroupProperty->AddSubItem(pProp);

		// ��ʸ����
		strName = _T("��ʸ����");
		strDescr = _T("������ʸ����");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLayerProperty->ArrowStep(), strDescr, (DWORD)(eControl_WindArrowStep));
		pProp->AllowEdit(FALSE);
		pProp->EnableSpinControl(TRUE,1,9);
		pGroupProperty->AddSubItem(pProp);

		// �����ܶ�
		strName = _T("�����ܶ�");
		strDescr = _T("���������ܶ�");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLayerProperty->StreamLineStep(), strDescr, (DWORD)(eControl_WindStreamLineStep));
		pProp->AllowEdit(FALSE);
		pProp->EnableSpinControl(TRUE,1,5);
		pGroupProperty->AddSubItem(pProp);
		//-------------------------------------------------
		return;
	}
	// ��ӿ��� - RpgLayer ����
	void KPropertyDockablePane::addPropertyGroup_Control_RpgLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData)
	{
		//-------------------------------------------------
		// ͼ��
		KLayer* pLayer = (KLayer*)lpvData;
		if(!pLayer)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// ͼ������
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType != LayerType_Rpg)
			return;
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		// ���ݼ���
		int iProductCode = ((KRpgData*)pDataObject)->GetProductCode();
		int iDataLevel = ((KRpgData*)pDataObject)->GetProductDataLevel(iProductCode);
		pProp = new CMFCPropertyGridProperty(_T("���ݼ���"),(_variant_t)iDataLevel,_T("�״����ݷּ�����"),(DWORD)eLayerProperty_None);
		pProp->AllowEdit(FALSE);
		pGroupProperty->AddSubItem(pProp);

		// ��ʾ����
		pProp =  new CMFCPropertyGridProperty(_T("��ʾ����"), (_variant_t)(int)pLayerProperty->ThresholdMin(), _T("�״�������ʾ����"), (DWORD)(eControl_Threshold_min));
		pProp->EnableSpinControl(TRUE,0,iDataLevel-1);
		pGroupProperty->AddSubItem(pProp);

		// ��ʾ����
		pProp =  new CMFCPropertyGridProperty(_T("��ʾ����"), (_variant_t)(int)pLayerProperty->ThresholdMax(), _T("�״�������ʾ����"), (DWORD)(eControl_Threshold_max));
		pProp->EnableSpinControl(TRUE,0,iDataLevel-1);
		pGroupProperty->AddSubItem(pProp);

		// ��ʾ������
		pProp = new CMFCPropertyGridProperty(_T("������"),(_variant_t)pLayerProperty->IsShowCoordinate() ,_T("��ʾ������"),(DWORD)eControl_ShowCoordinate);
		pGroupProperty->AddSubItem(pProp);
		//-------------------------------------------------
		return;
	}
	// ��ӿ��� - BlackboardLayer ����
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData)
	{
		//-------------------------------------------------
		// ͼ��
		KLayer* pLayer = (KLayer*)lpvData;
		if(!pLayer)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// ͼ������
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType != LayerType_Blackboard)
			return;
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		KBlackboardData* pBlackboardData = (KBlackboardData*)pDataObject;
		KGeometry* pGeometry = (KGeometry*)pBlackboardData->GetSelectedObject();
		if(!pGeometry)
			return;
		//-------------------------------------------------
		GeometryType eGeometryType = (GeometryType)pGeometry->GetType();

		// ͸����
		strName = _T("͸����(%)");
		strDescr = _T("���ö���͸���ȡ�");
		pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pGeometry->GetTransparency()),strDescr,(DWORD)eGeometry_Transparency);
		{
			pProp->AddOption(_T("0"));
			pProp->AddOption(_T("10"));
			pProp->AddOption(_T("20"));
			pProp->AddOption(_T("30"));
			pProp->AddOption(_T("40"));
			pProp->AddOption(_T("50"));
			pProp->AddOption(_T("60"));
			pProp->AddOption(_T("70"));
			pProp->AddOption(_T("80"));
			pProp->AddOption(_T("90"));
			pProp->AddOption(_T("100"));
		}
		pGroupProperty->AddSubItem(pProp);

		if(false
			|| eGeometryType == GeometryType_Point		// �߶�
			|| eGeometryType == GeometryType_Line		// �߶�
			|| eGeometryType == GeometryType_Rect		// ����
			|| eGeometryType == GeometryType_Ellipse	// ��Բ
			|| eGeometryType == GeometryType_Polyline	// ����
			|| eGeometryType == GeometryType_Polygon	// �����
			|| eGeometryType == GeometryType_Curve		// ����
			)
		{
			// ��ɫ
			strName = _T("��ɫ");
			strDescr = _T("���ü���ͼ�εĻ�����ɫ��");
			pColorProp =  new CMFCPropertyGridColorProperty(strName, pGeometry->GetColor(), (CPalette*)NULL, strDescr,(DWORD)(eGeometry_Color));
			pColorProp->AllowEdit(FALSE);
			//pColorProp->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
			pColorProp->EnableOtherButton(_T("����..."));
			pGroupProperty->AddSubItem(pColorProp);
		}

		switch(eGeometryType)
		{
		case GeometryType_Label:
			addPropertyGroup_Control_BlackboardLayer_Label(pGroupProperty,pGeometry);
			break;

		case GeometryType_Point:
			addPropertyGroup_Control_BlackboardLayer_Point(pGroupProperty,pGeometry);
			break;

		case GeometryType_Line:
			addPropertyGroup_Control_BlackboardLayer_Line(pGroupProperty,pGeometry);
			break;

		case GeometryType_Rect:
			addPropertyGroup_Control_BlackboardLayer_Rect(pGroupProperty,pGeometry);
			break;

		case GeometryType_Ellipse:
			addPropertyGroup_Control_BlackboardLayer_Ellipse(pGroupProperty,pGeometry);
			break;

		case GeometryType_Polyline:
			addPropertyGroup_Control_BlackboardLayer_Polyline(pGroupProperty,pGeometry);
			break;

		case GeometryType_Polygon:
			addPropertyGroup_Control_BlackboardLayer_Polygon(pGroupProperty,pGeometry);
			break;

		case GeometryType_Curve:
			addPropertyGroup_Control_BlackboardLayer_Curve(pGroupProperty,pGeometry);
			break;

		case GeometryType_Legend:
			addPropertyGroup_Control_BlackboardLayer_Legend(pGroupProperty,pGeometry);
			break;

		default:
			break;
		}
		//-------------------------------------------------
		return;
	}

	//-----------------------------------------------------
	// �༭ͼ����� BlackboardLayer - beg
	//------------------------------------
	// ��ӿ��� - BlackboardLayer �� Point ��������
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Point(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		//-------------------------------------------------
		return;
	}
	// ��ӿ��� - BlackboardLayer �� Line ��������
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Line(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_Common(pGroupProperty,pGeometry);
		//-------------------------------------------------
		return;
	}
	// ��ӿ��� - BlackboardLayer �� Rect ��������
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Rect(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_Common(pGroupProperty,pGeometry);
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_CanFill(pGroupProperty,pGeometry);
		//-------------------------------------------------
		return;
	}
	// ��ӿ��� - BlackboardLayer �� Ellipse ��������
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Ellipse(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_Common(pGroupProperty,pGeometry);
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_CanFill(pGroupProperty,pGeometry);
		//-------------------------------------------------
		return;
	}
	// ��ӿ��� - BlackboardLayer �� Polyline ��������
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Polyline(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_Common(pGroupProperty,pGeometry);
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_CanFill(pGroupProperty,pGeometry);
		//-------------------------------------------------
		return;
	}
	// ��ӿ��� - BlackboardLayer �� Polygon ��������
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Polygon(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_Common(pGroupProperty,pGeometry);
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_CanFill(pGroupProperty,pGeometry);
		//-------------------------------------------------
		return;
	}
	// ��ӿ��� - BlackboardLayer �� Curve ��������
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Curve(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_Common(pGroupProperty,pGeometry);
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_CanFill(pGroupProperty,pGeometry);
		//-------------------------------------------------
		// ����ϵ��
		strName = _T("����ϵ��");
		strDescr = _T("���ü���ͼ�ε�����ϵ����");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)(float)((KCurve*)pGeometry)->GetTension(), strDescr, (DWORD)(eGeometry_Tension));
		pGroupProperty->AddSubItem(pProp);
		//-------------------------------------------------
		return;
	}
	// ���������_������Ϣ(�ڰ�)
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Label(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		KLabel* pLabel = (KLabel*)pGeometry;
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridFontProperty* pFontProp = NULL;		// ���Զ��� - ����
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		// �ı�
		strName = _T("�ı�");
		strDescr = _T("�ı��ַ���");
		pProp = new CMFCPropertyGridProperty(strName, (_variant_t)(pLabel ? (LPCTSTR)pLabel->GetText() : _T("")), strDescr, (DWORD)eGeometry_Label);
		pGroupProperty->AddSubItem(pProp);

		// �߼�����
		LOGFONT lf;
		COLORREF clrFont = 0;
		HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
		::GetObject(hFont,sizeof(LOGFONT),&lf);
		if(pLabel)
		{
			lf = pLabel->GetLogFont();
			clrFont = pLabel->GetColor();
		}
		strName = _T("����");
		strDescr = _T("ָ������");
		pFontProp = new CMFCPropertyGridFontProperty(strName,lf,CF_EFFECTS | CF_SCREENFONTS,strDescr,(DWORD)eGeometry_LogFont,clrFont);
		pGroupProperty->AddSubItem(pFontProp);
		//-------------------------------------------------
		return;
	}
	// ��ӿ��� - BlackboardLayer �� Lengend ��������
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Legend(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		KLegend* pLegend = (KLegend*)pGeometry;
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		// ͼ�����
		strName = _T("ͼ�����");
		strDescr = _T("����ͼ�����");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLegend->Width, strDescr, (DWORD)(eGeometry_Legend_Width));
		pGroupProperty->AddSubItem(pProp);

		// ͼ���߶�
		strName = _T("ͼ���߶�");
		strDescr = _T("����ͼ���߶�");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)_T("�Զ�����"), strDescr, (DWORD)(eGeometry_Legend_Height));
		pProp->AllowEdit(FALSE);
		pGroupProperty->AddSubItem(pProp);

		// ҳ�߿հ�
		strName = _T("ҳ�߿հ�");
		strDescr = _T("����ҳ�߿հ�");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLegend->Margin, strDescr, (DWORD)(eGeometry_Legend_Margin));
		pGroupProperty->AddSubItem(pProp);

		// ͼ����Ŀ���
		strName = _T("ͼ����Ŀ���");
		strDescr = _T("����ͼ����Ŀ���");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLegend->ItemWidth, strDescr, (DWORD)(eGeometry_Legend_ItemWidth));
		pGroupProperty->AddSubItem(pProp);

		// ͼ����Ŀ�߶�
		strName = _T("ͼ����Ŀ�߶�");
		strDescr = _T("����ͼ����Ŀ�߶�");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLegend->ItemHeight, strDescr, (DWORD)(eGeometry_Legend_ItemHeight));
		pGroupProperty->AddSubItem(pProp);

		// ͼ����Ŀ��϶
		strName = _T("ͼ����Ŀ��϶");
		strDescr = _T("����ͼ����Ŀ��϶");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLegend->ItemGap, strDescr, (DWORD)(eGeometry_Legend_ItemGap));
		pGroupProperty->AddSubItem(pProp);

		// ��ע��λ���
		strName = _T("��ע��λ���");
		strDescr = _T("������ע��λ���");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLegend->UnitWidth, strDescr, (DWORD)(eGeometry_Legend_UnitWidth));
		pGroupProperty->AddSubItem(pProp);
		//-------------------------------------------------
		return;
	}
	// ��ӿ��� - BlackboardLayer �� LinearGeometry_Common ��������
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_LinearGeometry_Common(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		KPoint* pPoint = (KPoint*)pGeometry;
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		// �߿�
		strName = _T("�߿�");
		strDescr = _T("���ü���ͼ�ε��߿�");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pGeometry->GetLineWidth(), strDescr, (DWORD)(eGeometry_LineWidth));
		pProp->EnableSpinControl(TRUE,1,50);
		pGroupProperty->AddSubItem(pProp);

		// ����
		strName = _T("����");
		strDescr = _T("���ü���ͼ�ε����͡�");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pGeometry->GetLineStyle(), strDescr, (DWORD)(eGeometry_LineStyle));
		pProp->AllowEdit(FALSE);
		pProp->AddOption(_T("0 _______"));
		pProp->AddOption(_T("1 ------------"));
		pProp->AddOption(_T("2 ............"));
		pProp->AddOption(_T("3 _._._._._"));
		pProp->AddOption(_T("4 _.._.._.._"));
		pGroupProperty->AddSubItem(pProp);
		//-------------------------------------------------
		return;
	}
	// ��ӿ��� - BlackboardLayer �� LinearGeometry_CanFill ��������
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_LinearGeometry_CanFill(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		CString strName = _T("");							// ����
		CString strProp = _T("");							// ����
		CString strDescr = _T("");							// ����
		CMFCPropertyGridProperty* pProp = NULL;				// ���Զ��� - һ��
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// ���Զ��� - ��ɫ
		//-------------------------------------------------
		// ������� -1-�����  0-��ɫ���  1-Ӱ�����
		strName = _T("���");
		strDescr = _T("���ü���ͼ�ε�������͡�");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pGeometry->GetFillType(), strDescr, (DWORD)(eGeometry_FillType));
		pProp->AllowEdit(FALSE);
		pProp->AddOption(_T("-1 �����"));
		pProp->AddOption(_T(" 0 ��ɫ"));
		pProp->AddOption(_T(" 1 Ӱ��"));
		pGroupProperty->AddSubItem(pProp);

		// �����ɫ
		strName = _T("�����ɫ");
		strDescr = _T("���ü���ͼ�ε������ɫ��");
		pColorProp =  new CMFCPropertyGridColorProperty(strName, pGeometry->GetFillColor(), (CPalette*)NULL, strDescr,(DWORD)(eGeometry_FillColor));
		pColorProp->AllowEdit(FALSE);
		//pColorProp->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
		pColorProp->EnableOtherButton(_T("����..."));
		pGroupProperty->AddSubItem(pColorProp);

		// Ӱ�߷��
		strName = _T("Ӱ�߷��");
		strDescr = _T("���ü���ͼ�ε����Ӱ�߷��");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pGeometry->GetHatchStyle(), strDescr, (DWORD)(eGeometry_HatchStyle));
		pProp->AllowEdit(FALSE);
		pProp->AddOption(_T(" 0 HatchStyleHorizontal"));
		pProp->AddOption(_T(" 1 HatchStyleVertical"));
		pProp->AddOption(_T(" 2 HatchStyleForwardDiagonal"));
		pProp->AddOption(_T(" 3 HatchStyleBackwardDiagonal"));
		pProp->AddOption(_T(" 4 HatchStyleCross"));
		pProp->AddOption(_T(" 5 HatchStyleDiagonalCross"));
		pProp->AddOption(_T(" 6 HatchStyle05Percent"));
		pProp->AddOption(_T(" 7 HatchStyle10Percent"));
		pProp->AddOption(_T(" 8 HatchStyle20Percent"));
		pProp->AddOption(_T(" 9 HatchStyle25Percent"));
		pProp->AddOption(_T("10 HatchStyle30Percent"));
		pProp->AddOption(_T("11 HatchStyle40Percent"));
		pProp->AddOption(_T("12 HatchStyle50Percent"));
		pProp->AddOption(_T("13 HatchStyle60Percent"));
		pProp->AddOption(_T("14 HatchStyle70Percent"));
		pProp->AddOption(_T("15 HatchStyle75Percent"));
		pProp->AddOption(_T("16 HatchStyle80Percent"));
		pProp->AddOption(_T("17 HatchStyle90Percent"));
		pProp->AddOption(_T("18 HatchStyleLightDownwardDiagonal"));
		pProp->AddOption(_T("19 HatchStyleLightUpwardDiagonal"));
		pProp->AddOption(_T("20 HatchStyleDarkDownwardDiagonal"));
		pProp->AddOption(_T("21 HatchStyleDarkUpwardDiagonal"));
		pProp->AddOption(_T("22 HatchStyleWideDownwardDiagonal"));
		pProp->AddOption(_T("23 HatchStyleWideUpwardDiagonal"));
		pProp->AddOption(_T("24 HatchStyleLightVertical"));
		pProp->AddOption(_T("25 HatchStyleLightHorizontal"));
		pProp->AddOption(_T("26 HatchStyleNarrowVertical"));
		pProp->AddOption(_T("27 HatchStyleNarrowHorizontal"));
		pProp->AddOption(_T("28 HatchStyleDarkVertical"));
		pProp->AddOption(_T("29 HatchStyleDarkHorizontal"));
		pProp->AddOption(_T("30 HatchStyleDashedDownwardDiagonal"));
		pProp->AddOption(_T("31 HatchStyleDashedUpwardDiagonal"));
		pProp->AddOption(_T("32 HatchStyleDashedHorizontal"));
		pProp->AddOption(_T("33 HatchStyleDashedVertical"));
		pProp->AddOption(_T("34 HatchStyleSmallConfetti"));
		pProp->AddOption(_T("35 HatchStyleLargeConfetti"));
		pProp->AddOption(_T("36 HatchStyleZigZag"));
		pProp->AddOption(_T("37 HatchStyleWave"));
		pProp->AddOption(_T("38 HatchStyleDiagonalBrick"));
		pProp->AddOption(_T("39 HatchStyleHorizontalBrick"));
		pProp->AddOption(_T("40 HatchStyleWeave"));
		pProp->AddOption(_T("41 HatchStylePlaid"));
		pProp->AddOption(_T("42 HatchStyleDivot"));
		pProp->AddOption(_T("43 HatchStyleDottedGrid"));
		pProp->AddOption(_T("44 HatchStyleDottedDiamond"));
		pProp->AddOption(_T("45 HatchStyleShingle"));
		pProp->AddOption(_T("46 HatchStyleTrellis"));
		pProp->AddOption(_T("47 HatchStyleSphere"));
		pProp->AddOption(_T("48 HatchStyleSmallGrid"));
		pProp->AddOption(_T("49 HatchStyleSmallCheckerBoard"));
		pProp->AddOption(_T("50 HatchStyleLargeCheckerBoard"));
		pProp->AddOption(_T("51 HatchStyleOutlinedDiamond"));
		pProp->AddOption(_T("52 HatchStyleSolidDiamond"));
		pGroupProperty->AddSubItem(pProp);
		//-------------------------------------------------
		return;
	}
	//------------------------------------
	// �༭ͼ����� BlackboardLayer - end
	//-----------------------------------------------------

	//-----------------
	// ��������� - end
	//=====================================================

	//=====================================================
	// ���Ըı�(��ע����Ϣ����)
	//------------------------
	void KPropertyDockablePane::PropertyChanged(LPVOID lpvData)
	{
		// ��������
		CMFCPropertyGridProperty* pMFCPropertyGridProperty = (CMFCPropertyGridProperty*)lpvData;
		if(!pMFCPropertyGridProperty)
			return;

		// ȡ������
		DWORD dwData = pMFCPropertyGridProperty->GetData();
		if(dwData == 0)
			return;

		// ����
		if(dwData & eMisce)
		{
			// ����Ҫ�ı���ͼ����
		}
		// ͨ��(��ͨ�������ں���ʾ)
		else if(dwData & eChannel)
		{
			propertyChanged_Channel(pMFCPropertyGridProperty);
		}
		// ������Ϣ
		else if(dwData & eGeoInfo)
		{
			propertyChanged_GeoInfo(pMFCPropertyGridProperty);
		}
		// ͼ�����
		else if( (dwData & eControl) || (dwData & eGeometry) )
		{
			// ����
			if(dwData == eGeometry_Label || dwData == eGeometry_LogFont)
				propertyChanged_Label(pMFCPropertyGridProperty);
			else
				propertyChanged_Control(pMFCPropertyGridProperty);
		}
		// ͼ��
		else if(dwData & eLegend)
		{
			propertyChanged_Legend(pMFCPropertyGridProperty);
		}
		else
		{
		}

		return;
	}
	// ���Ըı�_��ע�ı�����
	void KPropertyDockablePane::propertyChanged_Label(LPVOID lpvData)
	{
		//-------------------------------------------------
		// ͼ��
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// ͼ������
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;

		// ��������
		CMFCPropertyGridProperty* pMFCPropertyGridProperty = (CMFCPropertyGridProperty*)lpvData;
		if(!pMFCPropertyGridProperty)
			return;

		// ȡ������
		DWORD dwData = pMFCPropertyGridProperty->GetData();
		if(dwData == 0)
			return;
		//-------------------------------------------------
		// �ڰ����ݶ���
		KBlackboardData* pBlackboardData = (KBlackboardData*)pDataObject;
		KGeometry* pGeometry = (KGeometry*)pBlackboardData->GetSelectedObject();
		if(!pGeometry)
			return;

		// �ı�
		if(dwData == eGeometry_Label && GeometryType_Label ==  pGeometry->GetType())
		{
			KLabel* pLabel = (KLabel*)pGeometry;
			pLabel->SetText(KString::ToTChar((LPCWSTR)pMFCPropertyGridProperty->GetValue().bstrVal));
		}
		else if(dwData == eGeometry_LogFont && GeometryType_Label ==  pGeometry->GetType())
		{
			KLabel* pLabel = (KLabel*)pGeometry;
			CMFCPropertyGridFontProperty* pMFCPropertyGridFontProperty  = (CMFCPropertyGridFontProperty*)pMFCPropertyGridProperty;
			LPLOGFONT pLogFont = pMFCPropertyGridFontProperty->GetLogFont();
			pLabel->SetLogFont(*pLogFont);
			pLabel->SetColor(pMFCPropertyGridFontProperty->GetColor());
		}

		// ˢ�»��ͼ
		this->RefreshActiveView();
		return;
	}
	// ���Ըı�_ͨ��
	void KPropertyDockablePane::propertyChanged_Channel(LPVOID lpvData)
	{
		// ͼ��
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		// ͼ������
		KLayerProperty* pLayerProperty = pLayer->GetRender()->GetLayerProperty();
		// �����ṩ��
		KProvider* pProvider = pLayerProperty->GetProvider();
		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();

		// ��������
		CMFCPropertyGridProperty* pMFCPropertyGridProperty = (CMFCPropertyGridProperty*)lpvData;
		if(!pMFCPropertyGridProperty)
			return;

		// ȡ������
		DWORD dwData = pMFCPropertyGridProperty->GetData();
		if(dwData == 0)
			return;

		// ң������
		KAgmrsData* pAgmrsData = (KAgmrsData*)pDataObject;

		// ȡ�þɵ�ͨ����
		int chrOld,chgOld,chbOld;
		pAgmrsData->GetDataChannel(&chrOld,&chgOld,&chbOld);

		// �����µ�ͨ����(���õ��ھɵ�ͨ����)
		int chrNew,chgNew,chbNew;
		pAgmrsData->GetDataChannel(&chrNew,&chgNew,&chbNew);
		// ͨ������ת����ͨ����(38)
		CString strValue = pMFCPropertyGridProperty->GetValue();
		int ch38 = pAgmrsData->ChannelName2ChannelCode(strValue);

		// ͼ������
		if(dwData == eRChannel)
		{
			chrNew = pAgmrsData->ChannelCode2Channel(ch38);
		}
		else if(dwData == eGChannel)
		{
			chgNew = pAgmrsData->ChannelCode2Channel(ch38);
		}
		else if(dwData == eBChannel)
		{
			chbNew = pAgmrsData->ChannelCode2Channel(ch38);
		}

		// ���Ȳ���
		ProgressParam oProgressParam;
		oProgressParam.indexPaneProgress = 1;
		oProgressParam.indexPaneText = 2;
		oProgressParam.SetInfo(_T("����ת��ΪDIB..."));
		// ״̬��
		CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

		// ȡ�����ݴ�����
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)pAgmrsData->GetDataProcessor();
		// ע�������ʾ
		pAgmrsDataProcessor->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback,&oProgressParam);
		// ����ת���� DIB
		pAgmrsDataProcessor->DataToDIB(chrNew,chgNew,chbNew);

		// ��λ������ʾ
		if(pMFCStatusBar)
		{
			pMFCStatusBar->SetPaneText(oProgressParam.indexPaneText,_T(""));
			pMFCStatusBar->EnablePaneProgressBar(oProgressParam.indexPaneProgress,-1);
		}

		// ˢ�»��ͼ
		this->RefreshActiveView();
		return;
	}
	// ���Ըı�_����
	void KPropertyDockablePane::propertyChanged_Control(LPVOID lpvData)
	{
		//-------------------------------------------------
		// ״̬��
		CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

		// ͼ��
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// ͼ������
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;

		// ��������
		CMFCPropertyGridProperty* pMFCPropertyGridProperty = (CMFCPropertyGridProperty*)lpvData;
		if(!pMFCPropertyGridProperty)
			return;

		// ȡ������
		DWORD dwData = pMFCPropertyGridProperty->GetData();
		if(dwData == 0)
			return;
		//-------------------------------------------------
		KGeometry* pGeometry = NULL;
		if(eLayerType == LayerType_Blackboard)
		{
			KBlackboardData* pBlackboardData = (KBlackboardData*)pDataObject;
			pGeometry = (KGeometry*)pBlackboardData->GetSelectedObject();
		}

		switch(dwData)
		{
		//-------------------------------------------------
		// eControl - beg
		//-----------------
		case eControl_Visible:				// �ɼ�
			// ��ʾ��ͼ�㴰�ڹ���(���β����ߵ�)
			pLayerProperty->EnableVisible((pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE));
			break;
		case eControl_Clip:					// ����
			pLayerProperty->EnableClip((pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE));
			break;
		case eControl_Fill:					// ���
			pLayerProperty->EnableFill((pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE));
			break;
		case eControl_Label:				// ��ע
			pLayerProperty->EnableLabel((pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE));
			break;
		case eControl_Rainbow:				// �ʺ�
			pLayerProperty->EnableRainbow((pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE));
			break;
		case eControl_ShowData:				// ��ʾ����
			pLayerProperty->EnableShowData((pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE));
			break;
		case eControl_ShowStationName:		// ��ʾվ������
			pLayerProperty->EnableShowStationName((pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE));
			break;
		case eControl_Transparency:			// ͼ��͸����
			pLayerProperty->SetTransparency(pMFCPropertyGridProperty->GetValue().iVal);
			break;
		case eControl_Color:				// ͼ����ɫ
			pLayerProperty->SetColor(((CMFCPropertyGridColorProperty*)pMFCPropertyGridProperty)->GetColor());
			break;
		case eControl_ShowCoordinate:		// ��ʾ����
			pLayerProperty->EnableShowCoordinate(pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE);
			break;
		case eControl_Threshold_min:		// ��ʾ��ֵ����
			pLayerProperty->ThresholdMin() = (pMFCPropertyGridProperty->GetValue().iVal);
			break;
		case eControl_Threshold_max:		// ��ʾ��ֵ����
			pLayerProperty->ThresholdMax() = (pMFCPropertyGridProperty->GetValue().iVal);
			break;
		case eControl_Tension:				// ��������ϵ��
			pLayerProperty->Tension() = (pMFCPropertyGridProperty->GetValue().dblVal);
			break;
		case eControl_WindBarb:				// ��ʾ�繳
			pLayerProperty->EnableBarb(pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE);
			break;
		case eControl_WindArrow:			// ��ʾ��ʸ
			pLayerProperty->EnableArrow(pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE);
			break;
		case eControl_WindStreamLine:		// ��ʾ����
			pLayerProperty->EnableStreamLine(pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE);
			break;
		case eControl_WindBarbStep:			// �����繳����
			pLayerProperty->BarbStep() = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eControl_WindArrowStep:		// �����繳����
			pLayerProperty->ArrowStep() = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eControl_WindStreamLineStep:	// �����繳����
			pLayerProperty->StreamLineStep() = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eControl_LineWidth:			// ͼ���߿�
			pLayerProperty->LineWidth() = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eControl_LineStyle:			// ͼ������
			pLayerProperty->LineStyle() = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eControl_LogFont:				// �߼�����
			{
				CMFCPropertyGridFontProperty* pMFCPropertyGridFontProperty  = (CMFCPropertyGridFontProperty*)pMFCPropertyGridProperty;
				pLayerProperty->LogFont() = (*pMFCPropertyGridFontProperty->GetLogFont());
				pLayerProperty->FontColor() = pMFCPropertyGridFontProperty->GetColor();
			}
			break;
		case eControl_CurrentElementIndex:		// ��ǰվ��Ҫ������
			{
				KStationData* pStationData = (KStationData*)pDataObject;
				pStationData->CurrentElementIndex() = pMFCPropertyGridProperty->GetValue().intVal - 1;
				if(pStationData->IsTarceContour())
				{
					pStationData->EnableRebuildContour();
					pStationData->BuildContours();
					this->Refresh(pLayer,TRUE);	// �б����̲������������󣬻�û�в鵽ԭ��
				}

				// ��λ������ʾ
				if(pMFCStatusBar)
				{
					pMFCStatusBar->SetPaneText(((ProgressParam*)pStationData->CallbackParam())->indexPaneText,_T(""));
					pMFCStatusBar->EnablePaneProgressBar(((ProgressParam*)pStationData->CallbackParam())->indexPaneProgress,-1);
				}
			}
			break;
		//-----------------
		// eControl - end
		//-------------------------------------------------
		// eGeometry - beg
		///-----------------
		case eGeometry_Transparency:		// �ڰ��ں�����ͼ�ζ���͸����
			if(pGeometry) pGeometry->SetTransparency(pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_Color:				// �ڰ��ں�����ͼ�ζ�����ɫ
			if(pGeometry) pGeometry->SetColor(((CMFCPropertyGridColorProperty*)pMFCPropertyGridProperty)->GetColor());
			break;
		case eGeometry_LineWidth:			// �ڰ��ں�����ͼ�ζ����߿�
			if(pGeometry) pGeometry->SetLineWidth(pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_LineStyle:			// �ڰ��ں�����ͼ�ζ�������
			if(pGeometry) pGeometry->SetLineStyle(pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_FillType:			// �ڰ��ں�����ͼ�ζ����������
			if(pGeometry) pGeometry->SetFillType(pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_FillColor:			// �ڰ��ں�����ͼ�ζ��������ɫ
			if(pGeometry) pGeometry->SetFillColor(pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_HatchStyle:			// �ڰ��ں�����ͼ�ζ������Ӱ�߷��
			if(pGeometry) pGeometry->SetHatchStyle(pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_Tension:				// �ڰ��ں�����ͼ�ζ������Ӱ�߷��
			if(pGeometry && GeometryType_Curve == pGeometry->GetType())
			{
				((KCurve*)pGeometry)->SetTension(pMFCPropertyGridProperty->GetValue().fltVal);
			}
			break;
		case eGeometry_Legend_Width:			// ͼ��ͼ�ο��
			if(pGeometry) ((KLegend*)pGeometry)->Width = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_Legend_Height:			// ͼ��ͼ�θ߶�(�Զ�����)
			//if(pGeometry) ((KLegend*)pGeometry)->Margin = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_Legend_Margin:			// ͼ��ͼ��ҳ�߿հ�
			if(pGeometry) ((KLegend*)pGeometry)->Margin = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_Legend_ItemWidth:		// ͼ����Ŀ���
			if(pGeometry) ((KLegend*)pGeometry)->ItemWidth = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_Legend_ItemHeight:		// ͼ����Ŀ�߶�
			if(pGeometry) ((KLegend*)pGeometry)->ItemHeight = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_Legend_ItemGap:			// ͼ����Ŀ��϶
			if(pGeometry) ((KLegend*)pGeometry)->ItemGap = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_Legend_UnitWidth:		//  ��ע��λ���
			if(pGeometry) ((KLegend*)pGeometry)->UnitWidth = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		///-----------------
		// eGeometry - end
		//-------------------------------------------------
		default:
			break;
		}

		// ˢ�»��ͼ
		this->RefreshActiveView();

		return;
	}
	// ���Ըı�_������Ϣ
	void KPropertyDockablePane::propertyChanged_GeoInfo(LPVOID lpvData)
	{
		// 
	}
	// ���Ըı�_ͼ��
	void KPropertyDockablePane::propertyChanged_Legend(LPVOID lpvData)
	{
		//-------------------------------------------------
		// ״̬��
		CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

		// ͼ��
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// ͼ������
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;

		// ��������
		CMFCPropertyGridProperty* pMFCPropertyGridProperty = (CMFCPropertyGridProperty*)lpvData;
		if(!pMFCPropertyGridProperty)
			return;

		// ȡ������
		DWORD dwData = pMFCPropertyGridProperty->GetData();
		if(dwData == 0)
			return;
		//-------------------------------------------------
		// ��ɫ��
		KPalette* pPalette = pDataObject->GetPalette();

		// ͼ������
		LayerProperties eLayerProperty = (LayerProperties)dwData;

		if(dwData<(eLegend+0x100))
		{
			// Ӧ�õ�ɫ������ڴ�й©
			int index = dwData-eLegend;
			COLORREF clr = (((CMFCPropertyGridColorProperty*)pMFCPropertyGridProperty)->GetColor());
			KPalette* pPaletteNew = pPalette->Clone();
			pPaletteNew->GetAt(index)->R = GetRValue(clr);
			pPaletteNew->GetAt(index)->G = GetGValue(clr);
			pPaletteNew->GetAt(index)->B = GetBValue(clr);
			pDataObject->UsePalette(pPaletteNew);
		}
		//eLegend

		switch(eLayerProperty)
		{
		case eLayerProperty_None:
			break;

		case eLegend_WindBarbColor:			// �繳��ɫ
			pLayerProperty->BarbColor() = (((CMFCPropertyGridColorProperty*)pMFCPropertyGridProperty)->GetColor());
			break;

		case eLegend_WindArrowColor:		// ��ʸ��ɫ
			pLayerProperty->ArrowColor() = (((CMFCPropertyGridColorProperty*)pMFCPropertyGridProperty)->GetColor());
			break;

		case eLegend_WindStreanLineColor:	// ������ɫ
			pLayerProperty->StreamLineColor() = (((CMFCPropertyGridColorProperty*)pMFCPropertyGridProperty)->GetColor());
			break;
		}

		// ˢ�»��ͼ
		this->RefreshActiveView();

		return;
	}
	//=====================================================

//---------------------------------------------------------
}// namespace DockablePanes - end
//---------------------------------------------------------


// KPropertyDockablePane.cpp : 实现文件
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
		// 属性改变(注册消息)
		ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
		// 来自 LayerWnd WM_USER_REFRESH_PROPERTY_WND 消息
		ON_MESSAGE(WM_USER_REFRESH_PROPERTY_WND,OnRefreshPropertyWnd)
	END_MESSAGE_MAP()


	// KPropertyDockablePane 消息处理程序

	// 刷新活动视图
	void KPropertyDockablePane::RefreshActiveView()
	{
		// 框架
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		CView* pView = pFrame->GetActiveView();
		if(!pView->IsKindOf(RUNTIME_CLASS(KGisViewBase)))
			return;

		// 文档
		KGisDocumentBase* pDoc = (KGisDocumentBase*)pView->GetDocument();
		if(!pDoc)
			return;

		// 地图控件
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if(!pMapEngine)
			return;

		pMapEngine->EnableRedraw();

   		pFrame->SetActiveView(pView);
		pView->Invalidate(false);

		return;
	}

	// 注册消息 AFX_WM_PROPERTY_CHANGED
	afx_msg LRESULT KPropertyDockablePane::OnPropertyChanged(WPARAM wParam,LPARAM lParam)
	{
		CMFCPropertyGridProperty* pMFCPropertyGridProperty = (CMFCPropertyGridProperty*)lParam;
		this->PropertyChanged(pMFCPropertyGridProperty);
		return 0L;
	}
	// 来自 LayerWnd WM_USER_REFRESH_PROPERTY_WND 消息
	sfx_msg LRESULT KPropertyDockablePane::OnRefreshPropertyWnd(WPARAM wParam,LPARAM lParam)
	{
		Refresh((KLayer*)lParam);
		return 0L;
	}

	// 建立停靠窗口内部控件
	BOOL KPropertyDockablePane::CreateCtrl()
	{
		CRect rectDummy;
		rectDummy.SetRectEmpty();

		// 创建属性网格
		if (m_pMfcPropertyGridCtrl && m_pMfcPropertyGridCtrl->Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
		{
			m_pMfcPropertyGridCtrl->EnableHeaderCtrl(FALSE);
			m_pMfcPropertyGridCtrl->EnableDescriptionArea();
			m_pMfcPropertyGridCtrl->SetVSDotNetLook();
			m_pMfcPropertyGridCtrl->MarkModifiedProperties();
		}
		else
		{
			TRACE0("未能创建属性网格\n");
			return FALSE;      // 未能创建
		}

		/*
		// 刷新属性网格控件
		// NULL - 清除信息
		RefreshPropertyWnd(NULL);
		*/

		return TRUE;
	}
	// 设置树控件字体
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
	// 调整布局
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
	// 刷新停靠窗口(必要时在派生类重载)
	void KPropertyDockablePane::Refresh(LPVOID lpvData, BOOL bForceRefresh)
	{
		// 如果是相同对象并且不强制刷新
		if(m_lpvPropertyOwner == lpvData && !bForceRefresh)
			return;

		// 对象为空时清除所有属性信息
		if(lpvData==NULL)
		{
			// 清除所有属性
			lpvData = NULL;
			m_pMfcPropertyGridCtrl->RemoveAll();
			m_pMfcPropertyGridCtrl->Invalidate(TRUE);
			return;
		}

		// 不同对象属性或者强制刷新
		if(m_lpvPropertyOwner != lpvData || bForceRefresh)
		{
			// 保存新对象指针,清除所有旧属性
			m_lpvPropertyOwner = lpvData;
			m_pMfcPropertyGridCtrl->RemoveAll();
			m_pMfcPropertyGridCtrl->Invalidate(TRUE);
		}

		// 添加属性组
		this->AddPropertyGroup();
	}

	//=====================================================
	// 添加属性组 - beg
	//-----------------
	void KPropertyDockablePane::AddPropertyGroup()
	{
		// 添加属性组_杂项
		addPropertyGroup_Misce();
		// 添加组_经度
		addPropertyGroup_Lon();
		// 添加属性组_纬度
		addPropertyGroup_Lat();
		// 添加属性组_数据尺寸
		addPropertyGroup_DataSize();
		// 添加属性组_站点信息
		addPropertyGroup_Station();
		// 添加属性组_字体信息
		addPropertyGroup_Font();
		// 添加属性组_通道
		addPropertyGroup_Channel();
		// 添加属性组_地理信息
		addPropertyGroup_GeoInfo();
		// 添加属性组_控制
		addPropertyGroup_Control();
		// 添加属性组_图例
		addPropertyGroup_Legend();
	}
	// 添加属性组_杂项
	void KPropertyDockablePane::addPropertyGroup_Misce()
	{
		//-------------------------------------------------
		// 图层
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// 数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// 图层类型
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;
		//-------------------------------------------------
		CMFCPropertyGridProperty* pGroup_Misce = new CMFCPropertyGridProperty(_T("杂项"),(DWORD)eMisce);
		{// pGroup_Misce - beg

			CString strName = _T("");				// 名称
			CString strProp = _T("");				// 属性
			CString strDescr = _T("");				// 描述
			CMFCPropertyGridProperty* pProp = NULL;	// 属性对象

			// 1.名称
			strName = _T("名称");
			strDescr = _T("图层名称");
			strProp.Format(_T("%s"),(LPCTSTR)pProvider->GetComment());
			pProp = new CMFCPropertyGridProperty(strName, (_variant_t)strProp, strDescr,(DWORD)eMisce_Name);
			pProp->AllowEdit(FALSE);
			pGroup_Misce->AddSubItem(pProp);
			// 2.日期
			strName = _T("日期");
			strDescr = _T("数据日期");
			DATETIME datm = pDataInfo->DateTime();
			COleDateTime dt = COleDateTime(
				datm.wYear,
				datm.wMonth,
				datm.wDay,
				datm.wHour,
				datm.wMinute,
				datm.wSecond
				);
			dt += COleDateTimeSpan(0,8,0,0);	//转化为北京时
			strProp = (COleDateTime::invalid == dt.GetStatus()) ? _T("---") : dt.Format(_T("%Y-%m-%d %H:%M:%S"));
			pProp = new CMFCPropertyGridProperty(strName, (_variant_t)strProp, strDescr,(DWORD)eMisce_DateTime);
			pProp->AllowEdit(FALSE);
			pGroup_Misce->AddSubItem(pProp);
			// 3.范围
			strName = _T("范围");
			strDescr = _T("图层范围");
			KExtents box = pProvider->GetExtents();
			strProp.Format(_T("(%.2f,%.2f) - (%.2f,%.2f)"),box.MinX(),box.MinY(),box.MaxX(),box.MaxY());
			pProp = new CMFCPropertyGridProperty(strName, (_variant_t)strProp, strDescr,(DWORD)eMisce_Extents);
			pProp->AllowEdit(FALSE);
			pGroup_Misce->AddSubItem(pProp);
			//-------------------------------------------------
			// StationLayer - beg
			if(eLayerType == LayerType_Station)
			{
				// 物理量数量
				strName = _T("物理量数量");
				strDescr = _T("每个站点记录包含的物理量数量");
				//pProp = new CMFCPropertyGridProperty(_T("物理量数量"), (_variant_t)pDataInfo->wFactorNum, _T("每个站点记录包含的物理量数量"));
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
		///杂项组(eMisce) - Beg
		///--------------------
		CMFCPropertyGridProperty* pGroup_Misce = new CMFCPropertyGridProperty(_T("杂项"),(DWORD)eMisce);
		{
			// LightningLayer - beg
			if(eLayerType==KLayer::LayerType::LightningLayer)
			{
				// 闪电数量
				pProp = new CMFCPropertyGridProperty(_T("闪电数量"), (_variant_t)pDataInfo->wTotalNum, _T("闪电数量"));
				pProp->AllowEdit(FALSE);
				pGroup_Misce->AddSubItem(pProp);
			}
			// LightningLayer - end
			//-------------------------------------------------
		}
		pGroup_Misce->Expand(TRUE);
		m_pMfcPropertyGridCtrl->AddProperty(pGroup_Misce);
		///--------------------
		///杂项组(eMisce) - End
		///----------------------------------------------------

		return;
		*/
	}
	// 添加组_经度
	void KPropertyDockablePane::addPropertyGroup_Lon()
	{

	}
	// 添加属性组_纬度
	void KPropertyDockablePane::addPropertyGroup_Lat()
	{

	}
	// 添加属性组_数据尺寸
	void KPropertyDockablePane::addPropertyGroup_DataSize()
	{

	}
	// 添加属性组_站点信息
	void KPropertyDockablePane::addPropertyGroup_Station()
	{

	}
	// 添加属性组_字体信息
	void KPropertyDockablePane::addPropertyGroup_Font()
	{
		return;
		//-------------------------------------------------
		// 图层
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// 数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// 图层类型
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;
		//-------------------------------------------------
		// 要求Shape图层 + 点数据
		if(eLayerType != LayerType_Shape)
			return;

		KShapeFile* pShapeFile = (KShapeFile*)pDataObject;
		ShapeType eShapeType = (ShapeType)pShapeFile->GetType();
		if(eShapeType != ShapeType_Point)
			return;
		//-------------------------------------------------
		CMFCPropertyGridProperty* pGroup_Font = new CMFCPropertyGridProperty(_T("标注文本"), (DWORD)eControl);
		{// pGroup_Font - beg

			CString strName = _T("");				// 名称
			CString strProp = _T("");				// 属性
			CString strDescr = _T("");				// 描述
			CMFCPropertyGridProperty* pProp = NULL;
			CMFCPropertyGridColorProperty* pColorProp = NULL;
			CMFCPropertyGridFontProperty* pFontProp = NULL;

			// 逻辑字体
			LOGFONT& lf = pLayerProperty->LogFont();
			COLORREF clrFont = pLayerProperty->FontColor();

			strName = _T("字体");
			strDescr = _T("指定字体");
			pFontProp = new CMFCPropertyGridFontProperty(strName,lf,CF_EFFECTS | CF_SCREENFONTS,strDescr,(DWORD)eControl_LogFont,clrFont);
			pGroup_Font->AddSubItem(pFontProp);

		}// pGroup_Font - end
		pGroup_Font->Expand(TRUE);
		m_pMfcPropertyGridCtrl->AddProperty(pGroup_Font);

		return;
	}
	// 添加属性组_通道
	void KPropertyDockablePane::addPropertyGroup_Channel()
	{
		//-------------------------------------------------
		// 图层
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// 数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// 图层类型
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
		// 数据通道组(eChannel)
		CMFCPropertyGridProperty* pGroup_Channel = new CMFCPropertyGridProperty(_T("数据通道"), (DWORD)eChannel);
		{// pGroup_Channel - beg

			CString strName = _T("");				// 名称
			CString strProp = _T("");				// 属性
			CString strDescr = _T("");				// 描述
			CMFCPropertyGridProperty* pProp = NULL;	// 属性对象

			KAgmrsData* pAgmrsData = (KAgmrsData*)pDataObject;

			// 取得三通道号
			int chr,chg,chb;
			pAgmrsData->GetDataChannel(&chr,&chg,&chb);
			int chr38 = pAgmrsData->Channel2ChannelCode(chr);
			int chg38 = pAgmrsData->Channel2ChannelCode(chg);
			int chb38 = pAgmrsData->Channel2ChannelCode(chb);

			// 取得通道映射数据
			AgmrsDataChannelMap*  pAgmrsDataChannelMap = pAgmrsData->GetAgmrsDataChannelMap();
			AgmrsDataChannelMap::iterator itr;
			AgmrsDataChannelMap::iterator itrbeg = pAgmrsDataChannelMap->begin();
			AgmrsDataChannelMap::iterator itrend = pAgmrsDataChannelMap->end();

			// 红色通道
			strName = _T("红色通道");
			strDescr = _T("通道融合之红色通道");
			strProp = pAgmrsData->ChannelCode2String(chr38);
			pProp = new CMFCPropertyGridProperty(strName, (_variant_t)strProp, strDescr,(DWORD)eRChannel);
			for(itr=itrbeg; itr!=itrend; itr++)
			{
				pProp->AddOption( (*itr).second.szName );
			}
			pProp->AllowEdit(FALSE);
			pGroup_Channel->AddSubItem(pProp);

			// 绿色通道
			strName = _T("绿色通道");
			strDescr = _T("通道融合之绿色通道");
			strProp = pAgmrsData->ChannelCode2String(chg38);
			pProp = new CMFCPropertyGridProperty(strName, (_variant_t)strProp, strDescr,(DWORD)eGChannel);
			for(itr=itrbeg; itr!=itrend; itr++)
			{
				pProp->AddOption( (*itr).second.szName );
			}
			pProp->AllowEdit(FALSE);
			pGroup_Channel->AddSubItem(pProp);

			// 蓝色通道
			strName = _T("蓝色通道");
			strDescr = _T("通道融合之蓝色通道");
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
	// 添加属性组_地理信息
	void KPropertyDockablePane::addPropertyGroup_GeoInfo()
	{
		// 
	}
	// 添加属性组_控制
	void KPropertyDockablePane::addPropertyGroup_Control()
	{
		//-------------------------------------------------
		// 图层
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// 数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// 图层类型
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;
		//-------------------------------------------------
		CMFCPropertyGridProperty* pGroup_Control = new CMFCPropertyGridProperty(_T("控制"),(DWORD)eControl);
		{// pGroup_Control - beg
			//---------------------------------------------
			// 添加控制 - 公共属性
			this->addPropertyGroup_Control_Common(pGroup_Control,pLayer);
			// 添加控制 - ShapeLayer 属性
			this->addPropertyGroup_Control_ShapeLayer(pGroup_Control,pLayer);
			// 添加控制 - StationLayer 属性
			this->addPropertyGroup_Control_StationLayer(pGroup_Control,pLayer);
			// 添加控制 - GridContourLayer 属性
			this->addPropertyGroup_Control_GridContourLayer(pGroup_Control,pLayer);
			// 添加控制 - WindVectorLayer 属性
			this->addPropertyGroup_Control_WindVectorLayer(pGroup_Control,pLayer);
			// 添加控制 - RpgLayer 属性
			this->addPropertyGroup_Control_RpgLayer(pGroup_Control,pLayer);
			// 添加控制 - BlackboardLayer 属性
			this->addPropertyGroup_Control_BlackboardLayer(pGroup_Control,pLayer);
			//---------------------------------------------
		}// pGroup_Control - end
		pGroup_Control->Expand(TRUE);
		m_pMfcPropertyGridCtrl->AddProperty(pGroup_Control);

		return;
	}
	// 添加属性组_图例(调色板)
	void KPropertyDockablePane::addPropertyGroup_Legend()
	{
		//-------------------------------------------------
		// 图层
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// 数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// 调色板
		KPalette* pPalette = pDataObject->GetPalette();
		//if(!pPalette)
		//	return;

		// 图层类型
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;
		if(eLayerType == LayerType_Blackboard)	// 黑板图层
			return;
		if(eLayerType == LayerType_Shape)		// 地图图层
			return;
		if(eLayerType == LayerType_Modis)		// 遥感MODIS图层
			return;
		// 站点图层(不追踪等值线)
		if(eLayerType == LayerType_Station && !((KStationData*)pDataObject)->IsTarceContour())
			return;
		//-------------------------------------------------
		CMFCPropertyGridProperty* pGroup_Legend = new CMFCPropertyGridProperty(_T("图例"),(DWORD)eLegend);
		{// pGroup_Legend - beg

			CString strName = _T("");				// 名称
			CString strProp = _T("");				// 属性
			CString strDescr = _T("");				// 描述
			CMFCPropertyGridProperty* pProp = NULL;
			CMFCPropertyGridColorProperty* pColorProp = NULL;
			//---------------------------------------------
			// 如果有调色板直接使用调色板数据
			if(pPalette)
			{
				int iColorEntries = pPalette->GetColorEntries();
				for(int i=0;i<iColorEntries;i++)
				{
					// 名称
					if(pPalette->HasComment())
						strName.Format(_T("%s"), pPalette->GetAt(i)->GetComment()==NULL ? _T("") : pPalette->GetAt(i)->GetComment());
					else
						strName.Format(_T("%.2f"),pPalette->GetAt(i)->Value());

					strName.Trim();
					if(strName.IsEmpty())
					{
						strName.Format(_T("%.2f"),pPalette->GetAt(i)->Value());
					}

					// 描述
					strDescr.Format(_T("%s的颜色"),strName);
					// 颜色
					COLORREF clr = pPalette->GetAt(i)->Rgb();
					// 颜色
					pColorProp =  new CMFCPropertyGridColorProperty(strName, clr, (CPalette*)NULL, strDescr, eLegend + i);
					pColorProp->EnableOtherButton(_T("其他..."));
					// 放开调色板
					//pColorProp->Enable(FALSE);
					pColorProp->Enable(TRUE);
					pGroup_Legend->AddSubItem(pColorProp);

				}// next i

			}// if(pPalette)
			else // 没有调色板
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

					strProp.Format(_T("单位(%s)"),pRpgData->GetProductUnit(iProductCode));
					pProp =  new CMFCPropertyGridProperty(strProp, (_variant_t)_T("颜色"), _T(""),(DWORD)(eLayerProperty_None));
					pProp->AllowEdit(FALSE);
					pGroup_Legend->AddSubItem(pProp);

					for(int i=1;i<iDataLevel;i++)
					{
						// 颜色
						strProp.Format(_T("%.0f"),pLegend[i].dValue);
						pColorProp =  new CMFCPropertyGridColorProperty(strProp, pLegend[i].dwColor, (CPalette*)NULL, _T("指定默认的颜色"));
						pColorProp->Enable(FALSE);
						pGroup_Legend->AddSubItem(pColorProp);
					}

				}
				// LayerType_Rpg - end
				//-----------------------------------------
				// LayerType_Wind_Vector - beg
				if(eLayerType==LayerType_Wind_Vector)
				{
					// 风钩
					strName = _T("风钩颜色");		// 名称
					strProp = _T("指定风钩的颜色");	// 属性
					pColorProp =  new CMFCPropertyGridColorProperty(strName, pLayerProperty->BarbColor(), NULL, strProp,(DWORD)(eLegend_WindBarbColor));
					pColorProp->EnableOtherButton(_T("其他..."));
					pGroup_Legend->AddSubItem(pColorProp);
					// 风矢
					strName = _T("风矢颜色");		// 名称
					strProp = _T("指定风矢的颜色");	// 属性
					pColorProp =  new CMFCPropertyGridColorProperty(strName, pLayerProperty->ArrowColor(), NULL, strProp,(DWORD)(eLegend_WindArrowColor));
					pColorProp->EnableOtherButton(_T("其他..."));
					pGroup_Legend->AddSubItem(pColorProp);
					// 风矢
					strName = _T("流线颜色");		// 名称
					strProp = _T("指定流线的颜色");	// 属性
					pColorProp =  new CMFCPropertyGridColorProperty(strName, pLayerProperty->StreamLineColor(), NULL, strProp,(DWORD)(eLegend_WindStreanLineColor));
					pColorProp->EnableOtherButton(_T("其他..."));
					pGroup_Legend->AddSubItem(pColorProp);
				}
				// LayerType_Wind_Vector - end
				//-----------------------------------------

			}// 没有调色板的图层
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

	// 添加属性组_控制 - 公共
	void KPropertyDockablePane::addPropertyGroup_Control_Common(CMFCPropertyGridProperty* pGroupProperty,void* lpvData)
	{
		//-------------------------------------------------
		// 图层
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// 图层类型
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		// 剪切
		if(eLayerType != LayerType_Shape)
		{
			strName = _T("剪切");
			strDescr = _T("图形限制在剪切区中显示");
			pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->IsClip()),strDescr,(DWORD)eControl_Clip);
			pGroupProperty->AddSubItem(pProp);
		}
		// 透明度
		if(eLayerType != LayerType_Blackboard)	// 编辑图层透明度又不同对象控制
		{
			strName = _T("透明度(%)");
			strDescr = _T("设置图层透明度。");
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
	// 添加控制 - ShapeLayer 属性
	void KPropertyDockablePane::addPropertyGroup_Control_ShapeLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData)
	{
		//-------------------------------------------------
		// 图层
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// 数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// 图层类型
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType != LayerType_Shape)
			return;
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		strName = _T("剪切");
		strDescr = _T("图形限制在剪切区中显示");
		pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->IsClip()),strDescr,(DWORD)eControl_Clip);
		pGroupProperty->AddSubItem(pProp);

		KShapeFile* pShapeFile = (KShapeFile*)pDataObject;
		ShapeType eShapeType = (ShapeType)pShapeFile->GetType();
		if(eShapeType == ShapeType_Point)	// 点 Shape
		{
			// 逻辑字体
			LOGFONT& lf = pLayerProperty->LogFont();
			COLORREF clrFont = pLayerProperty->FontColor();

			strName = _T("字体");
			strDescr = _T("指定字体");
			pProp = new CMFCPropertyGridFontProperty(strName,lf,CF_EFFECTS | CF_SCREENFONTS,strDescr,(DWORD)eControl_LogFont,clrFont);
			pGroupProperty->AddSubItem(pProp);
		}
		else
		{
			// 颜色
			strName = _T("颜色");
			strDescr = _T("指定默认的颜色");
			pColorProp =  new CMFCPropertyGridColorProperty(strName, pLayerProperty->GetColor(), (CPalette*)NULL, strDescr, (DWORD)(eControl_Color));
			pColorProp->EnableOtherButton(_T("其他..."));
			pGroupProperty->AddSubItem(pColorProp);

			// 线宽
			strName = _T("线宽");
			strDescr = _T("设置几何图形的线宽。");
			pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLayerProperty->LineWidth(), strDescr, (DWORD)(eControl_LineWidth));
			pProp->EnableSpinControl(TRUE,1,50);
			pGroupProperty->AddSubItem(pProp);

			// 线型
			strName = _T("线型");
			strDescr = _T("设置几何图形的线型。");
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
	// 添加控制 - StationLayer 属性
	void KPropertyDockablePane::addPropertyGroup_Control_StationLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData)
	{
		//-------------------------------------------------
		// 图层
		KLayer* pLayer = (KLayer*)lpvData;
		if(!pLayer)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// 数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// 图层类型
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType != LayerType_Station)
			return;
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		// 数据对象
		KStationData* pStationData = (KStationData*)pDataObject;

		// 画笔颜色
		strName = _T("颜色");
		strDescr = _T("指定默认的颜色");
		pColorProp =  new CMFCPropertyGridColorProperty(strName, pLayerProperty->GetColor(), NULL, strDescr,(DWORD)(eControl_Color));
		pColorProp->EnableOtherButton(_T("其他..."));
		pGroupProperty->AddSubItem(pColorProp);

		// 当前物理量索引
		strName = _T("当前物理量");
		strDescr = _T("显示当前物理量索引");
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

		// 显示站点名称
		strName = _T("显示站点名称");
		strDescr = _T("显示站点名称");
		pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->IsShowStationName()),strDescr,(DWORD)eControl_ShowStationName);
		pGroupProperty->AddSubItem(pProp);

		// 显示站点数据
		strName = _T("显示站点数据");
		strDescr = _T("显示站点数据");
		pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->IsShowData() ? true : false),strDescr,(DWORD)eControl_ShowData);
		pGroupProperty->AddSubItem(pProp);

		// 逻辑字体
		LOGFONT& lf = pLayerProperty->LogFont();
		COLORREF clrFont = pLayerProperty->FontColor();

		strName = _T("字体");
		strDescr = _T("指定字体");
		pProp = new CMFCPropertyGridFontProperty(strName,lf,CF_EFFECTS | CF_SCREENFONTS,strDescr,(DWORD)eControl_LogFont,clrFont);
		pGroupProperty->AddSubItem(pProp);

		// 等值线选项
		if(((KStationData*)pDataObject)->IsTarceContour())
		{
			// 填充
			strName = _T("色斑图");
			strDescr = _T("显示色斑图");
			pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->IsFill() ? true : false),strDescr,(DWORD)eControl_Fill);
			pGroupProperty->AddSubItem(pProp);

			// 彩虹线
			strName = _T("多彩等值线");
			strDescr = _T("显示多彩等值线");
			pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->IsRainbow() ? true : false),strDescr,(DWORD)eControl_Rainbow);
			pGroupProperty->AddSubItem(pProp);

			// 标注
			strName = _T("标注等值线");
			strDescr = _T("标注等值线值");
			pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->IsLabel() ? true : false),strDescr,(DWORD)eControl_Label);
			pGroupProperty->AddSubItem(pProp);
		}
		//-------------------------------------------------
		return;
	}
	// 添加控制 - GridContourLayer 属性
	void KPropertyDockablePane::addPropertyGroup_Control_GridContourLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData)
	{
		//-------------------------------------------------
		// 图层
		KLayer* pLayer = (KLayer*)lpvData;
		if(!pLayer)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// 数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// 图层类型
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType != LayerType_Grid_Contour)
			return;
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		// 图层画笔颜色
		pColorProp =  new CMFCPropertyGridColorProperty(_T("颜色"), pLayerProperty->GetColor(), NULL, _T("指定默认的颜色"),(DWORD)(eControl_Color));
		//pColorProp->EnableAutomaticButton(_T("默认"), RGB(255,255,255));
		pColorProp->EnableOtherButton(_T("其他..."));
		pGroupProperty->AddSubItem(pColorProp);

		// 是否填充
		pProp = new CMFCPropertyGridProperty(_T("色斑图"),(_variant_t)(pLayerProperty->IsFill() ? true : false),_T("显示色斑图"),(DWORD)eControl_Fill);
		pGroupProperty->AddSubItem(pProp);

		// 是否彩虹线
		pProp = new CMFCPropertyGridProperty(_T("多彩等值线"),(_variant_t)(pLayerProperty->IsRainbow() ? true : false),_T("显示多彩等值线"),(DWORD)eControl_Rainbow);
		pGroupProperty->AddSubItem(pProp);

		// 是否标注
		pProp = new CMFCPropertyGridProperty(_T("标注等值线"),(_variant_t)(pLayerProperty->IsLabel() ? true : false),_T("标注等值线值"),(DWORD)eControl_Label);
		pGroupProperty->AddSubItem(pProp);

		// 是否显示格点数据
		pProp = new CMFCPropertyGridProperty(_T("显示格点数据"),(_variant_t)(pLayerProperty->IsShowData() ? true : false),_T("显示格点数据"),(DWORD)eControl_ShowData);
		pGroupProperty->AddSubItem(pProp);

		// 曲线张力系数
		strName = _T("张力系数");
		strDescr = _T("设置几何图形的曲线张力系数。");
		pProp = new CMFCPropertyGridProperty(strName,(_variant_t)(pLayerProperty->Tension()),strDescr,(DWORD)eControl_Tension);
		pGroupProperty->AddSubItem(pProp);
		//-------------------------------------------------
		return;
	}
	// 添加控制 - WindVectorLayer 属性
	void KPropertyDockablePane::addPropertyGroup_Control_WindVectorLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData)
	{
		//-------------------------------------------------
		// 图层
		KLayer* pLayer = (KLayer*)lpvData;
		if(!pLayer)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// 数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// 图层类型
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType != LayerType_Wind_Vector)
			return;
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		// 风钩
		strName = _T("显示风钩");
		strDescr = _T("显示风钩");
		pProp = new CMFCPropertyGridProperty(strName, (_variant_t)(pLayerProperty->IsShowBarb() ? true : false), strDescr,(DWORD)eControl_WindBarb);
		pGroupProperty->AddSubItem(pProp);

		// 风矢
		strName = _T("显示风矢");
		strDescr = _T("显示风矢");
		pProp = new CMFCPropertyGridProperty(strName, (_variant_t)(pLayerProperty->IsShowBarb() ? true : false), strDescr,(DWORD)eControl_WindArrow);
		pGroupProperty->AddSubItem(pProp);

		// 流线
		strName = _T("显示流线");
		strDescr = _T("显示流线");
		pProp = new CMFCPropertyGridProperty(strName, (_variant_t)(pLayerProperty->IsShowBarb() ? true : false), strDescr,(DWORD)eControl_WindStreamLine);
		pGroupProperty->AddSubItem(pProp);

		// 风钩步长
		strName = _T("风钩步长");
		strDescr = _T("调整风钩步长");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLayerProperty->BarbStep(), strDescr, (DWORD)(eControl_WindBarbStep));
		pProp->AllowEdit(FALSE);
		pProp->EnableSpinControl(TRUE,1,9);
		pGroupProperty->AddSubItem(pProp);

		// 风矢步长
		strName = _T("风矢步长");
		strDescr = _T("调整风矢步长");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLayerProperty->ArrowStep(), strDescr, (DWORD)(eControl_WindArrowStep));
		pProp->AllowEdit(FALSE);
		pProp->EnableSpinControl(TRUE,1,9);
		pGroupProperty->AddSubItem(pProp);

		// 流线密度
		strName = _T("流线密度");
		strDescr = _T("调整流线密度");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLayerProperty->StreamLineStep(), strDescr, (DWORD)(eControl_WindStreamLineStep));
		pProp->AllowEdit(FALSE);
		pProp->EnableSpinControl(TRUE,1,5);
		pGroupProperty->AddSubItem(pProp);
		//-------------------------------------------------
		return;
	}
	// 添加控制 - RpgLayer 属性
	void KPropertyDockablePane::addPropertyGroup_Control_RpgLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData)
	{
		//-------------------------------------------------
		// 图层
		KLayer* pLayer = (KLayer*)lpvData;
		if(!pLayer)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// 数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// 图层类型
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType != LayerType_Rpg)
			return;
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		// 数据级别
		int iProductCode = ((KRpgData*)pDataObject)->GetProductCode();
		int iDataLevel = ((KRpgData*)pDataObject)->GetProductDataLevel(iProductCode);
		pProp = new CMFCPropertyGridProperty(_T("数据级别"),(_variant_t)iDataLevel,_T("雷达数据分级数量"),(DWORD)eLayerProperty_None);
		pProp->AllowEdit(FALSE);
		pGroupProperty->AddSubItem(pProp);

		// 显示下限
		pProp =  new CMFCPropertyGridProperty(_T("显示下限"), (_variant_t)(int)pLayerProperty->ThresholdMin(), _T("雷达数据显示下限"), (DWORD)(eControl_Threshold_min));
		pProp->EnableSpinControl(TRUE,0,iDataLevel-1);
		pGroupProperty->AddSubItem(pProp);

		// 显示上限
		pProp =  new CMFCPropertyGridProperty(_T("显示上限"), (_variant_t)(int)pLayerProperty->ThresholdMax(), _T("雷达数据显示上限"), (DWORD)(eControl_Threshold_max));
		pProp->EnableSpinControl(TRUE,0,iDataLevel-1);
		pGroupProperty->AddSubItem(pProp);

		// 显示极坐标
		pProp = new CMFCPropertyGridProperty(_T("极坐标"),(_variant_t)pLayerProperty->IsShowCoordinate() ,_T("显示极坐标"),(DWORD)eControl_ShowCoordinate);
		pGroupProperty->AddSubItem(pProp);
		//-------------------------------------------------
		return;
	}
	// 添加控制 - BlackboardLayer 属性
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData)
	{
		//-------------------------------------------------
		// 图层
		KLayer* pLayer = (KLayer*)lpvData;
		if(!pLayer)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// 数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// 图层类型
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType != LayerType_Blackboard)
			return;
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		KBlackboardData* pBlackboardData = (KBlackboardData*)pDataObject;
		KGeometry* pGeometry = (KGeometry*)pBlackboardData->GetSelectedObject();
		if(!pGeometry)
			return;
		//-------------------------------------------------
		GeometryType eGeometryType = (GeometryType)pGeometry->GetType();

		// 透明度
		strName = _T("透明度(%)");
		strDescr = _T("设置对象透明度。");
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
			|| eGeometryType == GeometryType_Point		// 线段
			|| eGeometryType == GeometryType_Line		// 线段
			|| eGeometryType == GeometryType_Rect		// 矩形
			|| eGeometryType == GeometryType_Ellipse	// 椭圆
			|| eGeometryType == GeometryType_Polyline	// 折线
			|| eGeometryType == GeometryType_Polygon	// 多边形
			|| eGeometryType == GeometryType_Curve		// 曲线
			)
		{
			// 颜色
			strName = _T("颜色");
			strDescr = _T("设置几何图形的绘制颜色。");
			pColorProp =  new CMFCPropertyGridColorProperty(strName, pGeometry->GetColor(), (CPalette*)NULL, strDescr,(DWORD)(eGeometry_Color));
			pColorProp->AllowEdit(FALSE);
			//pColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
			pColorProp->EnableOtherButton(_T("其他..."));
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
	// 编辑图层对象 BlackboardLayer - beg
	//------------------------------------
	// 添加控制 - BlackboardLayer 的 Point 对象属性
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Point(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		//-------------------------------------------------
		return;
	}
	// 添加控制 - BlackboardLayer 的 Line 对象属性
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Line(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_Common(pGroupProperty,pGeometry);
		//-------------------------------------------------
		return;
	}
	// 添加控制 - BlackboardLayer 的 Rect 对象属性
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Rect(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_Common(pGroupProperty,pGeometry);
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_CanFill(pGroupProperty,pGeometry);
		//-------------------------------------------------
		return;
	}
	// 添加控制 - BlackboardLayer 的 Ellipse 对象属性
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Ellipse(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_Common(pGroupProperty,pGeometry);
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_CanFill(pGroupProperty,pGeometry);
		//-------------------------------------------------
		return;
	}
	// 添加控制 - BlackboardLayer 的 Polyline 对象属性
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Polyline(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_Common(pGroupProperty,pGeometry);
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_CanFill(pGroupProperty,pGeometry);
		//-------------------------------------------------
		return;
	}
	// 添加控制 - BlackboardLayer 的 Polygon 对象属性
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Polygon(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_Common(pGroupProperty,pGeometry);
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_CanFill(pGroupProperty,pGeometry);
		//-------------------------------------------------
		return;
	}
	// 添加控制 - BlackboardLayer 的 Curve 对象属性
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Curve(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_Common(pGroupProperty,pGeometry);
		addPropertyGroup_Control_BlackboardLayer_LinearGeometry_CanFill(pGroupProperty,pGeometry);
		//-------------------------------------------------
		// 张力系数
		strName = _T("张力系数");
		strDescr = _T("设置几何图形的张力系数。");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)(float)((KCurve*)pGeometry)->GetTension(), strDescr, (DWORD)(eGeometry_Tension));
		pGroupProperty->AddSubItem(pProp);
		//-------------------------------------------------
		return;
	}
	// 添加属性组_字体信息(黑板)
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Label(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		KLabel* pLabel = (KLabel*)pGeometry;
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridFontProperty* pFontProp = NULL;		// 属性对象 - 字体
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		// 文本
		strName = _T("文本");
		strDescr = _T("文本字符串");
		pProp = new CMFCPropertyGridProperty(strName, (_variant_t)(pLabel ? (LPCTSTR)pLabel->GetText() : _T("")), strDescr, (DWORD)eGeometry_Label);
		pGroupProperty->AddSubItem(pProp);

		// 逻辑字体
		LOGFONT lf;
		COLORREF clrFont = 0;
		HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
		::GetObject(hFont,sizeof(LOGFONT),&lf);
		if(pLabel)
		{
			lf = pLabel->GetLogFont();
			clrFont = pLabel->GetColor();
		}
		strName = _T("字体");
		strDescr = _T("指定字体");
		pFontProp = new CMFCPropertyGridFontProperty(strName,lf,CF_EFFECTS | CF_SCREENFONTS,strDescr,(DWORD)eGeometry_LogFont,clrFont);
		pGroupProperty->AddSubItem(pFontProp);
		//-------------------------------------------------
		return;
	}
	// 添加控制 - BlackboardLayer 的 Lengend 对象属性
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_Legend(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		KLegend* pLegend = (KLegend*)pGeometry;
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		// 图例宽度
		strName = _T("图例宽度");
		strDescr = _T("调整图例宽度");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLegend->Width, strDescr, (DWORD)(eGeometry_Legend_Width));
		pGroupProperty->AddSubItem(pProp);

		// 图例高度
		strName = _T("图例高度");
		strDescr = _T("调整图例高度");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)_T("自动计算"), strDescr, (DWORD)(eGeometry_Legend_Height));
		pProp->AllowEdit(FALSE);
		pGroupProperty->AddSubItem(pProp);

		// 页边空白
		strName = _T("页边空白");
		strDescr = _T("调整页边空白");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLegend->Margin, strDescr, (DWORD)(eGeometry_Legend_Margin));
		pGroupProperty->AddSubItem(pProp);

		// 图例条目宽度
		strName = _T("图例条目宽度");
		strDescr = _T("调整图例条目宽度");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLegend->ItemWidth, strDescr, (DWORD)(eGeometry_Legend_ItemWidth));
		pGroupProperty->AddSubItem(pProp);

		// 图例条目高度
		strName = _T("图例条目高度");
		strDescr = _T("调整图例条目高度");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLegend->ItemHeight, strDescr, (DWORD)(eGeometry_Legend_ItemHeight));
		pGroupProperty->AddSubItem(pProp);

		// 图例条目间隙
		strName = _T("图例条目间隙");
		strDescr = _T("调整图例条目间隙");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLegend->ItemGap, strDescr, (DWORD)(eGeometry_Legend_ItemGap));
		pGroupProperty->AddSubItem(pProp);

		// 标注单位宽度
		strName = _T("标注单位宽度");
		strDescr = _T("调整标注单位宽度");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pLegend->UnitWidth, strDescr, (DWORD)(eGeometry_Legend_UnitWidth));
		pGroupProperty->AddSubItem(pProp);
		//-------------------------------------------------
		return;
	}
	// 添加控制 - BlackboardLayer 的 LinearGeometry_Common 对象属性
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_LinearGeometry_Common(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		KPoint* pPoint = (KPoint*)pGeometry;
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		// 线宽
		strName = _T("线宽");
		strDescr = _T("设置几何图形的线宽。");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pGeometry->GetLineWidth(), strDescr, (DWORD)(eGeometry_LineWidth));
		pProp->EnableSpinControl(TRUE,1,50);
		pGroupProperty->AddSubItem(pProp);

		// 线型
		strName = _T("线型");
		strDescr = _T("设置几何图形的线型。");
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
	// 添加控制 - BlackboardLayer 的 LinearGeometry_CanFill 对象属性
	void KPropertyDockablePane::addPropertyGroup_Control_BlackboardLayer_LinearGeometry_CanFill(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry)
	{
		//-------------------------------------------------
		CString strName = _T("");							// 名称
		CString strProp = _T("");							// 属性
		CString strDescr = _T("");							// 描述
		CMFCPropertyGridProperty* pProp = NULL;				// 属性对象 - 一般
		CMFCPropertyGridColorProperty* pColorProp = NULL;	// 属性对象 - 颜色
		//-------------------------------------------------
		// 填充类型 -1-不填充  0-单色填充  1-影线填充
		strName = _T("填充");
		strDescr = _T("设置几何图形的填充类型。");
		pProp =  new CMFCPropertyGridProperty(strName, (_variant_t)pGeometry->GetFillType(), strDescr, (DWORD)(eGeometry_FillType));
		pProp->AllowEdit(FALSE);
		pProp->AddOption(_T("-1 不填充"));
		pProp->AddOption(_T(" 0 单色"));
		pProp->AddOption(_T(" 1 影线"));
		pGroupProperty->AddSubItem(pProp);

		// 填充颜色
		strName = _T("填充颜色");
		strDescr = _T("设置几何图形的填充颜色。");
		pColorProp =  new CMFCPropertyGridColorProperty(strName, pGeometry->GetFillColor(), (CPalette*)NULL, strDescr,(DWORD)(eGeometry_FillColor));
		pColorProp->AllowEdit(FALSE);
		//pColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
		pColorProp->EnableOtherButton(_T("其他..."));
		pGroupProperty->AddSubItem(pColorProp);

		// 影线风格
		strName = _T("影线风格");
		strDescr = _T("设置几何图形的填充影线风格。");
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
	// 编辑图层对象 BlackboardLayer - end
	//-----------------------------------------------------

	//-----------------
	// 添加属性组 - end
	//=====================================================

	//=====================================================
	// 属性改变(有注册消息调用)
	//------------------------
	void KPropertyDockablePane::PropertyChanged(LPVOID lpvData)
	{
		// 属性网格
		CMFCPropertyGridProperty* pMFCPropertyGridProperty = (CMFCPropertyGridProperty*)lpvData;
		if(!pMFCPropertyGridProperty)
			return;

		// 取得数据
		DWORD dwData = pMFCPropertyGridProperty->GetData();
		if(dwData == 0)
			return;

		// 杂项
		if(dwData & eMisce)
		{
			// 不需要改变视图内容
		}
		// 通道(多通道数据融合显示)
		else if(dwData & eChannel)
		{
			propertyChanged_Channel(pMFCPropertyGridProperty);
		}
		// 地理信息
		else if(dwData & eGeoInfo)
		{
			propertyChanged_GeoInfo(pMFCPropertyGridProperty);
		}
		// 图层控制
		else if( (dwData & eControl) || (dwData & eGeometry) )
		{
			// 字体
			if(dwData == eGeometry_Label || dwData == eGeometry_LogFont)
				propertyChanged_Label(pMFCPropertyGridProperty);
			else
				propertyChanged_Control(pMFCPropertyGridProperty);
		}
		// 图例
		else if(dwData & eLegend)
		{
			propertyChanged_Legend(pMFCPropertyGridProperty);
		}
		else
		{
		}

		return;
	}
	// 属性改变_标注文本字体
	void KPropertyDockablePane::propertyChanged_Label(LPVOID lpvData)
	{
		//-------------------------------------------------
		// 图层
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// 数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// 图层类型
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;

		// 属性网格
		CMFCPropertyGridProperty* pMFCPropertyGridProperty = (CMFCPropertyGridProperty*)lpvData;
		if(!pMFCPropertyGridProperty)
			return;

		// 取得数据
		DWORD dwData = pMFCPropertyGridProperty->GetData();
		if(dwData == 0)
			return;
		//-------------------------------------------------
		// 黑板数据对象
		KBlackboardData* pBlackboardData = (KBlackboardData*)pDataObject;
		KGeometry* pGeometry = (KGeometry*)pBlackboardData->GetSelectedObject();
		if(!pGeometry)
			return;

		// 文本
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

		// 刷新活动视图
		this->RefreshActiveView();
		return;
	}
	// 属性改变_通道
	void KPropertyDockablePane::propertyChanged_Channel(LPVOID lpvData)
	{
		// 图层
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		// 图层属性
		KLayerProperty* pLayerProperty = pLayer->GetRender()->GetLayerProperty();
		// 数据提供者
		KProvider* pProvider = pLayerProperty->GetProvider();
		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();

		// 属性网格
		CMFCPropertyGridProperty* pMFCPropertyGridProperty = (CMFCPropertyGridProperty*)lpvData;
		if(!pMFCPropertyGridProperty)
			return;

		// 取得数据
		DWORD dwData = pMFCPropertyGridProperty->GetData();
		if(dwData == 0)
			return;

		// 遥感数据
		KAgmrsData* pAgmrsData = (KAgmrsData*)pDataObject;

		// 取得旧的通道号
		int chrOld,chgOld,chbOld;
		pAgmrsData->GetDataChannel(&chrOld,&chgOld,&chbOld);

		// 设置新的通道号(先让等于旧的通道号)
		int chrNew,chgNew,chbNew;
		pAgmrsData->GetDataChannel(&chrNew,&chgNew,&chbNew);
		// 通道名称转换成通道号(38)
		CString strValue = pMFCPropertyGridProperty->GetValue();
		int ch38 = pAgmrsData->ChannelName2ChannelCode(strValue);

		// 图层数据
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

		// 进度参数
		ProgressParam oProgressParam;
		oProgressParam.indexPaneProgress = 1;
		oProgressParam.indexPaneText = 2;
		oProgressParam.SetInfo(_T("数据转换为DIB..."));
		// 状态条
		CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

		// 取得数据处理器
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)pAgmrsData->GetDataProcessor();
		// 注册进度显示
		pAgmrsDataProcessor->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback,&oProgressParam);
		// 数据转换到 DIB
		pAgmrsDataProcessor->DataToDIB(chrNew,chgNew,chbNew);

		// 复位进度显示
		if(pMFCStatusBar)
		{
			pMFCStatusBar->SetPaneText(oProgressParam.indexPaneText,_T(""));
			pMFCStatusBar->EnablePaneProgressBar(oProgressParam.indexPaneProgress,-1);
		}

		// 刷新活动视图
		this->RefreshActiveView();
		return;
	}
	// 属性改变_控制
	void KPropertyDockablePane::propertyChanged_Control(LPVOID lpvData)
	{
		//-------------------------------------------------
		// 状态条
		CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

		// 图层
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// 数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// 图层类型
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;

		// 属性网格
		CMFCPropertyGridProperty* pMFCPropertyGridProperty = (CMFCPropertyGridProperty*)lpvData;
		if(!pMFCPropertyGridProperty)
			return;

		// 取得数据
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
		case eControl_Visible:				// 可见
			// 显示由图层窗口管理(本段不会走到)
			pLayerProperty->EnableVisible((pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE));
			break;
		case eControl_Clip:					// 剪切
			pLayerProperty->EnableClip((pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE));
			break;
		case eControl_Fill:					// 填充
			pLayerProperty->EnableFill((pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE));
			break;
		case eControl_Label:				// 标注
			pLayerProperty->EnableLabel((pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE));
			break;
		case eControl_Rainbow:				// 彩虹
			pLayerProperty->EnableRainbow((pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE));
			break;
		case eControl_ShowData:				// 显示数据
			pLayerProperty->EnableShowData((pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE));
			break;
		case eControl_ShowStationName:		// 显示站点名称
			pLayerProperty->EnableShowStationName((pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE));
			break;
		case eControl_Transparency:			// 图层透明度
			pLayerProperty->SetTransparency(pMFCPropertyGridProperty->GetValue().iVal);
			break;
		case eControl_Color:				// 图层颜色
			pLayerProperty->SetColor(((CMFCPropertyGridColorProperty*)pMFCPropertyGridProperty)->GetColor());
			break;
		case eControl_ShowCoordinate:		// 显示坐标
			pLayerProperty->EnableShowCoordinate(pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE);
			break;
		case eControl_Threshold_min:		// 显示阈值下限
			pLayerProperty->ThresholdMin() = (pMFCPropertyGridProperty->GetValue().iVal);
			break;
		case eControl_Threshold_max:		// 显示阈值上限
			pLayerProperty->ThresholdMax() = (pMFCPropertyGridProperty->GetValue().iVal);
			break;
		case eControl_Tension:				// 曲线张力系数
			pLayerProperty->Tension() = (pMFCPropertyGridProperty->GetValue().dblVal);
			break;
		case eControl_WindBarb:				// 显示风钩
			pLayerProperty->EnableBarb(pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE);
			break;
		case eControl_WindArrow:			// 显示风矢
			pLayerProperty->EnableArrow(pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE);
			break;
		case eControl_WindStreamLine:		// 显示流线
			pLayerProperty->EnableStreamLine(pMFCPropertyGridProperty->GetValue().boolVal == VARIANT_TRUE);
			break;
		case eControl_WindBarbStep:			// 调整风钩步长
			pLayerProperty->BarbStep() = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eControl_WindArrowStep:		// 调整风钩步长
			pLayerProperty->ArrowStep() = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eControl_WindStreamLineStep:	// 调整风钩步长
			pLayerProperty->StreamLineStep() = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eControl_LineWidth:			// 图层线宽
			pLayerProperty->LineWidth() = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eControl_LineStyle:			// 图层线型
			pLayerProperty->LineStyle() = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eControl_LogFont:				// 逻辑字体
			{
				CMFCPropertyGridFontProperty* pMFCPropertyGridFontProperty  = (CMFCPropertyGridFontProperty*)pMFCPropertyGridProperty;
				pLayerProperty->LogFont() = (*pMFCPropertyGridFontProperty->GetLogFont());
				pLayerProperty->FontColor() = pMFCPropertyGridFontProperty->GetColor();
			}
			break;
		case eControl_CurrentElementIndex:		// 当前站点要素索引
			{
				KStationData* pStationData = (KStationData*)pDataObject;
				pStationData->CurrentElementIndex() = pMFCPropertyGridProperty->GetValue().intVal - 1;
				if(pStationData->IsTarceContour())
				{
					pStationData->EnableRebuildContour();
					pStationData->BuildContours();
					this->Refresh(pLayer,TRUE);	// 列表框键盘操作该项会出错误，还没有查到原因
				}

				// 复位进度显示
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
		case eGeometry_Transparency:		// 黑板内含几何图形对象透明度
			if(pGeometry) pGeometry->SetTransparency(pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_Color:				// 黑板内含几何图形对象颜色
			if(pGeometry) pGeometry->SetColor(((CMFCPropertyGridColorProperty*)pMFCPropertyGridProperty)->GetColor());
			break;
		case eGeometry_LineWidth:			// 黑板内含几何图形对象线宽
			if(pGeometry) pGeometry->SetLineWidth(pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_LineStyle:			// 黑板内含几何图形对象线型
			if(pGeometry) pGeometry->SetLineStyle(pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_FillType:			// 黑板内含几何图形对象填充类型
			if(pGeometry) pGeometry->SetFillType(pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_FillColor:			// 黑板内含几何图形对象填充颜色
			if(pGeometry) pGeometry->SetFillColor(pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_HatchStyle:			// 黑板内含几何图形对象填充影线风格
			if(pGeometry) pGeometry->SetHatchStyle(pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_Tension:				// 黑板内含几何图形对象填充影线风格
			if(pGeometry && GeometryType_Curve == pGeometry->GetType())
			{
				((KCurve*)pGeometry)->SetTension(pMFCPropertyGridProperty->GetValue().fltVal);
			}
			break;
		case eGeometry_Legend_Width:			// 图例图形宽度
			if(pGeometry) ((KLegend*)pGeometry)->Width = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_Legend_Height:			// 图例图形高度(自动计算)
			//if(pGeometry) ((KLegend*)pGeometry)->Margin = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_Legend_Margin:			// 图例图形页边空白
			if(pGeometry) ((KLegend*)pGeometry)->Margin = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_Legend_ItemWidth:		// 图例条目宽度
			if(pGeometry) ((KLegend*)pGeometry)->ItemWidth = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_Legend_ItemHeight:		// 图例条目高度
			if(pGeometry) ((KLegend*)pGeometry)->ItemHeight = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_Legend_ItemGap:			// 图例条目间隙
			if(pGeometry) ((KLegend*)pGeometry)->ItemGap = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		case eGeometry_Legend_UnitWidth:		//  标注单位宽度
			if(pGeometry) ((KLegend*)pGeometry)->UnitWidth = (pMFCPropertyGridProperty->GetValue().intVal);
			break;
		///-----------------
		// eGeometry - end
		//-------------------------------------------------
		default:
			break;
		}

		// 刷新活动视图
		this->RefreshActiveView();

		return;
	}
	// 属性改变_地理信息
	void KPropertyDockablePane::propertyChanged_GeoInfo(LPVOID lpvData)
	{
		// 
	}
	// 属性改变_图例
	void KPropertyDockablePane::propertyChanged_Legend(LPVOID lpvData)
	{
		//-------------------------------------------------
		// 状态条
		CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

		// 图层
		KLayer* pLayer = (KLayer*)m_lpvPropertyOwner;
		if(!pLayer)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return;

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// 数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// 图层类型
		LayerType eLayerType = (LayerType)pLayer->GetType();
		if(eLayerType == LayerType_Unknown)
			return;

		// 属性网格
		CMFCPropertyGridProperty* pMFCPropertyGridProperty = (CMFCPropertyGridProperty*)lpvData;
		if(!pMFCPropertyGridProperty)
			return;

		// 取得数据
		DWORD dwData = pMFCPropertyGridProperty->GetData();
		if(dwData == 0)
			return;
		//-------------------------------------------------
		// 调色板
		KPalette* pPalette = pDataObject->GetPalette();

		// 图层数据
		LayerProperties eLayerProperty = (LayerProperties)dwData;

		if(dwData<(eLegend+0x100))
		{
			// 应用调色板后有内存泄漏
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

		case eLegend_WindBarbColor:			// 风钩颜色
			pLayerProperty->BarbColor() = (((CMFCPropertyGridColorProperty*)pMFCPropertyGridProperty)->GetColor());
			break;

		case eLegend_WindArrowColor:		// 风矢颜色
			pLayerProperty->ArrowColor() = (((CMFCPropertyGridColorProperty*)pMFCPropertyGridProperty)->GetColor());
			break;

		case eLegend_WindStreanLineColor:	// 流线颜色
			pLayerProperty->StreamLineColor() = (((CMFCPropertyGridColorProperty*)pMFCPropertyGridProperty)->GetColor());
			break;
		}

		// 刷新活动视图
		this->RefreshActiveView();

		return;
	}
	//=====================================================

//---------------------------------------------------------
}// namespace DockablePanes - end
//---------------------------------------------------------


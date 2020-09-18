========================================================================
    MICROSOFT 基础类库 : KindGis 项目概述
========================================================================


应用程序向导已为您创建了此 KindGis DLL。  此 DLL 不仅演示 
Microsoft 基础类的基本使用方法，还可作为您编写 DLL 的起点。

本文件概要介绍组成 KindGis DLL 的每个文件的内容。

KindGis.vcxproj
    这是使用应用程序向导生成的 VC++ 项目的主项目文件，
    其中包含生成该文件的 Visual C++ 
    的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

KindGis.vcxproj.filters
    这是使用“应用程序向导”生成的 VC++ 项目筛选器文件。 
    它包含有关项目文件与筛选器之间的关联信息。 在 IDE 
    中，通过这种关联，
    在特定节点下以分组形式显示具有相似扩展名的文件。
    例如，“.cpp”文件与“源文件”筛选器关联。

KindGis.cpp
    这是包含 DllMain() 定义的主 DLL 源文件。

KindGis.rc
    这是程序使用的所有 Microsoft Windows 资源的列表。  它包括 RES 
    子目录中存储的图标、位图和光标。  此文件可以直接在 Microsoft Visual C++ 
    中进行编辑。

res\KindGis.rc2
    此文件包含不在 Microsoft Visual C++ 中进行编辑的资源。  
    您应该将不可由资源编辑器编辑的所有资源放在此文件中。

KindGis.def
    此文件包含在 Microsoft Windows 中运行所必需的 DLL 的有关信息。它定义了 DLL 
    的名称和说明等参数，而且还从 DLL 导出函数。

/////////////////////////////////////////////////////////////////////////////
其他标准文件：

StdAfx.h，StdAfx.cpp
    这些文件用于生成名为 KindGis.pch 的预编译头 (PCH) 文件和名为 
    StdAfx.obj 的预编译类型文件。

Resource.h
    这是标准头文件，可用于定义新的资源 ID。
    Microsoft Visual C++ 将读取并更新此文件。

/////////////////////////////////////////////////////////////////////////////
其他注释：

应用程序向导使用“TODO:”来指示应添加或自定义的源代码部分。

/////////////////////////////////////////////////////////////////////////////
	
	0.根据向导生成...的单文档 MFC 应用程序,
	  设置KLibrary的包含文件路径、库文件路径、调试工作路径

	1.在 StdAfx.h 文件最后添加
			#include "KLibrary.h"

	2.替换 App、Frame、Document、View 的父类
		替换所有 CWinAppEx		==> KGisWinApp
		替换所有 CFrameWndEx	==> KGisFrameWnd
		替换所有 CDocument		==> KGisDocument
		替换所有 CView			==> KGisView

	3.修改 Document
	  [3.1]在文档的构造函数中添加图层
			CKindMapDoc::CKindMapDoc()
			{
				// TODO: 在此添加一次性构造代码
				// 图层属性
				KLayerProperty layerProperty;

				// 添加干净黑板
				this->AddEmptyBlackboardLayer(NULL);

				// 添加地图
				KString strShapeFile;
				strShapeFile.Format(_T("%s\\System\\Map\\市界.shp"),theApp.GetAppPath());
				{
					layerProperty.AllowDelete(false);
					layerProperty.EnableBuildClipPath(true);
					this->AddLayer(strShapeFile,&layerProperty);
					layerProperty.EnableBuildClipPath(false);
					layerProperty.AllowDelete(true);
				}
			}
			
		[3.2]重载 OnOpenDocument(...)
			BOOL CKindMapDoc::OnOpenDocument(LPCTSTR lpszPathName)
			{
				if (!KGisDocument::OnOpenDocument(lpszPathName))
					return FALSE;

				return TRUE;
			}

	4.修改 View
	  [4.1]在 OnDraw() 中添加语句
			void CKindMapView::OnDraw(CDC* pDC)
			{
				CKindMapDoc* pDoc = GetDocument();
				ASSERT_VALID(pDoc);
				if (!pDoc)
					return;

				// TODO: 在此处为本机数据添加绘制代码
				KGisView::OnDraw(pDC);
				KDib* pDib = pDoc->GetDib();
				pDib->AttachHBITMAP(pDoc->GetMapEngine()->GetHBitmap());
			}

			至此一个简单的轻量级 GIS 成功完成,只要在文档的构造函数中地图文件正确，
			编译成功运行后就可以操作了:
				双击鼠标左键			- 放大
			    双击鼠标右键			- 缩小
				按下鼠标左键并拖动	- 漫游

	  [4.2]由于在绘制图形是需要使用右键，可能不能使用用右键快捷菜单
	  [4.2]使用用右键快捷菜单
	  [4.2.0]VS2008	需要在试图的cpp文件中修改消息映射部分，添加 ON_WM_CONTEXTMENU(),VS2010不需此步骤
			BEGIN_MESSAGE_MAP(CKindMapView, KGView)
				// 标准打印命令
				ON_COMMAND(ID_FILE_PRINT, &KGisView::OnFilePrint)
				ON_COMMAND(ID_FILE_PRINT_DIRECT, &KGisView::OnFilePrint)
				ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKindMapView::OnFilePrintPreview)
					
				// (如果使用用右键快捷菜单)添加此行
				ON_WM_CONTEXTMENU()
			END_MESSAGE_MAP()

	  [4.2.1]根据需要修改 OnContextMenu() 函数代码
			void CMivadsView::OnContextMenu(CWnd* pWnd, CPoint point)
			{
				// 屏蔽掉原来的代码(弹出的是编辑菜单)
				//#ifndef SHARED_HANDLERS
				//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
				//#endif

				// 调用默认代码，或使用别的代码
				KGisView::OnContextMenu(pWnd, point);
			}
	

	  [4.3]如果需要屏幕打印，则屏蔽所有关于打印/打印预览的函数代码，添加使之执行父类同名函数的代码
			void CKindMapView::OnFilePrintPreview()
			{
				KGisView::OnFilePrintPreview();
			}
			BOOL CKindMapView::OnPreparePrinting(CPrintInfo* pInfo)
			{
				KGisView::OnPreparePrinting(pInfo);
			}
			void CKindMapView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
			{
				KGisView::OnBeginPrinting(pDC,pInfo);
			}
			void CKindMapView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
			{
				KGisView::OnEndPrinting(pDC,pInfo);
			}

	5.如果需要默认的漫游和图形编辑工具栏
	  [5.1]在 CMainFrame 的 OnCreate(...)中
	       (1)找到语句
			InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);
				修改为("MYSELF_TOOLBARS" 可以设置为你所喜欢的字符串)[也可以不修改]
			InitUserToolbars(_T("MYSELF_TOOLBARS"), uiFirstUserToolBarId, uiLastUserToolBarId);

			(2)找到语句
			DockPane(&m_wndToolBar);
				在此语句后添加下面语句
			//----------------------------------------------------
			// 用户定义的工具栏
			if(!CreateUserToolbars())
			{
				TRACE0("未能创建用户定义的工具栏\n");
				return -1;      // 未能创建
			}
			else
			{
				strCustomize;
				bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
				ASSERT(bNameValid);
				m_wndToolBarRoam.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
				m_wndToolBarBlackboard.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
				DockPaneLeftOf(&m_wndToolBar,&m_wndToolBarBlackboard);
				DockPaneLeftOf(&m_wndToolBarRoam,&m_wndToolBarBlackboard);
			}
			// 创建停靠窗口
			if(!CreateDockingWindows())
			{
				TRACE0("未能创建用户定义的停靠窗口\n");
				return -1;
			}
			//----------------------------------------------------

	  [5.2]在 CMainFrame 的构造函数中添加一下语句
			this->EnableToolbarBarRoam();
			this->EnableToolbarBarBlackboard();

	6.完成

/////////////////////////////////////////////////////////////////////////////

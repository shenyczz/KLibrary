================================================================================
MICROSOFT 基础类库: KindMap 项目概述
===============================================================================

应用程序向导已为您创建了这个 KindMap 应用程序。此应用程序不仅演示 Microsoft 基础类的基本使用方法，还可作为您编写应用程序的起点。

本文件概要介绍组成 KindMap 应用程序的每个文件的内容。

KindMap.vcxproj
这是使用应用程序向导生成的 VC++ 项目的主项目文件。 
它包含生成该文件的 Visual C++ 的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

KindMap.vcxproj.filters
    这是使用“应用程序向导”生成的 VC++ 项目筛选器文件。
    它包含有关项目文件与筛选器之间的关联信息。在 IDE 中，通过这种关联，在特定节点下以分组形式显示具有相似扩展名的文件。例如，“.cpp”文件与“源文件”筛选器关联。

KindMap.h
这是应用程序的主要头文件。它包括其他项目特定的头文件(包括 Resource.h)，并声明 CKindMapApp 应用程序类。

KindMap.cpp
这是包含应用程序类 CKindMapApp 的主要应用程序源文件。

KindMap.rc
这是程序使用的所有 Microsoft Windows 资源的列表。它包括 RES 子目录中存储的图标、位图和光标。此文件可以直接在 Microsoft Visual C++ 中进行编辑。项目资源位于 2052 中。

res\KindMap.ico
这是用作应用程序图标的图标文件。此图标包括在主要资源文件 KindMap.rc 中。

res\KindMap.rc2
此文件包含不在 Microsoft Visual C++ 中进行编辑的资源。您应该将不可由资源编辑器编辑的所有资源放在此文件中。

KindMap.reg
这是一个示例 .reg 文件，它显示了框架将为您设置的注册设置的种类。可以将它用作
将与您的应用程序一起使用的 .reg 文件，或者索性将它删除，而采用默认的 RegisterShellFileTypes 注册。


/////////////////////////////////////////////////////////////////////////////

对于主框架窗口:
项目包含标准 MFC 界面。

MainFrm.h, MainFrm.cpp
这些文件包含框架类 CMainFrame，该类派生自
CFrameWnd 并控制所有 SDI 框架功能。

/////////////////////////////////////////////////////////////////////////////

应用程序向导创建一个文档类型和一个视图:

KindMapDoc.h，KindMapDoc.cpp - 文档
这些文件包含 CKindMapDoc 类。编辑这些文件可以添加特殊文档数据并可实现文件保存和加载(通过 CKindMapDoc::Serialize)。
文档将具有下列字符串:
文件扩展名:      kdf
文件类型 ID:        KindMap.Document
主框架标题:  KindMap
文档类型名称:       KindMap
筛选器名:         KindMap Files (*.kdf)
文件的新简称: KindMap
文件类型全名: KindMap.Document

KindMapView.h，KindMapView.cpp - 文档的视图
这些文件包含 CKindMapView 类。
CKindMapView 对象用于查看 CKindMapDoc 对象。




/////////////////////////////////////////////////////////////////////////////

帮助支持:

hlp\KindMap.hhp
此文件是帮助项目文件。它包含将帮助文件编译为 .chm 文件所需的数据。

hlp\KindMap.hhc
此文件列出帮助项目的内容。

hlp\KindMap.hhk
此文件包含帮助主题的索引。

hlp\afxcore.htm
此文件包含标准 MFC 命令和屏幕对象的标准帮助主题。将您自己的帮助主题添加到此文件中。

hlp\afxprint.htm
此文件包含打印命令的帮助主题。

makehtmlhelp.bat
此文件由生成系统用来编译帮助文件。

hlp\Images\*.gif
这些是 Microsoft 基础类库标准命令的标准帮助文件主题所需的位图文件。


/////////////////////////////////////////////////////////////////////////////

其他功能:

ActiveX 控件
应用程序包括对使用 ActiveX 控件的支持。

打印及打印预览支持
应用程序向导已通过从 MFC 库调用 CView 类中的成员函数，生成了用于处理打印、打印设置和打印预览命令的代码。

MAPI 支持
生成的项目包含创建、操作、传输和存储邮件所需的代码。

Windows Sockets
应用程序支持通过 TCP/IP 网络建立通信。

/////////////////////////////////////////////////////////////////////////////

其他标准文件:

StdAfx.h，StdAfx.cpp
这些文件用于生成名为 KindMap.pch 的预编译头 (PCH) 文件和名为 StdAfx.obj 的预编译类型文件。

Resource.h
这是标准头文件，它定义新的资源 ID。
Microsoft Visual C++ 读取并更新此文件。

KindMap.manifest
	应用程序清单文件供 Windows XP 用来描述应用程序
	对特定版本并行程序集的依赖性。加载程序使用此
	信息从程序集缓存加载适当的程序集或
	从应用程序加载私有信息。应用程序清单可能为了重新分发而作为
	与应用程序可执行文件安装在相同文件夹中的外部 .manifest 文件包括，
	也可能以资源的形式包括在该可执行文件中。 
/////////////////////////////////////////////////////////////////////////////

其他注释:

应用程序向导使用“TODO:”指示应添加或自定义的源代码部分。

如果应用程序在共享的 DLL 中使用 MFC，则需要重新发布这些 MFC DLL；如果应用程序所用的语言与操作系统的当前区域设置不同，则还需要重新发布对应的本地化资源 MFC100XXX.DLL。有关这两个主题的更多信息，请参见 MSDN 文档中有关 Redistributing Visual C++ applications (重新发布 Visual C++ 应用程序)的章节。

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
			}

			至此一个简单的轻量级 GIS 成功完成,只要在文档的构造函数中地图文件正确，
			编译成功运行后就可以操作了:
				双击鼠标左键			- 放大
			    双击鼠标右键			- 缩小
				按下鼠标左键并拖动	- 漫游

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

	  [5.2]在 CMainFrame 的构造函数中添加以下语句
			this->EnableToolbarBarRoam();
			this->EnableToolbarBarBlackboard();

	6.完成

/////////////////////////////////////////////////////////////////////////////

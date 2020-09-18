// KindMapDoc.cpp : CKindMapDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "KindMap.h"
#endif

#include "KindMapDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKindMapDoc

IMPLEMENT_DYNCREATE(CKindMapDoc, KGisDocument)

BEGIN_MESSAGE_MAP(CKindMapDoc, KGisDocument)
	ON_COMMAND(ID_FILE_SEND_MAIL, &CKindMapDoc::OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, &CKindMapDoc::OnUpdateFileSendMail)
	ON_COMMAND(ID_FILE_SAVE_PICTURE, &CKindMapDoc::OnFileSavePicture)
END_MESSAGE_MAP()


// CKindMapDoc 构造/析构

KAvhrrThreshold	CKindMapDoc::s_AvhrrThreshold;	// 测试用

void XmlTest_w_CMarkup()
{
	CMarkup xml;
	xml.SetDoc(_T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"));
	xml.AddElem(_T("Equations"));
	{
		xml.IntoElem();

		xml.AddElem(_T("Lai1"));
		xml.AddAttrib(_T("LID"),_T("0501"));
		xml.AddAttrib(_T("VIX"),_T("RVI"));
		xml.AddAttrib(_T("EQU"),_T("X^2"));

		xml.AddElem(_T("Lai2"));
		xml.AddAttrib(_T("LID"),_T("0502"));
		xml.AddAttrib(_T("VIX"),_T("RVI"));
		xml.AddAttrib(_T("EQU"),_T("X^2+1"));

		xml.OutOfElem();
	}
	xml.Save(_T("e:\\temp\\Lai.xml"));
}

void XmlTest_r_CMarkup()
{
	CMarkup xml;
	xml.Load(_T("e:\\temp\\Lai.xml"));
}

//using namespace System;

CKindMapDoc::CKindMapDoc()
{
	//=====================================================

	LPCTSTR szFile = _T("E:\\TestData\\RadarProducts\\20160605\\洛阳\\R\\19\\20160605.100116.01.19.379");

	//LPCTSTR szFileOut = _T("d:\\Temp\\20160605.100116.01.19.379.bmp");
	LPCTSTR szFileOut = _T("d:\\Temp\\00.bmp");

	BOOL b, b1, b2, b3, b4, b5;

	KRpgData rpgData;
	b1 = rpgData.LoadData(szFile);
	b2 = rpgData.BuildImage();
	rpgData.SaveImage(szFileOut);

	int x = 0;


	//=====================================================

	this->GetMapEngine()->EnableDrawCrossHairs(TRUE);
	this->GetMapEngine()->SetCustomDataBound(110,31,117,37);

	// 添加干净黑板
	this->AddEmptyBlackboardLayer(NULL);

	// 图层属性
	KLayerProperty layerProperty;

	KString strShapeFile;
	strShapeFile.Format(_T("%s\\System\\Map\\市界.shp"),theApp.GetAppPath());
	//strShapeFile.Format(_T("%s\\System\\Map\\zhengzhou.shp"),theApp.GetAppPath());
	layerProperty.AllowDelete(false);
	layerProperty.AllowMove(false);
	layerProperty.EnableBuildClipPath(true);
	this->AddLayer(strShapeFile,&layerProperty);
	layerProperty.Reset();

	//strShapeFile.Format(_T("%s\\System\\Map\\县界.shp"),theApp.GetAppPath());
	//layerProperty.EnableVisible(false);
	//this->AddLayer(strShapeFile,&layerProperty);
	//layerProperty.EnableVisible(true);

	strShapeFile.Format(_T("%s\\System\\Map\\市标注.shp"),theApp.GetAppPath());
	layerProperty.EnableVisible(false);
	this->AddLayer(strShapeFile,&layerProperty);
	layerProperty.EnableVisible(true);

	return;
}

CKindMapDoc::~CKindMapDoc()
{
}

BOOL CKindMapDoc::OnNewDocument()
{
	if (!KGisDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}


// CKindMapDoc 序列化

void CKindMapDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CKindMapDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CKindMapDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CKindMapDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CKindMapDoc 诊断

#ifdef _DEBUG
void CKindMapDoc::AssertValid() const
{
	KGisDocument::AssertValid();
}

void CKindMapDoc::Dump(CDumpContext& dc) const
{
	KGisDocument::Dump(dc);
}
#endif //_DEBUG

// CKindMapDoc 命令

// 保存屏幕为图片
void CKindMapDoc::OnFileSavePicture()
{
	HBITMAP hBitmap = this->GetMapEngine()->GetHBitmap();
	KImage* pImage = (KImage*)Bitmap::FromHBITMAP(hBitmap,NULL);

	pImage->Save(_T("e:\\temp\\1.png"));

	_delete(pImage);

	// TODO: 在此添加命令处理程序代码
	/*
	CLightningDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	KGisView::OnDraw(pDC);
	KDib* pDib = pDoc->GetDib();
	pDib->AttachHBITMAP(pDoc->GetMapEngine()->GetHBitmap());
	*/
}

int readXmlCfg(const char *str)
{
	if (str == NULL) 
	{
		 printf("please input a path\n");
		 return -1;
	}

	char strTemp[256] = "";

	TiXmlDocument xml(str);
	if(!xml.LoadFile())
	{
		printf("invalid xml format");
		return -1;
	}

	//根节点元素
	TiXmlElement* root = xml.FirstChildElement("configuration");
	
	//标签名称为"log"的节点元素
	TiXmlElement* log = root->FirstChildElement("log");
	if(NULL != log)//不为空时
	{
		if(NULL == log->Attribute("path"))
		{
			fprintf(stderr, "get [%s] failed\n", "path");
			return -1;
		}
		else
		{
			printf("path = %s\n", log->Attribute("path"));
			sprintf(strTemp,"path = %s\n", log->Attribute("path"));
		}

		if(NULL == log->Attribute("level"))
		{
			fprintf(stderr, "get [%s] failed\n", "level");
			return -1;
		}
		else
		{
			printf("level = %s\n", log->Attribute("level"));
		}
	}
	else
	{
		fprintf(stderr, "get [%s] failed\n", "log");
		return -1;
	}

	//标签名称为"oracle"的节点元素
	TiXmlElement* rdb = root->FirstChildElement("oracle");
	if(NULL != rdb)
	{
		if(NULL == rdb->Attribute("connstr"))
		{
			fprintf(stderr, "get [%s] failed\n", "connstr");
			return -1;
		}
		else
		{
			printf("connstr=%s\n", rdb->Attribute("connstr"));
			sprintf(strTemp,"connstr=%s\n", rdb->Attribute("connstr"));
		}
	}
	else
	{
		fprintf(stderr, "get [%s] failed\n", "oracle");
		return -1;
	}

	return 0;
}
int writeXmlFile()
{
	TiXmlDocument *writeDoc = new TiXmlDocument; //xml文档指针
	
	//文档格式声明
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "UTF-8", "yes");
	writeDoc->LinkEndChild(decl); //写入文档

	//文档注释
	TiXmlComment* comment = new TiXmlComment("文档注释");
	writeDoc->LinkEndChild(comment);

	int n = 3;	//父节点个数

	TiXmlElement *RootElement = new TiXmlElement("Info");//根元素
	RootElement->SetAttribute("num", n); //属性
	//RootElement->LinkEndChild(comment);
	writeDoc->LinkEndChild(RootElement);
	
	for(int i=0; i<n; i++)//n个父节点
	{
		TiXmlElement *StuElement = new TiXmlElement("Stu");//Stu
		//设置属性
		StuElement->SetAttribute("class","A");
		if(2 == i)
		{
				StuElement->SetAttribute("class","B");
		}
		
		StuElement->SetAttribute("id",i+1);
		StuElement->SetAttribute("flag", (i+1)*10);
		RootElement->LinkEndChild(StuElement);//父节点写入文档
	
		//姓名
		TiXmlElement *nameElement = new TiXmlElement("name");
		StuElement->LinkEndChild(nameElement);

		TiXmlText *nameContent = new TiXmlText("mike-麦克");
		nameElement->LinkEndChild(nameContent);
		
		//分数
		TiXmlElement *scoreElement = new TiXmlElement("score");
		StuElement->LinkEndChild(scoreElement);

		TiXmlText *scoreContent = new TiXmlText("88");
		scoreElement->LinkEndChild(scoreContent);
		
		//城市
		TiXmlElement *cityElement = new TiXmlElement("city");
		StuElement->LinkEndChild(cityElement);

		TiXmlText *cityContent = new TiXmlText("Shenzhen");
		cityElement->LinkEndChild(cityContent);
		
	}
	
	writeDoc->SaveFile("e:/temp/stu_info.xml");
	delete writeDoc;
	
	return 1;
}
int readXmlFile()
{
	TiXmlDocument mydoc("e:/temp/stu_info.xml");//xml文档对象
	bool loadOk=mydoc.LoadFile();//加载文档
	if(!loadOk)
	{
		cout<<_T("could not load the test file.Error:")<<mydoc.ErrorDesc()<<endl;
		exit(1);
	}

	TiXmlElement *RootElement=mydoc.RootElement();	//根元素, Info
	cout<< "[root name]" << RootElement->Value() <<"\n";
	
	TiXmlElement *pEle=RootElement;

	//遍历该结点
	for(TiXmlElement *StuElement = pEle->FirstChildElement();//第一个子元素
		StuElement != NULL;
		StuElement = StuElement->NextSiblingElement())//下一个兄弟元素
	{
		// StuElement->Value() 节点名称
		cout<< StuElement->Value() <<" ";
		TiXmlAttribute *pAttr=StuElement->FirstAttribute();//第一个属性
		
		while( NULL != pAttr) //输出所有属性
		{
			cout<<pAttr->Name()<<":"<<pAttr->Value()<<" ";
			pAttr=pAttr->Next();
		}
		cout<<endl;
		
		//输出子元素的值
		for(TiXmlElement *sonElement=StuElement->FirstChildElement();
		sonElement;
		sonElement=sonElement->NextSiblingElement())
		{
			cout<<sonElement->FirstChild()->Value()<<endl;
		}
	}
	
	return 1;
}

void CKindMapDoc::Test()
{
	writeXmlFile();
	//readXmlCfg("e:/temp/sms_aoc_cfg.xml");
	return;
}



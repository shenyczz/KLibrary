// KindMapDoc.cpp : CKindMapDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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


// CKindMapDoc ����/����

KAvhrrThreshold	CKindMapDoc::s_AvhrrThreshold;	// ������

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

	LPCTSTR szFile = _T("E:\\TestData\\RadarProducts\\20160605\\����\\R\\19\\20160605.100116.01.19.379");

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

	// ��Ӹɾ��ڰ�
	this->AddEmptyBlackboardLayer(NULL);

	// ͼ������
	KLayerProperty layerProperty;

	KString strShapeFile;
	strShapeFile.Format(_T("%s\\System\\Map\\�н�.shp"),theApp.GetAppPath());
	//strShapeFile.Format(_T("%s\\System\\Map\\zhengzhou.shp"),theApp.GetAppPath());
	layerProperty.AllowDelete(false);
	layerProperty.AllowMove(false);
	layerProperty.EnableBuildClipPath(true);
	this->AddLayer(strShapeFile,&layerProperty);
	layerProperty.Reset();

	//strShapeFile.Format(_T("%s\\System\\Map\\�ؽ�.shp"),theApp.GetAppPath());
	//layerProperty.EnableVisible(false);
	//this->AddLayer(strShapeFile,&layerProperty);
	//layerProperty.EnableVisible(true);

	strShapeFile.Format(_T("%s\\System\\Map\\�б�ע.shp"),theApp.GetAppPath());
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

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}


// CKindMapDoc ���л�

void CKindMapDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CKindMapDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
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

// ������������֧��
void CKindMapDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
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

// CKindMapDoc ���

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

// CKindMapDoc ����

// ������ĻΪͼƬ
void CKindMapDoc::OnFileSavePicture()
{
	HBITMAP hBitmap = this->GetMapEngine()->GetHBitmap();
	KImage* pImage = (KImage*)Bitmap::FromHBITMAP(hBitmap,NULL);

	pImage->Save(_T("e:\\temp\\1.png"));

	_delete(pImage);

	// TODO: �ڴ���������������
	/*
	CLightningDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
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

	//���ڵ�Ԫ��
	TiXmlElement* root = xml.FirstChildElement("configuration");
	
	//��ǩ����Ϊ"log"�Ľڵ�Ԫ��
	TiXmlElement* log = root->FirstChildElement("log");
	if(NULL != log)//��Ϊ��ʱ
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

	//��ǩ����Ϊ"oracle"�Ľڵ�Ԫ��
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
	TiXmlDocument *writeDoc = new TiXmlDocument; //xml�ĵ�ָ��
	
	//�ĵ���ʽ����
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "UTF-8", "yes");
	writeDoc->LinkEndChild(decl); //д���ĵ�

	//�ĵ�ע��
	TiXmlComment* comment = new TiXmlComment("�ĵ�ע��");
	writeDoc->LinkEndChild(comment);

	int n = 3;	//���ڵ����

	TiXmlElement *RootElement = new TiXmlElement("Info");//��Ԫ��
	RootElement->SetAttribute("num", n); //����
	//RootElement->LinkEndChild(comment);
	writeDoc->LinkEndChild(RootElement);
	
	for(int i=0; i<n; i++)//n�����ڵ�
	{
		TiXmlElement *StuElement = new TiXmlElement("Stu");//Stu
		//��������
		StuElement->SetAttribute("class","A");
		if(2 == i)
		{
				StuElement->SetAttribute("class","B");
		}
		
		StuElement->SetAttribute("id",i+1);
		StuElement->SetAttribute("flag", (i+1)*10);
		RootElement->LinkEndChild(StuElement);//���ڵ�д���ĵ�
	
		//����
		TiXmlElement *nameElement = new TiXmlElement("name");
		StuElement->LinkEndChild(nameElement);

		TiXmlText *nameContent = new TiXmlText("mike-���");
		nameElement->LinkEndChild(nameContent);
		
		//����
		TiXmlElement *scoreElement = new TiXmlElement("score");
		StuElement->LinkEndChild(scoreElement);

		TiXmlText *scoreContent = new TiXmlText("88");
		scoreElement->LinkEndChild(scoreContent);
		
		//����
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
	TiXmlDocument mydoc("e:/temp/stu_info.xml");//xml�ĵ�����
	bool loadOk=mydoc.LoadFile();//�����ĵ�
	if(!loadOk)
	{
		cout<<_T("could not load the test file.Error:")<<mydoc.ErrorDesc()<<endl;
		exit(1);
	}

	TiXmlElement *RootElement=mydoc.RootElement();	//��Ԫ��, Info
	cout<< "[root name]" << RootElement->Value() <<"\n";
	
	TiXmlElement *pEle=RootElement;

	//�����ý��
	for(TiXmlElement *StuElement = pEle->FirstChildElement();//��һ����Ԫ��
		StuElement != NULL;
		StuElement = StuElement->NextSiblingElement())//��һ���ֵ�Ԫ��
	{
		// StuElement->Value() �ڵ�����
		cout<< StuElement->Value() <<" ";
		TiXmlAttribute *pAttr=StuElement->FirstAttribute();//��һ������
		
		while( NULL != pAttr) //�����������
		{
			cout<<pAttr->Name()<<":"<<pAttr->Value()<<" ";
			pAttr=pAttr->Next();
		}
		cout<<endl;
		
		//�����Ԫ�ص�ֵ
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



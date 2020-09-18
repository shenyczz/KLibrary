// DlgLandmarkCalibrate.cpp : 实现文件
//

#include "stdafx.h"
#include "KindMap.h"
#include "DlgLandmarkCalibrate.h"
#include "afxdialogex.h"


// CDlgLandmarkCalibrate 对话框

IMPLEMENT_DYNAMIC(CDlgLandmarkCalibrate, CDialogEx)

CDlgLandmarkCalibrate::CDlgLandmarkCalibrate(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLandmarkCalibrate::IDD, pParent)
{
	m_pView = NULL;
}

CDlgLandmarkCalibrate::~CDlgLandmarkCalibrate()
{
}

void CDlgLandmarkCalibrate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLandmarkCalibrate, CDialogEx)
END_MESSAGE_MAP()


// CDlgLandmarkCalibrate 消息处理程序

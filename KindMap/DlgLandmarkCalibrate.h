#pragma once


// CDlgLandmarkCalibrate 对话框

class CDlgLandmarkCalibrate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLandmarkCalibrate)

public:
	CDlgLandmarkCalibrate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLandmarkCalibrate();

// 对话框数据
	enum { IDD = IDD_DIALOG_LANDMARK_CALIBRATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CView* m_pView;
};

#pragma once


// CDlgLandmarkCalibrate �Ի���

class CDlgLandmarkCalibrate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLandmarkCalibrate)

public:
	CDlgLandmarkCalibrate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLandmarkCalibrate();

// �Ի�������
	enum { IDD = IDD_DIALOG_LANDMARK_CALIBRATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CView* m_pView;
};

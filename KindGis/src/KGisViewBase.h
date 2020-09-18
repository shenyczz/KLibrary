#pragma once

namespace Framework
{
//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

namespace Framework
{

	// KGisViewBase ��ͼ

	class KIND_EXT_CLASS KGisViewBase : public KPrintView
	{
		DECLARE_DYNCREATE(KGisViewBase)

	protected:
		KGisViewBase();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
		virtual ~KGisViewBase();

	public:
		virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	#ifdef _DEBUG
		virtual void AssertValid() const;
	#ifndef _WIN32_WCE
		virtual void Dump(CDumpContext& dc) const;
	#endif
	#endif

	protected:
		virtual void OnInitialUpdate();
		virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

	protected:
		afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
		afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

		// ���ͼ�� WM_USER_LAYER_ADD
		sfx_msg LRESULT OnAddLayer(WPARAM wParam, LPARAM lParam);
		// �Ƴ�ͼ�� WM_USER_LAYER_REMOVE
		sfx_msg LRESULT OnRemoveLayer(WPARAM wParam, LPARAM lParam);

		// ���β���
		sfx_msg void OnRoamOperate(UINT id);
		sfx_msg void OnRoamOperateUpdate(CCmdUI* pCmdUI);

		DECLARE_MESSAGE_MAP()

	public:
		// ˢ��
		virtual void Refresh(BOOL bErase = TRUE);

	public:
		bool IsMouseDraging() { return m_bMouseDraging; }
		bool IsShowGisInfo() { return m_bShowGisInfo; }

	protected:
		// ���ͼ��(����������������)
		virtual void AddLayerHappened();
		// �Ƴ�ͼ��(����������������)
		virtual void RemoveLayerHappened();

	protected:
		// �Ŵ�
		virtual void ZoomIn();
		// ��С
		virtual void ZoomOut();
		// ����
		virtual void ZoomPan();
		// ��λ
		virtual void ZoomReset();
		// ˢ��(������Ҫ�����������޸�)
		virtual void ZoomRefresh();
		// ȡ��
		virtual void ZoomCancel();

		// �ر�У׼
		virtual void LandmarkCalibrate();

	protected:
		// ��Ӧ WM_USER_LANDMARK_CALIBRATE
		sfx_msg LRESULT OnLandmarkCalibrate(WPARAM wParam,LPARAM lParam);
		// ��Ӧ WM_USER_LANDMARK_CALIBRATE_STEP
		sfx_msg LRESULT OnLandmarkCalibrateStep(WPARAM wParam,LPARAM lParam);

	protected:
		HCURSOR         m_hCurOld;  	//ԭ�������
		HCURSOR         m_hCurNew;		//���ڹ����
		HCURSOR         m_hCurDefault;	//ȱʡ�����
		HCURSOR         m_hCurAnimate;	//�����ȴ������

	protected:
		int m_iGisTool;
		bool m_bShowGisInfo;
		bool m_bMouseDraging;
	public:
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------


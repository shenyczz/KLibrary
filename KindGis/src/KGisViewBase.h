#pragma once

namespace Framework
{
//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

namespace Framework
{

	// KGisViewBase 视图

	class KIND_EXT_CLASS KGisViewBase : public KPrintView
	{
		DECLARE_DYNCREATE(KGisViewBase)

	protected:
		KGisViewBase();           // 动态创建所使用的受保护的构造函数
		virtual ~KGisViewBase();

	public:
		virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
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

		// 添加图层 WM_USER_LAYER_ADD
		sfx_msg LRESULT OnAddLayer(WPARAM wParam, LPARAM lParam);
		// 移除图层 WM_USER_LAYER_REMOVE
		sfx_msg LRESULT OnRemoveLayer(WPARAM wParam, LPARAM lParam);

		// 漫游操作
		sfx_msg void OnRoamOperate(UINT id);
		sfx_msg void OnRoamOperateUpdate(CCmdUI* pCmdUI);

		DECLARE_MESSAGE_MAP()

	public:
		// 刷星
		virtual void Refresh(BOOL bErase = TRUE);

	public:
		bool IsMouseDraging() { return m_bMouseDraging; }
		bool IsShowGisInfo() { return m_bShowGisInfo; }

	protected:
		// 添加图层(必须在派生类重载)
		virtual void AddLayerHappened();
		// 移除图层(必须在派生类重载)
		virtual void RemoveLayerHappened();

	protected:
		// 放大
		virtual void ZoomIn();
		// 缩小
		virtual void ZoomOut();
		// 漫游
		virtual void ZoomPan();
		// 复位
		virtual void ZoomReset();
		// 刷新(根据需要在派生类中修改)
		virtual void ZoomRefresh();
		// 取消
		virtual void ZoomCancel();

		// 地标校准
		virtual void LandmarkCalibrate();

	protected:
		// 响应 WM_USER_LANDMARK_CALIBRATE
		sfx_msg LRESULT OnLandmarkCalibrate(WPARAM wParam,LPARAM lParam);
		// 响应 WM_USER_LANDMARK_CALIBRATE_STEP
		sfx_msg LRESULT OnLandmarkCalibrateStep(WPARAM wParam,LPARAM lParam);

	protected:
		HCURSOR         m_hCurOld;  	//原来光标句柄
		HCURSOR         m_hCurNew;		//现在光标句柄
		HCURSOR         m_hCurDefault;	//缺省光标句柄
		HCURSOR         m_hCurAnimate;	//动画等待光标句柄

	protected:
		int m_iGisTool;
		bool m_bShowGisInfo;
		bool m_bMouseDraging;
	public:
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------


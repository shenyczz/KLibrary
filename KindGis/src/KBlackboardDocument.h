#pragma once

#include "KAgmrsDataDocument.h"
#include "KLayerProperty.h"

namespace Framework
{

	// KBlackboardDocument 文档

	class KIND_EXT_CLASS KBlackboardDocument : public KAgmrsDataDocument
	{
		DECLARE_DYNCREATE(KBlackboardDocument)

	public:
		KBlackboardDocument();
		virtual ~KBlackboardDocument();
	#ifndef _WIN32_WCE
		virtual void Serialize(CArchive& ar);   // 为文档 I/O 重写
	#endif
	#ifdef _DEBUG
		virtual void AssertValid() const;
	#ifndef _WIN32_WCE
		virtual void Dump(CDumpContext& dc) const;
	#endif
	#endif

	protected:
		virtual BOOL OnNewDocument();

		DECLARE_MESSAGE_MAP()

	public:
		// 添加空黑板图层
		BOOL AddEmptyBlackboardLayer(KLayerProperty* pProperty);

	public:
		// 添加点
		virtual BOOL AddPoint(LPVOID lpvData);
		// 添加线段
		virtual BOOL AddLine(LPVOID lpvData);
		// 添加矩形
		virtual BOOL AddRect(LPVOID lpvData);
		// 添加椭圆
		virtual BOOL AddEllipse(LPVOID lpvData);
		// 添加折线
		virtual BOOL AddPolyline(LPVOID lpvData);
		// 添加多边形
		virtual BOOL AddPolygon(LPVOID lpvData);
		// 添加曲线
		virtual BOOL AddCurve(LPVOID lpvData);
		// 添加闭合曲线
		virtual BOOL AddCloseCurve(LPVOID lpvData);
		// 添加标注
		virtual BOOL AddLabel(LPVOID lpvData);
		// 添加图层图例
		virtual BOOL AddLayerLegend(KLayer* pLayerCurrent);
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

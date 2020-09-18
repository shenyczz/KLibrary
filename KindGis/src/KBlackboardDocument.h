#pragma once

#include "KAgmrsDataDocument.h"
#include "KLayerProperty.h"

namespace Framework
{

	// KBlackboardDocument �ĵ�

	class KIND_EXT_CLASS KBlackboardDocument : public KAgmrsDataDocument
	{
		DECLARE_DYNCREATE(KBlackboardDocument)

	public:
		KBlackboardDocument();
		virtual ~KBlackboardDocument();
	#ifndef _WIN32_WCE
		virtual void Serialize(CArchive& ar);   // Ϊ�ĵ� I/O ��д
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
		// ��ӿպڰ�ͼ��
		BOOL AddEmptyBlackboardLayer(KLayerProperty* pProperty);

	public:
		// ��ӵ�
		virtual BOOL AddPoint(LPVOID lpvData);
		// ����߶�
		virtual BOOL AddLine(LPVOID lpvData);
		// ��Ӿ���
		virtual BOOL AddRect(LPVOID lpvData);
		// �����Բ
		virtual BOOL AddEllipse(LPVOID lpvData);
		// �������
		virtual BOOL AddPolyline(LPVOID lpvData);
		// ��Ӷ����
		virtual BOOL AddPolygon(LPVOID lpvData);
		// �������
		virtual BOOL AddCurve(LPVOID lpvData);
		// ��ӱպ�����
		virtual BOOL AddCloseCurve(LPVOID lpvData);
		// ��ӱ�ע
		virtual BOOL AddLabel(LPVOID lpvData);
		// ���ͼ��ͼ��
		virtual BOOL AddLayerLegend(KLayer* pLayerCurrent);
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

// KHistogramWnd.cpp : 实现文件
//

#include "stdafx.h"

#include "KHistogramWnd.h"

// KHistogramWnd
namespace Gui
{

	IMPLEMENT_DYNAMIC(KHistogramWnd, CWnd)

	KHistogramWnd::KHistogramWnd()
	{
		Init();
	}

	KHistogramWnd::~KHistogramWnd()
	{
		Cleanup();
	}

	BEGIN_MESSAGE_MAP(KHistogramWnd, CWnd)
		ON_WM_PAINT()
		ON_WM_MOUSEMOVE()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_SIZE()
		ON_WM_ERASEBKGND()
		ON_WM_CREATE()
	END_MESSAGE_MAP()

	// KHistogramWnd 消息处理程序

	int KHistogramWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CWnd::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  在此添加您专用的创建代码

		return 0;
	}
	void KHistogramWnd::OnPaint()
	{
		CPaintDC dc(this); // device context for painting
		// TODO: 在此处添加消息处理程序代码
		// 不为绘图消息调用 CWnd::OnPaint()
		CDC* pDC = &dc;
		pDC->SetBkMode(TRANSPARENT);

		if(!m_bValueCounted)
		{
			ComputeDataValueCount();
		}

		if(m_bValueCounted)
		{
			// 计算绘制矩形
			ComputeDrawRect();
			// 绘制坐标系
			DrawCoordinate(pDC);
			// 绘制数据
			DrawData(pDC);
			// 绘制上下限
			DrawUpDn(pDC);
		}

	}
	void KHistogramWnd::OnMouseMove(UINT nFlags, CPoint point)
	{
		int xdn,xup,yy;
		World2DrawRect(m_iUpValue,xup,yy);
		World2DrawRect(m_iDnValue,xdn,yy);

		CRect rectDraw(m_rectDraw);
		rectDraw.InflateRect(3,1);

		if(m_iDragState !=0 )
		{
			if(m_iDragState == eDragStateDown)	// 正在拖动下限
			{
				// 下限 < 上限
				if(point.x < v2x(m_iUpValue))
				{
					// 更改下限
					m_iDnValue = x2v(point.x);
				}
				else
				{
					// 下限拖过上限
					m_iDnValue = m_iUpValue - 1;

					// 重设鼠标位置
					point.x = v2x(m_iDnValue);
				}

				if(point.x < v2x(0))
				{
					m_iDnValue = 0;
					point.x = v2x(m_iDnValue);
				}
			}
			else	// 正在拖动上限
			{

				// 上限 > 下限
				if(point.x > v2x(m_iDnValue))
				{
					// 更改上限
					m_iUpValue = x2v(point.x);
				}
				else
				{
					// 上限拖过下限
					m_iUpValue = m_iDnValue + 1;

					// 重设鼠标位置
					point.x = v2x(m_iUpValue);
				}

				if(point.x > v2x(m_iMaxValue-1))
				{
					m_iUpValue = m_iMaxValue-1;
					point.x = v2x(m_iMaxValue-1);
				}
			}

			::SetCursor(m_hCursorDrag);

			// WM_SFX_HISTOGRAM_DRAG
			this->GetParent()->SendMessage(SFX_MSG_HISTOGRAM_DRAG,0,(LPARAM)this);

			//

			// 重绘制直方图
			CRect rect = m_rectClient;
			rect.DeflateRect(0,0,15,20);
			InvalidateRect(rect,TRUE);
		}
		if(rectDraw.PtInRect(point))
		{
			if( (point.x >= xdn-1 && point.x <= xdn+1) || (point.x >= xup-1 && point.x <= xup+1) )
			{
				// 更改光标
				::SetCursor(m_hCursorDrag);
			}
		}

		CWnd::OnMouseMove(nFlags, point);
	}
	void KHistogramWnd::OnLButtonDown(UINT nFlags, CPoint point)
	{
		int xdn,xup,yy;
		World2DrawRect(m_iUpValue,xup,yy);
		World2DrawRect(m_iDnValue,xdn,yy);

		CRect rectDraw(m_rectDraw);
		rectDraw.InflateRect(3,1);

		if(rectDraw.PtInRect(point))
		{
			if( (point.x >= xdn-1 && point.x <= xdn+1))
			{
				// 拖动下限
				m_iDragState = eDragStateDown;
				// 更改光标
				::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
				//捕获鼠标
				::SetCapture(GetSafeHwnd());
			}
			else if((point.x >= xup-1 && point.x <= xup+1) )
			{
				// 拖动上限
				m_iDragState = eDragStateUp;
				// 更改光标
				::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
				//捕获鼠标
				::SetCapture(GetSafeHwnd());
			}
		}

		CWnd::OnLButtonDown(nFlags, point);
	}
	void KHistogramWnd::OnLButtonUp(UINT nFlags, CPoint point)
	{
		if(m_iDragState !=0 )
		{
			m_iDragState = 0;
			//释放鼠标捕获
			::ReleaseCapture();
		}

		CWnd::OnLButtonUp(nFlags, point);
	}
	void KHistogramWnd::OnSize(UINT nType, int cx, int cy)
	{
		CWnd::OnSize(nType, cx, cy);

		// TODO: 在此处添加消息处理程序代码
		GetClientRect(&m_rectClient);
	}
	BOOL KHistogramWnd::OnEraseBkgnd(CDC* pDC)
	{
		pDC->FillSolidRect(&m_rectClient,m_clrBack);
		return TRUE;
	}
	void KHistogramWnd::PreSubclassWindow()
	{
		GetClientRect(&m_rectClient);
		ModifyStyle(0,SS_NOTIFY | SS_SUNKEN);
		CWnd::PreSubclassWindow();
	}
	BOOL KHistogramWnd::PreCreateWindow(CREATESTRUCT& cs)
	{
		return CWnd::PreCreateWindow(cs);
	}

	// 初始化
	void KHistogramWnd::Init()
	{
		m_lpData = 0;
		m_iDragState = 0;

		m_iMinValue = 0;
		m_iMaxValue = 255;

		m_iDnValue = m_iMinValue;
		m_iUpValue = m_iMaxValue;

		m_lpCount = 0;
		if(m_iMaxValue>0)
		{
			m_lpCount = new int[m_iMaxValue+1];
			memset(m_lpCount,0,sizeof(int)*(m_iMaxValue+1));
		}

		m_iMaxCount = 0;

		m_bValueCounted = FALSE;

		m_clrHist = RGB(0,0,0);
		m_clrBack = RGB(255,255,255);

		m_hCursorDrag = ::LoadCursor(NULL,IDC_SIZEWE);
	}

	// 清除垃圾
	void KHistogramWnd::Cleanup()
	{
		if(m_lpCount) { delete[] m_lpCount; m_lpCount=0; }

		::DestroyCursor(m_hCursorDrag);
	}

	// 计算数据各个数据值的计数
	void KHistogramWnd::ComputeDataValueCount()
	{
		if(!m_lpData)
		{
			m_bValueCounted = FALSE;
			return;
		}

		memset(m_lpCount,0,sizeof(int)*(m_iMaxValue+1));

		// 图像每行的字节数
		int iLineBytes = WIDTHBYTES(m_iWidth*8);

		// 计算各个数据值的计数
		for(int i=0;i<m_iHeight;i++)
		{
			for(int j=0;j<m_iWidth;j++)
			{
				LPBYTE lpSrc = m_lpData + iLineBytes * i + j;
				//TRACE("%d\n",*lpSrc);
				m_lpCount[(int)(BYTE)(*lpSrc)]++;
			}
		}

		m_bValueCounted = TRUE;
	}

	// 计算绘制区域矩形
	void KHistogramWnd::ComputeDrawRect()
	{
		m_rectDraw = m_rectClient;
		m_rectDraw.DeflateRect(50,15,15,20);
	}

	// 绘制坐标
	void KHistogramWnd::DrawCoordinate(CDC* pDC)
	{
		int i;
		//--------------------------------------------
		// GDI Initialize
		CPen* pOldPen = NULL;
		CPen pen(PS_SOLID,1,RGB(192,0,0));	// 画笔

		pOldPen = pDC->SelectObject(&pen);

		CFont font;
		font.CreatePointFont(90,_T("宋体"));	// 字体
		CFont* pOldFont = pDC->SelectObject(&font);

		UINT uOldTestAlign = pDC->SetTextAlign(TA_CENTER);
		pDC->SetTextColor(RGB(192,0,0));
		//--------------------------------------------
		// 坐标点
		CPoint ptTopLeft = m_rectDraw.TopLeft();
		CPoint ptBottomRight = m_rectDraw.BottomRight();

		// 绘制坐标轴
		pDC->MoveTo(ptTopLeft.x,ptTopLeft.y-10);
		pDC->LineTo(ptTopLeft.x,ptBottomRight.y);		// 垂直轴
		pDC->LineTo(ptBottomRight.x+10,ptBottomRight.y);	// 水平轴

		// 绘制X箭头
		pDC->MoveTo(ptTopLeft.x-2,ptTopLeft.y-4);
		pDC->LineTo(ptTopLeft.x,ptTopLeft.y-10);
		pDC->LineTo(ptTopLeft.x+2,ptTopLeft.y-4);
		// 绘制Y箭头
		pDC->MoveTo(ptBottomRight.x+10-6,ptBottomRight.y-2);
		pDC->LineTo(ptBottomRight.x+10,ptBottomRight.y);
		pDC->LineTo(ptBottomRight.x+10-6,ptBottomRight.y+2);

		// 绘制X刻度
		for(i=ptTopLeft.x;i<ptBottomRight.x;i+=5)
		{
			int kk = i-ptTopLeft.x;

			if(kk & 0x01)
			{// 5 15 25 ...
				pDC->MoveTo(i,ptBottomRight.y);
				pDC->LineTo(i,ptBottomRight.y+2);
			}
			else
			{// 0 10 20 30 ...
				pDC->MoveTo(i,ptBottomRight.y);
				pDC->LineTo(i,ptBottomRight.y+4);
			}
		}

		// 绘制Y刻度
		for(i=ptBottomRight.y;i>ptTopLeft.y;i-=5)
		{
			int kk = i-ptBottomRight.y;

			if(kk & 0x01)
			{// 5 15 25 ...
				pDC->MoveTo(ptTopLeft.x,i);
				pDC->LineTo(ptTopLeft.x-2,i);
			}
			else
			{// 0 10 20 30 ...
				pDC->MoveTo(ptTopLeft.x,i);
				pDC->LineTo(ptTopLeft.x-4,i);
			}
		}

		// 绘制X刻度值
		TCHAR szCoord[32] = _T("");
		float dx = (float)m_rectDraw.Width() / m_iMaxValue;
		for(i=0;i<=m_iMaxValue;i++)
		{
			int x = ptTopLeft.x + (int)(i*dx);
			int y = ptBottomRight.y;

			int v = 0;

			if(i==m_iMaxValue-1)
			{
				x = ptTopLeft.x + m_rectDraw.Width();
				_stprintf(szCoord,_T("%d"),m_iMaxValue);
				pDC->TextOut(x,y+5,szCoord);
				break;
			}
			else if((i%(20*m_iMaxValue/255)) ==0)	//
			{
				_stprintf(szCoord,_T("%d"),i);
				pDC->TextOut(x,y+5,szCoord);
			}
		}

		// 绘制y刻度值
		m_iMaxCount = 0;
		for(i=m_iDnValue;i<m_iUpValue;i++)
		{
			if(m_lpCount[i]>m_iMaxCount)
			{
				m_iMaxCount = m_lpCount[i];
			}
		}

		//pDC->SetBkMode(OPAQUE);
		pDC->SetTextAlign(TA_RIGHT | TA_BASELINE);
		_stprintf(szCoord,_T("%d"),m_iMaxCount);
		pDC->TextOut(ptTopLeft.x-5,ptTopLeft.y,szCoord);
		TRACE(_T("%s\n"),szCoord);
		//pDC->SetBkMode(TRANSPARENT);

		//--------------------------------------------
		// GDI Cleanup
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();

		pDC->SelectObject(pOldFont);
		font.DeleteObject();

		pDC->SetTextAlign(uOldTestAlign);
		//--------------------------------------------
	}

	// 绘制上下限
	void KHistogramWnd::DrawUpDn(CDC* pDC)
	{
		int x,y;
		//--------------------------------------------
		// GDI Initialize
		CPen* pOldPen = NULL;
		CPen pen(PS_SOLID,1,RGB(128,0,240));	// 画笔

		pOldPen = pDC->SelectObject(&pen);
		//--------------------------------------------
		// 坐标点
		CPoint ptTopLeft = m_rectDraw.TopLeft();
		CPoint ptBottomRight = m_rectDraw.BottomRight();

		// 下限
		World2DrawRect(m_iDnValue,x,y);
		pDC->MoveTo(x,ptBottomRight.y-1);
		pDC->LineTo(x,ptTopLeft.y);

		// 上限
		World2DrawRect(m_iUpValue,x,y);
		pDC->MoveTo(x,ptBottomRight.y-1);
		pDC->LineTo(x,ptTopLeft.y);
		//--------------------------------------------
		// GDI Cleanup
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}

	// 绘制数据
	void KHistogramWnd::DrawData(CDC* pDC)
	{
		int x,y;

		if(m_iMaxCount<=0) return;
		//--------------------------------------------
		// GDI Initialize
		CPen* pOldPen = NULL;
		CPen pen(PS_SOLID,1,m_clrHist);	// 画笔

		pOldPen = pDC->SelectObject(&pen);
		//--------------------------------------------
		// 坐标点
		CPoint ptTopLeft = m_rectDraw.TopLeft();
		CPoint ptBottomRight = m_rectDraw.BottomRight();

		for(int v=m_iDnValue;v<m_iUpValue;v++)
		{
			World2DrawRect(v,x,y);
			pDC->MoveTo(x,ptBottomRight.y-1);
			pDC->LineTo(x,y-1);
		}
		//--------------------------------------------
		// GDI Cleanup
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();

	}

	// 真实数据转换到绘制矩形
	void KHistogramWnd::World2DrawRect(int iValue,int& x,int& y)
	{
		// 坐标点
		CPoint ptTopLeft = m_rectDraw.TopLeft();
		CPoint ptBottomRight = m_rectDraw.BottomRight();

		float dx = (float)m_rectDraw.Width() / m_iMaxValue;
		float dy = (float)m_rectDraw.Height() / m_iMaxCount;

		x = ptTopLeft.x + (int)(iValue*dx + 0.0f);
		y = ptBottomRight.y - (int)(m_lpCount[iValue]*dy + 0.0f);
	}

	// x坐标转换为值
	int KHistogramWnd::x2v(int x)
	{
		// 坐标点
		CPoint ptTopLeft = m_rectDraw.TopLeft();
		CPoint ptBottomRight = m_rectDraw.BottomRight();

		float dx = (float)m_rectDraw.Width() / m_iMaxValue;
		float dy = (float)m_rectDraw.Height() / m_iMaxCount;

		int iValue = (int)((float)(x - ptTopLeft.x) / dx + 0.5f);

		return iValue;
	}

	// 值转换为x坐标
	int KHistogramWnd::v2x(int v)
	{
		// 坐标点
		CPoint ptTopLeft = m_rectDraw.TopLeft();
		CPoint ptBottomRight = m_rectDraw.BottomRight();

		float dx = (float)m_rectDraw.Width() / m_iMaxValue;
		float dy = (float)m_rectDraw.Height() / m_iMaxCount;

		int x = ptTopLeft.x + (int)(v*dx + 0.0f);

		return x;
	}

//---------------------------------------------------------
}// namespace Gui - End
//---------------------------------------------------------

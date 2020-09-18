#include "StdAfx.h"
#include "KStationDataRender.h"

#include "KMapEngine.h"

namespace Renders
{

	KStationDataRender::KStationDataRender(void)
	{
		m_pRectInfoPtrCollection = new TPtrCollection<RectInfo*>();
	}

	KStationDataRender::~KStationDataRender(void)
	{
		_delete(m_pRectInfoPtrCollection);
	}

	// 站点数据
	void KStationDataRender::Rendering(HDC hDC, LPVOID lpvData)
	{
		int i;

		if(!hDC || !lpvData)
			return;

		//-------------------------------------------------
		// 1.保存参数
		m_hDC = hDC;
		m_lpvData = lpvData;

		// 2.准备图案
		Graphics graphics(hDC);

		// 3.地图引擎
		KMapEngine *pMapEngine = (KMapEngine*)lpvData;
		if(!pMapEngine)
			return;

		// 4.图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		if(pLayerProperty==NULL)
			return;

		// 5.数据提供者
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// 6.数据对象
		KStationData* pStationData = (KStationData*)pProvider->GetDataObject();
		if(!pStationData)
			return;

		// 数据信息
		KDataInfo* pDataInfo = pStationData->GetDataInfo();
		if(!pDataInfo)
			return;
		//-------------------------------------------------
		// 是否可见
		if(!pLayerProperty->IsVisible())
			return;

		// 为渲染器注册回调
		this->RegiestCallbackFunc(pStationData->CallbackFunc(),pStationData->CallbackParam());

		// 取得剪切矩形
		int x,y,w,h;
		pMapEngine->GetClipRect(x,y,w,h);

		// 保存剪切矩形顶点坐标
		//
		//  p0         p2
		//  +----------+
		//  |          |
		//	|          |
		//	|          |
		//	+----------+
		//	p1         p3
		//
		double xs[4], ys[4];
		xs[0] = x;		ys[0] = y ;
		xs[1] = x;		ys[1] = y + h ;
		xs[2] = x + w;	ys[2] = y ;
		xs[3] = x + w;	ys[3] = y + h ;

		// 转换为地理坐标
		for ( i = 0; i < 4; i++ )
		{
			pMapEngine->ClientToWorld(xs[i],ys[i]);
		}

		// 剪切矩形的地理坐标矩形属性(left,top right,bottom)
		double clipL, clipT, clipR, clipB;
		clipL = clipR = xs[0];
		clipB = clipT = ys[0];
		for ( i = 1; i < 4; i ++ )
		{
			if ( clipL > xs[i] ) clipL = xs[i];	// left 取最小值
			if ( clipT < ys[i] ) clipT = ys[i];	// top 取最大值(这和屏幕坐标不同)

			if ( clipR < xs[i] ) clipR = xs[i];	// rignt 取最大值
			if ( clipB > ys[i] ) clipB = ys[i];	// bottom 取最小值(这和屏幕坐标不同)
		}

		// 增加判断以提高显示速度
		KExtents bbox = pStationData->GetExtents();
		double intersectL = max(clipL,bbox.Xmin);
		double intersectR = min(clipR,bbox.Xmax);
		double intersectB = max(clipB,bbox.Ymin);
		double intersectT = min(clipT,bbox.Ymax);
		if ( intersectL > intersectR || intersectB > intersectT )
		{
			return;
		}

		// 是否剪切(建立剪切路径)
		if(pLayerProperty->IsClip())
		{
			GraphicsPath* pClipPath = (GraphicsPath*)pMapEngine->BuildClipPath();
			if(pClipPath)
			{
				graphics.ResetClip();
				graphics.SetClip(pClipPath);
			}
		}

		//-------------------------------------------------
		// 绘制
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(0);
		BYTE g = GetGValue(0);
		BYTE b = GetBValue(0);

		// 字体
		LOGFONT& lf = pLayerProperty->LogFont();
		FontFamily fontFamily(KString::ToWChar(pLayerProperty->LogFont().lfFaceName));
		REAL fontPoint = (REAL)(-1.0 * pLayerProperty->LogFont().lfHeight / 12 * 9);
		WORD wFontStyle = 0;
		if(lf.lfWeight > 400)	wFontStyle |= 0x01;
		if(lf.lfItalic)			wFontStyle |= 0x02;
		if(lf.lfUnderline)		wFontStyle |= 0x04;
		if(lf.lfStrikeOut)		wFontStyle |= 0x08;

		// 字体颜色
		COLORREF clrFont = pLayerProperty->FontColor();
		r = GetRValue(clrFont);
		g = GetGValue(clrFont);
		b = GetBValue(clrFont);

		// 站点名称
		Gdiplus::Font fontName(&fontFamily,fontPoint,(FontStyle)wFontStyle,Unit::UnitPoint);

		// 图层颜色
		COLORREF colorLayer = pLayerProperty->GetColor();

		// 画刷
		//-------------------------------------------------
		// 是否要求等值线(对记录数量有限制)
		if(pStationData->IsTarceContour())
		{
			// 是否重新追踪过等值线
			if(pStationData->IsRebuildContour())
			{
				pStationData->BuildContours();
			}

			// 等值线构造器
			KContourBuilder* pContourBuilder = pStationData->GetContourBuilder();
			if(pContourBuilder==NULL)
				return;

			// 等值线集
			TPtrCollection<KContour*>* pContourCollection = pContourBuilder->GetContourCollection(KContourBuilder::CurveConvert::BeforeConvert);
			if(pContourCollection==NULL)
				return;

			// 先填充
			if(pLayerProperty->IsFill())
			{
				// 注意 GetContourCollection(1);
				TPtrCollection<KContour*>* pContourCollectionFill = pContourBuilder->GetContourCollection(KContourBuilder::CurveConvert::AfterConvert);
				int iContourCount = pContourCollectionFill->Count();
				for(i=0;i<iContourCount;i++)
				{
					KContour* pContour = pContourCollectionFill->GetAt(i);
					this->FillContour(&graphics,pContour);
				}
			}

			// 再绘制等值线
			int iContourCount = pContourCollection->Count();
			for(i=0;i<iContourCount;i++)
			{
				KContour* pContour = pContourCollection->GetAt(i);
				this->DrawContour(&graphics,pContour);	// 绘制等值线

				if(pLayerProperty->IsLabel())
				{
					this->LabelContour(&graphics,pContour);	// 绘制等值线
				}
			}
		}

		// ------------------------------------------------
		// 绘制文本
		Gdiplus::Font fontLabel(L"宋体",5);		// 标注
		Gdiplus::Font fontFactor(L"宋体",8);	// 要素
		Gdiplus::Font fontFire(L"宋体",3);		// 火点标注
		StringFormat strfmt;
		strfmt.SetAlignment(StringAlignment::StringAlignmentCenter);
		strfmt.SetLineAlignment(StringAlignment::StringAlignmentCenter);

		//SolidBrush brush;
		SolidBrush brushStationName(Color(a,r,g,b));			// 站点名称颜色
		SolidBrush brushStationLabel(Color(a,64,64,64));		// 站点标签颜色
		SolidBrush brushStationValue(Color(a,0,0,192));			// 站点要素值颜色

		// 火点画刷
		SolidBrush brushFire(Color(a,GetRValue(colorLayer),GetGValue(colorLayer),GetBValue(colorLayer)));

		// 站点信息集
		KObjectPtrCollection* pStationInfoPtrCollection = pStationData->GetStationInfoPtrCollection();
		if(!pStationInfoPtrCollection)
			return;

		// 清除站点名称矩形
		m_pRectInfoPtrCollection->Clear();

		TCHAR szTemp[MAX_PATH] = _T("");
		int iProductCode = pDataInfo->ProductCode();
		int iStationCount = pStationInfoPtrCollection->Count();
		for(int i=0;i<iStationCount;i++)
		{
			// 站点信息
			KStationInfo* pStationInfo = (KStationInfo*)pStationInfoPtrCollection->GetAt(i);

			// 判断站点级别
			//if(pStationInfo->iLevel>0)
			//{
			//	continue;
			//}

			// 坐标转换
			double x = pStationInfo->Lon();
			double y = pStationInfo->Lat();
			pMapEngine->WorldToClient(x,y);

			// 站点坐标
			PointF ptStation((REAL)x, (REAL)y);

			// 不同的产品
			/// 雷电监测记录数据
			if(iProductCode == LIGHTNING_PRODUCT_CODE_MONITOR_RECORD)	// 5001 雷电监测记录数据(每雷电一个记录)
			{
				int flag = pStationInfo->ElementValue(0)>0 ? 1 : -1;
				Gdiplus::Font fontLightning(L"宋体",9);		// 标注
				SolidBrush brushLightning(flag>0 ? Color(a,255,0,0) : Color(a,0,255,0));
				graphics.DrawString(flag>0 ? L"+" : L"-",-1,&fontLightning,ptStation,&strfmt,&brushLightning);
			}
			// 火点
			else if(iProductCode == AGMRS_PRODUCT_CODE_MISC_FIRE)
			{
				graphics.DrawString(L"●",-1,&fontFire,ptStation,&strfmt,&brushFire);
			}
			else // 绘制站点符号
			{
				graphics.DrawString(L"●",-1,&fontLabel,ptStation,&strfmt,&brushStationLabel);
			}

			// 是否显示站点名称
			KString strStationName = pStationInfo->StationName();
			if(pLayerProperty->IsShowStationName() && !strStationName.IsEmpty())
			{
				// 计算字符串的包围矩形
				RectF rcBox;
				graphics.MeasureString(KString::ToWChar((LPCTSTR)pStationInfo->StationName()),-1,&fontName,ptStation,&rcBox);
				rcBox.X -= rcBox.Width / 2;
				rcBox.Y -= rcBox.Height;

				// 取得站点名称占用的矩形空间
				CRect rect( (int)rcBox.X, (int)rcBox.Y, (int)(rcBox.X+rcBox.Width), (int)(rcBox.Y+rcBox.Height) );
				RectInfo* pRectInfo = new RectInfo();
				pRectInfo->rect = rect;
				_tcscpy(pRectInfo->szStationID,(LPCTSTR)pStationInfo->StationID());
				m_pRectInfoPtrCollection->Add(pRectInfo);

				// 绘制站点名称
				graphics.DrawString(KString::ToWChar((LPCTSTR)pStationInfo->StationName()),-1,&fontName,rcBox,&strfmt,&brushStationName);

				//=============================================
				// 测试 - 绘制矩形框
				//graphics.DrawRectangle(&Pen(Color::Red),rcBox);

				//RectF rct;
				//rct.X = (REAL)rect.left;
				//rct.Y = (REAL)rect.top;
				//rct.Width = (REAL)rect.Width();
				//rct.Height = (REAL)rect.Height();

				//graphics.DrawRectangle(&Pen(Color::Blue,1),rct);
				//=============================================
			}

			// 绘制数据
			if(pLayerProperty->IsShowData())
			{
				// 站点要素值
				CString strFactorValue = _T("");
				strFactorValue.Format(_T("%.1f"),pStationInfo->ElementValue(pStationData->CurrentElementIndex()));

				// 要素值绘制位置
				PointF ptFactor(ptStation);
				ptFactor.Y += 9;

				// 绘制站点要素值
				graphics.DrawString(KString::ToWChar((LPCTSTR)strFactorValue),-1,&fontFactor,ptFactor,&strfmt,&brushStationValue);
			}

		}// for(int i=0;i<iStationCount;i++) - end
		// ------------------------------------------------

		return;
	}

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------

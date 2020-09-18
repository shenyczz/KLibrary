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

	// վ������
	void KStationDataRender::Rendering(HDC hDC, LPVOID lpvData)
	{
		int i;

		if(!hDC || !lpvData)
			return;

		//-------------------------------------------------
		// 1.�������
		m_hDC = hDC;
		m_lpvData = lpvData;

		// 2.׼��ͼ��
		Graphics graphics(hDC);

		// 3.��ͼ����
		KMapEngine *pMapEngine = (KMapEngine*)lpvData;
		if(!pMapEngine)
			return;

		// 4.ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		if(pLayerProperty==NULL)
			return;

		// 5.�����ṩ��
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// 6.���ݶ���
		KStationData* pStationData = (KStationData*)pProvider->GetDataObject();
		if(!pStationData)
			return;

		// ������Ϣ
		KDataInfo* pDataInfo = pStationData->GetDataInfo();
		if(!pDataInfo)
			return;
		//-------------------------------------------------
		// �Ƿ�ɼ�
		if(!pLayerProperty->IsVisible())
			return;

		// Ϊ��Ⱦ��ע��ص�
		this->RegiestCallbackFunc(pStationData->CallbackFunc(),pStationData->CallbackParam());

		// ȡ�ü��о���
		int x,y,w,h;
		pMapEngine->GetClipRect(x,y,w,h);

		// ������о��ζ�������
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

		// ת��Ϊ��������
		for ( i = 0; i < 4; i++ )
		{
			pMapEngine->ClientToWorld(xs[i],ys[i]);
		}

		// ���о��εĵ��������������(left,top right,bottom)
		double clipL, clipT, clipR, clipB;
		clipL = clipR = xs[0];
		clipB = clipT = ys[0];
		for ( i = 1; i < 4; i ++ )
		{
			if ( clipL > xs[i] ) clipL = xs[i];	// left ȡ��Сֵ
			if ( clipT < ys[i] ) clipT = ys[i];	// top ȡ���ֵ(�����Ļ���겻ͬ)

			if ( clipR < xs[i] ) clipR = xs[i];	// rignt ȡ���ֵ
			if ( clipB > ys[i] ) clipB = ys[i];	// bottom ȡ��Сֵ(�����Ļ���겻ͬ)
		}

		// �����ж��������ʾ�ٶ�
		KExtents bbox = pStationData->GetExtents();
		double intersectL = max(clipL,bbox.Xmin);
		double intersectR = min(clipR,bbox.Xmax);
		double intersectB = max(clipB,bbox.Ymin);
		double intersectT = min(clipT,bbox.Ymax);
		if ( intersectL > intersectR || intersectB > intersectT )
		{
			return;
		}

		// �Ƿ����(��������·��)
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
		// ����
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(0);
		BYTE g = GetGValue(0);
		BYTE b = GetBValue(0);

		// ����
		LOGFONT& lf = pLayerProperty->LogFont();
		FontFamily fontFamily(KString::ToWChar(pLayerProperty->LogFont().lfFaceName));
		REAL fontPoint = (REAL)(-1.0 * pLayerProperty->LogFont().lfHeight / 12 * 9);
		WORD wFontStyle = 0;
		if(lf.lfWeight > 400)	wFontStyle |= 0x01;
		if(lf.lfItalic)			wFontStyle |= 0x02;
		if(lf.lfUnderline)		wFontStyle |= 0x04;
		if(lf.lfStrikeOut)		wFontStyle |= 0x08;

		// ������ɫ
		COLORREF clrFont = pLayerProperty->FontColor();
		r = GetRValue(clrFont);
		g = GetGValue(clrFont);
		b = GetBValue(clrFont);

		// վ������
		Gdiplus::Font fontName(&fontFamily,fontPoint,(FontStyle)wFontStyle,Unit::UnitPoint);

		// ͼ����ɫ
		COLORREF colorLayer = pLayerProperty->GetColor();

		// ��ˢ
		//-------------------------------------------------
		// �Ƿ�Ҫ���ֵ��(�Լ�¼����������)
		if(pStationData->IsTarceContour())
		{
			// �Ƿ�����׷�ٹ���ֵ��
			if(pStationData->IsRebuildContour())
			{
				pStationData->BuildContours();
			}

			// ��ֵ�߹�����
			KContourBuilder* pContourBuilder = pStationData->GetContourBuilder();
			if(pContourBuilder==NULL)
				return;

			// ��ֵ�߼�
			TPtrCollection<KContour*>* pContourCollection = pContourBuilder->GetContourCollection(KContourBuilder::CurveConvert::BeforeConvert);
			if(pContourCollection==NULL)
				return;

			// �����
			if(pLayerProperty->IsFill())
			{
				// ע�� GetContourCollection(1);
				TPtrCollection<KContour*>* pContourCollectionFill = pContourBuilder->GetContourCollection(KContourBuilder::CurveConvert::AfterConvert);
				int iContourCount = pContourCollectionFill->Count();
				for(i=0;i<iContourCount;i++)
				{
					KContour* pContour = pContourCollectionFill->GetAt(i);
					this->FillContour(&graphics,pContour);
				}
			}

			// �ٻ��Ƶ�ֵ��
			int iContourCount = pContourCollection->Count();
			for(i=0;i<iContourCount;i++)
			{
				KContour* pContour = pContourCollection->GetAt(i);
				this->DrawContour(&graphics,pContour);	// ���Ƶ�ֵ��

				if(pLayerProperty->IsLabel())
				{
					this->LabelContour(&graphics,pContour);	// ���Ƶ�ֵ��
				}
			}
		}

		// ------------------------------------------------
		// �����ı�
		Gdiplus::Font fontLabel(L"����",5);		// ��ע
		Gdiplus::Font fontFactor(L"����",8);	// Ҫ��
		Gdiplus::Font fontFire(L"����",3);		// ����ע
		StringFormat strfmt;
		strfmt.SetAlignment(StringAlignment::StringAlignmentCenter);
		strfmt.SetLineAlignment(StringAlignment::StringAlignmentCenter);

		//SolidBrush brush;
		SolidBrush brushStationName(Color(a,r,g,b));			// վ��������ɫ
		SolidBrush brushStationLabel(Color(a,64,64,64));		// վ���ǩ��ɫ
		SolidBrush brushStationValue(Color(a,0,0,192));			// վ��Ҫ��ֵ��ɫ

		// ��㻭ˢ
		SolidBrush brushFire(Color(a,GetRValue(colorLayer),GetGValue(colorLayer),GetBValue(colorLayer)));

		// վ����Ϣ��
		KObjectPtrCollection* pStationInfoPtrCollection = pStationData->GetStationInfoPtrCollection();
		if(!pStationInfoPtrCollection)
			return;

		// ���վ�����ƾ���
		m_pRectInfoPtrCollection->Clear();

		TCHAR szTemp[MAX_PATH] = _T("");
		int iProductCode = pDataInfo->ProductCode();
		int iStationCount = pStationInfoPtrCollection->Count();
		for(int i=0;i<iStationCount;i++)
		{
			// վ����Ϣ
			KStationInfo* pStationInfo = (KStationInfo*)pStationInfoPtrCollection->GetAt(i);

			// �ж�վ�㼶��
			//if(pStationInfo->iLevel>0)
			//{
			//	continue;
			//}

			// ����ת��
			double x = pStationInfo->Lon();
			double y = pStationInfo->Lat();
			pMapEngine->WorldToClient(x,y);

			// վ������
			PointF ptStation((REAL)x, (REAL)y);

			// ��ͬ�Ĳ�Ʒ
			/// �׵����¼����
			if(iProductCode == LIGHTNING_PRODUCT_CODE_MONITOR_RECORD)	// 5001 �׵����¼����(ÿ�׵�һ����¼)
			{
				int flag = pStationInfo->ElementValue(0)>0 ? 1 : -1;
				Gdiplus::Font fontLightning(L"����",9);		// ��ע
				SolidBrush brushLightning(flag>0 ? Color(a,255,0,0) : Color(a,0,255,0));
				graphics.DrawString(flag>0 ? L"+" : L"-",-1,&fontLightning,ptStation,&strfmt,&brushLightning);
			}
			// ���
			else if(iProductCode == AGMRS_PRODUCT_CODE_MISC_FIRE)
			{
				graphics.DrawString(L"��",-1,&fontFire,ptStation,&strfmt,&brushFire);
			}
			else // ����վ�����
			{
				graphics.DrawString(L"��",-1,&fontLabel,ptStation,&strfmt,&brushStationLabel);
			}

			// �Ƿ���ʾվ������
			KString strStationName = pStationInfo->StationName();
			if(pLayerProperty->IsShowStationName() && !strStationName.IsEmpty())
			{
				// �����ַ����İ�Χ����
				RectF rcBox;
				graphics.MeasureString(KString::ToWChar((LPCTSTR)pStationInfo->StationName()),-1,&fontName,ptStation,&rcBox);
				rcBox.X -= rcBox.Width / 2;
				rcBox.Y -= rcBox.Height;

				// ȡ��վ������ռ�õľ��οռ�
				CRect rect( (int)rcBox.X, (int)rcBox.Y, (int)(rcBox.X+rcBox.Width), (int)(rcBox.Y+rcBox.Height) );
				RectInfo* pRectInfo = new RectInfo();
				pRectInfo->rect = rect;
				_tcscpy(pRectInfo->szStationID,(LPCTSTR)pStationInfo->StationID());
				m_pRectInfoPtrCollection->Add(pRectInfo);

				// ����վ������
				graphics.DrawString(KString::ToWChar((LPCTSTR)pStationInfo->StationName()),-1,&fontName,rcBox,&strfmt,&brushStationName);

				//=============================================
				// ���� - ���ƾ��ο�
				//graphics.DrawRectangle(&Pen(Color::Red),rcBox);

				//RectF rct;
				//rct.X = (REAL)rect.left;
				//rct.Y = (REAL)rect.top;
				//rct.Width = (REAL)rect.Width();
				//rct.Height = (REAL)rect.Height();

				//graphics.DrawRectangle(&Pen(Color::Blue,1),rct);
				//=============================================
			}

			// ��������
			if(pLayerProperty->IsShowData())
			{
				// վ��Ҫ��ֵ
				CString strFactorValue = _T("");
				strFactorValue.Format(_T("%.1f"),pStationInfo->ElementValue(pStationData->CurrentElementIndex()));

				// Ҫ��ֵ����λ��
				PointF ptFactor(ptStation);
				ptFactor.Y += 9;

				// ����վ��Ҫ��ֵ
				graphics.DrawString(KString::ToWChar((LPCTSTR)strFactorValue),-1,&fontFactor,ptFactor,&strfmt,&brushStationValue);
			}

		}// for(int i=0;i<iStationCount;i++) - end
		// ------------------------------------------------

		return;
	}

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------

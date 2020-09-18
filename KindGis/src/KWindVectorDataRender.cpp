#include "StdAfx.h"
#include "KWindVectorDataRender.h"

#include "KMapEngine.h"

namespace Renders
{
	KWindVectorDataRender::KWindVectorDataRender(void)
	{
	}

	KWindVectorDataRender::~KWindVectorDataRender(void)
	{
	}

	// ��Ⱦ
	void KWindVectorDataRender::Rendering(HDC hDC, LPVOID lpvData)
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
		// �Ƿ�ɼ�
		if(!pLayerProperty->IsVisible())
			return;

		// 5.�����ṩ��
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// 6.���ݶ���(ע���޸�)
		KWindVectorData* pDataObject = (KWindVectorData*)pProvider->GetDataObject();
		if(!pDataObject)
			return;
		// Ϊ��Ⱦ��ע��ص�
		this->RegiestCallbackFunc(pDataObject->CallbackFunc(),pDataObject->CallbackParam());
		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		//-------------------------------------------------
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
		KExtents bbox = pDataObject->GetExtents();
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
		// ʸ����ʾ����
		WORD windVector = pLayerProperty->WindVector();
		//===========================================
		//windVector = 0
		//	| WindVector_Barb
		//	| WindVector_Arrow
		//	| WindVector_StreamLine
		//	;	// ����
		//===========================================
		if(windVector & WindVector_Barb)
		{
			this->DrawBarb(&graphics);
		}
		if(windVector & WindVector_Arrow)
		{
			this->DrawArrow(&graphics);
		}
		if(windVector & WindVector_StreamLine)
		{
			this->DrawStreamLine(&graphics);
		}

		return;
	}

	// ���Ʒ���
	void KWindVectorDataRender::DrawBarb(Graphics* pGraphics)
	{
		// 1.�豸����
		HDC hDC = m_hDC;
		// 2.��ͼ����
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		// 3.ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		// 4.�����ṩ��
		KProvider* pProvider = pLayerProperty->GetProvider();
		// 5.���ݶ���
		KWindVectorData* pDataObject = (KWindVectorData*)pProvider->GetDataObject();
		// 6.������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();

		//-------------------------------------------------
		// ������� WindBarb
		//-------------------------------------------------
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(0);
		BYTE g = GetGValue(0);
		BYTE b = GetBValue(0);

		if(pDataObject->IsEmpty())
			return;

		// ��ʱ����
		double x, y;		// λ��
		double lon, lat;	// ���ȡ�γ��
		double adj;			// �Ƕȵ���
		double u,v;			// �糡uv����
		double dir,spd;		// ����,�ٶ�

		int nx		= pDataInfo->XNum();		// x ��������
		int ny		= pDataInfo->YNum();		// y ��������
		double* px	= pDataObject->GetXData();	// x ��������ָ��
		double* py	= pDataObject->GetYData();	// y ��������ָ��
		double* pu	= pDataObject->GetUData();	// u ����ָ��
		double* pv	= pDataObject->GetVData();	// v ����ָ��

		// �繳��ɫ
		COLORREF clrBarb = pLayerProperty->BarbColor();
		r = GetRValue(clrBarb);
		g = GetGValue(clrBarb);
		b = GetBValue(clrBarb);
		// �繳����
		int iStep = pLayerProperty->BarbStep();
		if(iStep<1 || iStep>9) iStep = 1;
		for(int i = 0; i < ny; i += iStep)		// ��
		{
			for(int j = 0; j < nx; j += iStep)	// ��
			{
				// λ������
				x = px[j];			y = py[i];
				lon = x;			lat = y;
				u = *(pu+i*nx+j);	v = -*(pv+i*nx+j);

				// �Ƕȷ��� direction
				if (v==0.0 && u==0.0)
					dir = 0.0;
				else
					dir = atan2(v,u) * 180 / PI;

				//�ٶ�
				spd = hypot(u,v);

				// �Ե�ǰ��ͼͶӰ��λ�����Ƕȵ���,
				adj = AdjustmentAngle(lon,lat);

				// ͶӰ�任
				pMapEngine->WorldToClient(x,y);

				//����
				_DrawOneBarb (pGraphics, x, y, dir+adj, spd, clrBarb);

			}// next j

		}//next i

		return;
	}
	// ���Ʒ��
	void KWindVectorDataRender::DrawArrow(Graphics* pGraphics)
	{
		// 1.�豸����
		HDC hDC = m_hDC;
		// 2.��ͼ����
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		// 3.ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		// 4.�����ṩ��
		KProvider* pProvider = pLayerProperty->GetProvider();
		// 5.���ݶ���
		KWindVectorData* pDataObject = (KWindVectorData*)pProvider->GetDataObject();
		// 6.������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();

		//-------------------------------------------------
		// ������� WindArrow
		//-------------------------------------------------
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(0);
		BYTE g = GetGValue(0);
		BYTE b = GetBValue(0);

		if(pDataObject->IsEmpty())
			return;

		// ��ʱ����
		double x, y;		// λ��
		double lon, lat;	// ���ȡ�γ��
		double adj;			// �Ƕȵ���
		double u,v;			// ʸ���� u v ����
		//double plen,blen,rad,dir,spd;
		double dir,spd;

		int nx		= pDataInfo->XNum();		// x ��������
		int ny		= pDataInfo->YNum();		// y ��������
		double* px	= pDataObject->GetXData();	// x ��������ָ��
		double* py	= pDataObject->GetYData();	// y ��������ָ��
		double* pu	= pDataObject->GetUData();	// u ����ָ��
		double* pv	= pDataObject->GetVData();	// v ����ָ��

		// ��ʸ��ɫ
		COLORREF clrArrow = pLayerProperty->ArrowColor();
		r = GetRValue(clrArrow);
		g = GetGValue(clrArrow);
		b = GetBValue(clrArrow);
		// ��ʸ����
		int iStep = pLayerProperty->ArrowStep();
		if(iStep<1 || iStep>9) iStep = 1;
		for(int i=0; i<ny; i+=iStep)		//��
		{
			for(int j=0; j<nx; j+=iStep)	//��
			{
				x = px[j];			y = py[i];
				u = *(pu+i*nx+j);	v = -*(pv+i*nx+j);
				lon = x;			lat = y;

				if(i==iStep)
				{
					int iii=0;
				}

				//�Ƕȷ��� direction
				if (v==0.0 && u==0.0)
					dir = 0.0;
				else
					dir = atan2(v,u) * 180 / PI;

				//�ٶ�
				spd = (float)hypot(u,v);

				// �Ե�ǰ��ͼͶӰ��λ�����Ƕȵ���,
				adj = AdjustmentAngle(lon,lat);

				// ͶӰ�任
				pMapEngine->WorldToClient(x,y);

				//����
				_DrawOneArrow (pGraphics, x, y, dir+adj, spd, clrArrow);

			}//next j

		}// next i

		/*
		for(int i=0;i<ny;i+=iStep)		//��
		{
			for(int j=0;j<nx;j+=iStep)	//��
			{


			}

		}//for(i)

		return;
		*/
	}
	// ��������
	void KWindVectorDataRender::DrawStreamLine(Graphics* pGraphics)
	{
		// 1.�豸����
		HDC hDC = m_hDC;
		// 2.��ͼ����
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		// 3.ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		// 4.�����ṩ��
		KProvider* pProvider = pLayerProperty->GetProvider();
		// 5.���ݶ���
		KWindVectorData* pDataObject = (KWindVectorData*)pProvider->GetDataObject();
		// 6.������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();

		//-------------------------------------------------
		// ������� WindStreamLine
		//-------------------------------------------------
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(0);
		BYTE g = GetGValue(0);
		BYTE b = GetBValue(0);

		if(pDataObject->IsEmpty())
			return;

		// ��ʱ����
		// �����ֲ�����
		int ii,jj,ii1,jj1,ij1;
		int ipt,acc,ijsav;
		double x,y,xsav,ysav,xold,yold;
		double xx,yy,xxsav,yysav;
		//double xx,yy;
		int jcol,scol;
		int acnt,bflg;
		int loopStep,loopStepMax = 1000;
		double *up, *vp;				//u v����ָ��
		double uu,vv,uu1,uu2,vv1,vv2,auu,avv;

		int nx		= pDataInfo->XNum();			// x ��������
		int ny		= pDataInfo->YNum();			// y ��������
		double xmin	= pDataInfo->XMin();			// x ������Сֵ
		double xmax	= pDataInfo->XMax();			// x �������ֵ
		double ymin	= pDataInfo->YMin();			// y ������Сֵ
		double ymax	= pDataInfo->YMax();			// y �������ֵ
		double* px	= pDataObject->GetXData();		// x ��������ָ��
		double* py	= pDataObject->GetYData();		// y ��������ָ��
		double* pu	= pDataObject->GetUData();		// u ����ָ��
		double* pv	= pDataObject->GetVData();		// v ����ָ��

		// ������ɫ
		COLORREF clrStreamLine = pLayerProperty->StreamLineColor();
		r = GetRValue(clrStreamLine);
		g = GetGValue(clrStreamLine);
		b = GetBValue(clrStreamLine);
		// �����ܶ�
		int iDensity = pLayerProperty->StreamLineStep();
		if(iDensity<1 || iDensity>5) iDensity = 1;

		double YMAX	= 87.0;	// ��ֹy=90(��)

		scol = -9;
		jcol = 1;

		// ���������ܶȼ�����������ļ��
		int k = max(nx,ny);
		int iScale = 200/k + iDensity-5;
		if(iScale<1)	iScale = 1;
		if(iScale>10)	iScale = 10;

		iScale = iDensity;	// ��ԭ����һ������ֳ�iScale��
		double fact = 1.0 / iScale;
		double dx = (px[1]-px[0]) / iScale;
		double dy = (py[1]-py[0]) / iScale;

		// Ϊ�����������ڴ�
		int nCol = (nx-1)*iScale+1;	// ���ܺ������
		int nRow = (ny-1)*iScale+1;	// ���ܺ������
		int iSize = nCol*nRow;		// ���ܺ��ܵ��������
		int* pFlag = (int*)malloc(sizeof(int)*iSize);
		if(!pFlag) return;
		memset(pFlag,0,sizeof(int)*iSize);

		//Look through flag grid to lookfor start of streamline.
		//To start requires no streams drawn within surrounding flag box
		// ɨ��ȫ������������������ͷ,�ҵ�һ���ͻ��Ƹ�����
		// ͬʱ���ɨ����ĸ��
		int col=0,row=0;
		int iStep = 1;
		bool bAdd = true;

		for(int i=0;i<iSize;i++)	// 919 iSize
		{
			// �鿴��Χ�����Ƿ������߾���,���û�оͿ���ȷ��
			// �˰�Χ���д���������ͷ
			int dis = 2;			// ��Χ�б߳�
			if(iDensity<5) dis=3;	// �����ܶ�С,��Χ�д�
			if(iDensity>5) dis=1;	// �����ܶȴ�,��Χ��С

			// ���ݰ�Χ�б߳�����ǰ������ȷ����Χ�а�Χ������
			int cmin,cmax,rmin,rmax;
			cmin=col-dis; cmax=col+dis+1;
			rmin=row-dis; rmax=row+dis+1;
			if(cmin<0)		cmin=0;
			if(cmax>nCol)	cmax=nCol;
			if(rmin<0)		rmin=0;
			if(rmax>nRow)	rmax=nRow;

			// �鿴��Χ�����Ƿ������߾���
			acc=0;
			for(int rr=rmin;rr<rmax;rr++)
			{
				ipt = rr*nCol+cmin;
				for(int cc=cmin;cc<cmax;cc++)
				{
					acc += *(pFlag+ipt);
					ipt++;

				}// for(cc)

			}//for(rr)

			if(acc==0)	//acc=0 ������Χ����û�����߾���
			{
				// ȷ��������ͷ
				int oldCol = col/iScale;			// ����ǰ����
				int oldRow = row/iScale;			// ����ǰ����
				x = *(px+oldCol)+dx*(col%iScale);	// ��õ�ǰ������
				y = *(py+oldRow)+dy*(row%iScale);	// ��õ�ǰ������
				xsav = x; ysav = y;					// ��������

				xx = bAdd ? x+1.0 : x;
				yy = bAdd ? y+1.0 : y;

				//����任
				pMapEngine->WorldToClient(xx,yy);

				::MoveToEx(hDC,(int)xx,(int)yy,NULL);	// �ƶ�����ͷ
				xxsav = xx; yysav = yy;					// ��������任�������

				ijsav = -999;
				acc = 0;
				acnt = 0;	//
				bflg = 0;

				loopStep=0;
				while(x>=xmin && x<xmax && y>=ymin && y<ymax && y<YMAX) // while - 1
				{
					if(loopStep++ > loopStepMax)
						break;

					// ����x,y������ܺ����ڵ��С���
					ii = (int)((y-ymin)/dy); jj = (int)((x-xmin)/dx);

					// ת��Ϊ����ǰ���ڵ��С���
					ii /= iScale; jj /= iScale;

					// ��ǰ����ͱ�׼����Ĳ�ֵ
					xx = x - *(px+jj); yy = y - *(py+ii);

					// ����ǰ���ڵ��С��е�uvָ��
					up = pu+ii*nx+jj;	vp = pv+ii*nx+jj;	//

					// ��ֵ��õ�ǰ���괦��u vֵ
					uu1 = *up+(*(up+1)- *up)*xx;
					uu2 = *(up+nx)+(*(up+nx+1)-*(up+nx))*xx;
					uu = uu1+(uu2-uu1)*yy;
					auu = fabs(uu);

					vv1 = *vp+(*(vp+1)- *vp)*xx;
					vv2 = *(vp+nx)+(*(vp+nx+1)-*(vp+nx))*xx;
					vv = vv1+(vv2-vv1)*yy;
					avv = fabs(vv);

					//if((uu*uu+vv*vv)<EPSILON) break;
					if(auu<0.1 && avv<0.1)
						break;

					if(auu>avv)
					{
						uu=uu*fact/auu;
						vv=vv*fact/auu;
					}
					else
					{
						uu=uu*fact/avv;
						vv=vv*fact/avv;
					}

					// �����һ���������
					x=x+uu; y=y+vv;

					// ����x,y������ܺ����ڵ��С���
					ii1=(int)((y-ymin)/dy);	//��
					jj1=(int)((x-xmin)/dx);	//��

					ij1=ii1*nCol+jj1;		//
					if(ij1<0 || ij1>iSize) break;
					if(*(pFlag+ij1)==1)	break;

					if(ij1!=ijsav && ijsav>-1) *(pFlag+ijsav)=1;
					if(ij1==ijsav)  acc++;
					else			acc=0;
					if(acc>10) break;	//??
					ijsav=ij1;

					xx = bAdd ? x+1.0f : x;
					yy = bAdd ? y+1.0f : y;

					// ����任
					pMapEngine->WorldToClient(xx,yy);

					if(jcol>-1)
					{
						if(bflg)
						{
							bflg=0;
							//::MoveToEx(hDC,(int)xold,(int)yold,NULL);
							//::LineTo(hDC,(int)xx,(int)yy);
							pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)xold,(REAL)yold,(REAL)xx,(REAL)yy);
						}
						else
						{
							//::MoveToEx(hDC,(int)xxsav,(int)yysav,NULL);
							//::LineTo(hDC,(int)xx,(int)yy);
							pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)xxsav,(REAL)yysav,(REAL)xx,(REAL)yy);
						}
					}
					else
					{
						bflg=1;
					}

					xold=xx; yold=yy;
					acnt++;
					if(acnt>20)
					{
						// ���Ƽ�ͷ
						if(jcol>-1)
						{
							_StreamArrow(pGraphics,xxsav,yysav,xx,yy,clrStreamLine);
						}
						acnt=0;
					}

					xxsav=xx; yysav=yy;

				}// while(x>=xmin && x<xmax && y>=ymin && y<ymax && y<YMAX) // while - 1

				bflg=0;
				x=xsav; y=ysav;

				xx = bAdd ? x+1.0f : x;
				yy = bAdd ? y+1.0f : y;

				// ����任
				pMapEngine->WorldToClient(xx,yy);
				::MoveToEx(hDC,(int)xx,(int)yy,NULL);

				xxsav=xx; yysav=yy;
				ijsav=-999;
				acc=0;
				acnt=18;

				loopStep=0;
				while(x>=xmin && x<xmax && y>=ymin && y<ymax  && y<YMAX)	//while - 2
				{
					if(loopStep++ > loopStepMax) break;

					//����x,y������ܺ����ڵ��С���
					ii=(int)((y-ymin)/dy);	jj=(int)((x-xmin)/dx);

					//ת��Ϊ����ǰ���ڵ��С���
					ii/=iScale;	jj/=iScale;
					//��ǰ����ͱ�׼����Ĳ�ֵ
					//xx=dx; yy=dy;
					//xx=1/iScale; yy=1/iScale;
					xx = x - *(px+jj); yy = y - *(py+ii);

					//����ǰ���ڵ��С��е�uvָ��
					up=pu+ii*nx+jj;	vp=pv+ii*nx+jj;	//

					//��ֵ��õ�ǰ���괦��u vֵ
					uu1=*up+(*(up+1)- *up)*xx;
					uu2=*(up+nx)+(*(up+nx+1)-*(up+nx))*xx;
					uu=uu1+(uu2-uu1)*yy;
					auu=fabs(uu);

					vv1=*vp+(*(vp+1)- *vp)*xx;
					vv2=*(vp+nx)+(*(vp+nx+1)-*(vp+nx))*xx;
					vv=vv1+(vv2-vv1)*yy;
					avv=fabs(vv);

					//if((uu*uu+vv*vv)<EPSILON) break;
					if(auu<0.1 && avv<0.1) break;

					if(auu>avv)
					{
						uu=uu*fact/auu;
						vv=vv*fact/auu;
					}
					else
					{
						uu=uu*fact/avv;
						vv=vv*fact/avv;
					}

					//�����һ���������
					x=x-uu; y=y-vv;	

					//����x,y������ܺ����ڵ��С���
					ii1=(int)((y-ymin)/dy);	//��
					jj1=(int)((x-xmin)/dx);	//��

					ij1=ii1*nCol+jj1;		//
					if(ij1<0 || ij1>iSize) break;
					if(*(pFlag+ij1)==1)	break;

					if(ij1!=ijsav && ijsav>-1) *(pFlag+ijsav)=1;
					if(ij1==ijsav)  acc++;
					else			acc=0;
					if(acc>10) break;	//??
					ijsav=ij1;

					//xx=x; yy=y;
					//xx=x+1.0; yy=y+1.0;
					xx = bAdd ? x+1.0f : x;
					yy = bAdd ? y+1.0f : y;
					// ����任
					pMapEngine->WorldToClient(xx,yy);

					if(jcol>-1)
					{
						if(bflg)
						{
							bflg=0;
							//::MoveToEx(hDC,(int)xold,(int)yold,NULL);
							//::LineTo(hDC,(int)xx,(int)yy);
							pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)xold,(REAL)yold,(REAL)xx,(REAL)yy);
						}
						else
						{
							//::MoveToEx(hDC,(int)xxsav,(int)yysav,NULL);
							//::LineTo(hDC,(int)xx,(int)yy);
							pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)xxsav,(REAL)yysav,(REAL)xx,(REAL)yy);
						}
						//if(bflg)
						//{
						//	::MoveToEx(hDC,(int)xold,(int)yold,NULL);
						//	bflg=0;
						//}
						//::LineTo(hDC,(int)xx,(int)yy);
					}
					else
					{
						bflg=1;
					}
					xold=xx; yold=yy;
					acnt++;
					if(acnt>20)
					{
						if(jcol>-1)
						{
							_StreamArrow(pGraphics,xx,yy,xxsav,yysav,clrStreamLine);
						}	//
						acnt=0;
					}
					xxsav=xx; yysav=yy;

				}// while(x>=xmin && x<xmax && y>=ymin && y<ymax  && y<YMAX)	//while - 2

			}//if(acc==0)

			col+=iStep;
			if(col>=nCol) {col=0; row+=iStep;}
			if(row>=nRow)
				break;

		}// for(int i=0;i<iSize;i++)

		//�ͷ��ڴ�
		free(pFlag);

		return;
	}

	// ��ĳλ�û��Ʒ���
	void KWindVectorDataRender::_DrawOneBarb(Graphics* pGraphics,double x,double y,double dDirection,double dSpeed,COLORREF clr)
	{
		// ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		// �����ֲ�����
		int i;
		double x1,y1,x2,y2,x3,y3;
		//double dWindSpeed0;
		//int nTriangle;				// ������
		//double dLine0,dLine1;	// ����
		//BOOL bHalf;				// �Ƿ��а���

		// ����Ƕ�,ʹ���ָ���紵���ķ���
		dDirection += 180;

		//if(dDirection<0)
		//	dDirection += 360;
		//if(dDirection>=360)
		//	dDirection -= 360;

		//double a70 = 70.0 * PI / 180.0;
		double a70 = 70.0;

		// �������
		double x0 = x;
		double y0 = y;
		double dAangle = dDirection;	// ��˵ĽǶ�

		int lenTotal = 24;		// ������ܳ���
		int lenBarb = 12;		// ���ٸ�(����)����
		int nSpace = 5;		// ���ٸͼ��

		/// �������
		///
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(clr);
		BYTE g = GetGValue(clr);
		BYTE b = GetBValue(clr);

		double svA = sin(dAangle * PI / 180);
		double cvA = cos(dAangle * PI / 180);

		// Դ��
		//pGraphics->DrawEllipse(&Pen(Color(a,r,g,b)),(REAL)x0,(REAL)y0,(REAL)2,(REAL)2);

		// ����С��1�����100���������
		if(dSpeed<1 || dSpeed>100)
		{
			// Դ��
			pGraphics->DrawEllipse(&Pen(Color(a,r,g,b)),(REAL)x0,(REAL)y0,(REAL)4,(REAL)4);
			//pGraphics->FillEllipse(&SolidBrush(Color(a,r,g,b)),(REAL)x0,(REAL)y0,(REAL)4,(REAL)4);
			return;
		}

		// ���ٴ��ڵ���1С��2����������
		if(dSpeed<2)
		{
			return;
		}

		// �������
		double xx = x0 + lenTotal * cvA;
		double yy = y0 + lenTotal * svA;
		pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x0,(REAL)y0,(REAL)xx,(REAL)yy);

		// ��������
		double dBangle = dAangle + a70;	// ����ĽǶ�

		double svB = sin(dBangle * PI / 180);
		double cvB = cos(dBangle * PI / 180);

		//if(dBangle<20) svB = 0.683;
		//if(dBangle>70) cvB = 0.683;

		// ���ٴ��ڵ���2С��3����ͷ����
		if(dSpeed<3)
		{
			// �������
			//double xx = x0 + lenTotal * cvA;
			//double yy = y0 + lenTotal * svA;
			//pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x0,(REAL)y0,(REAL)xx,(REAL)yy);

			x1 = x0+(lenTotal)*cvA;
			y1 = y0+(lenTotal)*svA;
			x2 = x1+lenBarb*cvB/2;
			y2 = y1+lenBarb*svB/2;
			pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x1,(REAL)y1,(REAL)x2,(REAL)y2);
			return;
		}

		// ���ٴ��ڵ���3С�ڵ���100�����5�����ǻ�4�������ĸ�����
		if(dSpeed<100 || fabs(dSpeed-100)<EPS)
		{
			// �������
			//double xx = x0 + lenTotal * cvA;
			//double yy = y0 + lenTotal * svA;
			//pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x0,(REAL)y0,(REAL)xx,(REAL)yy);

			// 1.��������
			//----------
			// ��������������(����>20 ��һ��������)
			int nTriangle = (int)(dSpeed / 20);
			for(i=0;i<nTriangle;i++)
			{
				x1 = x0+(lenTotal-i*nSpace)*cvA;
				y1 = y0+(lenTotal-i*nSpace)*svA;
				x2 = x1+lenBarb*cvB;
				y2 = y1+lenBarb*svB;
				x3 = x0+(lenTotal-(i+1)*nSpace)*cvA;
				y3 = y0+(lenTotal-(i+1)*nSpace)*svA;
				pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x1,(REAL)y1,(REAL)x2,(REAL)y2);
				pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x2,(REAL)y2,(REAL)x3,(REAL)y3);
				pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x3,(REAL)y3,(REAL)x1,(REAL)y1);
			}

			// 2.��ֱ��
			//----------
			// ���µķ���1(<20)
			double dWindSpeed1	= dSpeed - nTriangle * 20;
			int nLine = (int)(dWindSpeed1 / 4);		// ����(4m/sһ����)
			// ���µķ���2(<4)
			double dWindSpeed2 = dWindSpeed1 - 4.0 * nLine;	// ����
			if(dWindSpeed2>2) nLine += 1;
			for(i=0;i<(int)nLine;i++)
			{
				x1 = x0+(lenTotal-(i+(int)nTriangle)*nSpace)*cvA;
				y1 = y0+(lenTotal-(i+(int)nTriangle)*nSpace)*svA;
				x2 = x1+lenBarb*cvB;
				y2 = y1+lenBarb*svB;
				pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x1,(REAL)y1,(REAL)x2,(REAL)y2);
			}

			// 3.������
			//--------
			// �Ƿ��а���
			BOOL bHalf = dWindSpeed2>0 && (dWindSpeed2<2 || fabs(dWindSpeed2-2)<EPS);
			if(bHalf)
			{
				x1 = x0+(lenTotal-(nTriangle+nLine)*nSpace)*cvA;
				y1 = y0+(lenTotal-(nTriangle+nLine)*nSpace)*svA;
				x2 = x1+lenBarb*cvB/2;	//����
				y2 = y1+lenBarb*svB/2;
				pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x1,(REAL)y1,(REAL)x2,(REAL)y2);
			}

			return;

		}//dSpeed<=100

		return;
	}
	// ��ĳλ�û��Ʒ��
	void KWindVectorDataRender::_DrawOneArrow(Graphics* pGraphics,double x,double y,double dDirection,double dSpeed,COLORREF clr)
	{
		// ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		// �����ֲ�����
		double x1,y1,x2,y2,x3,y3;

		double a150 = 150 * PI / 180;

		// �������
		double x0 = x;
		double y0 = y;


		double dAangle = dDirection;	//��ķ���(�Ƕ�->����)

		dAangle = dAangle * PI / 180;	//��ķ���(�Ƕ�->����)

		/// �������
		///
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(clr);
		BYTE g = GetGValue(clr);
		BYTE b = GetBValue(clr);

		//����ٶ�<0.0001,���ư뾶=0.01��Բ
		//if (dSpeed<0.0001) {//Cricle(x,y,0.01);}

		int iArrLen		= int(dSpeed);	// �������
		double iArrSize	= 4;			// ��ͷ��С

		// ���Ƽ���
		x1 = x0+iArrLen*cos(dAangle);
		y1 = y0+iArrLen*sin(dAangle);
		pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x0,(REAL)y0,(REAL)x1,(REAL)y1);

		// ���Ƽ�ͷ
		x2 = x1+iArrSize*cos(dAangle+a150);
		y2 = y1+iArrSize*sin(dAangle+a150);
		pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x1,(REAL)y1,(REAL)x2,(REAL)y2);

		x3 = x1+iArrSize*cos(dAangle-a150);
		y3 = y1+iArrSize*sin(dAangle-a150);
		pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x1,(REAL)y1,(REAL)x3,(REAL)y3);

		return;
	}
	// �������ϻ��Ƽ�ͷ
	void KWindVectorDataRender::_StreamArrow(Graphics* pGraphics,double xx1,double yy1,double xx2,double yy2,COLORREF clr)
	{
		// ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		/// �������
		///
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(clr);
		BYTE g = GetGValue(clr);
		BYTE b = GetBValue(clr);

		double a150=150.0*PI/180.0;
		double dir=atan2(yy2-yy1,xx2-xx1);

		int iArrSize	= 5;

		double xb,yb,xe,ye;
		xb = xx2;
		yb = yy2;
		xe = xb+iArrSize*cos(dir+a150);
		ye = yb+iArrSize*sin(dir+a150);
		pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)xb,(REAL)yb,(REAL)xe,(REAL)ye);
		xe = xb+iArrSize*cos(dir-a150);
		ye = yb+iArrSize*sin(dir-a150);
		pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)xb,(REAL)yb,(REAL)xe,(REAL)ye);

		return;

		/*
		int nLineWidth	= GetLineWidth();	//�ʿ�
		CSize size2(nLineWidth,iArrSize);
		pDC->DPtoLP(&size2);
		nLineWidth		= size2.cx;
		iArrSize		= size2.cy;

		COLORREF clrPen = m_clrBAS[2];
		CPen pen(PS_SOLID,nLineWidth,clrPen);
		CPen* pOldPen = pDC->SelectObject(&pen);
		//---------------------------------
		pDC->MoveTo((int)xx2,(int)yy2);
		pDC->LineTo((int)(xx2+iArrSize*cos(dir+a150)),(int)(yy2+iArrSize*sin(dir+a150)));
		pDC->MoveTo((int)xx2,(int)yy2);
		pDC->LineTo((int)(xx2+iArrSize*cos(dir-a150)),(int)(yy2+iArrSize*sin(dir-a150)));
		pDC->MoveTo((int)xx2,(int)yy2);
		//---------------------------------
		pen.DeleteObject();
		pDC->SelectObject(pOldPen);
		*/
	}

	// ��������Ƕ�(��ͶӰ�й�)
	double KWindVectorDataRender::AdjustmentAngle(double lon,double lat)
	{
		double dValue = 0;

		/*
		TProjectParam* pPrjParam = m_pGParameter->GetProjectParam();
		//-------------------------------------
		int    ps		= pPrjParam->ps;
		float lon0		= pPrjParam->dCNLon;
		float lat0		= pPrjParam->dCNLat;
		int iMapScale	= pPrjParam->iMapScale;
		//-------------------------------------

		if(ps==psNone)					dValue = 0.0;			//00 û��ͶӰ
		else if(ps==psLambert1)			dValue = lon - lon0;	//02 ������ͶӰ(��ͶӰ)
		else if(ps==psBothte)			dValue = lon - lon0;	//03 ��˹��ͶӰ
		else if(ps==psMecator)			dValue = lon - lon0;	//04 ī����ͶӰ
		else if(ps==psGauss_Krivger)	dValue = lon - lon0;	//05 ��˹-������ͶӰ
		else if(ps==psSanson)			dValue = 0.0;			//06 ɣ��ͶӰ
		else if(ps==psMelwit)			dValue = 0.0;			//07 Ħ������ͶӰ
		else if(ps==psEckert_Sin)		dValue = 0.0;			//08 ����������αԲ��ͶӰ
		else if(ps==psEckert_Ellipse)	dValue = 0.0;			//09 ��������ԲαԲ��ͶӰ
		else if(ps==psBonne)			dValue = 0.0;			//10 ����ͶӰ
		else if(ps==psAmerican)			dValue = 0.0;			//11 ������Բ׶ͶӰ
		else if(ps==psNorth)			dValue = 0.0;			//12 �������򼫵ȽǷ�λͶӰ
		else if(ps==psSouth)			dValue = 0.0;			//13 �ϰ����򼫵ȽǷ�λͶӰ
		else if(ps==psLambert2)			dValue = lon - lon0;	//14 ������ͶӰ(��ͶӰ)
		//-------------------------------------
		//-------------------------------------
		*/

		return dValue;
	}

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------

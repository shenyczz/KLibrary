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

	// 渲染
	void KWindVectorDataRender::Rendering(HDC hDC, LPVOID lpvData)
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
		// 是否可见
		if(!pLayerProperty->IsVisible())
			return;

		// 5.数据提供者
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// 6.数据对象(注意修改)
		KWindVectorData* pDataObject = (KWindVectorData*)pProvider->GetDataObject();
		if(!pDataObject)
			return;
		// 为渲染器注册回调
		this->RegiestCallbackFunc(pDataObject->CallbackFunc(),pDataObject->CallbackParam());
		// 数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		//-------------------------------------------------
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
		KExtents bbox = pDataObject->GetExtents();
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
		// 矢量显示类型
		WORD windVector = pLayerProperty->WindVector();
		//===========================================
		//windVector = 0
		//	| WindVector_Barb
		//	| WindVector_Arrow
		//	| WindVector_StreamLine
		//	;	// 测试
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

	// 绘制风羽
	void KWindVectorDataRender::DrawBarb(Graphics* pGraphics)
	{
		// 1.设备场景
		HDC hDC = m_hDC;
		// 2.地图引擎
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		// 3.图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		// 4.数据提供者
		KProvider* pProvider = pLayerProperty->GetProvider();
		// 5.数据对象
		KWindVectorData* pDataObject = (KWindVectorData*)pProvider->GetDataObject();
		// 6.数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();

		//-------------------------------------------------
		// 下面绘制 WindBarb
		//-------------------------------------------------
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(0);
		BYTE g = GetGValue(0);
		BYTE b = GetBValue(0);

		if(pDataObject->IsEmpty())
			return;

		// 临时变量
		double x, y;		// 位置
		double lon, lat;	// 经度、纬度
		double adj;			// 角度调整
		double u,v;			// 风场uv分量
		double dir,spd;		// 方向,速度

		int nx		= pDataInfo->XNum();		// x 方向格点数
		int ny		= pDataInfo->YNum();		// y 方向格点数
		double* px	= pDataObject->GetXData();	// x 坐标数据指针
		double* py	= pDataObject->GetYData();	// y 坐标数据指针
		double* pu	= pDataObject->GetUData();	// u 数据指针
		double* pv	= pDataObject->GetVData();	// v 数据指针

		// 风钩颜色
		COLORREF clrBarb = pLayerProperty->BarbColor();
		r = GetRValue(clrBarb);
		g = GetGValue(clrBarb);
		b = GetBValue(clrBarb);
		// 风钩步长
		int iStep = pLayerProperty->BarbStep();
		if(iStep<1 || iStep>9) iStep = 1;
		for(int i = 0; i < ny; i += iStep)		// 行
		{
			for(int j = 0; j < nx; j += iStep)	// 列
			{
				// 位置坐标
				x = px[j];			y = py[i];
				lon = x;			lat = y;
				u = *(pu+i*nx+j);	v = -*(pv+i*nx+j);

				// 角度方向 direction
				if (v==0.0 && u==0.0)
					dir = 0.0;
				else
					dir = atan2(v,u) * 180 / PI;

				//速度
				spd = hypot(u,v);

				// 对当前地图投影和位置做角度调整,
				adj = AdjustmentAngle(lon,lat);

				// 投影变换
				pMapEngine->WorldToClient(x,y);

				//绘制
				_DrawOneBarb (pGraphics, x, y, dir+adj, spd, clrBarb);

			}// next j

		}//next i

		return;
	}
	// 绘制风箭
	void KWindVectorDataRender::DrawArrow(Graphics* pGraphics)
	{
		// 1.设备场景
		HDC hDC = m_hDC;
		// 2.地图引擎
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		// 3.图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		// 4.数据提供者
		KProvider* pProvider = pLayerProperty->GetProvider();
		// 5.数据对象
		KWindVectorData* pDataObject = (KWindVectorData*)pProvider->GetDataObject();
		// 6.数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();

		//-------------------------------------------------
		// 下面绘制 WindArrow
		//-------------------------------------------------
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(0);
		BYTE g = GetGValue(0);
		BYTE b = GetBValue(0);

		if(pDataObject->IsEmpty())
			return;

		// 临时变量
		double x, y;		// 位置
		double lon, lat;	// 经度、纬度
		double adj;			// 角度调整
		double u,v;			// 矢量的 u v 分量
		//double plen,blen,rad,dir,spd;
		double dir,spd;

		int nx		= pDataInfo->XNum();		// x 方向格点数
		int ny		= pDataInfo->YNum();		// y 方向格点数
		double* px	= pDataObject->GetXData();	// x 坐标数据指针
		double* py	= pDataObject->GetYData();	// y 坐标数据指针
		double* pu	= pDataObject->GetUData();	// u 数据指针
		double* pv	= pDataObject->GetVData();	// v 数据指针

		// 风矢颜色
		COLORREF clrArrow = pLayerProperty->ArrowColor();
		r = GetRValue(clrArrow);
		g = GetGValue(clrArrow);
		b = GetBValue(clrArrow);
		// 风矢步长
		int iStep = pLayerProperty->ArrowStep();
		if(iStep<1 || iStep>9) iStep = 1;
		for(int i=0; i<ny; i+=iStep)		//行
		{
			for(int j=0; j<nx; j+=iStep)	//列
			{
				x = px[j];			y = py[i];
				u = *(pu+i*nx+j);	v = -*(pv+i*nx+j);
				lon = x;			lat = y;

				if(i==iStep)
				{
					int iii=0;
				}

				//角度方向 direction
				if (v==0.0 && u==0.0)
					dir = 0.0;
				else
					dir = atan2(v,u) * 180 / PI;

				//速度
				spd = (float)hypot(u,v);

				// 对当前地图投影和位置做角度调整,
				adj = AdjustmentAngle(lon,lat);

				// 投影变换
				pMapEngine->WorldToClient(x,y);

				//绘制
				_DrawOneArrow (pGraphics, x, y, dir+adj, spd, clrArrow);

			}//next j

		}// next i

		/*
		for(int i=0;i<ny;i+=iStep)		//行
		{
			for(int j=0;j<nx;j+=iStep)	//列
			{


			}

		}//for(i)

		return;
		*/
	}
	// 绘制流线
	void KWindVectorDataRender::DrawStreamLine(Graphics* pGraphics)
	{
		// 1.设备场景
		HDC hDC = m_hDC;
		// 2.地图引擎
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		// 3.图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		// 4.数据提供者
		KProvider* pProvider = pLayerProperty->GetProvider();
		// 5.数据对象
		KWindVectorData* pDataObject = (KWindVectorData*)pProvider->GetDataObject();
		// 6.数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();

		//-------------------------------------------------
		// 下面绘制 WindStreamLine
		//-------------------------------------------------
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(0);
		BYTE g = GetGValue(0);
		BYTE b = GetBValue(0);

		if(pDataObject->IsEmpty())
			return;

		// 临时变量
		// 声明局部变量
		int ii,jj,ii1,jj1,ij1;
		int ipt,acc,ijsav;
		double x,y,xsav,ysav,xold,yold;
		double xx,yy,xxsav,yysav;
		//double xx,yy;
		int jcol,scol;
		int acnt,bflg;
		int loopStep,loopStepMax = 1000;
		double *up, *vp;				//u v数据指针
		double uu,vv,uu1,uu2,vv1,vv2,auu,avv;

		int nx		= pDataInfo->XNum();			// x 方向格点数
		int ny		= pDataInfo->YNum();			// y 方向格点数
		double xmin	= pDataInfo->XMin();			// x 坐标最小值
		double xmax	= pDataInfo->XMax();			// x 坐标最大值
		double ymin	= pDataInfo->YMin();			// y 坐标最小值
		double ymax	= pDataInfo->YMax();			// y 坐标最大值
		double* px	= pDataObject->GetXData();		// x 坐标数据指针
		double* py	= pDataObject->GetYData();		// y 坐标数据指针
		double* pu	= pDataObject->GetUData();		// u 数据指针
		double* pv	= pDataObject->GetVData();		// v 数据指针

		// 流线颜色
		COLORREF clrStreamLine = pLayerProperty->StreamLineColor();
		r = GetRValue(clrStreamLine);
		g = GetGValue(clrStreamLine);
		b = GetBValue(clrStreamLine);
		// 流线密度
		int iDensity = pLayerProperty->StreamLineStep();
		if(iDensity<1 || iDensity>5) iDensity = 1;

		double YMAX	= 87.0;	// 防止y=90(度)

		scol = -9;
		jcol = 1;

		// 根据流线密度计算出标记网格的间隔
		int k = max(nx,ny);
		int iScale = 200/k + iDensity-5;
		if(iScale<1)	iScale = 1;
		if(iScale>10)	iScale = 10;

		iScale = iDensity;	// 把原来的一个网格分成iScale份
		double fact = 1.0 / iScale;
		double dx = (px[1]-px[0]) / iScale;
		double dy = (py[1]-py[0]) / iScale;

		// 为标记网格分配内存
		int nCol = (nx-1)*iScale+1;	// 加密后的列数
		int nRow = (ny-1)*iScale+1;	// 加密后的行数
		int iSize = nCol*nRow;		// 加密后总的网格点数
		int* pFlag = (int*)malloc(sizeof(int)*iSize);
		if(!pFlag) return;
		memset(pFlag,0,sizeof(int)*iSize);

		//Look through flag grid to lookfor start of streamline.
		//To start requires no streams drawn within surrounding flag box
		// 扫描全部标记网格查找流线线头,找到一个就绘制该流线
		// 同时标记扫描过的格点
		int col=0,row=0;
		int iStep = 1;
		bool bAdd = true;

		for(int i=0;i<iSize;i++)	// 919 iSize
		{
			// 查看包围盒中是否有流线经过,如果没有就可以确定
			// 此包围盒中存在流线线头
			int dis = 2;			// 包围盒边长
			if(iDensity<5) dis=3;	// 流线密度小,包围盒大
			if(iDensity>5) dis=1;	// 流线密度大,包围盒小

			// 根据包围盒边长、当前的行列确定包围盒包围的行列
			int cmin,cmax,rmin,rmax;
			cmin=col-dis; cmax=col+dis+1;
			rmin=row-dis; rmax=row+dis+1;
			if(cmin<0)		cmin=0;
			if(cmax>nCol)	cmax=nCol;
			if(rmin<0)		rmin=0;
			if(rmax>nRow)	rmax=nRow;

			// 查看包围盒中是否有流线经过
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

			if(acc==0)	//acc=0 表明包围盒中没有流线经过
			{
				// 确定流线线头
				int oldCol = col/iScale;			// 加密前的列
				int oldRow = row/iScale;			// 加密前的行
				x = *(px+oldCol)+dx*(col%iScale);	// 求得当前列坐标
				y = *(py+oldRow)+dy*(row%iScale);	// 求得当前行坐标
				xsav = x; ysav = y;					// 保存坐标

				xx = bAdd ? x+1.0 : x;
				yy = bAdd ? y+1.0 : y;

				//坐标变换
				pMapEngine->WorldToClient(xx,yy);

				::MoveToEx(hDC,(int)xx,(int)yy,NULL);	// 移动到线头
				xxsav = xx; yysav = yy;					// 保存坐标变换后的坐标

				ijsav = -999;
				acc = 0;
				acnt = 0;	//
				bflg = 0;

				loopStep=0;
				while(x>=xmin && x<xmax && y>=ymin && y<ymax && y<YMAX) // while - 1
				{
					if(loopStep++ > loopStepMax)
						break;

					// 根据x,y求出加密后所在的行、列
					ii = (int)((y-ymin)/dy); jj = (int)((x-xmin)/dx);

					// 转换为加密前所在的行、列
					ii /= iScale; jj /= iScale;

					// 当前坐标和标准坐标的差值
					xx = x - *(px+jj); yy = y - *(py+ii);

					// 加密前所在的行、列的uv指针
					up = pu+ii*nx+jj;	vp = pv+ii*nx+jj;	//

					// 插值求得当前坐标处的u v值
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

					// 求出下一个点的坐标
					x=x+uu; y=y+vv;

					// 根据x,y求出加密后所在的行、列
					ii1=(int)((y-ymin)/dy);	//列
					jj1=(int)((x-xmin)/dx);	//行

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

					// 坐标变换
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
						// 绘制箭头
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

				// 坐标变换
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

					//根据x,y求出加密后所在的行、列
					ii=(int)((y-ymin)/dy);	jj=(int)((x-xmin)/dx);

					//转换为加密前所在的行、列
					ii/=iScale;	jj/=iScale;
					//当前坐标和标准坐标的差值
					//xx=dx; yy=dy;
					//xx=1/iScale; yy=1/iScale;
					xx = x - *(px+jj); yy = y - *(py+ii);

					//加密前所在的行、列的uv指针
					up=pu+ii*nx+jj;	vp=pv+ii*nx+jj;	//

					//插值求得当前坐标处的u v值
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

					//求出下一个点的坐标
					x=x-uu; y=y-vv;	

					//根据x,y求出加密后所在的行、列
					ii1=(int)((y-ymin)/dy);	//列
					jj1=(int)((x-xmin)/dx);	//行

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
					// 坐标变换
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

		//释放内存
		free(pFlag);

		return;
	}

	// 在某位置绘制风羽
	void KWindVectorDataRender::_DrawOneBarb(Graphics* pGraphics,double x,double y,double dDirection,double dSpeed,COLORREF clr)
	{
		// 图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		// 声明局部变量
		int i;
		double x1,y1,x2,y2,x3,y3;
		//double dWindSpeed0;
		//int nTriangle;				// 三角数
		//double dLine0,dLine1;	// 线数
		//BOOL bHalf;				// 是否有半线

		// 处理角度,使风杆指明风吹来的方向
		dDirection += 180;

		//if(dDirection<0)
		//	dDirection += 360;
		//if(dDirection>=360)
		//	dDirection -= 360;

		//double a70 = 70.0 * PI / 180.0;
		double a70 = 70.0;

		// 风向风速
		double x0 = x;
		double y0 = y;
		double dAangle = dDirection;	// 风杆的角度

		int lenTotal = 24;		// 风向竿总长度
		int lenBarb = 12;		// 风速竿(倒钩)长度
		int nSpace = 5;		// 风速竿间隔

		/// 下面绘制
		///
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(clr);
		BYTE g = GetGValue(clr);
		BYTE b = GetBValue(clr);

		double svA = sin(dAangle * PI / 180);
		double cvA = cos(dAangle * PI / 180);

		// 源点
		//pGraphics->DrawEllipse(&Pen(Color(a,r,g,b)),(REAL)x0,(REAL)y0,(REAL)2,(REAL)2);

		// 风速小于1或大于100不画风向竿
		if(dSpeed<1 || dSpeed>100)
		{
			// 源点
			pGraphics->DrawEllipse(&Pen(Color(a,r,g,b)),(REAL)x0,(REAL)y0,(REAL)4,(REAL)4);
			//pGraphics->FillEllipse(&SolidBrush(Color(a,r,g,b)),(REAL)x0,(REAL)y0,(REAL)4,(REAL)4);
			return;
		}

		// 风速大于等于1小于2不画风速线
		if(dSpeed<2)
		{
			return;
		}

		// 画风向竿
		double xx = x0 + lenTotal * cvA;
		double yy = y0 + lenTotal * svA;
		pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x0,(REAL)y0,(REAL)xx,(REAL)yy);

		// 画风速线
		double dBangle = dAangle + a70;	// 风羽的角度

		double svB = sin(dBangle * PI / 180);
		double cvB = cos(dBangle * PI / 180);

		//if(dBangle<20) svB = 0.683;
		//if(dBangle>70) cvB = 0.683;

		// 风速大于等于2小于3画留头半线
		if(dSpeed<3)
		{
			// 画风向竿
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

		// 风速大于等于3小于等于100画最多5个三角或4个三角四根半线
		if(dSpeed<100 || fabs(dSpeed-100)<EPS)
		{
			// 画风向竿
			//double xx = x0 + lenTotal * cvA;
			//double yy = y0 + lenTotal * svA;
			//pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x0,(REAL)y0,(REAL)xx,(REAL)yy);

			// 1.画三角形
			//----------
			// 计算三角形数量(风速>20 画一个三角形)
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

			// 2.画直线
			//----------
			// 余下的风速1(<20)
			double dWindSpeed1	= dSpeed - nTriangle * 20;
			int nLine = (int)(dWindSpeed1 / 4);		// 线数(4m/s一条线)
			// 余下的风速2(<4)
			double dWindSpeed2 = dWindSpeed1 - 4.0 * nLine;	// 线数
			if(dWindSpeed2>2) nLine += 1;
			for(i=0;i<(int)nLine;i++)
			{
				x1 = x0+(lenTotal-(i+(int)nTriangle)*nSpace)*cvA;
				y1 = y0+(lenTotal-(i+(int)nTriangle)*nSpace)*svA;
				x2 = x1+lenBarb*cvB;
				y2 = y1+lenBarb*svB;
				pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x1,(REAL)y1,(REAL)x2,(REAL)y2);
			}

			// 3.画半线
			//--------
			// 是否有半线
			BOOL bHalf = dWindSpeed2>0 && (dWindSpeed2<2 || fabs(dWindSpeed2-2)<EPS);
			if(bHalf)
			{
				x1 = x0+(lenTotal-(nTriangle+nLine)*nSpace)*cvA;
				y1 = y0+(lenTotal-(nTriangle+nLine)*nSpace)*svA;
				x2 = x1+lenBarb*cvB/2;	//半线
				y2 = y1+lenBarb*svB/2;
				pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x1,(REAL)y1,(REAL)x2,(REAL)y2);
			}

			return;

		}//dSpeed<=100

		return;
	}
	// 在某位置绘制风箭
	void KWindVectorDataRender::_DrawOneArrow(Graphics* pGraphics,double x,double y,double dDirection,double dSpeed,COLORREF clr)
	{
		// 图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		// 声明局部变量
		double x1,y1,x2,y2,x3,y3;

		double a150 = 150 * PI / 180;

		// 风向风速
		double x0 = x;
		double y0 = y;


		double dAangle = dDirection;	//风的方向(角度->弧度)

		dAangle = dAangle * PI / 180;	//风的方向(角度->弧度)

		/// 下面绘制
		///
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(clr);
		BYTE g = GetGValue(clr);
		BYTE b = GetBValue(clr);

		//如果速度<0.0001,绘制半径=0.01的圆
		//if (dSpeed<0.0001) {//Cricle(x,y,0.01);}

		int iArrLen		= int(dSpeed);	// 风箭长度
		double iArrSize	= 4;			// 箭头大小

		// 绘制箭杆
		x1 = x0+iArrLen*cos(dAangle);
		y1 = y0+iArrLen*sin(dAangle);
		pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x0,(REAL)y0,(REAL)x1,(REAL)y1);

		// 绘制箭头
		x2 = x1+iArrSize*cos(dAangle+a150);
		y2 = y1+iArrSize*sin(dAangle+a150);
		pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x1,(REAL)y1,(REAL)x2,(REAL)y2);

		x3 = x1+iArrSize*cos(dAangle-a150);
		y3 = y1+iArrSize*sin(dAangle-a150);
		pGraphics->DrawLine(&Pen(Color(a,r,g,b)),(REAL)x1,(REAL)y1,(REAL)x3,(REAL)y3);

		return;
	}
	// 在流线上绘制箭头
	void KWindVectorDataRender::_StreamArrow(Graphics* pGraphics,double xx1,double yy1,double xx2,double yy2,COLORREF clr)
	{
		// 图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		/// 下面绘制
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
		int nLineWidth	= GetLineWidth();	//笔宽
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

	// 调整风向角度(和投影有关)
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

		if(ps==psNone)					dValue = 0.0;			//00 没有投影
		else if(ps==psLambert1)			dValue = lon - lon0;	//02 兰勃托投影(切投影)
		else if(ps==psBothte)			dValue = lon - lon0;	//03 波斯托投影
		else if(ps==psMecator)			dValue = lon - lon0;	//04 墨卡托投影
		else if(ps==psGauss_Krivger)	dValue = lon - lon0;	//05 高斯-克吕格投影
		else if(ps==psSanson)			dValue = 0.0;			//06 桑生投影
		else if(ps==psMelwit)			dValue = 0.0;			//07 摩尔威特投影
		else if(ps==psEckert_Sin)		dValue = 0.0;			//08 爱凯特正弦伪圆柱投影
		else if(ps==psEckert_Ellipse)	dValue = 0.0;			//09 爱凯特椭圆伪圆柱投影
		else if(ps==psBonne)			dValue = 0.0;			//10 彭纳投影
		else if(ps==psAmerican)			dValue = 0.0;			//11 美国多圆锥投影
		else if(ps==psNorth)			dValue = 0.0;			//12 北半球球极等角方位投影
		else if(ps==psSouth)			dValue = 0.0;			//13 南半球球极等角方位投影
		else if(ps==psLambert2)			dValue = lon - lon0;	//14 兰勃托投影(割投影)
		//-------------------------------------
		//-------------------------------------
		*/

		return dValue;
	}

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------

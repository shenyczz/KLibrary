#include "StdAfx.h"
#include "KQContourTracer.h"

namespace ContourBuilder
{

	KQContourTracer::KQContourTracer(void)
		:m_nx(0) ,m_ny(0)
		,m_x0(0) ,m_y0(0)
		,m_xInterval(0)
		,m_yInterval(0)
		,m_xcols(0)
		,m_ycols(0)
		,m_xSide(0)
		,m_ySide(0)
	{
	}

	KQContourTracer::~KQContourTracer(void)
	{
		FreeMemory();
	}

	void KQContourTracer::PrepareMemory()
	{
		if(m_nx==0 || m_ny==0)
			throw;

		int rows = m_ny;
		int cols = m_nx;

		_deletea(m_xSide);
		_deletea(m_ySide);

		// 网格中存在rows*(cols-1)条横边,
		// 所以需要为m_xSide分配rows*(cols-1)空间就行
		m_xSide = new double[rows*(cols-1)];
		m_xcols = cols-1;

		// 网格中存在(rows-1)*cols条纵边
		// 所以需要为m_ySide分配(rows-1)*cols空间就行
		m_ySide = new double[(rows-1)*cols];
		m_ycols = cols;
	}

	void KQContourTracer::FreeMemory()
	{
		_deletea(m_xSide);
		_deletea(m_ySide);
	}

	// 追踪
	BOOL KQContourTracer::Tracing()
	{
		// 取得数据信息
		KDataInfo* pDataInfo = (KDataInfo*)this->GetDataInfo();
		if(pDataInfo==NULL)
			return FALSE;

		m_nx = pDataInfo->xNum;
		m_ny = pDataInfo->yNum;

		m_x0 = pDataInfo->xMin;
		m_y0 = pDataInfo->yMin;

		m_xInterval = pDataInfo->xInterval;
		m_yInterval = pDataInfo->yInterval;

		if(fabs(m_xInterval)<1.0e-12)
			return FALSE;

		if(fabs(m_yInterval)<1.0e-12)
			return FALSE;

		// 1.为xSide和ySide分配内存空间
		PrepareMemory();

		// 2.扫描网格纵横边,内插等值点
		BOOL b1 = InterpolateTracingValue();

		// 3.先追踪开路曲线
		BOOL b2 = TracingOpenedContour();

		// 4.再追踪闭合曲线
		BOOL b3 = TracingClosedContour();

		return b1&b2&b3;
	}

	// 扫描网格纵横边,内插等值点
	// 扫描网格的纵、横边，并线性插值计算等值点的情况
	// 将各边上的等值点情况存储于xSide和ySide数组中，
	// xSide存储所有横边上的等值线情况, ySide存储所有纵边上的等值点情况
	// 在插值计算时，对『与追踪值相等的数据』要进行修正处理后才计算，但在做修正处理时不要改变原来的数据
	BOOL KQContourTracer::InterpolateTracingValue()
	{
	/*
			 网格点标识如下:
	        
				(i+1,j)·--------·(i+1,j+1)
						|        |
						|        |
						|        |
						|        |
				 (i,j) ·--------·(i,j+1)

				  i:表示行号(向上增加)
				  j:表示列号(向右增加)
				  标识一个网格交点时，行号在前，列号在右，如：(i,j)

			  xSide,ySide中存储r值，(w为追踪值)
			  对于网格横边，r = (w - pData[i][j]) / (pData[i][j+1]-pData[i][j]);
			  对于网格纵边，r = (w - pData[i][j]) / (pData[i+1][j]-pData[i][j]);

			  由于浮点运算的误差，xSide[i][j],ySide[i][j]有可能等于1.0或0.0 
			  考虑如下情况：
			  (1)当追踪值与网格点上的值很接近(但不相等)时，由于运算误差，就会等于1.0
				 比如追踪0值时，遇到如下边:
					   20 ·--------·-0.00000016   此边上有0值，但计算 (0-20)/(-0.00000016-20) == 1.0 

	       
			  (2)当网格边上两端点上的值相差很悬殊时。
				 比如追踪2值，遇到如下边：
				 1.70141E+038 ·--------·1   此边上有2值，计算(2-1.70141E+038) / (1-1.70141E+038) == 1.0 

			网格边上有等值点时，理论上比例值不会等于0或1；
			 但由于计算误差，我们在算法中判断时，认为0或1也是有等值点的

			所以xSide,ySide中存储的值是[0,1]的闭区间，不是(0,1)的开区间
	*/

		Assert( m_pData != NULL );
		Assert( m_xSide != NULL );
		Assert( m_ySide != NULL );

		int i,j;
		int rows = m_ny;
		int cols = m_nx;

		double w   = m_dTraceValue; 
		double* pData = (double*)m_pData;	//网格数据

		double Z1,Z2; //分别记录一条边的两个点上的数据值
		double flag; 
		double shift = 0.001;  //修正值

		// 1.扫描并计算横边上的等值点,有rows*(cols-1)条横边需要扫描
		for(i=0;i<rows;i++)
		{
			for(j=0;j<cols-1;j++)
			{
				Z1 = *(pData+i*cols+j);
				Z2 = *(pData+i*cols+j+1);

				//两个点上的数据值相等则该边上不存在等值点
				if( Z1 == Z2 )
				{
					*(m_xSide+i*m_xcols+j) = -2.0;	// -2表示该边上不存在等值点
					continue;	//下一个横边
				}

				//Z1 != Z2,判断横边是否存在等值点
				Z1 = (w==Z1) ? Z1+shift : Z1;	//Z1和要追踪的值w相等，加一小的偏移量修正
				Z2 = (w==Z2) ? Z2+shift : Z2;	//Z2和要追踪的值w相等，加一小的偏移量修正

				flag = (w-Z1) * (w-Z2);
				if( flag > 0 )	//不存在等值点
				{
					//m_xSide[i][j] = -2.0f;
					*(m_xSide+i*m_xcols+j) = -2.0f;	//-2表示该边上不存在等值点
				}
				else if( flag < 0 )	//存在等值点
				{
					//计算等值点位置并保存在m_xSide[i][j],该值应 >=0 && <=1 
					*(m_xSide+i*m_xcols+j) = (w-Z1) / (Z2-Z1); 
					Assert(*(m_xSide+i*m_xcols+j)>=0 && *(m_xSide+i*m_xcols+j)<=1.0f);
				}

			}//end for(j)

		}//end for(i)

		// 2.扫描并计算纵边上等值点,有(rows-1)*cols条纵边需要扫描
		for(i=0;i<rows-1;i++)
		{
			for(j=0;j<cols;j++)
			{
				Z1 = *(pData+i*cols+j);            
				Z2 = *(pData+(i+1)*cols+j);		     
				
				//两个点上的数据值相等则该边上不存在等值点? waiting =0.5如何??????             
				if( Z1 == Z2 )
				{
					*(m_ySide+i*m_ycols+j) = -2.0;
					continue;
				}

				//Z1 != Z2,判断横边是否存在等值点
				Z1 = (w==Z1) ? Z1+shift : Z1;	//Z1和要追踪的值w相等，加一小的偏移量修正
				Z2 = (w==Z2) ? Z2+shift : Z2;	//Z2和要追踪的值w相等，加一小的偏移量修正

				flag = (w-Z1) * (w-Z2);
				if( flag > 0 )	//不存在等值点
				{
					*(m_ySide+i*m_ycols+j) = -2.0;
				}
				else if( flag < 0 )	//存在等值点
				{
					//计算等值点位置并保存在m_ySide[i][j],该值应 >=0 && <=1 
					*(m_ySide+i*m_ycols+j) = (w-Z1) / (Z2-Z1) ; 
					Assert(*(m_ySide+i*m_ycols+j)>=0 && *(m_ySide+i*m_ycols+j)<=1.0f);
				}

			}//end for(j)

		}//end for(i)

		return TRUE;
	}
	// 追踪开路等值线
	// 该函数的关键是找出开曲线的线头，并设置好追踪前进的方向的条件
	BOOL KQContourTracer::TracingOpenedContour()
	{
		int i,j;

		int rows = m_ny;	
		int cols = m_nx;

		// 1.搜索底边框m_xSide[0][j](自下而上追踪)	
		for(j=0; j<cols-1; j++)
		{
			double flag = *(m_xSide+0*m_xcols+j);
			if( IsExistPoint(flag) )	//a2点
			{
				//按自下向上的前进方向虚设前一点a1点的i,j
				m_PreviousPoint.i = -1; // 假设前一点在-1行,这样下一点到当前点的前进方向就是自下向上的
				m_PreviousPoint.j = j;
				m_PreviousPoint.bHorV = TRUE;	//横边

				m_CurrentPoint.i = 0; //底边的行号为0，所以设置线头的i为0
				m_CurrentPoint.j = j;
				m_CurrentPoint.bHorV = TRUE; //底边是横边

				TracingOneOpenedContour();	
			}
		}

		// 2.搜索左边框m_ySide[i][0](自左而右追踪)
		for(i=0; i<rows-1; i++)
		{
			if( IsExistPoint(*(m_ySide+i*m_ycols+0)) )
			{
				//按自左向右的前进方向虚设前一点a1的i,j
				m_PreviousPoint.i = i; 
				m_PreviousPoint.j = -1; //假设前一点在-1列,使其满足左-->右的前进方向
				m_PreviousPoint.bHorV = FALSE;	//纵边
				
				m_CurrentPoint.i = i; 
				m_CurrentPoint.j = 0; //左边框在第0列，所以设置线头的j为0
				m_CurrentPoint.bHorV = FALSE;		//纵边
				
				TracingOneOpenedContour();	
			}
		}

		// 3.搜索上边框m_xSide[rows-1][j](自上而下追踪)
		double deltX = m_xInterval;
		double curPt_Col_X = m_x0 + m_CurrentPoint.j * deltX; //当前等值点所在边(i,j)的所标识的j列的X坐标
		
		for(j=0; j<cols-1; j++)
		{
			if( IsExistPoint(*(m_xSide+(rows-1)*m_xcols+j)) )
			{
				//虚设出由上向下追踪的条件
				//  由上向下追踪的条件如下： 	 
				//  1. Not( CurrentPoint.i > PreviousPoint.i )
				//  2. Not( CurrentPoint.j > PreviousPoint.j )
				//  3. CurrentPoint.x > m_gridDataInfo.xMin + CurrentPoint.j * deltX; 即:要求在横边上
				//
				m_PreviousPoint.i = rows-1; 
				m_PreviousPoint.j = j;
				m_PreviousPoint.bHorV = TRUE;
				
				m_CurrentPoint.i = rows-1; //上边框的行号为rows-1
				m_CurrentPoint.j = j;
				m_CurrentPoint.bHorV = TRUE; //使其符合第三个条件

				//第三个条件条件不用虚设，只要边上存在就会满足第三个条件，因为上边框是横边
	//			if( CurrentPoint.x < curPt_Col_X )
	//			{
	//				CurrentPoint.x = curPt_Col_X + 1; //为了保证让其满足第3个条件
	//			}
				
				TracingOneOpenedContour();	
			}
		}

		//4.搜索右边框m_ySide[i][cols-1](自右而左追踪)
		for(i=0; i<rows-1; i++)
		{
			if( IsExistPoint(*(m_ySide+i*m_ycols+cols-1)) )
			{
				//虚设出由右向左追踪的条件
				//
				//  由右向左追踪的条件如下： 	 
				//  1. Not( CurrentPoint.i > PreviousPoint.i )
				//  2. Not( CurrentPoint.j > PreviousPoint.j )
				//  3. Not( CurrentPoint.x > m_gridDataInfo.xMin + CurrentPoint.j * deltX ); 要求在纵边上			
				//
				m_PreviousPoint.i = i; 
				m_PreviousPoint.j = cols-1; 
				m_PreviousPoint.bHorV = FALSE;
				
				m_CurrentPoint.i = i; 
				m_CurrentPoint.j = cols-1; //右边框在第cols-1列
				m_CurrentPoint.bHorV = FALSE;  //使其符合第三个条件

	//			//虚设出第三个条件
	//			CurrentPoint.x = curPt_Col_X - 1;//减去1，使之比curPt_Col_X小
				
				TracingOneOpenedContour();	
			}
		}

		return TRUE;
	}
	// 追踪闭合曲线
	BOOL KQContourTracer::TracingClosedContour()
	{
		int cols = m_nx;
		int rows = m_ny;

		//搜索所有的除了边框外的纵边,从左到右搜索每一列上的纵边，对于一列，从下到上搜索
		for(int j=1; j<cols-1; j++) //j从1开始
		{
			for(int i=0; i<rows-1; i++) //i从0开始
			{
				if( IsExistPoint(*(m_ySide+i*m_ycols+j))) 
				{
					TracingOneClosedContour(i,j);				
				}
				
			}
		}

		return TRUE;
	}

	// 是否存在等值点
	BOOL KQContourTracer::IsExistPoint(double flag)
	{
		return (flag<0.0 || flag>1.0) ? FALSE : TRUE;
	}

	// 追踪一条开路等值线
	void KQContourTracer::TracingOneOpenedContour()
	{
		//记录下线头所在边的i,j 和 横纵边标识
		int startPt_i = m_CurrentPoint.i; 
		int startPt_j = m_CurrentPoint.j;
		BOOL startPt_bHoriz = m_CurrentPoint.bHorV;	// 横纵边标识

		// 验证线头在边界上
		Assert( startPt_i == 0  ||  //线头在底边界上
			startPt_i == m_ny-1 ||  //线头在上边界上
			startPt_j == 0      ||  //线头在左边界上
			startPt_j == m_nx-1     //线头在右边界上 
			);

		int rows = m_ny;
		int cols = m_nx;

		// 注释信息
		TCHAR szComment[MAX_PATH]=_T("");
		_stprintf(szComment,_T("Contour-%.3f"),m_dTraceValue);

		// 分配一条曲线
		KCurve* pCurve = new KCurve();	// 为一条等值线分配内存空间
		pCurve->SetFlag(++m_dwTracerFlag);

		// 设置等值线属性
		pCurve->SetValue(m_dTraceValue);	// 等值线的值

		//pSfxPolygon->SetLineWidth(1);						//多边形线宽
		//pSfxPolygon->SetLineType(PS_SOLID);					//多边形线型
		//pSfxPolygon->SetFillType(ftNone);					//多边形填充类型
		//pSfxPolygon->SetColorFlag(SNYC_COLOR_FLAG_INDEX);	//多边形线条颜色标记
		//pSfxPolygon->SetColorIndex(SNYC_CTI_FORE);			//多边形线条颜色
		//pSfxPolygon->SetComment(szComment);					//多边形注释信息

		// 添加一条等值线
		this->AddCurve(pCurve);

		// 计算出等值线头的坐标
		CalcOnePointCoord(startPt_i,startPt_j,startPt_bHoriz,m_CurrentPoint.x,m_CurrentPoint.y);

		// 保存头坐标
		pCurve->Add( new KPoint(m_CurrentPoint.x,m_CurrentPoint.y) );

		// 表示已经追踪过
		if( startPt_bHoriz )	// 横边
		{
			*(m_xSide+startPt_i*m_xcols+startPt_j) = -2.0;
		}
		else
		{
			*(m_ySide+startPt_i*m_ycols+startPt_j) = -2.0;
		}

		// 追踪下一个点
		TracingNextPoint(pCurve);
		
		m_PreviousPoint = m_CurrentPoint;
		m_CurrentPoint = m_NextPoint;

		//遇到网格边界就结束追踪

		//------2004/03/09修改------------------
		//为了不让浮点数计算的误差引起追踪结束条件的判断失误，
		//我们不能用『CurrentPoint.y <= yMin』来判断是否遇到底边框,
		//  也不能用『CurrentPoint.x <= xMin』来判断是否遇到左边框
		BOOL bIsFinish = (m_CurrentPoint.i == 0 && m_CurrentPoint.bHorV == TRUE )  ||  //遇到底边界,(注:不能仅仅用CurrentPoint.i == 0判断)
			(m_CurrentPoint.i == rows-1)                            ||  //遇到上边界
			(m_CurrentPoint.j == 0 && m_CurrentPoint.bHorV == FALSE ) ||  //遇到左边界,(注:不能仅仅用CurrentPoint.j == 0判断)
			(m_CurrentPoint.j == cols-1) ;                              //遇到右边界

		while( !bIsFinish )	//遇到边界
		{
			TracingNextPoint(pCurve);
			
			m_PreviousPoint = m_CurrentPoint;
			m_CurrentPoint = m_NextPoint;
			
			bIsFinish = (m_CurrentPoint.i == 0 && m_CurrentPoint.bHorV == TRUE )  ||
				(m_CurrentPoint.i == rows-1)                            || 
				(m_CurrentPoint.j == 0 && m_CurrentPoint.bHorV == FALSE ) ||
				(m_CurrentPoint.j == cols-1) ;
			
		}
	}

	// 追踪一条闭合等值线
	void KQContourTracer::TracingOneClosedContour(int startPt_i, int startPt_j)
	{
		// 参数i,j是该闭曲线的第一个等值点的i,j

		// 虚设前一等值点的i,j，让其满足从左向右追踪的条件
		m_PreviousPoint.i = startPt_i;
		m_PreviousPoint.j = 0;
		m_PreviousPoint.bHorV = FALSE;
		
		m_CurrentPoint.i = startPt_i;
		m_CurrentPoint.j = startPt_j;
		m_CurrentPoint.bHorV = FALSE;   // 是FALSE，因为是在纵边上

		int rows = m_ny;
		int cols = m_nx;

		// 注释信息
		TCHAR szComment[MAX_PATH]=_T("");
		_stprintf(szComment,_T("Contour-%.3f"),m_dTraceValue);

		// 分配一条曲线
		KCurve* pCurve = new KCurve();	// 为一条等值线分配内存空间
		pCurve->SetFlag(++m_dwTracerFlag);

		// 设置等值线属性
		pCurve->SetValue(m_dTraceValue);	// 等值线的值

		//pSfxPolygon->SetLineWidth(1);						//多边形线宽
		//pSfxPolygon->SetLineType(PS_SOLID);					//多边形线型
		//pSfxPolygon->SetFillType(ftNone);					//多边形填充类型
		//pSfxPolygon->SetColorFlag(SNYC_COLOR_FLAG_INDEX);	//多边形线条颜色标记
		//pSfxPolygon->SetColorIndex(SNYC_CTI_FORE);			//多边形线条颜色
		//pSfxPolygon->SetComment(szComment);					//多边形注释信息

		// 添加一条等值线
		this->AddCurve(pCurve);

		// 计算出等值线头的坐标
		// FALSE => 线头在纵边上
		CalcOnePointCoord(startPt_i,startPt_j,FALSE,m_CurrentPoint.x,m_CurrentPoint.y);

		// 保存头坐标
		pCurve->Add( new KPoint(m_CurrentPoint.x,m_CurrentPoint.y) );

		// 追踪下一个等值点
		TracingNextPoint(pCurve);

		m_PreviousPoint = m_CurrentPoint;
		m_CurrentPoint = m_NextPoint;

		BOOL bColsed = FALSE;

		while(!bColsed) 
		{
			TracingNextPoint(pCurve);
			
			m_PreviousPoint = m_CurrentPoint;
			m_CurrentPoint = m_NextPoint;
			
			//------2004/03/09修改------------------
			//用等值点所在边的标识i,j来判断曲线是否封闭，不用x、y坐标来比较，因为浮点数计算会出现误差
			//	bColsed = (CurrentPoint.x == startPt_x) && (CurrentPoint.y == startPt_y);
			bColsed = ( m_CurrentPoint.i == startPt_i ) && 
				( m_CurrentPoint.j == startPt_j ) && 
				( m_CurrentPoint.bHorV == FALSE );
			
			
		}

		*(m_ySide+startPt_i*m_ycols+startPt_j) = -2.0f; //已经追踪过
	}

	// 计算出等值线头的坐标
	void KQContourTracer::CalcOnePointCoord(int i, int j, BOOL bHorizon,double &x, double &y)
	{
		//X方向上两点间的距离
		double deltX = m_xInterval;

		//Y方向上两点间的距离
		double deltY = m_yInterval;
		
		
		if(bHorizon)	//在横边上
		{
			x = m_x0 + (j + *(m_xSide+i*m_xcols+j)) * deltX;
			y = m_y0 + i * deltY;		
		}
		else	//在纵边上
		{
			x = m_x0 + j * deltX;
			y = m_y0 + ( i + *(m_ySide+i*m_ycols+j)) * deltY;
		}
	}

	// 追踪出下一个点a3
	void KQContourTracer::TracingNextPoint(KCurve* pCurve)
	{
	 // 1.先确定出等值线的前进方向(自下向上、由左向右、自上向下、由右向左，其中之一)
	 // 2.再追踪下一个等值点

	 // 前进方向可以如下判定：
	 //   if( 当前点.行号 > 前一点.行号 )
	 //   {
		//    下---->上
		//}
		//else if( 当前点.列号 > 前一点.列号 )
	 //   {
		//	左---->右
		//}
	 //   else if( 当前点在横边上 )
		//{
		//	上---->下
		//}
		//else
		//{
		//     右---->左
		//}
	       

	#define  _CALL_TRACINGFUN_VERSION2

		if(m_CurrentPoint.i > m_PreviousPoint.i )
		{
			//TRACE0("下--->上\n");
			#ifdef _CALL_TRACINGFUN_VERSION2
				FromBottom2TopTracingV2();
			#else
				FromBottom2TopTracing();
			#endif
		}
		else if(m_CurrentPoint.j > m_PreviousPoint.j )
		{
			//TRACE0("左--->右\n");
			#ifdef _CALL_TRACINGFUN_VERSION2
				FromLeft2RightTracingV2();
			#else
				FromLeft2RightTracing();
			#endif
			
		}
		else if( m_CurrentPoint.bHorV == TRUE /*curPt_Col_X < CurrentPoint.x*/ ) 
		{//在横边上。CurrentPoint.bHorizon == TRUE 和 curPt_Col_X < CurrentPoint.x ，这两个条件等价
			
			
			Assert( m_CurrentPoint.i <= m_PreviousPoint.i &&
					m_CurrentPoint.j <= m_PreviousPoint.j );

			//TRACE0("上--->下\n");
			#ifdef _CALL_TRACINGFUN_VERSION2
				FromTop2BottomTracingV2();
			#else
				FromTop2BottomTracing();
			#endif
		
		}
		else
		{
			Assert(m_CurrentPoint.bHorV==FALSE);//在纵边上

			Assert( m_CurrentPoint.i <= m_PreviousPoint.i &&
					m_CurrentPoint.j <= m_PreviousPoint.j );
			
			//TRACE0("右--->左\n");
			#ifdef _CALL_TRACINGFUN_VERSION2
				FromRight2LeftTracingV2();
			#else
				FromRight2LeftTracing();
			#endif
		
		}

		// 添加下一点
		pCurve->Add( new KPoint(m_NextPoint.x,m_NextPoint.y) );
	}

	//自下向上
	void KQContourTracer::FromBottom2TopTracing()
	{
	/*   
						   横边(i+1,j)
						  xSide[i+1][j]

							   ↑
					(i+1,j)    ∣    (i+1,j+1)
						|-----------|
						|           |
		  纵边(i,j)---→|           |←----纵边(i,j+1)
		  ySide[i][j]	|           |     ySide[i][j+1]
						|           |
						|-----·----|    
					(i,j)      ↖     (i,j+1)
								 ＼
								   等前等值点(P2)
	        
			  等值线自下向上前进时，网格单元的情况如上：
			  当前等值点(用P2表示)在网格单元的底边上，那么下一等值点(用P3表示)所在的位置有三种情况：
				 1.在纵边(i,j)上
				 2.在纵边(i,j+1)上
				 3.在横边(i+1,j)上
			 但实际追踪时只能选择其中之一

			 程序判断的依据即是：ySide[i][j] 、 ySide[i][j+1] 、xSide[i+1][j] 

		   假设纵边(i,j)上存在等值点P31, 纵边(i,j+1)上存在等值点P33, 横边(i+1,j)上存在等值点P32
		   选择的次序如下：
		   1.当P31，P33都存在时，选择靠近网格底边者为P3(比较ySide[i][j]、ySide[i][j+1])
		   2.若P31,P33靠近底边的距离相同，则选择与P2点距离近者为P3
		   3.当P31,P33中只有一个存在时，则存在点即为P3
		   4.当无P31,P33存在时，对边必定存在P32作为P3

	*/
		Assert( m_CurrentPoint.i > m_PreviousPoint.i );   //当前点的行号 > 前一点的行号
		Assert( m_CurrentPoint.bHorV == TRUE );
		

		int i = m_CurrentPoint.i; 
		int j = m_CurrentPoint.j;

		double* pGridData = (double*)this->GetData();

		if(IsExistPoint(*(m_ySide+i*m_ycols+j)) && IsExistPoint(*(m_ySide+i*m_ycols+j+1)))
		{
			
			if( *(m_ySide+i*m_ycols+j) < *(m_ySide+i*m_ycols+j+1) )
			{
				HandlingAfterNextPointFounded(i,j,FALSE);//在纵边(i,j)上
			}
			else if( *(m_ySide+i*m_ycols+j) == *(m_ySide+i*m_ycols+j+1) )
			{
				//注意:这里需要重新计算xSide[i][j]，不能直接用xSide[i][j],因为在上一次已经被置为-2.0
				double xSideIJ = (m_dTraceValue - *(pGridData+i*m_nx+j)) / ( *(pGridData+i*m_nx+j+1) - *(pGridData+i*m_nx+j) );
				if( xSideIJ <= 0.5f )
				//if( xSide[i][j] <= 0.5f )
				{
					//TRACE3("m_xSide[%d][%d]=%f,左-->右----遇到两边r值相等\n",i,j,*(m_xSide+i*m_xcols+j));
					HandlingAfterNextPointFounded(i,j,FALSE);//在纵边(i,j)上
				}
				else
				{
					HandlingAfterNextPointFounded(i,j+1,FALSE);//在纵边(i,j+1)上
				}

			}
			else
			{
				HandlingAfterNextPointFounded(i,j+1,FALSE);//在纵边(i,j+1)上
			}

		}
		else
		{
			if( IsExistPoint(*(m_ySide+i*m_ycols+j)) )
			{
				HandlingAfterNextPointFounded(i,j,FALSE);//在纵边(i,j)上
			}
			else if( IsExistPoint(*(m_ySide+i*m_ycols+j+1)) )
			{
				HandlingAfterNextPointFounded(i,j+1,FALSE);//在纵边(i,j+1)上
			}
			else if( IsExistPoint(*(m_xSide+(i+1)*m_xcols+j)) )
			{//两纵边上都没有
				HandlingAfterNextPointFounded(i+1,j,TRUE);//在横边(i+1,j)上
			}
			else
			{//三边上都没有,数据插值出现错误
				Assert(FALSE);
			}
		}

	}

	//自下向上(Ver 2)
	void KQContourTracer::FromBottom2TopTracingV2()
	{
	/*	                   横边(i+1,j)
						  xSide[i+1][j]

							   ↑
					(i+1,j)    ∣    (i+1,j+1)
						|-----------|
						|           |
		  纵边(i,j)---→|           |←----纵边(i,j+1)
		  ySide[i][j]	|           |     ySide[i][j+1]
						|           |
						|-----·----|    
					(i,j)      ↖     (i,j+1)
								 ＼
								   等前等值点
	*/
		Assert( m_CurrentPoint.i > m_PreviousPoint.i );   //当前点的行号 > 前一点的行号
		Assert( m_CurrentPoint.bHorV == TRUE );
		

		int i = m_CurrentPoint.i; 
		int j = m_CurrentPoint.j;

		double* pGridData = (double*)this->GetData();

		if( *(m_ySide+i*m_ycols+j) < *(m_ySide+i*m_ycols+j+1) )
		{
			if( *(m_ySide+i*m_ycols+j) > 0 )
				HandlingAfterNextPointFounded(i,j,FALSE);
			else
				HandlingAfterNextPointFounded(i,j+1,FALSE);
		}
		else if( *(m_ySide+i*m_ycols+j) == *(m_ySide+i*m_ycols+j+1) )
		{
			if( *(m_ySide+i*m_ycols+j) < 0)
			{
				HandlingAfterNextPointFounded(i+1,j,TRUE);
			}
			else
			{
				double xSideIJ = (m_dTraceValue - *(pGridData+i*m_nx+j)) / ( *(pGridData+i*m_nx+j+1) - *(pGridData+i*m_nx+j) );
				if( xSideIJ <=0.5f )
				{
					HandlingAfterNextPointFounded(i,j,FALSE);
				}
				else
				{
					HandlingAfterNextPointFounded(i,j+1,FALSE);
				}
			}
		}
		else if( *(m_ySide+i*m_ycols+j) > *(m_ySide+i*m_ycols+j+1) )
		{
			if( *(m_ySide+i*m_ycols+j+1) > 0 )
				HandlingAfterNextPointFounded(i,j+1,FALSE);
			else
				HandlingAfterNextPointFounded(i,j,FALSE);		
		}
	}

	//由上向下
	void KQContourTracer::FromTop2BottomTracing()
	{
	/*   
						   等前等值点(P2)
							  ∣
							  ∣
					(i,j)     ↓    (i,j+1)
						|-----·----|
						|           |
		纵边(i-1,j)---→|           |←----纵边(i-1,j+1)
		ySide[i-1][j]	|           |     ySide[i-1][j+1]
						|           |
						|-----------|    
				 (i-1,j)      ↑     (i-1,j+1)
							  ∣   
							横边(i-1,j)
						   xSide[i-1][j]
	            
	*/

		Assert( m_CurrentPoint.bHorV == TRUE );	

		int i = m_CurrentPoint.i;
		int j = m_CurrentPoint.j;

		double* pGridData = (double*)this->GetData();

		if( IsExistPoint(*(m_ySide+(i-1)*m_ycols+j)) && IsExistPoint(*(m_ySide+(i-1)*m_ycols+j+1)) )
		{
			
			if( *(m_ySide+(i-1)*m_ycols+j) < *(m_ySide+(i-1)*m_ycols+j+1) )
			{
				HandlingAfterNextPointFounded(i-1,j,FALSE);//在纵边(i-1,j)上
			}
			else if( *(m_ySide+(i-1)*m_ycols+j) == *(m_ySide+(i-1)*m_ycols+j+1) )
			{
				//注意:这里需要重新计算xSide[i][j]，因为在上一次已经被置为-2.0
				double xSideIJ = ( m_dTraceValue - *(pGridData+i*m_nx+j)) / ( *(pGridData+i*m_nx+j+1) - *(pGridData+i*m_nx+j) );
				if( /*xSide[i][j]*/ xSideIJ <= 0.5f )
				{
					//TRACE0("xSide[i][j] <= 0.5f\n");
					HandlingAfterNextPointFounded(i-1,j,FALSE);//在纵边(i-,j)上
				}
				else
				{
					HandlingAfterNextPointFounded(i-1,j+1,FALSE);//在纵边(i-1,j+1)上
				}
				
			}
			else
			{
				HandlingAfterNextPointFounded(i-1,j+1,FALSE);//在纵边(i-1,j+1)上
			}
			
		}
		else
		{
			if( IsExistPoint(*(m_ySide+(i-1)*m_ycols+j)) )
			{
				HandlingAfterNextPointFounded(i-1,j,FALSE);//在纵边(i-1,j)上
			}
			else if( IsExistPoint(*(m_ySide+(i-1)*m_ycols+j+1)) )
			{
				HandlingAfterNextPointFounded(i-1,j+1,FALSE);//在纵边(i-1,j+1)上
			}
			else if( IsExistPoint(*(m_xSide+(i-1)*m_xcols+j)) )
			{//两纵边上都没有
				HandlingAfterNextPointFounded(i-1,j,TRUE);//在横边(i-1,j)上
			}
			else
			{//三边上都没有
				Assert(FALSE);
			}
		}
	}

	//由上向下(V2)
	void KQContourTracer::FromTop2BottomTracingV2()
	{
		
		int i = m_CurrentPoint.i; 
		int j = m_CurrentPoint.j;

		double* pGridData = (double*)this->GetData();
		
		//比较：ySide[i-1][j]      ySide[i-1][j+1]
		//                 
		//               xSide[i-1][j] 
		
		if( *(m_ySide+(i-1)*m_ycols+j) < *(m_ySide+(i-1)*m_ycols+j+1) )
		{
			if( *(m_ySide+(i-1)*m_ycols+j) > 0 )
				HandlingAfterNextPointFounded(i-1,j,FALSE);
			else
				HandlingAfterNextPointFounded(i-1,j+1,FALSE);
		}
		else if( *(m_ySide+(i-1)*m_ycols+j) == *(m_ySide+(i-1)*m_ycols+j+1) )
		{
			if( *(m_ySide+(i-1)*m_ycols+j) < 0 /*即ySide[i-1][j] == ySide[i-1][j+1] ==-2*/) 
			{
				HandlingAfterNextPointFounded(i-1,j,TRUE);//下一点在对面的横边上
			}
			else
			{
				
				//注意:这里需要重新计算xSide[i][j]，因为在上一次已经被置为-2.0
				double xSideIJ = ( m_dTraceValue - *(pGridData+i*m_nx+j) ) / ( *(pGridData+i*m_nx+j+1) - *(pGridData+i*m_nx+j) );
				if(  xSideIJ <= 0.5f )
				{
					HandlingAfterNextPointFounded(i-1,j,FALSE);
				}
				else
				{
					HandlingAfterNextPointFounded(i-1,j+1,FALSE);
				}
			}
		}
		else if( *(m_ySide+(i-1)*m_ycols+j) > *(m_ySide+(i-1)*m_ycols+j+1) )
		{
			if( *(m_ySide+(i-1)*m_ycols+j+1) > 0 )
				HandlingAfterNextPointFounded(i-1,j+1,FALSE);
			else
				HandlingAfterNextPointFounded(i-1,j,FALSE);
		}
	}

	//由左向右追踪
	void KQContourTracer::FromLeft2RightTracing()
	{
	/*   
						   横边(i+1,j)
						  xSide[i+1][j]
							   ∣ 
							   ∣
					(i+1,j)    ↓    (i+1,j+1)
						|-----------|
						|           |
	 等前等值点(P2)--→·           |←----纵边(i,j+1)
     					|           |     ySide[i][j+1]
						|           |
						|-----------|    
					(i,j)     ↑   (i,j+1)
							  ∣   
							  横边(i,j)
							 xSide[i][j]	   
	*/
		Assert( m_CurrentPoint.j > m_PreviousPoint.j );
		Assert( m_CurrentPoint.bHorV == FALSE );

		int i = m_CurrentPoint.i; 
		int j = m_CurrentPoint.j;	

		double* pGridData = (double*)this->GetData();

		if( IsExistPoint(*(m_xSide+i*m_xcols+j)) && IsExistPoint(*(m_xSide+(i+1)*m_xcols+j)) )
		{
			
			if( *(m_xSide+i*m_xcols+j) < *(m_xSide+(i+1)*m_xcols+j) )
			{
				HandlingAfterNextPointFounded(i,j,TRUE);//在横边(i,j)上
			}
			else if( *(m_xSide+i*m_xcols+j) == *(m_xSide+(i+1)*m_xcols+j) )
			{
				//注意:这里需要重新计算ySide[i][j]，因为在上一次已经被置为-2.0
				double ySideIJ = ( m_dTraceValue - *(pGridData+i*m_nx+j)) / ( *(pGridData+(i+1)*m_nx+j) - *(pGridData+i*m_nx+j) );
				if( ySideIJ <= 0.5f )
				//if( ySide[i][j] <= 0.5f )
				{
					//TRACE3("ySide[%d][%d]=%f,左-->右----遇到两边r值相等\n",i,j,*(m_ySide+i*m_ycols+j));
					HandlingAfterNextPointFounded(i,j,TRUE);//在横边(i,j)上
				}
				else
				{
					HandlingAfterNextPointFounded(i+1,j,TRUE);//在横边(i+1,j)上
				}
				
			}
			else
			{
				HandlingAfterNextPointFounded(i+1,j,TRUE);//在横边(i+1,j)上
			}
			
		}
		else
		{
			if( IsExistPoint(*(m_xSide+i*m_xcols+j)) )
			{
				HandlingAfterNextPointFounded(i,j,TRUE);//在横边(i,j)上
			}
			else if( IsExistPoint(*(m_xSide+(i+1)*m_xcols+j)) )
			{
				HandlingAfterNextPointFounded(i+1,j,TRUE);//在横边(i+1,j)上
			}
			else if( IsExistPoint(*(m_ySide+i*m_ycols+j+1)) )
			{//两横边上都没有
				HandlingAfterNextPointFounded(i,j+1,FALSE);//在纵边(i,j+1)上
			}
			else
			{//三边上都没有
				Assert(FALSE);
			}
		}
		
	}

	//由左向右追踪(V2)
	void KQContourTracer::FromLeft2RightTracingV2()
	{
		Assert( m_CurrentPoint.j > m_PreviousPoint.j );
		Assert( m_CurrentPoint.bHorV == FALSE );
		
		int i = m_CurrentPoint.i; 
		int j = m_CurrentPoint.j;	

		double* pGridData = (double*)this->GetData();

		if( *(m_xSide+i*m_xcols+j) < *(m_xSide+(i+1)*m_xcols+j) )
		{
			if( *(m_xSide+i*m_xcols+j) > 0 )
				HandlingAfterNextPointFounded(i,j,TRUE);
			else
				HandlingAfterNextPointFounded(i+1,j,TRUE);
		}
		else if( *(m_xSide+i*m_xcols+j) == *(m_xSide+(i+1)*m_xcols+j) )
		{
			if( *(m_xSide+i*m_xcols+j) < 0 /*namely, xSide[i][j] == xSide[i+1][j] == -2.0*/)
			{
				HandlingAfterNextPointFounded(i,j+1,FALSE);
			}
			else
			{		
				double ySideIJ = ( m_dTraceValue - *(pGridData+i*m_nx+j)) / ( *(pGridData+(i+1)*m_nx+j) - *(pGridData+i*m_nx+j) );
				if( ySideIJ <= 0.5f )
					HandlingAfterNextPointFounded(i,j,TRUE);
				else
					HandlingAfterNextPointFounded(i+1,j,TRUE);
			}
		}
		else if( *(m_xSide+i*m_xcols+j) > *(m_xSide+(i+1)*m_xcols+j) )
		{
			if( *(m_xSide+(i+1)*m_xcols+j) > 0 )
				HandlingAfterNextPointFounded(i+1,j,TRUE);
			else
				HandlingAfterNextPointFounded(i,j,TRUE);

		}
	}

	//由右向左
	void KQContourTracer::FromRight2LeftTracing()
	{
	/*   
						   横边(i+1,j-1)
						  xSide[i+1][j-1]
							   ∣ 
							   ∣
				  (i+1,j-1)    ↓    (i+1,j)
						|-----------|
						|           |
		纵边(i,j-1)--→ |           ·←----等前等值点(P2)
	   ySide[i][j-1]    |           |     
						|           |
						|-----------|    
				  (i,j-1)     ↑   (i,j)
							  ∣   
							  横边(i,j-1)
							 xSide[i][j-1]	   
	*/

		Assert( m_CurrentPoint.bHorV == FALSE );
		

		int i = m_CurrentPoint.i; 
		int j = m_CurrentPoint.j;	

		double* pGridData = (double*)this->GetData();

		if( IsExistPoint(*(m_xSide+i*m_xcols+j-1)) && IsExistPoint(*(m_xSide+(i+1)*m_xcols+j-1)) )
		{
			
			if( *(m_xSide+i*m_xcols+j-1) < *(m_xSide+(i+1)*m_xcols+j-1) )
			{
				HandlingAfterNextPointFounded(i,j-1,TRUE);//在横边(i,j-1)上
			}
			//else if( *(m_xSide+i*m_xcols+j) == *(m_xSide+(i+1)*m_xcols+j-1) )
			else if( *(m_xSide+i*m_xcols+j-1) == *(m_xSide+(i+1)*m_xcols+j-1) )
			{
				//注意:这里需要重新计算ySide[i][j]，因为在上一次已经被置为-2.0
				double ySideIJ = ( m_dTraceValue - *(pGridData+i*m_nx+j)) / ( *(pGridData+(i+1)*m_nx+j) - *(pGridData+i*m_nx+j) );
				if( /*ySide[i][j]*/ ySideIJ <= 0.5f )
				{
					HandlingAfterNextPointFounded(i,j-1,TRUE);//在横边(i,j-1)上
				}
				else
				{
					HandlingAfterNextPointFounded(i+1,j-1,TRUE);//在横边(i+1,j-1)上
				}
				
			}
			else
			{
				HandlingAfterNextPointFounded(i+1,j-1,TRUE);//在横边(i+1,j-1)上
			}
			
		}
		else
		{
			if( IsExistPoint(*(m_xSide+i*m_xcols+j-1)) )
			{
				HandlingAfterNextPointFounded(i,j-1,TRUE);//在横边(i,j-1)上
			}
			else if( IsExistPoint(*(m_xSide+(i+1)*m_xcols+j-1)) )
			{
				HandlingAfterNextPointFounded(i+1,j-1,TRUE);//在横边(i+1,j-1)上
			}
			else if( IsExistPoint(*(m_ySide+i*m_ycols+j-1)) )
			{//两横边上都没有
				HandlingAfterNextPointFounded(i,j-1,FALSE);//在纵边(i,j-1)上
			}
			else
			{//三边上都没有
				Assert(FALSE);
			}
		}
	}

	//由右向左(V2)
	void KQContourTracer::FromRight2LeftTracingV2()
	{
		/*   
						   横边(i+1,j-1)
						  xSide[i+1][j-1]
							   ∣
				  (i+1,j-1)    ↓    (i+1,j)
						|-----------|
						|           |
		纵边(i,j-1)--→ |           ·←----等前等值点(P2)
	   ySide[i][j-1]    |           |     
						|           |
						|-----------|    
				  (i,j-1)     ↑   (i,j)
							  ∣   
							  横边(i,j-1)
							 xSide[i][j-1]	   
	*/
		Assert( m_CurrentPoint.bHorV == FALSE );

		int i = m_CurrentPoint.i; 
		int j = m_CurrentPoint.j;	

		double* pGridData = (double*)this->GetData();

		if( *(m_xSide+i*m_xcols+j-1) < *(m_xSide+(i+1)*m_xcols+j-1) )
		{
			if( *(m_xSide+i*m_xcols+j-1) > 0 )
				HandlingAfterNextPointFounded(i,j-1,TRUE);
			else
				HandlingAfterNextPointFounded(i+1,j-1,TRUE);
		}
		else if( *(m_xSide+i*m_xcols+j-1) == *(m_xSide+(i+1)*m_xcols+j-1) )
		{
			if( *(m_xSide+i*m_xcols+j-1) < 0 /*namely, xSide[i][j-1] == xSide[i+1][j-1] == -2.0f*/ )
			{
				HandlingAfterNextPointFounded(i,j-1,FALSE);
			}
			else
			{
				double ySideIJ = ( m_dTraceValue - *(pGridData+i*m_nx+j)) / ( *(pGridData+(i+1)*m_nx+j) - *(pGridData+i*m_nx+j) );
				if( ySideIJ <= 0.5f )
					HandlingAfterNextPointFounded(i,j-1,TRUE);
				else
					HandlingAfterNextPointFounded(i+1,j-1,TRUE);
			}
		}
		else if( *(m_xSide+i*m_xcols+j-1) > *(m_xSide+(i+1)*m_xcols+j-1) )
		{
			if( *(m_xSide+(i+1)*m_xcols+j-1) > 0 )
				HandlingAfterNextPointFounded(i+1,j-1,TRUE);
			else
				HandlingAfterNextPointFounded(i,j-1,TRUE);
			
		}
	}

	//当下一个等值点找到后做相应的处理
	//参数说明：i,j分别是等值点所在边的编号，bHorizon指明所在边是横边还是纵边
	void KQContourTracer::HandlingAfterNextPointFounded(int i, int j, BOOL bHorizon)
	{

		//当下一个等值点找到后做相应的处理,如下：
		//1.记录该等值点的i,j
		//2.计算并保存该等值点的坐标
		//3.标志该等值点所在边的已经搜索过

		//验证i∈[0,rows-1], j∈[0,cols-1]
		Assert( i>=0 && i<=m_ny-1 && j>=0 && j<=m_nx-1 );

		//1.
		m_NextPoint.i = i;
		m_NextPoint.j = j;
		m_NextPoint.bHorV = bHorizon;

		//2.
		CalcOnePointCoord(i,j,bHorizon,m_NextPoint.x,m_NextPoint.y);

		//3.
		if( m_NextPoint.bHorV )
		{
			*(m_xSide+i*m_xcols+j) = -2.0f; //已经追踪过
		}
		else
		{	
			*(m_ySide+i*m_ycols+j) = -2.0f; //已经追踪过
		}
	}

//---------------------------------------------------------
}// namespace ContourBuilder - end
//---------------------------------------------------------

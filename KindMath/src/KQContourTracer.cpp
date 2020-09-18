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

		// �����д���rows*(cols-1)�����,
		// ������ҪΪm_xSide����rows*(cols-1)�ռ����
		m_xSide = new double[rows*(cols-1)];
		m_xcols = cols-1;

		// �����д���(rows-1)*cols���ݱ�
		// ������ҪΪm_ySide����(rows-1)*cols�ռ����
		m_ySide = new double[(rows-1)*cols];
		m_ycols = cols;
	}

	void KQContourTracer::FreeMemory()
	{
		_deletea(m_xSide);
		_deletea(m_ySide);
	}

	// ׷��
	BOOL KQContourTracer::Tracing()
	{
		// ȡ��������Ϣ
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

		// 1.ΪxSide��ySide�����ڴ�ռ�
		PrepareMemory();

		// 2.ɨ�������ݺ��,�ڲ��ֵ��
		BOOL b1 = InterpolateTracingValue();

		// 3.��׷�ٿ�·����
		BOOL b2 = TracingOpenedContour();

		// 4.��׷�ٱպ�����
		BOOL b3 = TracingClosedContour();

		return b1&b2&b3;
	}

	// ɨ�������ݺ��,�ڲ��ֵ��
	// ɨ��������ݡ���ߣ������Բ�ֵ�����ֵ������
	// �������ϵĵ�ֵ������洢��xSide��ySide�����У�
	// xSide�洢���к���ϵĵ�ֵ�����, ySide�洢�����ݱ��ϵĵ�ֵ�����
	// �ڲ�ֵ����ʱ���ԡ���׷��ֵ��ȵ����ݡ�Ҫ�������������ż��㣬��������������ʱ��Ҫ�ı�ԭ��������
	BOOL KQContourTracer::InterpolateTracingValue()
	{
	/*
			 ������ʶ����:
	        
				(i+1,j)��--------��(i+1,j+1)
						|        |
						|        |
						|        |
						|        |
				 (i,j) ��--------��(i,j+1)

				  i:��ʾ�к�(��������)
				  j:��ʾ�к�(��������)
				  ��ʶһ�����񽻵�ʱ���к���ǰ���к����ң��磺(i,j)

			  xSide,ySide�д洢rֵ��(wΪ׷��ֵ)
			  ���������ߣ�r = (w - pData[i][j]) / (pData[i][j+1]-pData[i][j]);
			  ���������ݱߣ�r = (w - pData[i][j]) / (pData[i+1][j]-pData[i][j]);

			  ���ڸ����������xSide[i][j],ySide[i][j]�п��ܵ���1.0��0.0 
			  �������������
			  (1)��׷��ֵ��������ϵ�ֵ�ܽӽ�(�������)ʱ�������������ͻ����1.0
				 ����׷��0ֵʱ���������±�:
					   20 ��--------��-0.00000016   �˱�����0ֵ�������� (0-20)/(-0.00000016-20) == 1.0 

	       
			  (2)������������˵��ϵ�ֵ��������ʱ��
				 ����׷��2ֵ���������±ߣ�
				 1.70141E+038 ��--------��1   �˱�����2ֵ������(2-1.70141E+038) / (1-1.70141E+038) == 1.0 

			��������е�ֵ��ʱ�������ϱ���ֵ�������0��1��
			 �����ڼ������������㷨���ж�ʱ����Ϊ0��1Ҳ���е�ֵ���

			����xSide,ySide�д洢��ֵ��[0,1]�ı����䣬����(0,1)�Ŀ�����
	*/

		Assert( m_pData != NULL );
		Assert( m_xSide != NULL );
		Assert( m_ySide != NULL );

		int i,j;
		int rows = m_ny;
		int cols = m_nx;

		double w   = m_dTraceValue; 
		double* pData = (double*)m_pData;	//��������

		double Z1,Z2; //�ֱ��¼һ���ߵ��������ϵ�����ֵ
		double flag; 
		double shift = 0.001;  //����ֵ

		// 1.ɨ�貢�������ϵĵ�ֵ��,��rows*(cols-1)�������Ҫɨ��
		for(i=0;i<rows;i++)
		{
			for(j=0;j<cols-1;j++)
			{
				Z1 = *(pData+i*cols+j);
				Z2 = *(pData+i*cols+j+1);

				//�������ϵ�����ֵ�����ñ��ϲ����ڵ�ֵ��
				if( Z1 == Z2 )
				{
					*(m_xSide+i*m_xcols+j) = -2.0;	// -2��ʾ�ñ��ϲ����ڵ�ֵ��
					continue;	//��һ�����
				}

				//Z1 != Z2,�жϺ���Ƿ���ڵ�ֵ��
				Z1 = (w==Z1) ? Z1+shift : Z1;	//Z1��Ҫ׷�ٵ�ֵw��ȣ���һС��ƫ��������
				Z2 = (w==Z2) ? Z2+shift : Z2;	//Z2��Ҫ׷�ٵ�ֵw��ȣ���һС��ƫ��������

				flag = (w-Z1) * (w-Z2);
				if( flag > 0 )	//�����ڵ�ֵ��
				{
					//m_xSide[i][j] = -2.0f;
					*(m_xSide+i*m_xcols+j) = -2.0f;	//-2��ʾ�ñ��ϲ����ڵ�ֵ��
				}
				else if( flag < 0 )	//���ڵ�ֵ��
				{
					//�����ֵ��λ�ò�������m_xSide[i][j],��ֵӦ >=0 && <=1 
					*(m_xSide+i*m_xcols+j) = (w-Z1) / (Z2-Z1); 
					Assert(*(m_xSide+i*m_xcols+j)>=0 && *(m_xSide+i*m_xcols+j)<=1.0f);
				}

			}//end for(j)

		}//end for(i)

		// 2.ɨ�貢�����ݱ��ϵ�ֵ��,��(rows-1)*cols���ݱ���Ҫɨ��
		for(i=0;i<rows-1;i++)
		{
			for(j=0;j<cols;j++)
			{
				Z1 = *(pData+i*cols+j);            
				Z2 = *(pData+(i+1)*cols+j);		     
				
				//�������ϵ�����ֵ�����ñ��ϲ����ڵ�ֵ��? waiting =0.5���??????             
				if( Z1 == Z2 )
				{
					*(m_ySide+i*m_ycols+j) = -2.0;
					continue;
				}

				//Z1 != Z2,�жϺ���Ƿ���ڵ�ֵ��
				Z1 = (w==Z1) ? Z1+shift : Z1;	//Z1��Ҫ׷�ٵ�ֵw��ȣ���һС��ƫ��������
				Z2 = (w==Z2) ? Z2+shift : Z2;	//Z2��Ҫ׷�ٵ�ֵw��ȣ���һС��ƫ��������

				flag = (w-Z1) * (w-Z2);
				if( flag > 0 )	//�����ڵ�ֵ��
				{
					*(m_ySide+i*m_ycols+j) = -2.0;
				}
				else if( flag < 0 )	//���ڵ�ֵ��
				{
					//�����ֵ��λ�ò�������m_ySide[i][j],��ֵӦ >=0 && <=1 
					*(m_ySide+i*m_ycols+j) = (w-Z1) / (Z2-Z1) ; 
					Assert(*(m_ySide+i*m_ycols+j)>=0 && *(m_ySide+i*m_ycols+j)<=1.0f);
				}

			}//end for(j)

		}//end for(i)

		return TRUE;
	}
	// ׷�ٿ�·��ֵ��
	// �ú����Ĺؼ����ҳ������ߵ���ͷ�������ú�׷��ǰ���ķ��������
	BOOL KQContourTracer::TracingOpenedContour()
	{
		int i,j;

		int rows = m_ny;	
		int cols = m_nx;

		// 1.�����ױ߿�m_xSide[0][j](���¶���׷��)	
		for(j=0; j<cols-1; j++)
		{
			double flag = *(m_xSide+0*m_xcols+j);
			if( IsExistPoint(flag) )	//a2��
			{
				//���������ϵ�ǰ����������ǰһ��a1���i,j
				m_PreviousPoint.i = -1; // ����ǰһ����-1��,������һ�㵽��ǰ���ǰ����������������ϵ�
				m_PreviousPoint.j = j;
				m_PreviousPoint.bHorV = TRUE;	//���

				m_CurrentPoint.i = 0; //�ױߵ��к�Ϊ0������������ͷ��iΪ0
				m_CurrentPoint.j = j;
				m_CurrentPoint.bHorV = TRUE; //�ױ��Ǻ��

				TracingOneOpenedContour();	
			}
		}

		// 2.������߿�m_ySide[i][0](�������׷��)
		for(i=0; i<rows-1; i++)
		{
			if( IsExistPoint(*(m_ySide+i*m_ycols+0)) )
			{
				//���������ҵ�ǰ����������ǰһ��a1��i,j
				m_PreviousPoint.i = i; 
				m_PreviousPoint.j = -1; //����ǰһ����-1��,ʹ��������-->�ҵ�ǰ������
				m_PreviousPoint.bHorV = FALSE;	//�ݱ�
				
				m_CurrentPoint.i = i; 
				m_CurrentPoint.j = 0; //��߿��ڵ�0�У�����������ͷ��jΪ0
				m_CurrentPoint.bHorV = FALSE;		//�ݱ�
				
				TracingOneOpenedContour();	
			}
		}

		// 3.�����ϱ߿�m_xSide[rows-1][j](���϶���׷��)
		double deltX = m_xInterval;
		double curPt_Col_X = m_x0 + m_CurrentPoint.j * deltX; //��ǰ��ֵ�����ڱ�(i,j)������ʶ��j�е�X����
		
		for(j=0; j<cols-1; j++)
		{
			if( IsExistPoint(*(m_xSide+(rows-1)*m_xcols+j)) )
			{
				//�������������׷�ٵ�����
				//  ��������׷�ٵ��������£� 	 
				//  1. Not( CurrentPoint.i > PreviousPoint.i )
				//  2. Not( CurrentPoint.j > PreviousPoint.j )
				//  3. CurrentPoint.x > m_gridDataInfo.xMin + CurrentPoint.j * deltX; ��:Ҫ���ں����
				//
				m_PreviousPoint.i = rows-1; 
				m_PreviousPoint.j = j;
				m_PreviousPoint.bHorV = TRUE;
				
				m_CurrentPoint.i = rows-1; //�ϱ߿���к�Ϊrows-1
				m_CurrentPoint.j = j;
				m_CurrentPoint.bHorV = TRUE; //ʹ����ϵ���������

				//���������������������裬ֻҪ���ϴ��ھͻ������������������Ϊ�ϱ߿��Ǻ��
	//			if( CurrentPoint.x < curPt_Col_X )
	//			{
	//				CurrentPoint.x = curPt_Col_X + 1; //Ϊ�˱�֤���������3������
	//			}
				
				TracingOneOpenedContour();	
			}
		}

		//4.�����ұ߿�m_ySide[i][cols-1](���Ҷ���׷��)
		for(i=0; i<rows-1; i++)
		{
			if( IsExistPoint(*(m_ySide+i*m_ycols+cols-1)) )
			{
				//�������������׷�ٵ�����
				//
				//  ��������׷�ٵ��������£� 	 
				//  1. Not( CurrentPoint.i > PreviousPoint.i )
				//  2. Not( CurrentPoint.j > PreviousPoint.j )
				//  3. Not( CurrentPoint.x > m_gridDataInfo.xMin + CurrentPoint.j * deltX ); Ҫ�����ݱ���			
				//
				m_PreviousPoint.i = i; 
				m_PreviousPoint.j = cols-1; 
				m_PreviousPoint.bHorV = FALSE;
				
				m_CurrentPoint.i = i; 
				m_CurrentPoint.j = cols-1; //�ұ߿��ڵ�cols-1��
				m_CurrentPoint.bHorV = FALSE;  //ʹ����ϵ���������

	//			//���������������
	//			CurrentPoint.x = curPt_Col_X - 1;//��ȥ1��ʹ֮��curPt_Col_XС
				
				TracingOneOpenedContour();	
			}
		}

		return TRUE;
	}
	// ׷�ٱպ�����
	BOOL KQContourTracer::TracingClosedContour()
	{
		int cols = m_nx;
		int rows = m_ny;

		//�������еĳ��˱߿�����ݱ�,����������ÿһ���ϵ��ݱߣ�����һ�У����µ�������
		for(int j=1; j<cols-1; j++) //j��1��ʼ
		{
			for(int i=0; i<rows-1; i++) //i��0��ʼ
			{
				if( IsExistPoint(*(m_ySide+i*m_ycols+j))) 
				{
					TracingOneClosedContour(i,j);				
				}
				
			}
		}

		return TRUE;
	}

	// �Ƿ���ڵ�ֵ��
	BOOL KQContourTracer::IsExistPoint(double flag)
	{
		return (flag<0.0 || flag>1.0) ? FALSE : TRUE;
	}

	// ׷��һ����·��ֵ��
	void KQContourTracer::TracingOneOpenedContour()
	{
		//��¼����ͷ���ڱߵ�i,j �� ���ݱ߱�ʶ
		int startPt_i = m_CurrentPoint.i; 
		int startPt_j = m_CurrentPoint.j;
		BOOL startPt_bHoriz = m_CurrentPoint.bHorV;	// ���ݱ߱�ʶ

		// ��֤��ͷ�ڱ߽���
		Assert( startPt_i == 0  ||  //��ͷ�ڵױ߽���
			startPt_i == m_ny-1 ||  //��ͷ���ϱ߽���
			startPt_j == 0      ||  //��ͷ����߽���
			startPt_j == m_nx-1     //��ͷ���ұ߽��� 
			);

		int rows = m_ny;
		int cols = m_nx;

		// ע����Ϣ
		TCHAR szComment[MAX_PATH]=_T("");
		_stprintf(szComment,_T("Contour-%.3f"),m_dTraceValue);

		// ����һ������
		KCurve* pCurve = new KCurve();	// Ϊһ����ֵ�߷����ڴ�ռ�
		pCurve->SetFlag(++m_dwTracerFlag);

		// ���õ�ֵ������
		pCurve->SetValue(m_dTraceValue);	// ��ֵ�ߵ�ֵ

		//pSfxPolygon->SetLineWidth(1);						//������߿�
		//pSfxPolygon->SetLineType(PS_SOLID);					//���������
		//pSfxPolygon->SetFillType(ftNone);					//������������
		//pSfxPolygon->SetColorFlag(SNYC_COLOR_FLAG_INDEX);	//�����������ɫ���
		//pSfxPolygon->SetColorIndex(SNYC_CTI_FORE);			//�����������ɫ
		//pSfxPolygon->SetComment(szComment);					//�����ע����Ϣ

		// ���һ����ֵ��
		this->AddCurve(pCurve);

		// �������ֵ��ͷ������
		CalcOnePointCoord(startPt_i,startPt_j,startPt_bHoriz,m_CurrentPoint.x,m_CurrentPoint.y);

		// ����ͷ����
		pCurve->Add( new KPoint(m_CurrentPoint.x,m_CurrentPoint.y) );

		// ��ʾ�Ѿ�׷�ٹ�
		if( startPt_bHoriz )	// ���
		{
			*(m_xSide+startPt_i*m_xcols+startPt_j) = -2.0;
		}
		else
		{
			*(m_ySide+startPt_i*m_ycols+startPt_j) = -2.0;
		}

		// ׷����һ����
		TracingNextPoint(pCurve);
		
		m_PreviousPoint = m_CurrentPoint;
		m_CurrentPoint = m_NextPoint;

		//��������߽�ͽ���׷��

		//------2004/03/09�޸�------------------
		//Ϊ�˲��ø�����������������׷�ٽ����������ж�ʧ��
		//���ǲ����á�CurrentPoint.y <= yMin�����ж��Ƿ������ױ߿�,
		//  Ҳ�����á�CurrentPoint.x <= xMin�����ж��Ƿ�������߿�
		BOOL bIsFinish = (m_CurrentPoint.i == 0 && m_CurrentPoint.bHorV == TRUE )  ||  //�����ױ߽�,(ע:���ܽ�����CurrentPoint.i == 0�ж�)
			(m_CurrentPoint.i == rows-1)                            ||  //�����ϱ߽�
			(m_CurrentPoint.j == 0 && m_CurrentPoint.bHorV == FALSE ) ||  //������߽�,(ע:���ܽ�����CurrentPoint.j == 0�ж�)
			(m_CurrentPoint.j == cols-1) ;                              //�����ұ߽�

		while( !bIsFinish )	//�����߽�
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

	// ׷��һ���պϵ�ֵ��
	void KQContourTracer::TracingOneClosedContour(int startPt_i, int startPt_j)
	{
		// ����i,j�Ǹñ����ߵĵ�һ����ֵ���i,j

		// ����ǰһ��ֵ���i,j�����������������׷�ٵ�����
		m_PreviousPoint.i = startPt_i;
		m_PreviousPoint.j = 0;
		m_PreviousPoint.bHorV = FALSE;
		
		m_CurrentPoint.i = startPt_i;
		m_CurrentPoint.j = startPt_j;
		m_CurrentPoint.bHorV = FALSE;   // ��FALSE����Ϊ�����ݱ���

		int rows = m_ny;
		int cols = m_nx;

		// ע����Ϣ
		TCHAR szComment[MAX_PATH]=_T("");
		_stprintf(szComment,_T("Contour-%.3f"),m_dTraceValue);

		// ����һ������
		KCurve* pCurve = new KCurve();	// Ϊһ����ֵ�߷����ڴ�ռ�
		pCurve->SetFlag(++m_dwTracerFlag);

		// ���õ�ֵ������
		pCurve->SetValue(m_dTraceValue);	// ��ֵ�ߵ�ֵ

		//pSfxPolygon->SetLineWidth(1);						//������߿�
		//pSfxPolygon->SetLineType(PS_SOLID);					//���������
		//pSfxPolygon->SetFillType(ftNone);					//������������
		//pSfxPolygon->SetColorFlag(SNYC_COLOR_FLAG_INDEX);	//�����������ɫ���
		//pSfxPolygon->SetColorIndex(SNYC_CTI_FORE);			//�����������ɫ
		//pSfxPolygon->SetComment(szComment);					//�����ע����Ϣ

		// ���һ����ֵ��
		this->AddCurve(pCurve);

		// �������ֵ��ͷ������
		// FALSE => ��ͷ���ݱ���
		CalcOnePointCoord(startPt_i,startPt_j,FALSE,m_CurrentPoint.x,m_CurrentPoint.y);

		// ����ͷ����
		pCurve->Add( new KPoint(m_CurrentPoint.x,m_CurrentPoint.y) );

		// ׷����һ����ֵ��
		TracingNextPoint(pCurve);

		m_PreviousPoint = m_CurrentPoint;
		m_CurrentPoint = m_NextPoint;

		BOOL bColsed = FALSE;

		while(!bColsed) 
		{
			TracingNextPoint(pCurve);
			
			m_PreviousPoint = m_CurrentPoint;
			m_CurrentPoint = m_NextPoint;
			
			//------2004/03/09�޸�------------------
			//�õ�ֵ�����ڱߵı�ʶi,j���ж������Ƿ��գ�����x��y�������Ƚϣ���Ϊ�����������������
			//	bColsed = (CurrentPoint.x == startPt_x) && (CurrentPoint.y == startPt_y);
			bColsed = ( m_CurrentPoint.i == startPt_i ) && 
				( m_CurrentPoint.j == startPt_j ) && 
				( m_CurrentPoint.bHorV == FALSE );
			
			
		}

		*(m_ySide+startPt_i*m_ycols+startPt_j) = -2.0f; //�Ѿ�׷�ٹ�
	}

	// �������ֵ��ͷ������
	void KQContourTracer::CalcOnePointCoord(int i, int j, BOOL bHorizon,double &x, double &y)
	{
		//X�����������ľ���
		double deltX = m_xInterval;

		//Y�����������ľ���
		double deltY = m_yInterval;
		
		
		if(bHorizon)	//�ں����
		{
			x = m_x0 + (j + *(m_xSide+i*m_xcols+j)) * deltX;
			y = m_y0 + i * deltY;		
		}
		else	//���ݱ���
		{
			x = m_x0 + j * deltX;
			y = m_y0 + ( i + *(m_ySide+i*m_ycols+j)) * deltY;
		}
	}

	// ׷�ٳ���һ����a3
	void KQContourTracer::TracingNextPoint(KCurve* pCurve)
	{
	 // 1.��ȷ������ֵ�ߵ�ǰ������(�������ϡ��������ҡ��������¡�������������֮һ)
	 // 2.��׷����һ����ֵ��

	 // ǰ��������������ж���
	 //   if( ��ǰ��.�к� > ǰһ��.�к� )
	 //   {
		//    ��---->��
		//}
		//else if( ��ǰ��.�к� > ǰһ��.�к� )
	 //   {
		//	��---->��
		//}
	 //   else if( ��ǰ���ں���� )
		//{
		//	��---->��
		//}
		//else
		//{
		//     ��---->��
		//}
	       

	#define  _CALL_TRACINGFUN_VERSION2

		if(m_CurrentPoint.i > m_PreviousPoint.i )
		{
			//TRACE0("��--->��\n");
			#ifdef _CALL_TRACINGFUN_VERSION2
				FromBottom2TopTracingV2();
			#else
				FromBottom2TopTracing();
			#endif
		}
		else if(m_CurrentPoint.j > m_PreviousPoint.j )
		{
			//TRACE0("��--->��\n");
			#ifdef _CALL_TRACINGFUN_VERSION2
				FromLeft2RightTracingV2();
			#else
				FromLeft2RightTracing();
			#endif
			
		}
		else if( m_CurrentPoint.bHorV == TRUE /*curPt_Col_X < CurrentPoint.x*/ ) 
		{//�ں���ϡ�CurrentPoint.bHorizon == TRUE �� curPt_Col_X < CurrentPoint.x �������������ȼ�
			
			
			Assert( m_CurrentPoint.i <= m_PreviousPoint.i &&
					m_CurrentPoint.j <= m_PreviousPoint.j );

			//TRACE0("��--->��\n");
			#ifdef _CALL_TRACINGFUN_VERSION2
				FromTop2BottomTracingV2();
			#else
				FromTop2BottomTracing();
			#endif
		
		}
		else
		{
			Assert(m_CurrentPoint.bHorV==FALSE);//���ݱ���

			Assert( m_CurrentPoint.i <= m_PreviousPoint.i &&
					m_CurrentPoint.j <= m_PreviousPoint.j );
			
			//TRACE0("��--->��\n");
			#ifdef _CALL_TRACINGFUN_VERSION2
				FromRight2LeftTracingV2();
			#else
				FromRight2LeftTracing();
			#endif
		
		}

		// �����һ��
		pCurve->Add( new KPoint(m_NextPoint.x,m_NextPoint.y) );
	}

	//��������
	void KQContourTracer::FromBottom2TopTracing()
	{
	/*   
						   ���(i+1,j)
						  xSide[i+1][j]

							   ��
					(i+1,j)    �O    (i+1,j+1)
						|-----------|
						|           |
		  �ݱ�(i,j)---��|           |��----�ݱ�(i,j+1)
		  ySide[i][j]	|           |     ySide[i][j+1]
						|           |
						|-----��----|    
					(i,j)      �I     (i,j+1)
								 ��
								   ��ǰ��ֵ��(P2)
	        
			  ��ֵ����������ǰ��ʱ������Ԫ��������ϣ�
			  ��ǰ��ֵ��(��P2��ʾ)������Ԫ�ĵױ��ϣ���ô��һ��ֵ��(��P3��ʾ)���ڵ�λ�������������
				 1.���ݱ�(i,j)��
				 2.���ݱ�(i,j+1)��
				 3.�ں��(i+1,j)��
			 ��ʵ��׷��ʱֻ��ѡ������֮һ

			 �����жϵ����ݼ��ǣ�ySide[i][j] �� ySide[i][j+1] ��xSide[i+1][j] 

		   �����ݱ�(i,j)�ϴ��ڵ�ֵ��P31, �ݱ�(i,j+1)�ϴ��ڵ�ֵ��P33, ���(i+1,j)�ϴ��ڵ�ֵ��P32
		   ѡ��Ĵ������£�
		   1.��P31��P33������ʱ��ѡ�񿿽�����ױ���ΪP3(�Ƚ�ySide[i][j]��ySide[i][j+1])
		   2.��P31,P33�����ױߵľ�����ͬ����ѡ����P2��������ΪP3
		   3.��P31,P33��ֻ��һ������ʱ������ڵ㼴ΪP3
		   4.����P31,P33����ʱ���Ա߱ض�����P32��ΪP3

	*/
		Assert( m_CurrentPoint.i > m_PreviousPoint.i );   //��ǰ����к� > ǰһ����к�
		Assert( m_CurrentPoint.bHorV == TRUE );
		

		int i = m_CurrentPoint.i; 
		int j = m_CurrentPoint.j;

		double* pGridData = (double*)this->GetData();

		if(IsExistPoint(*(m_ySide+i*m_ycols+j)) && IsExistPoint(*(m_ySide+i*m_ycols+j+1)))
		{
			
			if( *(m_ySide+i*m_ycols+j) < *(m_ySide+i*m_ycols+j+1) )
			{
				HandlingAfterNextPointFounded(i,j,FALSE);//���ݱ�(i,j)��
			}
			else if( *(m_ySide+i*m_ycols+j) == *(m_ySide+i*m_ycols+j+1) )
			{
				//ע��:������Ҫ���¼���xSide[i][j]������ֱ����xSide[i][j],��Ϊ����һ���Ѿ�����Ϊ-2.0
				double xSideIJ = (m_dTraceValue - *(pGridData+i*m_nx+j)) / ( *(pGridData+i*m_nx+j+1) - *(pGridData+i*m_nx+j) );
				if( xSideIJ <= 0.5f )
				//if( xSide[i][j] <= 0.5f )
				{
					//TRACE3("m_xSide[%d][%d]=%f,��-->��----��������rֵ���\n",i,j,*(m_xSide+i*m_xcols+j));
					HandlingAfterNextPointFounded(i,j,FALSE);//���ݱ�(i,j)��
				}
				else
				{
					HandlingAfterNextPointFounded(i,j+1,FALSE);//���ݱ�(i,j+1)��
				}

			}
			else
			{
				HandlingAfterNextPointFounded(i,j+1,FALSE);//���ݱ�(i,j+1)��
			}

		}
		else
		{
			if( IsExistPoint(*(m_ySide+i*m_ycols+j)) )
			{
				HandlingAfterNextPointFounded(i,j,FALSE);//���ݱ�(i,j)��
			}
			else if( IsExistPoint(*(m_ySide+i*m_ycols+j+1)) )
			{
				HandlingAfterNextPointFounded(i,j+1,FALSE);//���ݱ�(i,j+1)��
			}
			else if( IsExistPoint(*(m_xSide+(i+1)*m_xcols+j)) )
			{//���ݱ��϶�û��
				HandlingAfterNextPointFounded(i+1,j,TRUE);//�ں��(i+1,j)��
			}
			else
			{//�����϶�û��,���ݲ�ֵ���ִ���
				Assert(FALSE);
			}
		}

	}

	//��������(Ver 2)
	void KQContourTracer::FromBottom2TopTracingV2()
	{
	/*	                   ���(i+1,j)
						  xSide[i+1][j]

							   ��
					(i+1,j)    �O    (i+1,j+1)
						|-----------|
						|           |
		  �ݱ�(i,j)---��|           |��----�ݱ�(i,j+1)
		  ySide[i][j]	|           |     ySide[i][j+1]
						|           |
						|-----��----|    
					(i,j)      �I     (i,j+1)
								 ��
								   ��ǰ��ֵ��
	*/
		Assert( m_CurrentPoint.i > m_PreviousPoint.i );   //��ǰ����к� > ǰһ����к�
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

	//��������
	void KQContourTracer::FromTop2BottomTracing()
	{
	/*   
						   ��ǰ��ֵ��(P2)
							  �O
							  �O
					(i,j)     ��    (i,j+1)
						|-----��----|
						|           |
		�ݱ�(i-1,j)---��|           |��----�ݱ�(i-1,j+1)
		ySide[i-1][j]	|           |     ySide[i-1][j+1]
						|           |
						|-----------|    
				 (i-1,j)      ��     (i-1,j+1)
							  �O   
							���(i-1,j)
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
				HandlingAfterNextPointFounded(i-1,j,FALSE);//���ݱ�(i-1,j)��
			}
			else if( *(m_ySide+(i-1)*m_ycols+j) == *(m_ySide+(i-1)*m_ycols+j+1) )
			{
				//ע��:������Ҫ���¼���xSide[i][j]����Ϊ����һ���Ѿ�����Ϊ-2.0
				double xSideIJ = ( m_dTraceValue - *(pGridData+i*m_nx+j)) / ( *(pGridData+i*m_nx+j+1) - *(pGridData+i*m_nx+j) );
				if( /*xSide[i][j]*/ xSideIJ <= 0.5f )
				{
					//TRACE0("xSide[i][j] <= 0.5f\n");
					HandlingAfterNextPointFounded(i-1,j,FALSE);//���ݱ�(i-,j)��
				}
				else
				{
					HandlingAfterNextPointFounded(i-1,j+1,FALSE);//���ݱ�(i-1,j+1)��
				}
				
			}
			else
			{
				HandlingAfterNextPointFounded(i-1,j+1,FALSE);//���ݱ�(i-1,j+1)��
			}
			
		}
		else
		{
			if( IsExistPoint(*(m_ySide+(i-1)*m_ycols+j)) )
			{
				HandlingAfterNextPointFounded(i-1,j,FALSE);//���ݱ�(i-1,j)��
			}
			else if( IsExistPoint(*(m_ySide+(i-1)*m_ycols+j+1)) )
			{
				HandlingAfterNextPointFounded(i-1,j+1,FALSE);//���ݱ�(i-1,j+1)��
			}
			else if( IsExistPoint(*(m_xSide+(i-1)*m_xcols+j)) )
			{//���ݱ��϶�û��
				HandlingAfterNextPointFounded(i-1,j,TRUE);//�ں��(i-1,j)��
			}
			else
			{//�����϶�û��
				Assert(FALSE);
			}
		}
	}

	//��������(V2)
	void KQContourTracer::FromTop2BottomTracingV2()
	{
		
		int i = m_CurrentPoint.i; 
		int j = m_CurrentPoint.j;

		double* pGridData = (double*)this->GetData();
		
		//�Ƚϣ�ySide[i-1][j]      ySide[i-1][j+1]
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
			if( *(m_ySide+(i-1)*m_ycols+j) < 0 /*��ySide[i-1][j] == ySide[i-1][j+1] ==-2*/) 
			{
				HandlingAfterNextPointFounded(i-1,j,TRUE);//��һ���ڶ���ĺ����
			}
			else
			{
				
				//ע��:������Ҫ���¼���xSide[i][j]����Ϊ����һ���Ѿ�����Ϊ-2.0
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

	//��������׷��
	void KQContourTracer::FromLeft2RightTracing()
	{
	/*   
						   ���(i+1,j)
						  xSide[i+1][j]
							   �O 
							   �O
					(i+1,j)    ��    (i+1,j+1)
						|-----------|
						|           |
	 ��ǰ��ֵ��(P2)--����           |��----�ݱ�(i,j+1)
     					|           |     ySide[i][j+1]
						|           |
						|-----------|    
					(i,j)     ��   (i,j+1)
							  �O   
							  ���(i,j)
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
				HandlingAfterNextPointFounded(i,j,TRUE);//�ں��(i,j)��
			}
			else if( *(m_xSide+i*m_xcols+j) == *(m_xSide+(i+1)*m_xcols+j) )
			{
				//ע��:������Ҫ���¼���ySide[i][j]����Ϊ����һ���Ѿ�����Ϊ-2.0
				double ySideIJ = ( m_dTraceValue - *(pGridData+i*m_nx+j)) / ( *(pGridData+(i+1)*m_nx+j) - *(pGridData+i*m_nx+j) );
				if( ySideIJ <= 0.5f )
				//if( ySide[i][j] <= 0.5f )
				{
					//TRACE3("ySide[%d][%d]=%f,��-->��----��������rֵ���\n",i,j,*(m_ySide+i*m_ycols+j));
					HandlingAfterNextPointFounded(i,j,TRUE);//�ں��(i,j)��
				}
				else
				{
					HandlingAfterNextPointFounded(i+1,j,TRUE);//�ں��(i+1,j)��
				}
				
			}
			else
			{
				HandlingAfterNextPointFounded(i+1,j,TRUE);//�ں��(i+1,j)��
			}
			
		}
		else
		{
			if( IsExistPoint(*(m_xSide+i*m_xcols+j)) )
			{
				HandlingAfterNextPointFounded(i,j,TRUE);//�ں��(i,j)��
			}
			else if( IsExistPoint(*(m_xSide+(i+1)*m_xcols+j)) )
			{
				HandlingAfterNextPointFounded(i+1,j,TRUE);//�ں��(i+1,j)��
			}
			else if( IsExistPoint(*(m_ySide+i*m_ycols+j+1)) )
			{//������϶�û��
				HandlingAfterNextPointFounded(i,j+1,FALSE);//���ݱ�(i,j+1)��
			}
			else
			{//�����϶�û��
				Assert(FALSE);
			}
		}
		
	}

	//��������׷��(V2)
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

	//��������
	void KQContourTracer::FromRight2LeftTracing()
	{
	/*   
						   ���(i+1,j-1)
						  xSide[i+1][j-1]
							   �O 
							   �O
				  (i+1,j-1)    ��    (i+1,j)
						|-----------|
						|           |
		�ݱ�(i,j-1)--�� |           ����----��ǰ��ֵ��(P2)
	   ySide[i][j-1]    |           |     
						|           |
						|-----------|    
				  (i,j-1)     ��   (i,j)
							  �O   
							  ���(i,j-1)
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
				HandlingAfterNextPointFounded(i,j-1,TRUE);//�ں��(i,j-1)��
			}
			//else if( *(m_xSide+i*m_xcols+j) == *(m_xSide+(i+1)*m_xcols+j-1) )
			else if( *(m_xSide+i*m_xcols+j-1) == *(m_xSide+(i+1)*m_xcols+j-1) )
			{
				//ע��:������Ҫ���¼���ySide[i][j]����Ϊ����һ���Ѿ�����Ϊ-2.0
				double ySideIJ = ( m_dTraceValue - *(pGridData+i*m_nx+j)) / ( *(pGridData+(i+1)*m_nx+j) - *(pGridData+i*m_nx+j) );
				if( /*ySide[i][j]*/ ySideIJ <= 0.5f )
				{
					HandlingAfterNextPointFounded(i,j-1,TRUE);//�ں��(i,j-1)��
				}
				else
				{
					HandlingAfterNextPointFounded(i+1,j-1,TRUE);//�ں��(i+1,j-1)��
				}
				
			}
			else
			{
				HandlingAfterNextPointFounded(i+1,j-1,TRUE);//�ں��(i+1,j-1)��
			}
			
		}
		else
		{
			if( IsExistPoint(*(m_xSide+i*m_xcols+j-1)) )
			{
				HandlingAfterNextPointFounded(i,j-1,TRUE);//�ں��(i,j-1)��
			}
			else if( IsExistPoint(*(m_xSide+(i+1)*m_xcols+j-1)) )
			{
				HandlingAfterNextPointFounded(i+1,j-1,TRUE);//�ں��(i+1,j-1)��
			}
			else if( IsExistPoint(*(m_ySide+i*m_ycols+j-1)) )
			{//������϶�û��
				HandlingAfterNextPointFounded(i,j-1,FALSE);//���ݱ�(i,j-1)��
			}
			else
			{//�����϶�û��
				Assert(FALSE);
			}
		}
	}

	//��������(V2)
	void KQContourTracer::FromRight2LeftTracingV2()
	{
		/*   
						   ���(i+1,j-1)
						  xSide[i+1][j-1]
							   �O
				  (i+1,j-1)    ��    (i+1,j)
						|-----------|
						|           |
		�ݱ�(i,j-1)--�� |           ����----��ǰ��ֵ��(P2)
	   ySide[i][j-1]    |           |     
						|           |
						|-----------|    
				  (i,j-1)     ��   (i,j)
							  �O   
							  ���(i,j-1)
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

	//����һ����ֵ���ҵ�������Ӧ�Ĵ���
	//����˵����i,j�ֱ��ǵ�ֵ�����ڱߵı�ţ�bHorizonָ�����ڱ��Ǻ�߻����ݱ�
	void KQContourTracer::HandlingAfterNextPointFounded(int i, int j, BOOL bHorizon)
	{

		//����һ����ֵ���ҵ�������Ӧ�Ĵ���,���£�
		//1.��¼�õ�ֵ���i,j
		//2.���㲢����õ�ֵ�������
		//3.��־�õ�ֵ�����ڱߵ��Ѿ�������

		//��֤i��[0,rows-1], j��[0,cols-1]
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
			*(m_xSide+i*m_xcols+j) = -2.0f; //�Ѿ�׷�ٹ�
		}
		else
		{	
			*(m_ySide+i*m_ycols+j) = -2.0f; //�Ѿ�׷�ٹ�
		}
	}

//---------------------------------------------------------
}// namespace ContourBuilder - end
//---------------------------------------------------------

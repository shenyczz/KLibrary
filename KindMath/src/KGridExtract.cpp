#include "StdAfx.h"
#include "KGridExtract.h"

KGridExtract::KGridExtract(void)
{
	Init();
}

KGridExtract::~KGridExtract(void)
{
	Cleanup();
}

void KGridExtract::Init()
{
	//---源网格属性参数
	m_nx0 = 0;				// 网格点X向格点数(列数)
	m_ny0 = 0;				// 网格点Y向格点数(行数)

	m_x0 = 0;				// 网格点X向起始值
	m_y0 = 0;				// 网格点Y向起始值

	m_xInterval = 0.0f;		// 网格点X向间隔
	m_yInterval = 0.0f;		// 网格点Y向间隔

	m_pv0 = NULL;			// 网格点要素值指针(由调用者分配和释放内存)

	//---子网格属性参数
	m_nxStart = 0;			//x方向抽取的开始格点序号
	m_nyStart = 0;			//y方向抽取的开始格点序号

	m_nxCount = 0;			//x方向抽取的格点数(列数)
	m_nyCount = 0;			//y方向抽取的格点数(行数)

	m_xStart = 0.0f;		// 网格点X向起始值
	m_yStart = 0.0f;		// 网格点Y向起始值

	m_pv1 = NULL;			// 网格点要素值指针(由本类分配和释放内存)
}

void KGridExtract::Cleanup()
{
	if(m_pv1) {delete []m_pv1; m_pv1=NULL;}
}

///////////////////////////////////////////////////////////////////////////
// 名    称: Empty
// 功    能: 清理类成员数据
// 访    问: public
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KGridExtract::Empty()
{
	//顺序不可颠倒
	Cleanup();	//step 1
	Init();		//step 2
}

///////////////////////////////////////////////////////////////////////////
// 名    称: SetInput
// 功    能: 设置源网格属性参数
// 访    问: public
// 参    数:
//			[i] nx - 输入源网格x方向格点数
//			[i] ny - 输入源网格y方向格点数
//			[i] x0 - 网格点x向最小值
//			[i] y0 - 网格点y向最小值
//			[i] xInterval - x方向网格点间隔.子网格和源网格具有相同的间距
//			[i] yInterval - y方向网格点间隔.子网格和源网格具有相同的间距
//			[i] pData - 网格点要素值指针.(有调用这负责内存的分配和释放)
//                                       
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KGridExtract::SetInput(int nx,int ny,double x0,double y0,const double xInterval,const double yInterval,double* pData)
{
	if(!pData) throw;
	if( !(xInterval>0) ) throw;	// x方向间隔必须>0
	//-----------------------------
	int i;
	//-----------------------------
	m_nx0 = nx;
	m_ny0 = ny;
	m_x0 = x0;
	m_y0 = y0;
	m_xInterval = xInterval;
	m_yInterval = yInterval;
	m_pv0 = pData;
	//-----------------------------
	if(m_yInterval<0)
	{
		//如果y向间隔小于0,要调整数据
		m_y0 = m_y0 + m_yInterval*(ny-1);
		m_yInterval = -m_yInterval;

		double* pDataTemp = new double[nx*ny];
		//-------------------------
		double* pEndLine = pData + nx*ny-nx;
		for(i=0;i<ny;i++)
		{
			double* pCurrentLine = pEndLine - i*nx;
			memcpy(pDataTemp+i*nx,pCurrentLine,sizeof(double)*nx);
		}

		memcpy(pData,pDataTemp,sizeof(double)*nx*ny);

		delete[] pDataTemp;
		pDataTemp = NULL;
	}
	//-----------------------------
}

///////////////////////////////////////////////////////////////////////////
// 名    称: SetExtractParam
// 功    能: 设置抽取参数
// 访    问: public
// 参    数:
//			[i] nxStart - x方向抽取的起始格点序号
//			[i] nxCount - x方向抽取的格点数
//			[i] nyStart - y方向抽取的起始格点序号
//			[i] nyCount - y方向抽取的格点数
//                                       
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KGridExtract::SetExtractParam(int nxStart,int nxCount,int nyStart,int nyCount)
{
	////起始行号要>=0,抽取点数要>0
	if( !(nyStart>=0) ) throw;
	if( !(nyCount>0) ) throw;

	//起始列号要>=0,抽取点数要>0
	if( !(nxStart>=0) ) throw;
	if( !(nxCount>0) ) throw;

	//起始列号要>=0,抽取点数要>0
	if( !(nyStart+nyCount<=m_ny0) ) throw;	//抽取的最大行号要<=元网格的行数
	if( !(nxStart+nxCount<=m_nx0) ) throw;	//抽取的最大列号要<=元网格的列数
	//-----------------------------
	m_nxStart = nxStart;			//x方向抽取的起始格点序号
	m_nxCount = nxCount;			//x方向抽取的格点数
	m_nyStart = nyStart;			//y方向抽取的起始格点序号
	m_nyCount = nyCount;			//y方向抽取的格点数
	//-----------------------------
}

///////////////////////////////////////////////////////////////////////////
// 名    称: SetExtractParam
// 功    能: 设置抽取参数
// 访    问: public
// 参    数:
//			[i] xmin - 网格所在的最小包围盒的x最小坐标
//			[i] ymin - 网格所在的最小包围盒的y最小坐标
//			[i] xmax - 网格所在的最小包围盒的x最大坐标
//			[i] ymax - 网格所在的最小包围盒的y最大坐标
//                                       
// 返    回: 
// 注    释: 设置抽取网格所在的最小包围盒
//			 要把最小包围盒范围转化成x,y方向上抽取的
//			 开始格点序号和抽取的格点数
///////////////////////////////////////////////////////////////////////////
void KGridExtract::SetExtractParam(double xmin,double ymin,double xmax,double ymax)
{
	int i,j;

	int nxStart,nxCount;	//x方向抽取的起始格点序号和x方向抽取的格点数
	int nyStart,nyCount;	//y方向抽取的起始格点序号和y方向抽取的格点数

	BOOL bFind_nxStart=FALSE;	//是否找到x方向抽取的起始格点序号
	BOOL bFind_nyStart=FALSE;	//是否找到y方向抽取的起始格点序号

	double x, y;
	//-----------------------------
	//查找x方向起始始格点序号
	i=0;
	while(1)
	{
		x = m_x0 + m_xInterval*i;
		//查找x方向起始始格点序号
		if(x>=xmin && !bFind_nxStart)
		{
			nxStart = i;
			bFind_nxStart = TRUE;
		}
		i++;
		x = m_x0 + m_xInterval*i;
		if(bFind_nxStart && x>=xmax)
		{
			if(x>xmax) i--;

			nxCount = i-nxStart+1;
			break;
		}

		if((x>m_x0+m_xInterval*(m_nx0-1)))	//超出源网格范围
		{
			nxCount = 0;
			break;
		}
	}
	//-----------------------------
	//查找y方向起始始格点序号
	j=0;
	while(1)
	{
		y = m_y0 + m_yInterval*j;
		//查找x方向起始始格点序号
		if(y>=ymin && !bFind_nyStart)
		{
			nyStart = j;
			bFind_nyStart = TRUE;
		}
		j++;
		y = m_y0 + m_yInterval*j;
		if(bFind_nyStart && y>=ymax)
		{
			if(y>ymax) j--;

			nyCount = j-nyStart+1;
			break;
		}

		if(y>m_y0+m_yInterval*(m_ny0-1))	//超出源网格范围
		{
			nyCount = 0;
			break;
		}
	}
	//-----------------------------
	SetExtractParam(nxStart,nxCount,nyStart,nyCount);
}

///////////////////////////////////////////////////////////////////////////
// 名    称: Prepare
// 功    能: 准备
// 访    问: public
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
BOOL KGridExtract::Prepare()
{
	if(!VerifyInputData()) return FALSE;
	//-----------------------------
	//计算子网格起始x,y坐标
	m_xStart = m_x0+m_xInterval*m_nxStart;
	m_yStart = m_y0+m_yInterval*m_nyStart;
	//-----------------------------
	//准备内存
	PrepareMemory();
	//-----------------------------
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: Transact
// 功    能: 实施
// 访    问: public
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
BOOL KGridExtract::Transact()
{
	int i,j;
	int ii,jj;
	//-----------------------------
	if(!Prepare()) return FALSE;
	//-----------------------------
	for(i=0;i<m_nyCount;i++)
	{
		ii = m_nyStart+i;		//ii表示在源网格中的行号
		for(j=0;j<m_nxCount;j++)
		{
			jj = m_nxStart+j;	//jj表示在源网格中的列号
			*(m_pv1+i*m_nxCount+j) = *(m_pv0+ii*m_nx0+jj);
		}
	}
	//-----------------------------
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: GetOutput
// 功    能: 取得子网格数据
// 访    问: public
// 参    数:
//			[i] nx - x方向格点数
//			[i] ny - y方向格点数
//			[i] xStart - 网格点x向起始值
//			[i] xStart - 网格点y向起始值
//			[i] xInterval - x方向网格点间隔
//			[i] yInterval - y方向网格点间隔
//			[i] pData - 网格点要素值指针
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KGridExtract::GetOutput(int* nx,int* ny,double* xStart,double* yStart,double* xInterval,double* yInterval,double*& pData)
{
	*nx = m_nxCount;
	*ny = m_nyCount;
	*xStart = m_xStart;
	*yStart = m_yStart;
	*xInterval = m_xInterval;
	*yInterval = m_yInterval;
	pData = m_pv1;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: VerifyInputData
// 功    能: 校验输入数据
// 访    问: protected
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
BOOL KGridExtract::VerifyInputData()
{
	//-----------------------------
	if(m_nyStart<0) 
	{
		::MessageBox(NULL, _T("起始行号要大于等于0"), _T("信息"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	if(m_nxStart<0) 
	{
		::MessageBox(NULL, _T("起始列号要大于等于0"), _T("信息"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	
	if(m_nyCount<1)
	{
		::MessageBox(NULL, _T("抽取的网格行数要大于等于1"), _T("信息"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	if(m_nxCount<1)
	{
		::MessageBox(NULL, _T("抽取的网格列数要大于等于1"), _T("信息"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	if(m_nyStart+m_nyCount>m_ny0)
	{
		::MessageBox(NULL, _T("抽取的最大行号超出源网格范围!"), _T("信息"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	if(m_nxStart+m_nxCount>m_nx0)
	{
		::MessageBox(NULL, _T("抽取的最大列号超出源网格范围!"), _T("信息"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	//-----------------------------
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: PrepareMemory
// 功    能: 准备内存
// 访    问: protected
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KGridExtract::PrepareMemory()
{
	if(m_pv1) {delete []m_pv1; m_pv1=NULL;}

	m_pv1 = new double[m_nxCount*m_nyCount];	//抽取后网格点要素指针
}

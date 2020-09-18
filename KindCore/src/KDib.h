/******************************************************************************
 ** KImage.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KImage
 **  Version: 
 ** Function: 
 ** Explain: 位图文件去掉 BITMAPFILEHEADER 就是一个DIB
 **
 **		位图文件头	--> BITMAPFILEHEADER	-----------------|
 **											                 |
 **		位图信息头	--> BITMAPINFOHEADER	--------|		 |
 **													|		 | bmp 文件
 **		颜色表		--> RGBQUAD                     | DIB	 |
 **                                                 |		 |
 ** 	数据        --> IMAGE DATA			--------|--------|
 **
 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

#include "KObject.h"

namespace Images
{

	class KIND_EXT_CLASS KDib : public KObject
	{
	public:
		enum DibChannel : int
		{
			eBChannel = 0,	// 蓝色通道
			eGChannel,		// 绿色通道
			eRChannel,		// 红色通道

			eYChannel,		// 亮度通道
			eIChannel,		// 亮度通道
			eQChannel,		// 饱和度通道
		};

		enum DibTransform : DWORD
		{
			DibTransform_Reverse			= 0x0001,	// 反相
			DibTransform_Bright				= 0x0002,	// 亮度
			DibTransform_Contrast			= 0x0004,	// 对比度
			DibTransform_ToGray				= 0x0008,	// 转换为灰度图像
			DibTransform_Smooth				= 0x0010,	// 平滑
			DibTransform_HistogramAverage	= 0x0020,	// 直方图均衡
			DibTransform_Adjust_Color_Rgb	= 0x0100,	// 颜色调整 - RGB
			DibTransform_Adjust_Color_Cmy	= 0x0200,	// 颜色调整 - CMY
			DibTransform_Adjust_Color_Hsi	= 0x0400,	// 颜色调整 - HSI
			DibTransform_Adjust_Color_Hls	= 0x0800,	// 颜色调整 - HLS
		};

	public:
		KDib(void);
		virtual ~KDib(void);

	///========================================================
	///成员函数 - Beg
	///---------------
	public:
		// 是否为空
		BOOL IsEmpty() { return m_bEmpty; }

		// 是灰度图象
		BOOL IsGray() { return m_nBitCount == 8; }

		// 是24位彩色图象
		BOOL IsColor24bits() { return m_nBitCount == 24; }

		// 获取尺寸
		DWORD Width() { return m_dwWidth; }
		DWORD Height() { return m_dwHeight; }

		// DIB 图像数据大小
		DWORD ImageSize() { return m_dwSizeImage; }

		// BITMAPINFOHEADER 结构指针
		LPBITMAPINFOHEADER BitmapInfoHeader() { return m_lpBitmapInfoHeader;}

		// 图像数据指针
		LPBYTE ImageData() { return m_lpImageData; }
		LPBYTE DIBits() { return m_lpImageData; }

		// 图像位数
		WORD BitCount() { return m_nBitCount; }

		// 颜色表入口数
		DWORD ColorTableEntries() { return m_dwColorTableEntries; }
		LPVOID ColorTable() { return m_lpvColorTable; }

		// 位图句柄
		HBITMAP Bitmap() { return m_hBitmap; }
		//HBITMAP GetBitmapHandel() { return m_hBitmap; }

		// 调色板句柄
		HPALETTE Palette() { return m_hPalette; }

		HANDLE GetSafeHandle() { return (HANDLE)m_lpBitmapInfoHeader; }

		// 操作符重载
		operator HBITMAP() { return m_hBitmap; }

	public:/// 基本操作

		// 清空DIB,释放已经分配的内存,必要时候关闭映射文件
		void Empty();

		// 读取文件数据
		BOOL Load(LPCTSTR lpszPathName);

		// 写文件
		BOOL Save(LPCTSTR lpszFile);

		// 关联内存资源(必须注意是否需要本类负责内存回收)
		BOOL AttachMemory(LPVOID lpvMem,BOOL bMustDelete = FALSE);

		// 关联图像文件
		BOOL AttachFile(LPCTSTR lpszPathName,BOOL bMapfile = FALSE);

		// 关联 HBITMAP
		BOOL AttachHBITMAP(HBITMAP hBitmap,HPALETTE hPalette = NULL,DWORD dwCompression = BI_RGB);

		// 分离 DIB
		HGLOBAL Detch();

		// DIB 转换为 DDB
		const HBITMAP GetHBITMAP();

		// 取得 DIB 通道数据
		LPBYTE DIBits(int iChannel);

		// 应用调色板
		UINT UsePalette(HDC hDC, BOOL bBackground  = FALSE);

		// 捕捉客户区矩形区域内图形
		void Capture(HDC hDC, LPCRECT lpRect, HPALETTE hPalette = NULL, BOOL bForce256 = FALSE,int* pData = NULL);

		// 绘制半透明图像
		BOOL TranslucentBlt(HDC hDC, int x, int y, HBITMAP hBitmap,int nTranslucent);
		BOOL TranslucentBlt(int x, int y, HBITMAP hBitmap,int nTranslucent);

		// 绘制透明图像
		BOOL TransparentBlt(HDC hDC, int x, int y, HBITMAP hBitmap, UINT clrTransparent);
		BOOL TransparentBlt(int x, int y, HBITMAP hBitmap, UINT clrTransparent);

		// 克隆
		KDib* Clone();

		// 打印
		void Print(HDC hDC,HANDLE hDIB = NULL,BOOL bMustDelete = FALSE);

	public: /// 灰度变化

		// 替换颜色表
		BOOL ReplaceColorTable(LPRGBQUAD lpRrgbQuad);

		// 彩色图像变换成灰度图象
		BOOL Color2Gray(LPRGBQUAD lpRrgbQuad = NULL);

		// 灰度图象变换成彩色图像
		BOOL Gray2Color(LPRGBQUAD lpRrgbQuad = NULL);

		// 反相
		BOOL Reverse();

		// 二值化
		BOOL Binary(int iThreshold);

		// 灰度线性变换
		BOOL LinerTransform(float a,float b);

		// 分段灰度线性拉伸
		BOOL LinearStrech(int x[2],int y[2]);

		// 对数拉伸
		BOOL LogarithmStrech(double a,double b,double c);

		// 指数拉伸
		BOOL ExponentStrech(double a,double b,double c);

		// 图象直方图统计
		BOOL HistogramCompute(int histogram[4][256],double average[4] ,double deviation[4]);

		// 图象直方图均衡
		BOOL HistogramAverage(BOOL bRed = TRUE,BOOL bGreen = TRUE,BOOL bBlue = TRUE);


	public: /// 颜色调整

		// 颜色调整 - RGB
		BOOL AdjustColorRgb(double dRScale, double dGScale, double dBScale);
		BOOL AdjustColorCmy();
		BOOL AdjustColorHsi();
		BOOL AdjustColorHls();


	public: /// 图像增强

		// 图像平滑 - 邻域平均法
		BOOL Smooth_Neighbour(int iDimOfTemplate =3,BYTE byFill = 0);

		// 图像平滑 - 加权平均法
		BOOL Smooth_Power(int iTemplate = 16,BYTE byFill = 0);

		// 图像平滑 - 自适应局部平滑
		BOOL Smooth_Auto(BYTE byFill = 0);

		// 图像平滑 - 中值滤波
		BOOL Smooth_MedianFilter(int iTemplate = 3,BYTE byFill = 0);

		// 图像锐化 - 梯度锐化
		BOOL Sharp_Grad(BYTE byThreshold = 0);

		// 图像锐化 - 拉普拉斯模板锐化
		BOOL Sharp_LapTemplate(int iNumberOfTemplate = 0,BYTE byFill = 0);



	protected: /// 基本操作

		// 关联图像文件
		BOOL AttachMapFile(LPCTSTR lpszPathName);
		// 分离内存映射文件
		void DetachMapFile();

		// 计算DIB数据尺寸
		BOOL ComputeMetrics(LPBITMAPINFOHEADER lpBitmapInfoHeader);

		// 计算调色板尺寸(颜色表入口数)
		BOOL ComputePaletteSize(LPBITMAPINFOHEADER lpBitmapInfoHeader);

		// 由 DIB 颜色表生成调色板
		HPALETTE MakePalette();

		// DIB 转换为 DDB
		HBITMAP DIB2DDB();

		// DDB 转换为 DIB
		HANDLE DDB2DIB(HBITMAP hBitmap,HPALETTE hPalette = NULL,DWORD dwCompression = BI_RGB);
		HANDLE DDB2DIB(HBITMAP hBitmap,BOOL bForce256,int *pbmData);

	protected: /// 灰度变化

		// 单通道分段灰度线形拉伸
		BOOL LinearStrech_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,BYTE x[2],BYTE y[2]);

		// 单通道对数变换
		BOOL LogarithmStrech_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double a,double b,double c);

		// 单通道指数拉伸
		BOOL ExponentStrech_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double a,double b,double c);

		// 单通道图象直方图统计
		BOOL HistogramCompute_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,int histogram[256],double& average ,double& deviation);
		// 图象灰度直方图统计
		BOOL HistogramCompute_Gray(int histogram[256],double& average ,double& deviation);
		// 图象亮度直方图统计
		BOOL HistogramCompute_Bright(int histogram[256],double& average ,double& deviation);
		// 图象红色直方图统计
		BOOL HistogramCompute_Red(int histogram[256],double& average ,double& deviation);
		// 图象绿色直方图统计
		BOOL HistogramCompute_Green(int histogram[256],double& average ,double& deviation);
		// 图象蓝色直方图统计
		BOOL HistogramCompute_Blue(int histogram[256],double& average ,double& deviation);

		// 单通道图象直方图均衡
		BOOL HistogramAverage_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight);
		// 图象灰度直方图均衡
		BOOL HistogramAverage_Gray();
		// 单通道图象直方图均衡 - 红色
		BOOL HistogramAverage_Red();
		// 单通道图象直方图均衡 - 绿色
		BOOL HistogramAverage_Green();
		// 单通道图象直方图均衡 - 蓝色
		BOOL HistogramAverage_Blue();

	protected: /// 颜色调整

		// 单通道颜色调整
		BOOL AdjustColorRgb_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double dScale);


	protected: /// 图像增强

		// 单通道图像平滑 - 模板法
		BOOL Smooth_Template_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double* pTemplate,int w,int h,int x0,int y0,double coef,BYTE byFill);

		// 单通道图像平滑 - 自适应局部平滑
		BOOL Smooth_Auto_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,BYTE byFill);

		// 单通道图像平滑 - 中值滤波
		BOOL Smooth_MedianFilter_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,int w,int h,int x0,int y0,BYTE byFill);

		// 单通道图像锐化 - 梯度锐化
		BOOL Sharp_Grad_SingleChanne(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,BYTE byThreshold);

		// 单通道图像锐化 - 拉普拉斯模板锐化
		BOOL Sharp_LapTemplate_SingleChanne(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double* pTemplate,int w,int h,int x0,int y0,double coef,BYTE byFill);

	private:
		// 查找中值
		BYTE MedianValue(LPBYTE lpBuf,int len);

	///---------------
	///成员函数 - End
	///========================================================

	///========================================================
	///成员数据 - Beg
	///---------------

	protected:
		BOOL				m_bEmpty;				// 是否为空DIB

		HANDLE				m_hFile;				// 打开的文件句柄
		HANDLE				m_hMap;					// 内存映射文件句柄
		LPVOID				m_lpvMapFile;			// 内存映射文件指针

		HBITMAP				m_hBitmap;				// 位图句柄
		HPALETTE			m_hPalette;				// 调色板句柄

		LPBITMAPINFOHEADER	m_lpBitmapInfoHeader;	// BITMAPINFOHEADER 结构指针
		LPVOID				m_lpvColorTable;		// 颜色表地址指针
		LPBYTE				m_lpImageData;			// DIB数据开始地址

		DWORD				m_dwColorTableEntries;	// 颜色表入口数
		DWORD				m_dwSizeImage;			// DIB数据大小

		DWORD				m_dwWidth;				// 图像宽度
		DWORD				m_dwHeight;				// 图像高度

		WORD				m_nBitCount;			// 图像位数

		BOOL				m_bMustDelete;			// 由本类释放 m_lpBitmapInfoHeader 指针

		LPBYTE				m_lpBytesBuf;			// 通道数据的临时缓冲区

	///---------------
	///成员数据 - End
	///========================================================

	};

//---------------------------------------------------------
}// namespace Images - end
//---------------------------------------------------------

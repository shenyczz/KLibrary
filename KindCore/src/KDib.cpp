#include "StdAfx.h"
#include "KDib.h"

namespace Images
{

	KDib::KDib(void)
	{
		m_lpBitmapInfoHeader = NULL;
		m_lpvColorTable = NULL;
		m_lpImageData = NULL;

		m_hFile = NULL;
		m_hMap = NULL;
		m_lpvMapFile = NULL;

		m_hBitmap = NULL;
		m_hPalette = NULL;

		m_dwColorTableEntries = 0;
		m_dwSizeImage = 0;

		m_dwWidth = 0;
		m_dwHeight = 0;

		m_bMustDelete = FALSE;
		m_bEmpty = TRUE;

		m_lpBytesBuf = NULL;
	}

	KDib::~KDib(void)
	{
		Empty();
	}

	///============================================================================
	///  _public                         下面是保护成员函数
	///============================================================================

	//---------------------------------------------------------
	// 名    称: Empty
	// 访    问: public
	// 功    能: 清空DIB,释放已经分配的内存,关闭映射文件
	// 参    数:
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//---------------------------------------------------------
	void KDib::Empty()
	{
		DetachMapFile();	//关闭内存映射文件

		if(m_bMustDelete)
		{
			_delete(m_lpBitmapInfoHeader);
		}

		if(m_hBitmap != NULL)
		{//HBITMAP
			::DeleteObject(m_hBitmap);	
			m_hBitmap = NULL;
		}

		if(m_hPalette != NULL) 
		{//HPALETTE
			::DeleteObject(m_hPalette);	
			m_hPalette = NULL;
		}

		m_dwColorTableEntries = 0;
		m_dwSizeImage = 0;

		m_dwWidth = 0;
		m_dwHeight = 0;

		m_bMustDelete = FALSE;

		m_bEmpty=TRUE;

		_deletea(m_lpBytesBuf);
	}

	//---------------------------------------------------------
	// 名    称: Load
	// 访    问: public
	// 功    能: 读取文件数据
	// 参    数:
	//			 [i] lpszPathName - DIB 文件名
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::Load(LPCTSTR lpszPathName)
	{
		Empty();

		FILE* fp = _tfopen(lpszPathName,_T("rb"));
		if(fp==NULL) return FALSE;

		// 读取文件头
		BITMAPFILEHEADER bmfh;
		if(fread(&bmfh,sizeof(BITMAPFILEHEADER),1,fp)<1)
		{
			fclose(fp);
			return FALSE;
		}

		// 是否 BMP 文件
		if(bmfh.bfType != 0x4d42)
		{
			fclose(fp);
			return FALSE;
		}

		// 读取 BITMAPINFOHEADER
		BITMAPINFOHEADER bmif;
		if(fread(&bmif,sizeof(BITMAPINFOHEADER),1,fp)<1)
		{
			fclose(fp);
			return FALSE;
		}
		// 回移指针
		fseek(fp,(int)sizeof(BITMAPINFOHEADER) * -1,SEEK_CUR);

		// 计算图像数据大小
		bmif.biSizeImage = DIBWIDTHBYTES(bmif.biWidth * bmif.biBitCount) * bmif.biHeight;

		// 计算调色板入口数量
		if(!ComputePaletteSize(&bmif))
		{
			fclose(fp);
			return FALSE;
		}

		DWORD dwDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * ColorTableEntries() + bmif.biSizeImage;

		LPVOID lpvDib = new BYTE[dwDibSize];
		if(fread(lpvDib,sizeof(BYTE)*dwDibSize,1,fp)<1)
		{
			fclose(fp);
			return FALSE;
		}

		fclose(fp);

		this->AttachMemory(lpvDib,TRUE);

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: Save
	// 访    问: public
	// 功    能: 把DIB对象写入文件中
	// 参    数:
	//			 [i] iRChannel - 红色通道
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::Save(LPCTSTR lpszFile)
	{
		if(!m_lpBitmapInfoHeader) return FALSE;

		BITMAPFILEHEADER bmfh;
		bmfh.bfType = BMP_HEADER_MARKER;	// ((WORD)('M'<<8)|'B')
		bmfh.bfSize = sizeof(BITMAPFILEHEADER) +
					  sizeof(BITMAPINFOHEADER) + 
					  sizeof(RGBQUAD) * m_dwColorTableEntries +
					  m_dwSizeImage;	// or = 0
		bmfh.bfReserved1 = 0;
		bmfh.bfReserved2 = 0;
		bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) +
						 sizeof(BITMAPINFOHEADER) + 
						 sizeof(RGBQUAD) * m_dwColorTableEntries;

		FILE* fp = _tfopen(lpszFile,_T("wb"));
		if(fp==NULL) return FALSE;

		try
		{
			// 写文件头
			fwrite(&bmfh,sizeof(BITMAPFILEHEADER),1,fp);
			// 写 DIB 头和颜色表
			fwrite(m_lpBitmapInfoHeader,sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_dwColorTableEntries,1,fp);
			// 写数据
			fwrite(m_lpImageData,m_dwSizeImage,1,fp);
		}
		catch(...)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		fclose(fp);	fp = NULL;

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: AttachMemory
	// 访    问: public
	// 功    能: 关联内存资源
	// 参    数:
	//			 [i] lpvMem - 内存地址指针
	//			 [i] bMustDelete - 是否负责回收内存, 默认 = FALSE
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::AttachMemory(LPVOID lpvMem,BOOL bMustDelete /*=FALSE*/)
	{
		if(lpvMem == NULL)
			return FALSE;

		Empty();

		// 是否负责释放内存
		m_bMustDelete = bMustDelete;

		// DIB 数据指针
		m_lpBitmapInfoHeader = (LPBITMAPINFOHEADER)lpvMem;

		// 设置颜色表地址
		m_lpvColorTable = (LPBYTE)m_lpBitmapInfoHeader + sizeof(BITMAPINFOHEADER);

		// 计算DIB数据尺寸
		if(!ComputeMetrics(m_lpBitmapInfoHeader))
			return FALSE;

		// 计算调色板尺寸(颜色表入口数 m_dwColorTableEntries)
		if(!ComputePaletteSize(m_lpBitmapInfoHeader))
			return FALSE;

		// 生成调色板 m_hPalette
		MakePalette();

		// DIB 数据指针
		m_lpImageData = (LPBYTE)m_lpvColorTable + sizeof(RGBQUAD) * m_dwColorTableEntries;

		// 句柄
		m_hBitmap = this->DIB2DDB();

		m_bEmpty = FALSE;

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: AttachFile
	// 访    问: public
	// 功    能: 关联图像文件
	// 参    数:
	//			 [i] lpszPathName - DIB 文件
	//			 [i] bMustDelete - 是否负责回收内存, 默认 = FALSE
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::AttachFile(LPCTSTR lpszPathName,BOOL bMapfile /*= FALSE*/)
	{
		if(bMapfile) 
			return AttachMapFile(lpszPathName);
		else
			return Load(lpszPathName);
	}

	//---------------------------------------------------------
	// 名    称: AttachFile
	// 访    问: public
	// 功    能: HBITMAP
	// 参    数:
	//			 [i] hBitmap - 位图句柄
	//			 [i] hPalette - 调色板指针
	//			 [i] dwCompression - 压缩方式
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::AttachHBITMAP(HBITMAP hBitmap,HPALETTE hPalette /*=NULL*/,DWORD dwCompression /*=BI_RGB*/)
	{
		return DDB2DIB(hBitmap,hPalette,dwCompression) != NULL;
	}

	//---------------------------------------------------------
	// 名    称: Detch
	// 访    问: public
	// 功    能: 分离 DIB
	// 参    数:
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 分离后本类不负责释放内存
	//---------------------------------------------------------
	HGLOBAL KDib::Detch()
	{ 
		m_bMustDelete = FALSE;
		return (HGLOBAL)m_lpBitmapInfoHeader;
	}

	//---------------------------------------------------------
	// 名    称: GetHBITMAP
	// 访    问: public
	// 功    能: 取得位图句柄
	// 参    数:
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 分离后本类不负责释放内存
	//---------------------------------------------------------
	const HBITMAP KDib::GetHBITMAP( )
	{
		return  this->DIB2DDB();
	}

	//---------------------------------------------------------
	// 名    称: DIBits
	// 访    问: public
	// 功    能: 取得 DIB 通道数据
	// 参    数:
	//			 [i] iChannel - 通道
	// 返    回: 
	//			 LPBYTE - 通道数据缓冲区指针
	// 注    释: 
	//---------------------------------------------------------
	LPBYTE KDib::DIBits(int iChannel)
	{
		DWORD i,j;

		_deletea(m_lpBytesBuf);

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return NULL;
		}

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 8 位
		if(wBitCount == 8)
		{
			return lpDIBits;
		}

		// 多通道每行像素占用的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// 单通道每行像素占用的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// 临时缓冲区
		m_lpBytesBuf = new BYTE[dwBytesPerLineSingleChannel * dwHeight];

		if(iChannel == eBChannel || iChannel == eGChannel || iChannel == eRChannel)
		{// B G R
			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(m_lpBytesBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine +j*3 + iChannel);
				}
			}
		}
		else if(iChannel == eYChannel)
		{// Y
			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(m_lpBytesBuf + i*dwBytesPerLineSingleChannel + j) =
						(BYTE)(WORD)( 
									  (*(lpDIBits + i*dwBytesPerLine+j*3 + 2) * 0.299) +	// R
									  (*(lpDIBits + i*dwBytesPerLine+j*3 + 1) * 0.587) +	// G
									  (*(lpDIBits + i*dwBytesPerLine+j*3 + 0) * 0.114) +	// B
									   0.5
									 );
				}
			}
		}
		else if(iChannel == eIChannel)
		{// I
			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(m_lpBytesBuf + i*dwBytesPerLineSingleChannel + j) =
						(BYTE)(WORD)( 
									  (*(lpDIBits + i*dwBytesPerLine+j*3 + 2) * 0.596) +	// R
									  (*(lpDIBits + i*dwBytesPerLine+j*3 + 1) * (-0.274)) +	// G
									  (*(lpDIBits + i*dwBytesPerLine+j*3 + 0) * (-0.322)) +	// B
									   0.5
									 );
				}
			}
		}
		else if(iChannel == eQChannel)
		{// Q
			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(m_lpBytesBuf + i*dwBytesPerLineSingleChannel + j) =
						(BYTE)(WORD)( 
									  (*(lpDIBits + i*dwBytesPerLine+j*3 + 2) * 0.211) +	// R
									  (*(lpDIBits + i*dwBytesPerLine+j*3 + 1) * (-0.523)) +	// G
									  (*(lpDIBits + i*dwBytesPerLine+j*3 + 0) * (0.312)) +	// B
									   0.5
									 );
				}
			}
		}
		else
		{
			return NULL;
		}

		return m_lpBytesBuf;
	}

	//---------------------------------------------------------
	// 名    称: UsePalette
	// 访    问: public
	//
	// 功    能: 应用调色板
	//
	// 参    数:
	//			 [i] hDC - DC 句柄
	//			 [i] bBackground - 背景应用
	//
	// 返    回: 调色板颜色数
	//
	// 注    释: 
	//
	//---------------------------------------------------------
	UINT KDib::UsePalette(HDC hDC, BOOL bBackground /*= FALSE*/)
	{
		if(m_hPalette == NULL) return 0;

		::SelectPalette(hDC, m_hPalette, bBackground);

		return ::RealizePalette(hDC);
	}

	//---------------------------------------------------------
	// 名    称: Capture
	// 访    问: public
	// 功    能: 捕捉客户区矩形区域内图形
	// 参    数:
	//			 [i] hDC - 设备场景指针
	//			 [i] rect - 抓取区域矩形
	//			 [i] hPalette - 调色板,默认 = NULL
	//			 [i] bForce256 - 256色,默认 = FALSE
	//			 [i] pData - 当 bForce256 = TRUE 时,不能为 NULL
	// 返    回: 
	// 注    释: 
	//---------------------------------------------------------
	void KDib::Capture(HDC hDC, LPCRECT lpRect, HPALETTE hPalette /*= NULL*/, BOOL bForce256 /*= FALSE*/,int* pData /*= NULL*/)
	{
		if(bForce256 && pData == NULL)
			throw;

		Empty();

		RECT rect = *lpRect;

		//保存抓取区域大小
		int nWidth = abs(rect.right - rect.left);
		int nHeight = abs(rect.bottom - rect.top);

		//HDC hScreenDC=::CreateDC(_T("DISPLAY"),NULL,NULL,NULL);	//桌面DC
		//HDC hCaptureDC	= hScreenDC;
		HDC hCaptureDC	= hDC;
		HDC hMemDC		= ::CreateCompatibleDC(hCaptureDC);
		HBITMAP hBitmap	= ::CreateCompatibleBitmap(hCaptureDC,nWidth,nHeight);

		::SelectObject(hMemDC,hBitmap);
	
		//将屏幕内容拷贝到空白位图
		::BitBlt(hMemDC,0,0,nWidth,nHeight,hDC,rect.left,rect.top,SRCCOPY);

		if(!hPalette)
		{
			//将系统调色板保存到位图调色板中
			//创建一个足够大的调色板
			//根据当前系统取得颜色数
			int nBITSPIXEL	= ::GetDeviceCaps(hCaptureDC,BITSPIXEL);
			int nPLANES		= ::GetDeviceCaps(hCaptureDC,PLANES);
			int nColors=(1<<nBITSPIXEL*nPLANES);

			//初始化空白逻辑调色板 用系统调色板填充空白调色板
			LOGPALETTE* pLogPal		= (LOGPALETTE*)new BYTE[sizeof(LOGPALETTE)+nColors*sizeof(PALETTEENTRY)];
			pLogPal->palVersion		= PALVERSION;
			pLogPal->palNumEntries	= nColors;
			nColors					= ::GetSystemPaletteEntries(hCaptureDC,0,nColors,(LPPALETTEENTRY)(pLogPal->palPalEntry));

			//创建新的调色板
			m_hPalette = ::CreatePalette(pLogPal);

			delete []pLogPal;

			//m_hPalette = ::CreateHalftonePalette(hDC);
		}
		else
		{
			m_hPalette = hPalette;
		}


		::DeleteObject(m_hBitmap);
		m_hBitmap=hBitmap;

		if(!bForce256)
			DDB2DIB(hBitmap,m_hPalette);
		else
			DDB2DIB(hBitmap,bForce256,pData);

		::DeleteDC(hMemDC);

		return;
	}

	//---------------------------------------------------------
	// 名    称: TranslucentBlt
	// 访    问: public
	// 功    能: 绘制半透明图像
	// 参    数:
	//			 [i] hDC - DC 句柄
	//			 [i] x - hBitmap 的左上角x坐标
	//			 [i] y - hBitmap 的左上角y坐标
	//			 [i] hBitmap - 位图句柄
	//			 [i] nTranslucent - 透明比例 0 - 100
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 白色不能透明
	//---------------------------------------------------------
	BOOL KDib::TranslucentBlt(HDC hDC, int x, int y, HBITMAP hBitmap,int nTranslucent)
	{
		if(nTranslucent>100) nTranslucent = 100;

		// 计算要半透明显示位图尺寸
		BITMAP bm;
		::GetObject(hBitmap,sizeof(BITMAP),&bm);
		int nWidth = bm.bmWidth;
		int nHeight = bm.bmHeight;

		// 创建内存 DC 以便选入图像
		HDC hMemDC = ::CreateCompatibleDC(hDC);
		HBITMAP hOldMemBitmap = (HBITMAP)::SelectObject(hMemDC,hBitmap);

		// 创建掩模 DC
		HDC hMaskDC = ::CreateCompatibleDC(hDC);
		HBITMAP hMaskBitmap = ::CreateCompatibleBitmap(hDC,nWidth,nHeight);
		HBITMAP hOldMaskBitmap = (HBITMAP)::SelectObject(hMaskDC,hMaskBitmap);

		RECT rectFill;
		rectFill.left = 0;
		rectFill.top = 0;
		rectFill.right = nWidth;
		rectFill.bottom = nHeight;
		HBRUSH hBrush = ::CreateSolidBrush(RGB(255*nTranslucent/100,255*nTranslucent/100,255*nTranslucent/100));
		::FillRect(hMaskDC,&rectFill,hBrush);

		::BitBlt(hDC,x,y,nWidth,nHeight,hMaskDC,0,0,SRCAND);
		::BitBlt(hDC,x,y,nWidth,nHeight,hMemDC,0,0,SRCPAINT);

		// 清理
		::SelectObject(hMemDC,hOldMemBitmap);
		::DeleteDC(hMemDC);

		::SelectObject(hMaskDC,hOldMaskBitmap);
		::DeleteObject(hMaskBitmap);
		::DeleteDC(hMaskDC);

		return TRUE;
	}
	//---------------------------------------------------------
	// 名    称: TranslucentBlt
	// 访    问: public
	// 功    能: 绘制半透明图像
	// 参    数:
	//			 [i] x - hBitmap 的左上角x坐标
	//			 [i] y - hBitmap 的左上角y坐标
	//			 [i] hBitmap - 位图句柄
	//			 [i] nTranslucent - 透明比例 0 - 100
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 白色不能透明
	//---------------------------------------------------------
	BOOL KDib::TranslucentBlt(int x, int y, HBITMAP hBitmap,int nTranslucent)
	{
		if(this->IsEmpty())
		{
			return FALSE;
		}

		if(nTranslucent>100) nTranslucent = 100;

		HDC hDC = ::GetDC(NULL);

		// 先把本身选入 DC
		HDC hDestDC = ::CreateCompatibleDC(hDC);
		HBITMAP hDestBitmap = this->DIB2DDB();
		HBITMAP hOldDestBitmap = (HBITMAP)::SelectObject(hDestDC,hDestBitmap);

		// 计算要半透明显示位图尺寸
		BITMAP bm;
		::GetObject(hBitmap,sizeof(BITMAP),&bm);
		int nWidth = bm.bmWidth;
		int nHeight = bm.bmHeight;

		// 创建内存 DC 以便选入图像
		HDC hMemDC = ::CreateCompatibleDC(hDC);
		HBITMAP hOldMemBitmap = (HBITMAP)::SelectObject(hMemDC,hBitmap);

		// 创建掩模 DC
		HDC hMaskDC = ::CreateCompatibleDC(hDC);
		HBITMAP hMaskBitmap = ::CreateCompatibleBitmap(hDC,nWidth,nHeight);
		HBITMAP hOldMaskBitmap = (HBITMAP)::SelectObject(hMaskDC,hMaskBitmap);

		RECT rectFill;
		rectFill.left = 0;
		rectFill.top = 0;
		rectFill.right = nWidth;
		rectFill.bottom = nHeight;
		HBRUSH hBrush = ::CreateSolidBrush(RGB(255*nTranslucent/100,255*nTranslucent/100,255*nTranslucent/100));
		::FillRect(hMaskDC,&rectFill,hBrush);

		::BitBlt(hDestDC,x,y,nWidth,nHeight,hMaskDC,0,0,SRCAND);
		::BitBlt(hDestDC,x,y,nWidth,nHeight,hMemDC,0,0,SRCPAINT);

		this->AttachHBITMAP(hDestBitmap);

		// 清理
		::SelectObject(hDestDC,hOldDestBitmap);
		::DeleteObject(hDestBitmap);
		::DeleteDC(hDestDC);

		::SelectObject(hMemDC,hOldMemBitmap);
		::DeleteDC(hMemDC);

		::SelectObject(hMaskDC,hOldMaskBitmap);
		::DeleteObject(hMaskBitmap);
		::DeleteDC(hMaskDC);

		::ReleaseDC(NULL,hDC);

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: TransparentBlt
	// 访    问: public
	// 功    能: 绘制半透明图像
	// 参    数:
	//			 [i] hDC - DC 句柄
	//			 [i] x - hBitmap 的左上角x坐标
	//			 [i] y - hBitmap 的左上角y坐标
	//			 [i] hBitmap - 位图句柄
	//			 [i] clrTransparent - 透明色
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::TransparentBlt(HDC hDC, int x, int y, HBITMAP hBitmap, UINT clrTransparent)
	{
		// 计算要半透明显示位图尺寸
		BITMAP bm;
		::GetObject(hBitmap,sizeof(BITMAP),&bm);
		int nWidth = bm.bmWidth;
		int nHeight = bm.bmHeight;

		// 创建内存 DC 以便选入图像
		HDC hMemDC = ::CreateCompatibleDC(hDC);
		HBITMAP hOldImageBitmap = (HBITMAP)::SelectObject(hMemDC,hBitmap);

		HDC hMaskDC = ::CreateCompatibleDC(hDC);
		HBITMAP hMaskBitmap = ::CreateBitmap(nWidth,nHeight,1,1,NULL);
		HBITMAP hOldMaskBitmap = (HBITMAP)::SelectObject(hMaskDC,hMaskBitmap);

		::SetBkColor(hMemDC,clrTransparent);
		::BitBlt(hMaskDC,0,0,nWidth,nHeight,hMemDC,0,0,SRCCOPY);

		::SetBkColor(hMemDC,RGB(0,0,0));
		::SetTextColor(hMemDC,RGB(255,255,255));
		::BitBlt(hMemDC,0,0,nWidth,nHeight,hMaskDC,0,0,SRCAND);

		::BitBlt(hDC,x,y,nWidth,nHeight,hMaskDC,0,0,SRCAND);
		::BitBlt(hDC,x,y,nWidth,nHeight,hMemDC,0,0,SRCPAINT);

		::SelectObject(hMemDC,hOldImageBitmap);
		::DeleteDC(hMemDC);

		::SelectObject(hMaskDC,hOldMaskBitmap);
		::DeleteObject(hMaskBitmap);
		::DeleteDC(hMaskDC);

		return TRUE;
	}
	//---------------------------------------------------------
	// 名    称: TransparentBlt
	// 访    问: public
	// 功    能: 绘制半透明图像
	// 参    数:
	//			 [i] x - hBitmap 的左上角x坐标
	//			 [i] y - hBitmap 的左上角y坐标
	//			 [i] hBitmap - 位图句柄
	//			 [i] clrTransparent - 透明色
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::TransparentBlt(int x, int y, HBITMAP hBitmap, UINT clrTransparent)
	{
		if(this->IsEmpty())
		{
			return FALSE;
		}

		KDib dibtmp;
	

		HDC hDC = ::GetDC(NULL);

		// 先把本身选入 DC
		HDC hDestDC = ::CreateCompatibleDC(hDC);
		HBITMAP hDestBitmap = this->DIB2DDB();
		HBITMAP hOldDestBitmap = (HBITMAP)::SelectObject(hDestDC,hDestBitmap);

		// 计算要透明显示位图尺寸
		BITMAP bm;
		::GetObject(hBitmap,sizeof(BITMAP),&bm);
		int nWidth = bm.bmWidth;
		int nHeight = bm.bmHeight;

		// 创建内存 DC 以便选入图像
		HDC hMemDC = ::CreateCompatibleDC(hDC);
		HBITMAP hOldImageBitmap = (HBITMAP)::SelectObject(hMemDC,hBitmap);

		HDC hMaskDC = ::CreateCompatibleDC(hDC);
		HBITMAP hMaskBitmap = ::CreateBitmap(nWidth,nHeight,1,1,NULL);
		HBITMAP hOldMaskBitmap = (HBITMAP)::SelectObject(hMaskDC,hMaskBitmap);

		::SetBkColor(hMemDC,clrTransparent);
		::BitBlt(hMaskDC,0,0,nWidth,nHeight,hMemDC,0,0,SRCCOPY);

		::SetBkColor(hMemDC,RGB(0,0,0));
		::SetTextColor(hMemDC,RGB(255,255,255));
		::BitBlt(hMemDC,0,0,nWidth,nHeight,hMaskDC,0,0,SRCAND);

		::BitBlt(hDestDC,x,y,nWidth,nHeight,hMaskDC,0,0,SRCAND);
		::BitBlt(hDestDC,x,y,nWidth,nHeight,hMemDC,0,0,SRCPAINT);

		this->AttachHBITMAP(hDestBitmap);

		::SelectObject(hDestDC,hOldDestBitmap);
		::DeleteObject(hDestBitmap);
		::DeleteDC(hDestDC);

		::SelectObject(hMemDC,hOldImageBitmap);
		::DeleteDC(hMemDC);

		::SelectObject(hMaskDC,hOldMaskBitmap);
		::DeleteObject(hMaskBitmap);
		::DeleteDC(hMaskDC);

		::ReleaseDC(NULL,hDC);

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: Clone
	// 访    问: public
	// 功    能: 克隆
	// 参    数:
	// 返    回: 
	// 注    释: 
	//---------------------------------------------------------
	KDib* KDib::Clone()
	{
		if(IsEmpty())
			return NULL;

		DWORD dwDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_dwColorTableEntries + m_dwSizeImage;

		LPBYTE lpBytes = new BYTE[dwDibSize];
		memcpy(lpBytes, m_lpBitmapInfoHeader, dwDibSize);

		KDib* pDib = new KDib;
		pDib->AttachMemory(lpBytes,TRUE);

		return pDib;
	}

	//---------------------------------------------------------
	// 名    称: Print
	// 访    问: public
	// 功    能: 打印 DIB
	// 参    数:
	//			 [i] hDC - DC 句柄
	//			 [i] hDIB - 外部传来 DIB 句柄,默认 = NULL
	// 返    回: 
	// 注    释: 
	//---------------------------------------------------------
	void KDib::Print(HDC hDC,HANDLE hDIB /*= NULL*/,BOOL bMustDelete /*= FALSE*/)
	{
		if(hDIB)
		{
			AttachMemory(hDIB,bMustDelete);
		}

		if(IsEmpty())
			return;

		int nMapMode = ::GetMapMode(hDC);
		::SetMapMode(hDC,MM_LOENGLISH);		//映射模式 MM_LOENGLISH

		//LPBITMAPINFOHEADER lpBitmapInfoHeader = m_lpBitmapInfoHeader;
		LPBITMAPINFOHEADER lpBitmapInfoHeader = this->BitmapInfoHeader();

		DWORD dwWidth = m_dwWidth;		// DIB 宽度
		DWORD dwHeight = m_dwHeight;	// DIB 高度

		// DIB头尺寸
		DWORD dwDIBHeadSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_dwColorTableEntries;

		//数据指针
		LPSTR lpDIBBits=(LPSTR)m_lpImageData;	//only image data

		//计算可打印区域,要求各边有1/8英寸空隙 1 inch=25.4mm
		//在MM_LOENGLISH下
		int cxBorder = ::GetDeviceCaps(hDC,LOGPIXELSX)/8;
		int cyBorder = ::GetDeviceCaps(hDC,LOGPIXELSY)/8;
		//TRACE("cxBorder=%d cyborder=%d\n",cxBorder,cyBorder);

		POINT point;
		//取得打印区域的宽度和高度(象素)	A4=210*297mm
		point.x = ::GetDeviceCaps(hDC,HORZRES);
		point.y = ::GetDeviceCaps(hDC,VERTRES);
		//TRACE("Pixel cxPage=%d cyPage=%d\n",size.cx,size.cy);	//1488*2104
		::DPtoLP(hDC,&point,1);
		//TRACE("LP cxPage=%d cyPage=%d\n",size.cx,size.cy);		//827*1169

		int cxPage = abs(point.x);	// 页款
		int cyPage = abs(point.y);	// 页高

		if(cxPage < cyPage)
		{
			cxPage = point.x;
			cyPage=(int)(((double)cxPage / (double)dwWidth) * (double)dwHeight);
		}
		else
		{
			cxPage = point.x;
			cyPage = point.y;
		}

		//伸展位图以充满打印纸
		::SetStretchBltMode(hDC,COLORONCOLOR);
		::StretchDIBits(hDC,
						cxBorder,
						-cyBorder,
						cxPage-cxBorder*2,
						(cyPage-cyBorder*2),
						0,0,
						dwWidth,
						dwHeight,
						lpDIBBits,
						(LPBITMAPINFO)lpBitmapInfoHeader,
						DIB_RGB_COLORS,
						SRCCOPY
						);
	}


	//---------------------------------------------------------
	// 名    称: ReplaceColorTable
	// 访    问: public
	// 功    能: 替换颜色表
	// 参    数:
	// 返    回: 
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::ReplaceColorTable(LPRGBQUAD lpRrgbQuad)
	{
		// 8位图才有颜色表
		if(BitCount() != 8 || lpRrgbQuad==NULL)
		{
			return FALSE;
		}

		LPVOID lpColorTable = ColorTable();
		memcpy(lpColorTable,lpRrgbQuad,sizeof(RGBQUAD)*256);

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: Color2Gray
	// 访    问: public
	// 功    能: 彩色图像变换成灰度图象
	// 参    数:
	//			 [i] lpRrgbQuad - 颜色表指针
	// 返    回: 
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::Color2Gray(LPRGBQUAD lpRrgbQuad /*= NULL*/)
	{
		if(BitCount() != 24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		DWORD dwSizeImage = DIBWIDTHBYTES(dwWidth*8) * dwHeight;
		DWORD dwDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + dwSizeImage;

		LPBYTE lpDib = new BYTE[dwDibSize];

		PBITMAPINFO pBitmapInfo = (PBITMAPINFO)lpDib;

		pBitmapInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
		pBitmapInfo->bmiHeader.biWidth			= dwWidth;
		pBitmapInfo->bmiHeader.biHeight			= dwHeight;
		pBitmapInfo->bmiHeader.biPlanes			= 1;
		pBitmapInfo->bmiHeader.biBitCount		= 8;
		pBitmapInfo->bmiHeader.biCompression	= BI_RGB;
		pBitmapInfo->bmiHeader.biSizeImage		= dwSizeImage;	//用BI_RGB方式时设可为0
		pBitmapInfo->bmiHeader.biXPelsPerMeter	= 0;
		pBitmapInfo->bmiHeader.biYPelsPerMeter	= 0;
		pBitmapInfo->bmiHeader.biClrUsed		= 0;
		pBitmapInfo->bmiHeader.biClrImportant	= 0;

		// 生成灰度图的颜色表
		LPRGBQUAD lpRgbQuad = (LPRGBQUAD)(lpDib + sizeof(BITMAPINFOHEADER));
		for(int i=0; i<256; i++)
		{
			lpRgbQuad[i].rgbRed			= i;
			lpRgbQuad[i].rgbGreen		= i;
			lpRgbQuad[i].rgbBlue		= i;
			lpRgbQuad[i].rgbReserved	= 0;
		}

		DWORD dwDIBWidthSource = DIBWIDTHBYTES(dwWidth*8*3);
		// 源数据指针
		LPBYTE lpDIBitsSource = DIBits();


		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);
		// DIB 图像数据地址
		LPBYTE lpDIBits = lpDib + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;

		// I = 0.3B + 0.59G + 0.11R
		for(size_t i=0;i<dwHeight;i++)
		{
			for(size_t j=0;j<dwWidth;j++)
			{
				*(lpDIBits + i*dwBytesPerLine + j) =
					(BYTE)(WORD)( (*(lpDIBitsSource + i*dwDIBWidthSource+j*3 + 0) * 0.11) +	// R
								  (*(lpDIBitsSource + i*dwDIBWidthSource+j*3 + 1) * 0.59) +	// G
								  (*(lpDIBitsSource + i*dwDIBWidthSource+j*3 + 2) * 0.30) +	// B
								   0.5 );
			}
		}

		this->AttachMemory(lpDib,TRUE);

		if(lpRrgbQuad)
		{
			ReplaceColorTable(lpRrgbQuad);
		}

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: Gray2Color
	// 访    问: public
	// 功    能: 灰度图象变换成彩色图像
	// 参    数:
	//			 [i] lpRrgbQuad - 颜色表指针
	// 返    回: 
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::Gray2Color(LPRGBQUAD lpRrgbQuad /*= NULL*/)
	{
		if(BitCount() != 8)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		DWORD dwSizeImage = DIBWIDTHBYTES(dwWidth*8*3) * dwHeight;
		DWORD dwDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 0 + dwSizeImage;

		LPBYTE lpDib = new BYTE[dwDibSize];

		PBITMAPINFO pBitmapInfo = (PBITMAPINFO)lpDib;

		pBitmapInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
		pBitmapInfo->bmiHeader.biWidth			= dwWidth;
		pBitmapInfo->bmiHeader.biHeight			= dwHeight;
		pBitmapInfo->bmiHeader.biPlanes			= 1;
		pBitmapInfo->bmiHeader.biBitCount		= 24;
		pBitmapInfo->bmiHeader.biCompression	= BI_RGB;
		pBitmapInfo->bmiHeader.biSizeImage		= dwSizeImage;	//用BI_RGB方式时设可为0
		pBitmapInfo->bmiHeader.biXPelsPerMeter	= 0;
		pBitmapInfo->bmiHeader.biYPelsPerMeter	= 0;
		pBitmapInfo->bmiHeader.biClrUsed		= 0;
		pBitmapInfo->bmiHeader.biClrImportant	= 0;


		// 源数据指针
		LPBYTE lpDIBitsSource = DIBits();
		DWORD dwDIBWidthSource = DIBWIDTHBYTES(dwWidth*8);

		// DIB 图像数据地址
		LPBYTE lpDIBits = lpDib + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 0;
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*3);

		// 用像素灰度值为彩色格式中每个通道赋值，三个通道相等
		for(size_t i=0;i<dwHeight;i++)
		{
			for(size_t j=0;j<dwWidth;j++)
			{
				BYTE byValue = *(lpDIBitsSource + i*dwDIBWidthSource + j);

				*(lpDIBits + i*dwBytesPerLine +j*3 + 0) = lpRrgbQuad ? lpRrgbQuad[byValue].rgbRed	: byValue;
				*(lpDIBits + i*dwBytesPerLine +j*3 + 1) = lpRrgbQuad ? lpRrgbQuad[byValue].rgbGreen : byValue;
				*(lpDIBits + i*dwBytesPerLine +j*3 + 2) = lpRrgbQuad ? lpRrgbQuad[byValue].rgbBlue	: byValue;
			}
		}


		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: Reverse
	// 访    问: public
	// 功    能: 反相
	// 参    数:
	// 返    回: 
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::Reverse()
	{
		// 颜色位数(只处理8位和24位位图数据)
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 颜色表入口数
		DWORD dwColorTableEntries = ColorTableEntries();

		DWORD dwSizeImage = DIBWIDTHBYTES(dwWidth*8*wBitCount/8) * dwHeight;
		DWORD dwDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * dwColorTableEntries + dwSizeImage;

		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*wBitCount/8);

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		for(size_t i=0;i<dwHeight;i++)
		{
			for(size_t j=0;j<dwWidth;j++)
			{
				for(WORD k=0; k<wBitCount/8; k++)
				{
					*(lpDIBits + i*dwBytesPerLine + j*wBitCount/8 + k) = (BYTE)(255 - *(lpDIBits + i*dwBytesPerLine + j*wBitCount/8 + k));
				}
			}
		}

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: Reverse
	// 访    问: public
	// 功    能: 二值化
	// 参    数:
	//			 [i] iThreshold - 阈值
	// 返    回: 
	// 注    释: 
	//			 小于 iThreshold 的是背景,大于等于 iThreshold 的是目标值
	//---------------------------------------------------------
	BOOL KDib::Binary(int iThreshold)
	{
		if(iThreshold<0 || iThreshold>255)
		{
			return FALSE;
		}

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		if(wBitCount==24)
		{// 先灰度化
			if(!Color2Gray()) return FALSE;
		}

		// 重新取得颜色位数
		wBitCount = BitCount();
		if(wBitCount!=8)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 颜色表入口数(=256)
		DWORD dwColorTableEntries = ColorTableEntries();

		DWORD dwSizeImage = DIBWIDTHBYTES(dwWidth*8*wBitCount/8) * dwHeight;
		DWORD dwDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * dwColorTableEntries + dwSizeImage;

		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*wBitCount/8);

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		for(size_t i=0;i<dwHeight;i++)
		{
			for(size_t j=0;j<dwWidth;j++)
			{
				BYTE byValue = *(lpDIBits + i*dwBytesPerLine + j);
				*(lpDIBits + i*dwBytesPerLine + j) = (BYTE)(byValue < (BYTE)iThreshold ? 0 : 255);
			}
		}

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: LinerTransform
	// 访    问: public
	// 功    能: 线性变换
	// 参    数:
	//			 [i] a - 线性变换系数A(斜率),决定对比度
	//			 [i] b - 线性变换系数B(截距),决定亮度
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 成功
	// 注    释: 
	//			线性变换公式 f(x) = a*x + b
	//				(1) a=1; b=0 --> 图像不变
	//				(2) a=1; b!=0 --> 调整亮度 b=[-255,+255]
	//				(3) a<0 --> 求补
	//				(4) a<-1;b=255 --> 反相
	//				(5) a>1, b=0 --> 增加对比度 a=(1,4]
	//				(6) a<1, b=0 --> 减小对比度 a=(0.1,1]
	//---------------------------------------------------------
	BOOL KDib::LinerTransform(float a,float b)
	{
		DWORD i,j;

		KDib* pDib = this;

		if(!pDib || pDib->IsEmpty())
		{
			return TRUE;
		}

		// 颜色位数
		WORD wBitCount = pDib->BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		// 数据通道数
		int ch = wBitCount / 8;

		DWORD dwWidth = pDib->Width();
		DWORD dwHeight = pDib->Height();

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*ch);

		// 图像数据指针
		LPBYTE lpDIBits = pDib->DIBits();

		PBYTE lpSrc=NULL;
		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				for(int k=0;k<ch;k++)
				{
					//指向DIB第(i,j)像素的指针
					PBYTE lpPixel = lpDIBits + i*dwBytesPerLine + j*ch + k;

					//线性变换
					float fTemp = (*lpPixel)*a + b;

					if(fTemp>255)		fTemp = 255;
					else if(fTemp<0)	fTemp = 0;
					else				fTemp = fTemp+0.5f;	//四舍五入

					*lpPixel = (BYTE)(fTemp);
				}//k
			}//j
		}//i

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: LinearStrech
	// 访    问: public
	// 功    能: 分段灰度线形拉伸
	// 参    数:
	//			 [i] x[2] - 
	//			 [i] y[2] - 
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 成功
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::LinearStrech(int x[2],int y[2])
	{
		DWORD i,j;

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		BYTE xx[2],yy[2];
		xx[0] = (BYTE)x[0];
		xx[1] = (BYTE)x[1];

		yy[0] = (BYTE)y[0];
		yy[1] = (BYTE)y[1];

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		if(wBitCount == 8)
		{
			return LinearStrech_SingleChannel(lpDIBits,dwWidth,dwHeight,xx,yy);
		}

		// 如果是彩色图像,则把每个通道的数据分离并线性拉伸,
		// 然后再合成彩色图像

		// 多通道每行像素占用的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// 单通道每行像素占用的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// 临时缓冲区
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel * dwHeight];

		for(int k=0;k<3;k++)
		{// 三通道
			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine +j*3 + k);
				}
			}

			LinearStrech_SingleChannel(lpBuf,dwWidth,dwHeight,xx,yy);

			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpDIBits + i*dwBytesPerLine +j*3 + k) = *(lpBuf + i*dwBytesPerLineSingleChannel + j);
				}
			}
		}


		delete []lpBuf; lpBuf = NULL;

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: LogarithmStrech
	// 访    问: public
	// 功    能: 对数拉伸
	// 参    数:
	//			 [i] a - 
	//			 [i] b - 
	//			 [i] c - 
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 成功
	// 注    释: 
	//			 y = a + ln(x+1)/(b*lnc)
	//			 b!=0 c!=1
	//---------------------------------------------------------
	BOOL KDib::LogarithmStrech(double a,double b,double c)
	{
		DWORD i,j;

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		// 参数要求
		if(b==0 || c==1)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		if(wBitCount == 8)
		{
			return LogarithmStrech_SingleChannel(lpDIBits,dwWidth,dwHeight,a,b,c);
		}

		// 如果是彩色图像,则把每个通道的数据分离并对数拉伸,
		// 然后再合成彩色图像

		// 多通道每行像素占用的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// 单通道每行像素占用的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// 临时缓冲区
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel * dwHeight];

		for(int k=0;k<3;k++)
		{
			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine +j*3 + k);
				}
			}

			LogarithmStrech_SingleChannel(lpBuf,dwWidth,dwHeight,a,b,c);

			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpDIBits + i*dwBytesPerLine +j*3 + k) = *(lpBuf + i*dwBytesPerLineSingleChannel + j);
				}
			}
		}

		delete []lpBuf; lpBuf = NULL;

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: ExponentStrech
	// 访    问: public
	// 功    能: 指数拉伸
	// 参    数:
	//			 [i] a - 
	//			 [i] b - 
	//			 [i] c - 
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 成功
	// 注    释: 
	//			 y = b^[c*(x-a)] - 1
	//---------------------------------------------------------
	BOOL KDib::ExponentStrech(double a,double b,double c)
	{
		DWORD i,j;

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		if(wBitCount == 8)
		{
			return ExponentStrech_SingleChannel(lpDIBits,dwWidth,dwHeight,a,b,c);
		}

		// 如果是彩色图像,则把每个通道的数据分离并对数拉伸,
		// 然后再合成彩色图像

		// 多通道每行像素占用的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// 单通道每行像素占用的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// 临时缓冲区
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel * dwHeight];

		for(int k=0;k<3;k++)
		{
			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine +j*3 + k);
				}
			}

			ExponentStrech_SingleChannel(lpBuf,dwWidth,dwHeight,a,b,c);

			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpDIBits + i*dwBytesPerLine +j*3 + k) = *(lpBuf + i*dwBytesPerLineSingleChannel + j);
				}
			}
		}

		delete []lpBuf; lpBuf = NULL;

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: ComputeHistogram
	// 访    问: public
	// 功    能: 图象直方图统计
	// 参    数:
	//			 [o] histogram - 依次存放各通道的灰度统计数据
	//			     histogram[3] - 亮度通道统计数据
	//			     histogram[2] - 红色通道统计数据
	//			     histogram[1] - 绿色通道统计数据
	//			     histogram[0] - 兰色通道统计数据 / 灰度图的灰度统计数据
	//
	//			 [o] average - 依次存放各通道的均值
	//				 average[3] - 亮度通道均值
	//				 average[2] - 红色通道均值
	//				 average[1] - 绿色通道均值
	//				 average[0] - 兰色通道均值 / 灰度图的灰度均值
	//
	//			 [o] deviation - 依次存放各通道的方差
	//				 deviation[3] - 亮度通道方差
	//				 deviation[2] - 红色通道方差
	//				 deviation[1] - 绿色通道方差
	//				 deviation[0] - 兰色通道方差 / 灰度图的灰度方差
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::HistogramCompute(int histogram[4][256],double average[4] ,double deviation[4])
	{
		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		if(wBitCount==8)
		{
			return HistogramCompute_Gray(histogram[0],average[0],deviation[0]);
		}

		BOOL bRet[4];
		if(wBitCount==24)
		{
			bRet[3] = HistogramCompute_Bright(histogram[3],average[3],deviation[3]);
			bRet[2]	= HistogramCompute_Red(histogram[2],average[2],deviation[2]);
			bRet[1]	= HistogramCompute_Green(histogram[1],average[1],deviation[1]);
			bRet[0]	= HistogramCompute_Blue(histogram[0],average[0],deviation[0]);
		}

		return (bRet[3] && bRet[2] && bRet[1] && bRet[0]);
	}

	// 图象直方图均衡
	BOOL KDib::HistogramAverage(BOOL bRed /*= TRUE*/,BOOL bGreen /*= TRUE*/,BOOL bBlue /*= TRUE*/)
	{
		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		if(wBitCount==24 && !bRed && !bGreen && !bBlue)
		{// 彩色图像必须均衡一个通道
			return FALSE;
		}

		if(wBitCount==8)
		{
			return HistogramAverage_Gray();
		}

		BOOL br(TRUE),bg(TRUE),bb(TRUE);
		if(wBitCount==24)
		{
			if(bRed)	br = HistogramAverage_Red();
			if(bGreen)	bg = HistogramAverage_Green();
			if(bBlue)	bb = HistogramAverage_Blue();
		}

		return (br && bg && bb);
	}

	//---------------------------------------------------------
	// 名    称: Smooth_Neighbour
	// 访    问: public
	// 功    能: 图像平滑 - 邻域平均法
	// 参    数:
	//			 [i] iDimOfTemplate - 模板维数,默认 = 3 (3 5 7 9)
	//			 [i] byFill - 边缘的填充值,默认 = 0
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 默认模板如下所示
	//
	//			--       --
	//			| 1  1  1 |
	//			| 1  1  1 | * 1 / 9
	//			| 1  1  1 |
	//			--       --
	//
	//---------------------------------------------------------
	BOOL KDib::Smooth_Neighbour(int iDimOfTemplate /*=3*/,BYTE byFill /*= 0*/)
	{
		DWORD i,j;

		KDib* pDib = this;

		if(!pDib || pDib->IsEmpty())
		{
			return FALSE;
		}

		if(iDimOfTemplate!=3 && iDimOfTemplate!=5 && iDimOfTemplate!=7 && iDimOfTemplate!=9)
		{
			return FALSE;
		}

		// 构造模板
		DWORD dwSizeTmplatte = iDimOfTemplate * iDimOfTemplate;
		double* pTemplate = new double[dwSizeTmplatte];
		for(i=0;i<dwSizeTmplatte;i++)
		{
			*(pTemplate+i) = 1.0;
		}

		int w = iDimOfTemplate;			// 模板宽度
		int h = iDimOfTemplate;			// 模板高度

		int x0 = iDimOfTemplate / 2;	// 模板中心元素 X 坐标
		int y0 = iDimOfTemplate / 2;	// 模板中心元素 Y 坐标

		// 模板系数
		double coef = 1.0 / (dwSizeTmplatte);

		// 颜色位数
		WORD wBitCount = pDib->BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = pDib->Width();
		DWORD dwHeight = pDib->Height();

		//// 数据通道数
		int ch = wBitCount / 8;

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*ch);

		// 单通道每行像素的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth*8);

		// 图像数据指针
		LPBYTE lpDIBits = pDib->DIBits();

		//
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel*dwHeight];

		for(int k=0;k<ch;k++)
		{
			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine + j*ch + k);
				}
			}

			//Smooth_Neighbour_SingleChannel(lpBuf,dwWidth,dwHeight,pTemplate,w,h,x0,y0,coef,byFill);
			Smooth_Template_SingleChannel(lpBuf,dwWidth,dwHeight,pTemplate,w,h,x0,y0,coef,byFill);

			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpDIBits + i*dwBytesPerLine + j*ch + k) = *(lpBuf + i*dwBytesPerLineSingleChannel + j);
				}
			}
		}

		_deletea(lpBuf);
		_deletea(pTemplate);

		return TRUE;

	}

	//---------------------------------------------------------
	// 名    称: Smooth_power
	// 访    问: public
	// 功    能: 图像平滑 - 加权平均法
	// 参    数:
	//			 [i] iTemplate - 模板维数,默认 = 5 (5 10 16 48)
	//			 [i] byFill - 边缘的填充值,默认 = 0
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	//
	// 注    释: 默认模板如下所示
	//
	//	iTemplate = 5
	//			--       --
	//			| 0  1  0 |
	//			| 1  1  1 | * 1 / 5
	//			| 0  1  0 |
	//			--       --
	//	iTemplate = 10
	//			--       --
	//			| 1  1  1 |
	//			| 1  2  1 | * 1 / 10
	//			| 1  1  1 |
	//			--       --
	//	iTemplate = 16
	//			--       --
	//			| 1  2  1 |
	//			| 2  4  2 | * 1 / 16
	//			| 1  2  1 |
	//			--       --
	//	iTemplate = 48
	//			--         --
	//			| 0 1 2 1 0 |
	//			| 1 2 4 2 1 |
	//			| 2 4 8 4 2 | * 1 / 48
	//			| 1 2 4 2 1 |
	//			| 0 1 2 1 0 |
	//			--         --
	//
	//---------------------------------------------------------
	BOOL KDib::Smooth_Power(int iTemplate /*= 16*/,BYTE byFill /*= 0*/)
	{
		DWORD i,j;

		KDib* pDib = this;

		// 构造模板
		double* pTemplate = NULL;
		int w,h;
		int x0,y0;
		double coef(0);

		if(iTemplate==5)
		{
			w = 3; h = 3;
			x0 = w/2; y0 = h/2;
			pTemplate = new double[w*h];
			coef = 1.0 / iTemplate;

			*(pTemplate + 0) = 0;
			*(pTemplate + 1) = 1;
			*(pTemplate + 2) = 0;
			*(pTemplate + 3) = 1;
			*(pTemplate + 4) = 1;
			*(pTemplate + 5) = 1;
			*(pTemplate + 6) = 0;
			*(pTemplate + 7) = 1;
			*(pTemplate + 8) = 0;
		}
		else if(iTemplate==10)
		{
			w = 3; h = 3;
			x0 = w/2; y0 = h/2;
			pTemplate = new double[w*h];
			coef = 1.0 / iTemplate;

			*(pTemplate + 0) = 1;
			*(pTemplate + 1) = 1;
			*(pTemplate + 2) = 1;
			*(pTemplate + 3) = 1;
			*(pTemplate + 4) = 2;
			*(pTemplate + 5) = 1;
			*(pTemplate + 6) = 1;
			*(pTemplate + 7) = 1;
			*(pTemplate + 8) = 1;
		}
		else if(iTemplate==16)
		{
			w = 3; h = 3;
			x0 = w/2; y0 = h/2;
			pTemplate = new double[w*h];
			coef = 1.0 / iTemplate;

			*(pTemplate + 0) = 1;
			*(pTemplate + 1) = 2;
			*(pTemplate + 2) = 1;
			*(pTemplate + 3) = 2;
			*(pTemplate + 4) = 4;
			*(pTemplate + 5) = 2;
			*(pTemplate + 6) = 1;
			*(pTemplate + 7) = 2;
			*(pTemplate + 8) = 1;
		}
		else if(iTemplate==48)
		{
			w = 5; h = 5;
			x0 = w/2; y0 = h/2;
			pTemplate = new double[w*h];
			coef = 1.0 / iTemplate;

			*(pTemplate + 0) = 0;
			*(pTemplate + 1) = 1;
			*(pTemplate + 2) = 2;
			*(pTemplate + 3) = 1;
			*(pTemplate + 4) = 0;

			*(pTemplate + 5) = 1;
			*(pTemplate + 6) = 2;
			*(pTemplate + 7) = 4;
			*(pTemplate + 8) = 2;
			*(pTemplate + 9) = 1;

			*(pTemplate + 10) = 2;
			*(pTemplate + 11) = 4;
			*(pTemplate + 12) = 8;
			*(pTemplate + 13) = 4;
			*(pTemplate + 14) = 2;

			*(pTemplate + 15) = 1;
			*(pTemplate + 16) = 2;
			*(pTemplate + 17) = 4;
			*(pTemplate + 18) = 2;
			*(pTemplate + 19) = 1;

			*(pTemplate + 20) = 0;
			*(pTemplate + 21) = 1;
			*(pTemplate + 22) = 2;
			*(pTemplate + 23) = 1;
			*(pTemplate + 24) = 0;
		}
		else
		{
			return FALSE;
		}

		// 颜色位数
		WORD wBitCount = pDib->BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = pDib->Width();
		DWORD dwHeight = pDib->Height();

		//// 数据通道数
		int ch = wBitCount / 8;

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*ch);

		// 单通道每行像素的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth*8);

		// 图像数据指针
		LPBYTE lpDIBits = pDib->DIBits();

		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel*dwHeight];

		for(int k=0;k<ch;k++)
		{
			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine + j*ch + k);
				}
			}

			//Smooth_Power_SingleChannel(lpBuf,dwWidth,dwHeight,pTemplate,w,h,x0,y0,coef,byFill);
			Smooth_Template_SingleChannel(lpBuf,dwWidth,dwHeight,pTemplate,w,h,x0,y0,coef,byFill);

			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpDIBits + i*dwBytesPerLine + j*ch + k) = *(lpBuf + i*dwBytesPerLineSingleChannel + j);
				}
			}
		}

		_deletea(lpBuf);
		_deletea(pTemplate);

		return TRUE;
	}

	// 图像平滑 - 自适应局部平滑
	BOOL KDib::Smooth_Auto(BYTE byFill /*= 0*/)
	{
		DWORD i,j;

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 数据通道数
		int ch = wBitCount / 8;

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*ch);

		// 单通道每行像素的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth*8);

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();
		//
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel*dwHeight];

		for(int k=0;k<ch;k++)
		{
			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine + j*ch + k);
				}
			}

			Smooth_Auto_SingleChannel(lpBuf,dwWidth,dwHeight,byFill);

			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpDIBits + i*dwBytesPerLine + j*ch + k) = *(lpBuf + i*dwBytesPerLineSingleChannel + j);
				}
			}
		}

		_deletea(lpBuf);

		return TRUE;
	}

	// 图像平滑 - 中值滤波
	BOOL KDib::Smooth_MedianFilter(int iTemplate /*= 3*/,BYTE byFill /*= 0*/)
	{
		DWORD i,j;

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		if(iTemplate<3)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		int w = iTemplate;
		int h = iTemplate;

		int x0 = w/2;
		int y0 = h/2;

		// 数据通道数
		int ch = wBitCount / 8;

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*ch);

		// 单通道每行像素的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth*8);

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		//
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel*dwHeight];

		for(int k=0;k<ch;k++)
		{
			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine + j*ch + k);
				}
			}

			Smooth_MedianFilter_SingleChannel(lpBuf,dwWidth,dwHeight,w,h,x0,y0,byFill);

			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpDIBits + i*dwBytesPerLine + j*ch + k) = *(lpBuf + i*dwBytesPerLineSingleChannel + j);
				}
			}
		}

		_deletea(lpBuf);

		return TRUE;
	}

	// 锐化 - 梯度锐化
	BOOL KDib::Sharp_Grad(BYTE byThreshold /*= 0*/)
	{
		DWORD i,j;

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 数据通道数
		int ch = wBitCount / 8;

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*ch);

		// 单通道每行像素的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth*8);

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		//
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel*dwHeight];

		for(int k=0;k<ch;k++)
		{
			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine + j*ch + k);
				}
			}

			Sharp_Grad_SingleChanne(lpBuf,dwWidth,dwHeight,byThreshold);

			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpDIBits + i*dwBytesPerLine + j*ch + k) = *(lpBuf + i*dwBytesPerLineSingleChannel + j);
				}
			}
		}

		_deletea(lpBuf);

		return TRUE;
	}

	// 图像锐化 - 拉普拉斯模板锐化
	//
	//	pTemplate[0]                                                        
	//
	//	| -1 -1 -1 |                                                      
	//	| -1  9 -1 | * 1 / 1                                                           
	//	| -1 -1 -1 |                                                
	//
	//	pTemplate[1]            pTemplate[2]                                                           
	//
	//	| 0  1  0 |           | 1  1  1 |                                                           
	//	| 1 -4  1 | * 1 / 1   | 1 -8  1 | * 1 / 1                                                                
	//	| 0  1  0 |           | 1  1  1 |                                                     
	//
	//	pTemplate[3]            pTemplate[4]                                                           
	//
	//	| 0 -1  0 |           |-1  1 -1 |                                                           
	//	|-1  4 -1 | * 1 / 1   | 1  8  1 | * 1 / 8                                                          
	//	| 0 -1  0 |           |-1  1 -1 |                                                           
	//
	BOOL KDib::Sharp_LapTemplate(int iNumberOfTemplate /*= 0*/,BYTE byFill /*= 0*/)
	{
		//构造模板0
		int w = 3;
		int h = 3;
		int x0 = w/2;
		int y0 = h/2;
		double coef = 1.0;

		double pTemplate[9];
		for(int k=0;k<w*h;k++)
		{
			pTemplate[k] = -1;
		}
		pTemplate[w*y0 + x0] = 9;

		DWORD i,j;

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 数据通道数
		int ch = wBitCount / 8;

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*ch);

		// 单通道每行像素的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth*8);

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		//
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel*dwHeight];

		for(int k=0;k<ch;k++)
		{
			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine + j*ch + k);
				}
			}

			Sharp_LapTemplate_SingleChanne(lpBuf,dwWidth,dwHeight,pTemplate,w,h,x0,y0,coef,byFill);

			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpDIBits + i*dwBytesPerLine + j*ch + k) = *(lpBuf + i*dwBytesPerLineSingleChannel + j);
				}
			}
		}

		_deletea(lpBuf);

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: AdjustColorRgb
	// 访    问: public
	// 功    能: 调整DIB颜色 - RGB
	// 参    数:
	//			 [i] dRScale - 红色分量缩放比例[-1,1]
	//			 [i] dGScale - 绿色分量缩放比例[-1,1]
	//			 [i] dBScale - 蓝色分量缩放比例[-1,1]
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::AdjustColorRgb(double dRScale, double dGScale, double dBScale)
	{
		DWORD i,j;

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=24)
		{
			return FALSE;
		}

		// 图像数据尺寸
		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 通道数量
		DWORD dwChannel = wBitCount / 8;

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		// 多通道每行像素占用的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * dwChannel * 8);

		// 单通道每行像素占用的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// 临时缓冲区(一个通道)
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel * dwHeight];

		for(int k=0;k<3;k++)	// BGRA -> 0123
		{
			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine +j*3 + k);
				}
			}

			double scale = 1.0;

			if(k==0)		// B
			{
				scale += dBScale;
			}
			else if(k==1)	// G
			{
				scale += dGScale;
			}
			else if(k==2)	// R
			{
				scale += dRScale;
			}
			else			// A
			{
				// 
			}

			//if(scale>1) scale = 1;
			//if(scale<0) scale = 0;

			// 处理单通道数据
			AdjustColorRgb_SingleChannel(lpBuf,dwWidth,dwHeight,scale);

			// 回填
			for(i=0;i<dwHeight;i++)
			{
				for(j=0;j<dwWidth;j++)
				{
					*(lpDIBits + i*dwBytesPerLine +j*3 + k) = *(lpBuf + i*dwBytesPerLineSingleChannel + j);
				}
			}

		}

		delete []lpBuf; lpBuf = NULL;

		return TRUE;
	}
	BOOL KDib::AdjustColorCmy(){return FALSE;}
	BOOL KDib::AdjustColorHsi(){return FALSE;}
	BOOL KDib::AdjustColorHls(){return FALSE;}


	///============================================================================
	///  _protected                         下面是保护成员函数
	///============================================================================

	//---------------------------------------------------------
	// 名    称: DIB2DDB
	// 访    问: protected
	// 功    能: DIB 转换为 DDB
	// 参    数:
	// 返    回: HBITMAP 句柄
	// 注    释: 
	//---------------------------------------------------------
	HBITMAP KDib::DIB2DDB()
	{
		if (m_dwSizeImage == 0) return NULL;	//没有数据

		HDC hDC = ::GetDC(NULL);

		HBITMAP hBitmap = NULL;
		try
		{
			hBitmap = ::CreateDIBitmap(hDC, 
									   m_lpBitmapInfoHeader,
									   CBM_INIT,
									   m_lpImageData,
									   (LPBITMAPINFO) m_lpBitmapInfoHeader,
									   DIB_RGB_COLORS);
		}
		catch(...)
		{
			::DeleteObject(hBitmap);
			hBitmap = NULL;
		}

		::ReleaseDC(NULL,hDC);

		return hBitmap;
	}

	//---------------------------------------------------------
	// 名    称: DDB2DIB
	// 访    问: public
	// 功    能: DDB 转换为 DIB
	// 参    数:
	//			 [i] hBitmap - 位图句柄
	//			 [i] hPalette - 调色板句柄
	//			 [i] dwCompression - 压缩方式
	// 返    回: HBITMAP 句柄
	// 注    释: 
	//---------------------------------------------------------
	HANDLE KDib::DDB2DIB(HBITMAP hBitmap,HPALETTE hPalette /*= NULL*/,DWORD dwCompression /*= BI_RGB*/)
	{
		if(hBitmap == NULL || dwCompression == BI_BITFIELDS)
			return NULL;

		Empty();

		// 取得桌面 DC
		HDC hDC=::GetDC(NULL);

		// 调色板
		HPALETTE hOldPal = NULL;
		HPALETTE hPal = hPalette;
		if(hPal==NULL)
		{
			//hPal=(HPALETTE)::GetStockObject(DEFAULT_PALETTE);
			hPal = ::CreateHalftonePalette(hDC);	//半色调色板
		}

		// 应用调色板
		hOldPal=::SelectPalette(hDC,hPal,FALSE);
		UINT uEntries = ::RealizePalette(hDC);	//返回调色板入口数量

		//DDB - 取得 HBITMAP 信息
		BITMAP bm;
		::GetObject(hBitmap,sizeof(BITMAP),(LPSTR)&bm);

		//DIB - 初始化 BitmapInfoHead
		BITMAPINFOHEADER bi;

		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = bm.bmWidth;
		bi.biHeight = bm.bmHeight;
		bi.biPlanes = 1;
		bi.biBitCount = bm.bmPlanes*bm.bmBitsPixel;
		bi.biCompression = dwCompression;			//BI_RGB
		bi.biSizeImage = 0;							//由GetDIBits()计算
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;

		// 计算颜色表入口数
		int nColorEntrys = (bi.biBitCount > 8) ? 0 : (1 << bi.biBitCount);

		// 计算 DIB 头尺寸
		DWORD dwDIBHeaderSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColorEntrys;

		// 分配 DIB 头内存,不包含数据
		PBYTE pByte = new BYTE[dwDIBHeaderSize];
		if(!pByte)
		{
			::SelectPalette(hDC,hOldPal,FALSE);
			::ReleaseDC(NULL,hDC);
			return NULL;
		}

		LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)pByte;
		*lpbi = bi;

		// 调用 GetDIBits() 函数
		// 如果 lpBits = NULL, GetDIBits() 会自动计算 biSizeImage
		UINT uUsage = DIB_PAL_COLORS;
		::GetDIBits(hDC,hBitmap,
					0L,						// first scan line to set in destination bitmap
					(DWORD)bi.biHeight,		// number of scan lines to copy
					(LPBYTE)NULL,			// address of array for bitmap bits
											// If this parameter is NULL, the function passes
											// the dimensions and format of the bitmap to 
											// the BITMAPINFO structure pointed to by the lpbi parameter
					(LPBITMAPINFO)&bi,		// specifies the desired format for the device-independent bitmap (DIB) data.
					(DWORD)uUsage);			// DIB_PAL_COLORS / DIB_RGB_COLORS

		// fill the biSizeImage field 得到biSizeImage
		bi = *lpbi;

		//If the driver did not fill in the biSizeImage field,then compute it
		//Each scan line of the images is aligned on a DWORD(32bits)boundary
		if(bi.biSizeImage==0)
		{
			//bi.biSizeImage = ((((bi.biWidth*bi.biBitCount)+31)&~31)/8)*bi.biHeight;
			bi.biSizeImage = DIBWIDTHBYTES( bi.biWidth * bi.biBitCount ) * bi.biHeight;

			//if a compression scheme is used the result may infact be lager
			//Increase the size to account for this
			if(dwCompression != BI_RGB)
			{
				bi.biSizeImage = (bi.biSizeImage * 3) / 2;
			}
		}

		// DIB 尺寸 = DIB头 + DIB数据
		DWORD dwDIBSize = dwDIBHeaderSize + bi.biSizeImage;

		// 分配 DIB 内存
		PBYTE pDib = new BYTE[dwDIBSize];
		if(!pDib)
		{
			::SelectPalette(hDC,hOldPal,FALSE);
			::ReleaseDC(NULL,hDC);

			delete []pByte;
			pByte = NULL;

			return NULL;
		}
		memcpy(pDib,pByte,dwDIBHeaderSize);

		// 回收内存
		delete []pByte;
		pByte = NULL;

		//Get the bitmap bits
		lpbi = (LPBITMAPINFOHEADER)pDib;
		BOOL bGotBits=::GetDIBits(hDC,hBitmap,
								0L,								// Start scan line
								(DWORD)bi.biHeight,				// end of scan line
								(LPBYTE)lpbi + dwDIBHeaderSize,	// adress of bitmap bits
								(LPBITMAPINFO)lpbi,				// adress of bitmapinfo
								(DWORD)uUsage);					// RGB or palette index

		if(!bGotBits)
		{
			::SelectPalette(hDC,hOldPal,FALSE);
			::ReleaseDC(NULL,hDC);

			delete []pDib;
			pDib = NULL;

			return NULL;
		}

		::SelectPalette(hDC,hPal,FALSE);

		::ReleaseDC(NULL,hDC);

		AttachMemory(pDib,TRUE);	// TRUE 表示析构函数回收内存 pDib

		return (HANDLE)pDib;
	}

	//---------------------------------------------------------
	// 名    称: DDB2DIB
	// 访    问: public
	// 功    能: DDB 转换为 DIB
	// 参    数:
	//			 [i] hBitmap - 位图句柄
	//			 [i] bForce256 - 强制转换为256色位图
	//			 [i] pbmData - 数据指针
	// 返    回: HBITMAP 句柄
	// 注    释: 
	//---------------------------------------------------------
	HANDLE KDib::DDB2DIB(HBITMAP hBitmap,BOOL bForce256,int *pbmData)
	{
		BITMAP bm;
		::GetObject(hBitmap,sizeof(BITMAP),&bm);
		WORD nBitsPixel = bm.bmBitsPixel;	//1 4 8 16 24 32

		BITMAPINFOHEADER bi;
		memset(&bi, 0, sizeof(bi));
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = bm.bmWidth;
		bi.biHeight = bm.bmHeight;
		bi.biPlanes = 1;
		bi.biCompression = BI_RGB;

		// get number of bits required per pixel
		int bits = bm.bmPlanes * bm.bmBitsPixel;

		if(bForce256)
		{
			bi.biBitCount = 8;
		}
		else
		{
			if (bits <= 1)
				bi.biBitCount = 1;
			else if (bits <= 4)
				bi.biBitCount = 4;
			else if (bits <= 8)
				bi.biBitCount = 8;
			else
				bi.biBitCount = 24;
		}

		// calculate color table size
		int biColorSize = 0;
		if (bi.biBitCount!=24)
			biColorSize = (1<<bi.biBitCount);

		biColorSize *= sizeof(RGBQUAD);

		// calculate picture data size
		bi.biSizeImage = (DWORD)bm.bmWidth * bi.biBitCount;		//bits per row
		bi.biSizeImage = (((bi.biSizeImage) + 31) / 32) * 4;	//DWORD aligned
		bi.biSizeImage *= bm.bmHeight;							//bytes required for whole bitmap

		// return size to caler in case they want to save to file
		if (pbmData)
			*pbmData = bi.biSize + biColorSize;

		// 分配内存 header + color table + picture data
		PBYTE pDib = new BYTE[bi.biSize + biColorSize + bi.biSizeImage];

		// get a memory pointer to this hunk by locking it
		LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)pDib;
		LPBITMAPINFOHEADER lptmp = (LPBITMAPINFOHEADER)pDib;

		// copy our header structure into hunk
		*lpbi = bi;

		// get a device context and select our bitmap's palette into it
		HDC hDC = ::GetDC(NULL);

		HPALETTE hPal = m_hPalette;
		if(hPal==NULL)
		{
			//hPal=(HPALETTE)::GetStockObject(DEFAULT_PALETTE);
			hPal = ::CreateHalftonePalette(hDC);
		}

		HPALETTE hOldPal=::SelectPalette(hDC,hPal,FALSE);
		UINT uEntries = ::RealizePalette(hDC);

		UINT uUsage = DIB_PAL_COLORS;
		// load our memory hunk with the color table and picture data
		int iGotBits = ::GetDIBits(hDC, (HBITMAP)hBitmap, 0, (UINT)bi.biHeight, (LPSTR)lpbi +
				(WORD)lpbi->biSize + biColorSize, (LPBITMAPINFO)lpbi,
				DIB_RGB_COLORS);

		::SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);

		AttachMemory(pDib,TRUE);

		return pDib;
	}


	//---------------------------------------------------------
	// 名    称: AttachMapFile
	// 访    问: protected
	// 功    能: 关联图像文件
	// 参    数:
	//			 [i] lpszPathName - 图像文件
	//			 [i] bShare - 是否共享打开,默认为FALSE
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 利用内存映射文件
	//---------------------------------------------------------
	BOOL KDib::AttachMapFile(LPCTSTR lpszPathName)
	{
		// 先分离内存映射文件
		DetachMapFile();
		//--------------------------------------------
		// 分3个步骤映射一个内存映像文件
		// 1. 创建或打开一个文件内核对象,该对象表示了要用作内存映射文件的磁盘文件
		HANDLE hFile = ::CreateFile(lpszPathName,					// FileName
									GENERIC_WRITE | GENERIC_READ,	// Desire Access
									//bShare ? FILE_SHARE_READ : 0,	// Share mode
									0,	// Share mode
									NULL,							// SECURITY_ATTRIBUTES Point
									OPEN_EXISTING,					// How to create
									FILE_ATTRIBUTE_NORMAL,			// File attributes
									NULL);							// Specifies a handle with GENERIC_READ access to a template file

		if(hFile == INVALID_HANDLE_VALUE) return FALSE;

		// 2. 创建一个文件映射内核对象来告诉系统该文件的大小以及如何访问
		HANDLE hMap = ::CreateFileMapping(hFile,			// 要映射的文件句柄
										  NULL,				// 安全属性
										  PAGE_READWRITE,	// 对文件映像的保护
										  0,				// high-order 32 bits of object size
										  0,				// low-order 32 bits of object size
										  NULL);			// if NULL, the mapping object is created without a name

		if(hMap == NULL) return FALSE;

		// 3. 映射整个文件到调用进程的地址空间
		LPVOID lpvMapFile = ::MapViewOfFile(hMap,			// file-mapping object 
											FILE_MAP_WRITE,	// access mode 
											0,				// high-order 32 bits of object size
											0,				// low-order 32 bits of object size
											0);				// number of bytes to map,if zero, the entire file is mapped

		if(lpvMapFile == NULL) return FALSE;
		//--------------------------------------------
		if(((LPBITMAPFILEHEADER)lpvMapFile)->bfType != BMP_HEADER_MARKER)
		{// 不是BMP文件
			DetachMapFile();
			return FALSE;
		}
		//--------------------------------------------
		// 使用内存映射文件不用释放内存
		if(!AttachMemory((LPBYTE) lpvMapFile + sizeof(BITMAPFILEHEADER),FALSE))
		{// 跳过 BITMAPFILEHEADER, 关联 DIB 数据
			return FALSE;
		}
		//--------------------------------------------
		m_hFile = hFile;			// 文件句柄
		m_hMap = hMap;				// 内存映射文件句柄
		m_lpvMapFile = lpvMapFile;	// 内存映射文件指针

		m_bEmpty = FALSE;			// 非空
		//--------------------------------------------
		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: DetachFile
	// 访    问: protected
	// 功    能: 分离内存映射文件
	// 参    数:
	// 返    回: 
	// 注    释: 
	//---------------------------------------------------------
	void KDib::DetachMapFile()
	{
		if(m_hFile == NULL) return;
		//--------------------------------------------
		// 分离内存映射文件分 3 步骤
		// 1. 取消对文件映射内核对象的映射
		::UnmapViewOfFile(m_lpvMapFile);

		// 2. 关闭文件映射内核对象
		::CloseHandle(m_hMap);

		// 3. 关闭文件内核对象
		::CloseHandle(m_hFile);
		//--------------------------------------------
		m_hFile = NULL;
	}

	//---------------------------------------------------------
	// 名    称: ComputeMetrics
	// 访    问: protected
	// 功    能: 计算DIB数据尺寸
	// 参    数:
	//			 [i] lpBitmapInfoHeader - LPBITMAPINFOHEADER
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	//
	// 注    释: 计算 
	//			 m_dwWidth - 图像宽度
	//			 m_dwHeight - 图像高度
	//			 m_nBitCount - 图像位数
	//			 m_dwSizeImage - DIB 数据字节数
	//---------------------------------------------------------
	BOOL KDib::ComputeMetrics(LPBITMAPINFOHEADER lpBitmapInfoHeader)
	{
		if(lpBitmapInfoHeader->biSize != sizeof(BITMAPINFOHEADER))
		{// 不是 BITMAPINFOHEADER 结构
			return FALSE;
		}

		// 图像宽度和高度
		m_dwWidth = (DWORD)lpBitmapInfoHeader->biWidth;
		m_dwHeight = (DWORD)lpBitmapInfoHeader->biHeight;

		// 图像位数
		m_nBitCount = lpBitmapInfoHeader->biBitCount;

		// 每行数据字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(m_dwWidth * m_nBitCount);
		DWORD dwSizeImage = dwBytesPerLine * m_dwHeight;

		// DIB 数据字节数
		m_dwSizeImage = lpBitmapInfoHeader->biSizeImage;
		if(m_dwSizeImage==0)
		{// 计算 DIB 数据字节数
			lpBitmapInfoHeader->biSizeImage = dwSizeImage;
			m_dwSizeImage = dwSizeImage;
		}

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: ComputePaletteSize
	// 访    问: protected
	// 功    能: 计算调色板尺寸(颜色表入口数)
	// 参    数:
	//			 [i] iRChannel - 红色通道
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::ComputePaletteSize(LPBITMAPINFOHEADER lpBitmapInfoHeader)
	{
		if(lpBitmapInfoHeader->biClrUsed == 0)
		{
			switch(lpBitmapInfoHeader->biBitCount)	//1<<nBitCount
			{
			case 1:		// 1 bpp
				m_dwColorTableEntries = 2;
				break;

			case 4:		// 4 bpp
				m_dwColorTableEntries = 16;
				break;

			case 8:		// 8 bpp
				m_dwColorTableEntries = 256;
				break;

			case 24:	// 24bpp
				m_dwColorTableEntries = 0;
				break;

			case 16:	// 16 bpp
			case 32:	// 32 bpp
				if (lpBitmapInfoHeader->biCompression == BI_BITFIELDS)
					m_dwColorTableEntries = 3; // 16 or 32 bpp have 3 colors(masks) in the color table if bitfield compression
				else
					m_dwColorTableEntries = 0; // 16 or 32 bpp have no color table if no bitfield compression
				break;

			default:
				return FALSE;
			}
		}
		else
		{
			m_dwColorTableEntries = lpBitmapInfoHeader->biClrUsed;
		}

		return ((m_dwColorTableEntries >= 0) && (m_dwColorTableEntries <= 256));
	}

	//---------------------------------------------------------
	// 名    称: MakePalette
	// 访    问: protected
	// 功    能: 由 DIB 颜色表生成调色板
	// 参    数:
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//---------------------------------------------------------
	HPALETTE KDib::MakePalette()
	{
		if(m_hPalette != NULL)
		{
			::DeleteObject(m_hPalette);
			m_hPalette = NULL;
		}

		// 没有调色板
		if(m_dwColorTableEntries == 0) return NULL;

		// 颜色表指针
		LPRGBQUAD pDibQuad = (LPRGBQUAD)m_lpvColorTable;

		// 逻辑调色板
		LPLOGPALETTE pLogPal = NULL;
		pLogPal = (LPLOGPALETTE) new char[sizeof(WORD) * 2 + sizeof(PALETTEENTRY) * m_dwColorTableEntries];
		pLogPal->palVersion = PALVERSION; // 0x300
		pLogPal->palNumEntries = (WORD)m_dwColorTableEntries;
		for(int i=0; i<(int)m_dwColorTableEntries; i++)
		{
			pLogPal->palPalEntry[i].peRed	= pDibQuad->rgbRed;
			pLogPal->palPalEntry[i].peGreen = pDibQuad->rgbGreen;
			pLogPal->palPalEntry[i].peBlue	= pDibQuad->rgbBlue;
			pLogPal->palPalEntry[i].peFlags = 0;

			pDibQuad++;
		}
		//生成调色板
		m_hPalette = ::CreatePalette(pLogPal);

		delete []pLogPal;

		return m_hPalette;
	}

	//---------------------------------------------------------
	// 名    称: LinearStrech_SingleChannel
	// 访    问: protected
	// 功    能: 单通道分段灰度线形拉伸
	// 参    数:
	//			 [i] lpDIBits - 
	//			 [i] dwWidth - 
	//			 [i] dwHeight - 
	//			 [i] dwHeight - 
	//			 [i] x[2] - 
	//			 [i] y[2] - 
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::LinearStrech_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,BYTE x[2],BYTE y[2])
	{
		DWORD i,j;

		if(lpDIBits==NULL || dwWidth==0	|| dwHeight==0) 
			return FALSE;

		// x 不能为 0 和 255 并且 x[0] != x[1]
		if(x[0]>=255 || x[1]>=255 || x[0]==0 || x[1]==0 || x[0]==x[1])
			return FALSE;

		if(y[0]>255 || y[1]>255)
			return FALSE;

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);

		// (x1,y1) 和 (x2,y2) 为两个分段点坐标
		double x1 = x[0];
		double y1 = y[0];

		double x2 = x[1];
		double y2 = y[1];

		// 三个分段的直线斜率
		double slop1 = y1/x1;
		double slop2 = (y2-y1)/(x2-x1);
		double slop3 = (255.0-y2)/(255.0-x2);

		// 三个直线坐标的斜率
		double dis1 = 0;
		double dis2 = y1 - slop2 * x1;
		double dis3 = y2 - slop3 * x2;

		// 映射表
		BYTE map[256];
		for(i=0;i<256;i++)
		{
			if(i<x1)
				map[i] = (BYTE)(WORD)(slop1*i + dis1 + 0.5);
			else if(i<x2)
				map[i] = (BYTE)(WORD)(slop2*i + dis2 + 0.5);
			else
				map[i] = (BYTE)(WORD)(slop3*i + dis3 + 0.5);
		}

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwBytesPerLine;j++)
			{
				BYTE byGray = *(lpDIBits + i*dwBytesPerLine + j);
				*(lpDIBits + i*dwBytesPerLine + j) = map[byGray];
			}
		}

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: LogarithmStrech_SingleChannel
	// 访    问: protected
	// 功    能: 单通道对数变换
	// 参    数:
	//			 [i] lpDIBits - 
	//			 [i] dwWidth - 
	//			 [i] dwHeight - 
	//			 [i] dwHeight - 
	//			 [i] x[2] - 
	//			 [i] y[2] - 
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//			 y = a + ln(x+1)/(b*lnc)
	//			 b!=0 c!=1
	//---------------------------------------------------------
	BOOL KDib::LogarithmStrech_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double a,double b,double c)
	{
		DWORD i,j;

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwBytesPerLine;j++)
			{
				// 变换灰度
				double tmp1 = log((double)(*(lpDIBits + i*dwBytesPerLine + j))) + 1.0;
				double tmp2 = a + tmp1 / (b*log(c)) + 0.5;

				if(tmp2>255)
				{
					*(lpDIBits + i*dwBytesPerLine + j) = 255;
				}
				else if(tmp2<0)
				{
					*(lpDIBits + i*dwBytesPerLine + j) = 0;
				}
				else
				{
					*(lpDIBits + i*dwBytesPerLine + j) = (BYTE)(WORD)tmp2;
				}
			}
		}

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: ExponentStrech_SingleChannel
	// 访    问: protected
	// 功    能: 单通道指数拉伸
	// 参    数:
	//			 [i] lpDIBits - 
	//			 [i] dwWidth - 
	//			 [i] dwHeight - 
	//			 [i] dwHeight - 
	//			 [i] x[2] - 
	//			 [i] y[2] - 
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//			 y = b^[c*(x-a)] - 1
	//---------------------------------------------------------
	BOOL KDib::ExponentStrech_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double a,double b,double c)
	{
		DWORD i,j;

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwBytesPerLine;j++)
			{
				// 变换灰度
				double tmp1 = ((double)(*(lpDIBits + i*dwBytesPerLine + j)) - a) * c;
				double tmp2 = pow(b,tmp1) - 1.0;

				if(tmp2>255)
				{
					*(lpDIBits + i*dwBytesPerLine + j) = 255;
				}
				else if(tmp2<0)
				{
					*(lpDIBits + i*dwBytesPerLine + j) = 0;
				}
				else
				{
					*(lpDIBits + i*dwBytesPerLine + j) = (BYTE)(WORD)tmp2;
				}
			}
		}

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: HistogramCompute_SingleChannel
	// 访    问: protected
	// 功    能: 图象单通道直方图统计
	// 参    数:
	//			 [i] lpDIBits - 
	//			 [i] dwWidth - 
	//			 [i] dwHeight - 
	//			 [i] dwHeight - 
	//			 [i] x[2] - 
	//			 [i] y[2] - 
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::HistogramCompute_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,int histogram[256],double& average ,double& deviation)
	{
		DWORD i,j;

		// 直方图数组清零
		for(i=0;i<256;i++)
		{
			histogram[i] = 0;
		}

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8);

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				histogram[*(lpDIBits + i*dwBytesPerLine + j)]++;
			}
		}

		// 平均值
		average = 0;
		for(i=0;i<256;i++)
		{
			average += i*histogram[i];
		}
		average /= dwWidth*dwHeight;

		// 方差
		deviation = 0;
		for(i=0;i<256;i++)
		{
			deviation += (i-average) * (i-average)*histogram[i];
		}
		deviation /= dwWidth*dwHeight;
		deviation = sqrt(deviation);

		return TRUE;
	}

	// 图象灰度直方图统计
	BOOL KDib::HistogramCompute_Gray(int histogram[256],double& average ,double& deviation)
	{
		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=8)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		return HistogramCompute_SingleChannel(lpDIBits,dwWidth,dwHeight,histogram,average,deviation);
	}

	// 图象亮度直方图统计
	BOOL KDib::HistogramCompute_Bright(int histogram[256],double& average ,double& deviation)
	{
		DWORD i,j;

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		// 多通道每行像素占用的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// 单通道每行像素占用的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// 临时缓冲区
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel * dwHeight];

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				*(lpBuf + i*dwBytesPerLineSingleChannel + j) =
					(BYTE)(WORD)( 
								  (*(lpDIBits + i*dwBytesPerLine+j*3 + 0) * 0.114) +	// B
								  (*(lpDIBits + i*dwBytesPerLine+j*3 + 1) * 0.587) +	// G
								  (*(lpDIBits + i*dwBytesPerLine+j*3 + 2) * 0.299) +	// R
								   0.5
								 );
			}
		}

		BOOL bCompute = HistogramCompute_SingleChannel(lpBuf,dwWidth,dwHeight,histogram,average,deviation);

		delete []lpBuf; lpBuf = NULL;

		return bCompute;
	}

	// 图象红色直方图统计
	BOOL KDib::HistogramCompute_Red(int histogram[256],double& average ,double& deviation)
	{
		DWORD i,j;

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		// 多通道每行像素占用的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// 单通道每行像素占用的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// 临时缓冲区
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel * dwHeight];

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				*(lpBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine+j*3 + 2);	// R
			}
		}

		BOOL bCompute = HistogramCompute_SingleChannel(lpBuf,dwWidth,dwHeight,histogram,average,deviation);

		delete []lpBuf; lpBuf = NULL;

		return bCompute;
	}

	// 图象绿色直方图统计
	BOOL KDib::HistogramCompute_Green(int histogram[256],double& average ,double& deviation)
	{
		DWORD i,j;

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		// 多通道每行像素占用的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// 单通道每行像素占用的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// 临时缓冲区
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel * dwHeight];

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				*(lpBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine+j*3 + 1);	// G
			}
		}

		BOOL bCompute = HistogramCompute_SingleChannel(lpBuf,dwWidth,dwHeight,histogram,average,deviation);

		delete []lpBuf; lpBuf = NULL;

		return bCompute;
	}

	// 图象蓝色直方图统计
	BOOL KDib::HistogramCompute_Blue(int histogram[256],double& average ,double& deviation)
	{
		DWORD i,j;

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		// 多通道每行像素占用的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// 单通道每行像素占用的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// 临时缓冲区
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel * dwHeight];

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				*(lpBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine+j*3 + 0);	// B
			}
		}

		BOOL bCompute = HistogramCompute_SingleChannel(lpBuf,dwWidth,dwHeight,histogram,average,deviation);

		delete []lpBuf; lpBuf = NULL;

		return bCompute;
	}

	// 图象单通道直方图均衡
	BOOL KDib::HistogramAverage_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight)
	{
		DWORD i,j;

		int hist[256];
		double avg,dev;

		if(!HistogramCompute_SingleChannel(lpDIBits,dwWidth,dwHeight,hist,avg,dev))
		{
			return FALSE;
		}

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8);

		// 映射表
		double map[256];

		// 计算映射表
		double sum = 0;
		for(i=0;i<256;i++)
		{
			sum += hist[i];
			map[i] = sum * 255 /(dwHeight * dwWidth) + 0.5;
		}

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				*(lpDIBits + i*dwBytesPerLine + j) = (BYTE)(WORD)map[*(lpDIBits + i*dwBytesPerLine + j)];
			}
		}

		return TRUE;
	}
	// 图象灰度直方图均衡
	BOOL KDib::HistogramAverage_Gray()
	{
		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=8)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		return HistogramAverage_SingleChannel(lpDIBits,dwWidth,dwHeight);
	}

	// 图象单通道直方图均衡 - Red
	BOOL KDib::HistogramAverage_Red()
	{
		DWORD i,j;

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		// 多通道每行像素占用的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// 单通道每行像素占用的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// 临时缓冲区
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel * dwHeight];

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				*(lpBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine+j*3 + 2);	// R
			}
		}

		if(!HistogramAverage_SingleChannel(lpBuf,dwWidth,dwHeight))
		{
			return FALSE;
		}

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				*(lpDIBits + i*dwBytesPerLine+j*3 + 2) = *(lpBuf + i*dwBytesPerLineSingleChannel + j);	// R
			}
		}

		delete []lpBuf; lpBuf = NULL;

		return TRUE;
	}

	// 图象单通道直方图均衡 - Green
	BOOL KDib::HistogramAverage_Green()
	{
		DWORD i,j;

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		// 多通道每行像素占用的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// 单通道每行像素占用的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// 临时缓冲区
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel * dwHeight];

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				*(lpBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine+j*3 + 1);	// G
			}
		}

		if(!HistogramAverage_SingleChannel(lpBuf,dwWidth,dwHeight))
		{
			return FALSE;
		}

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				*(lpDIBits + i*dwBytesPerLine+j*3 + 1) = *(lpBuf + i*dwBytesPerLineSingleChannel + j);	// G
			}
		}

		delete []lpBuf; lpBuf = NULL;

		return TRUE;
	}

	// 图象单通道直方图均衡 - Blue
	BOOL KDib::HistogramAverage_Blue()
	{
		DWORD i,j;

		// 颜色位数
		WORD wBitCount = BitCount();
		if(wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 图像数据指针
		LPBYTE lpDIBits = DIBits();

		// 多通道每行像素占用的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// 单通道每行像素占用的字节数
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// 临时缓冲区
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel * dwHeight];

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				*(lpBuf + i*dwBytesPerLineSingleChannel + j) = *(lpDIBits + i*dwBytesPerLine+j*3 + 0);	// B
			}
		}

		if(!HistogramAverage_SingleChannel(lpBuf,dwWidth,dwHeight))
		{
			return FALSE;
		}

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				*(lpDIBits + i*dwBytesPerLine+j*3 + 0) = *(lpBuf + i*dwBytesPerLineSingleChannel + j);	// B
			}
		}

		delete []lpBuf; lpBuf = NULL;

		return TRUE;
	}

	// 
	//---------------------------------------------------------
	// 名    称: AdjustColorRgb_SingleChannel
	// 访    问: protected
	// 功    能: 单通道颜色调整
	// 参    数:
	//			 [i] lpDIBits - 
	//			 [i] dwWidth - 
	//			 [i] dwHeight - 
	//			 [i] dScale - 变换比例[0,1]
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::AdjustColorRgb_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double dScale)
	{
		DWORD i,j;

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwBytesPerLine;j++)
			{
				double tmp = *(lpDIBits + i*dwBytesPerLine + j) * dScale + 0.5;
				if(tmp>255)
				{
					*(lpDIBits + i*dwBytesPerLine + j) = 255;
				}
				else if(tmp<0)
				{
					*(lpDIBits + i*dwBytesPerLine + j) = 0;
				}
				else
				{
					*(lpDIBits + i*dwBytesPerLine + j) = (BYTE)(WORD)tmp;
				}
			}
		}

		return TRUE;
	}

	//---------------------------------------------------------
	// 名    称: Smooth_Template_SingleChannel
	// 访    问: protected
	// 功    能: 单通道图像平滑 - 模板法
	// 参    数:
	//			 [i] lpDIBits - DIB 数据指针
	//			 [i] dwWidth - DIB 数据宽度
	//			 [i] dwHeight - DIB 数据高度
	//
	//			 [i] pTemplate - 模板指针
	//			 [i] w - 模板宽度
	//			 [i] h - 模板高度
	//			 [i] x0 - 模板中心元素 X 坐标
	//			 [i] y0 - 模板中心元素 Y 坐标
	//			 [i] coef - 模板系数
	//
	//			 [i] byFill - 边缘的填充值
	// 返    回: 
	//			 TRUE - 成功
	//			 FALSE - 失败
	// 注    释: 
	//---------------------------------------------------------
	BOOL KDib::Smooth_Template_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double* pTemplate,int w,int h,int x0,int y0,double coef,BYTE byFill)
	{
		DWORD i,j;

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);

		// 备份通道数据
		LPBYTE lpDIBits_bak = new BYTE[dwBytesPerLine*dwHeight];
		memcpy(lpDIBits_bak,lpDIBits,sizeof(BYTE)*dwBytesPerLine*dwHeight);

		for(i=0; i<dwHeight; i++)
		{
			for(j=0; j<dwWidth; j++)
			{
				LPBYTE lpDest = lpDIBits + dwBytesPerLine*(dwHeight-1-i) + j;

				// 去掉边缘的 y0 行 x0 列
				if((int)i<y0 || (int)i>=dwHeight-y0 || (int)j<x0 || (int)j>=dwWidth-x0)
				{
					*lpDest = byFill;
					continue;
				}

				double dValue = 0;
				for(int k=0;k<h;k++)
				{
					for(int l=0;l<w;l++)
					{
						LPBYTE lpSrc = lpDIBits_bak + dwBytesPerLine * (dwHeight-1-i+y0-k) + (j-x0+l);

						// 计算加权平均
						dValue += *(lpSrc) * pTemplate[k*w+l];
					}
				}
				dValue = fabs(dValue * coef + 0.5);

				if(dValue>255)	*lpDest = (BYTE)255;
				else			*lpDest = (BYTE)(WORD)dValue;
			}
		}

		_deletea(lpDIBits_bak);

		return TRUE;
	}

	// 单通道图像平滑 - 自适应局部平滑
	//
	//	pTemplate[0]            pTemplate[1]            pTemplate[2]                                                            
	//	--         --           --         --           --         --                                                             
	//	| 0 0 0 0 0 |           | 0 0 0 0 0 |           | 0 1 1 1 0 |                                                            
	//	| 0 1 1 1 0 |           | 1 1 0 0 0 |           | 0 1 1 1 0 |                                                            
	//	| 0 1 1 1 0 | * 1 / 9   | 1 1 1 0 0 | * 1 / 7   | 0 0 1 0 0 | * 1 / 7                                                            
	//	| 0 1 1 1 0 |           | 1 1 0 0 0 |           | 0 0 0 0 0 |                                                            
	//	| 0 0 0 0 0 |           | 0 0 0 0 0 |           | 0 0 0 0 0 |                                                            
	//	--         --           --         --           --         --                                                            
	//
	//	pTemplate[3]            pTemplate[4]            pTemplate[5]                                                            
	//	--         --           --         --           --         --                                                             
	//	| 0 0 0 0 0 |           | 0 0 0 0 0 |           | 1 1 0 0 0 |                                                            
	//	| 0 0 0 1 1 |           | 0 0 0 0 0 |           | 1 1 1 0 0 |                                                            
	//	| 0 0 1 1 1 | * 1 / 7   | 0 0 1 0 0 | * 1 / 7   | 0 1 1 0 0 | * 1 / 7                                                            
	//	| 0 0 0 1 1 |           | 0 1 1 1 0 |           | 0 0 0 0 0 |                                                            
	//	| 0 0 0 0 0 |           | 0 1 1 1 0 |           | 0 0 0 0 0 |                                                            
	//	--         --           --         --           --         --                                                            
	//
	//	pTemplate[6]            pTemplate[7]            pTemplate[8]                                                            
	//	--         --           --         --           --         --                                                             
	//	| 0 0 0 1 1 |           | 0 0 0 0 0 |           | 0 0 0 0 0 |                                                            
	//	| 0 0 1 1 1 |           | 0 0 0 0 0 |           | 0 0 0 0 0 |                                                            
	//	| 0 0 1 1 0 | * 1 / 7   | 0 0 1 1 0 | * 1 / 7   | 0 1 1 0 0 | * 1 / 7                                                            
	//	| 0 0 0 0 0 |           | 0 0 1 1 1 |           | 1 1 1 0 0 |                                                            
	//	| 0 0 0 0 0 |           | 0 0 0 1 1 |           | 1 1 0 0 0 |                                                            
	//	--         --           --         --           --         --                                                            
	//
	BOOL KDib::Smooth_Auto_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,BYTE byFill)
	{
		int i,k;
		DWORD row,col;

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);

		LPBYTE lpDIBits_bak = new BYTE[dwBytesPerLine*dwHeight];
		memcpy(lpDIBits_bak,lpDIBits,sizeof(BYTE)*dwBytesPerLine*dwHeight);

		double mean[9];		// 均值
		double bunsan[9];	// 均值的方差

		double dtmp[9];
		for(row=0; row<dwHeight;row++)
		{
			for(col=0; col<dwWidth; col++)
			{
				// 去掉边缘的 y0 行 x0 列
				if((int)row<2 || (int)row>=dwHeight-2 || (int)col<2 || (int)col>=dwWidth-2)
				{
					lpDIBits[ dwBytesPerLine * row + col ] = byFill;
					continue;
				}

				///求 9 种近邻域的均值和方差
				//------------------------------------
				// 第 0 种
				k = 0;
				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col-1) ];
				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+0) ];
				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+1) ];

				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col-1) ];
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];
				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+1) ];

				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col-1) ];
				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+0) ];
				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+1) ];

				// 均值
				mean[k] = 0;
				for(i=0;i<9;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 9;

				// 均值的方差
				bunsan[k] = 0;
				for(i=0;i<9;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// 第 1 种
				k = 1;
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col-2) ];
				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col-1) ];

				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col-2) ];
				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col-1) ];
				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];

				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col-2) ];
				dtmp[6] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col-1) ];

				// 均值
				mean[k] = 0;
				for(i=0;i<7;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 7;

				// 均值的方差
				bunsan[k] = 0;
				for(i=0;i<7;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// 第 2 种
				k = 2;
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row-2) + (col-1) ];
				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row-2) + (col+0) ];
				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row-2) + (col+1) ];

				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col-1) ];
				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+0) ];
				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+1) ];

				dtmp[6] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];

				// 均值
				mean[k] = 0;
				for(i=0;i<7;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 7;

				// 均值的方差
				bunsan[k] = 0;
				for(i=0;i<7;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// 第 3 种
				k = 3;
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+1) ];
				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+2) ];

				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];
				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+1) ];
				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+2) ];

				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+1) ];
				dtmp[6] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+2) ];

				// 均值
				mean[k] = 0;
				for(i=0;i<7;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 7;

				// 均值的方差
				bunsan[k] = 0;
				for(i=0;i<7;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// 第 4 种
				k = 4;
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];

				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col-1) ];
				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+0) ];
				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+1) ];

				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row+2) + (col-1) ];
				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row+2) + (col+0) ];
				dtmp[6] = lpDIBits_bak[ dwBytesPerLine * (row+2) + (col+1) ];

				// 均值
				mean[k] = 0;
				for(i=0;i<7;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 7;

				// 均值的方差
				bunsan[k] = 0;
				for(i=0;i<7;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// 第 5 种
				k = 5;
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row-2) + (col-2) ];
				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row-2) + (col-1) ];

				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col-2) ];
				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col-1) ];
				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+0) ];

				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col-1) ];
				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];

				// 均值
				mean[k] = 0;
				for(i=0;i<7;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 7;

				// 均值的方差
				bunsan[k] = 0;
				for(i=0;i<7;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// 第 6 种
				k = 6;
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];
				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+1) ];

				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+0) ];
				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+1) ];
				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+2) ];

				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row-2) + (col+1) ];
				dtmp[6] = lpDIBits_bak[ dwBytesPerLine * (row-2) + (col+2) ];

				// 均值
				mean[k] = 0;
				for(i=0;i<7;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 7;

				// 均值的方差
				bunsan[k] = 0;
				for(i=0;i<7;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// 第 7 种
				k = 7;
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];
				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+1) ];

				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+0) ];
				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+1) ];
				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+2) ];

				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row+2) + (col+1) ];
				dtmp[6] = lpDIBits_bak[ dwBytesPerLine * (row+2) + (col+2) ];

				// 均值
				mean[k] = 0;
				for(i=0;i<7;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 7;

				// 均值的方差
				bunsan[k] = 0;
				for(i=0;i<7;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// 第 8 种
				k = 8;
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];
				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col-1) ];

				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+0) ];
				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col-1) ];
				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col-2) ];

				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row+2) + (col-1) ];
				dtmp[6] = lpDIBits_bak[ dwBytesPerLine * (row+2) + (col-2) ];

				// 均值
				mean[k] = 0;
				for(i=0;i<7;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 7;

				// 均值的方差
				bunsan[k] = 0;
				for(i=0;i<7;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// 求方差最小的近邻域
				double bmin = bunsan[0];
				int index = 0;
				for(i=0;i<9;i++)
				{
					if(bunsan[i] < bmin)
					{
						bmin = bunsan[i];
						index = i;
					}
				}

				lpDIBits[ dwBytesPerLine * row + col ] = (BYTE)(WORD)(mean[index] + 0.5);

			}//for(col)
		}//for(row)

		_deletea(lpDIBits_bak);

		return TRUE;
	}

	// 图像平滑 - 中值滤波
	BOOL KDib::Smooth_MedianFilter_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,int w,int h,int x0,int y0,BYTE byFill)
	{
		DWORD i,j;

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);

		LPBYTE lpDIBits_bak = new BYTE[dwBytesPerLine*dwHeight];
		memcpy(lpDIBits_bak,lpDIBits,sizeof(BYTE)*dwBytesPerLine*dwHeight);

		int iBufLength = w * h;
		LPBYTE lpBuf = new BYTE[iBufLength];

		LPBYTE lpSrc = NULL;
		LPBYTE lpDst = NULL;

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				// 去掉边缘的 y0 行 x0 列
				if((int)i<2 || (int)i>=dwHeight-2 || (int)j<2 || (int)j>=dwWidth-2)
				{
					lpDIBits[ dwBytesPerLine * i + j ] = byFill;
					continue;
				}

				lpDst = lpDIBits + dwBytesPerLine * (dwHeight-1-i) + j;

				for(int m=0;m<h;m++)
				{
					for(int n=0;n<w;n++)
					{
						lpSrc = lpDIBits_bak + dwBytesPerLine * (dwHeight-1-i+y0-m) + (j-x0+n);
						*(lpBuf + m*w + n) = *lpSrc;
					}
				}

				*lpDst = MedianValue(lpBuf,iBufLength);
			}
		}

		_deletea(lpBuf);
		_deletea(lpDIBits_bak);

		return TRUE;
	}

	// 单通道图像锐化 - 梯度锐化
	BOOL KDib::Sharp_Grad_SingleChanne(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,BYTE byThreshold)
	{
		DWORD i,j;

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);

		// 备份通道数据
		LPBYTE lpDIBits_bak = new BYTE[dwBytesPerLine*dwHeight];
		memcpy(lpDIBits_bak,lpDIBits,sizeof(BYTE)*dwBytesPerLine*dwHeight);

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				LPBYTE lpDst = lpDIBits + dwBytesPerLine * i +j;

				// 进行梯度运算
				// i 行 j 列
				LPBYTE lpSrc	= lpDIBits_bak + dwBytesPerLine * (i+0) + (j+0);
				// i+1 行 j 列
				LPBYTE lpSrc1	= lpDIBits_bak + dwBytesPerLine * (i+1) + (j+0);
				// i 行 j+1 列
				LPBYTE lpSrc2	= lpDIBits_bak + dwBytesPerLine * (i+0) + (j+1);

				// 梯度
				int iTemp = abs((*lpSrc) - (*lpSrc1)) + abs((*lpSrc) - (*lpSrc2));
				int C = 100;

				if((iTemp+C)<255)
				{
					if(iTemp>=byThreshold)
					{
						*lpDst = (BYTE)(iTemp+C);
					}
				}
				else
				{
					*lpDst = (BYTE)0xFF;
				}

			}//for(j)
		}//for(i)

		_deletea(lpDIBits_bak);

		return TRUE;
	}

	// 单通道图像锐化 - 拉普拉斯模板锐化
	BOOL KDib::Sharp_LapTemplate_SingleChanne(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double* pTemplate,int w,int h,int x0,int y0,double coef,BYTE byFill)
	{
		DWORD i,j;

		// 每行像素的字节数
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);

		// 备份通道数据
		LPBYTE lpDIBits_bak = new BYTE[dwBytesPerLine*dwHeight];
		memcpy(lpDIBits_bak,lpDIBits,sizeof(BYTE)*dwBytesPerLine*dwHeight);

		for(i=0; i<dwHeight; i++)
		{
			for(j=0; j<dwWidth; j++)
			{
				//LPBYTE lpDest = lpDIBits + dwBytesPerLine*(dwHeight-1-i) + j;
				LPBYTE lpDest = lpDIBits + dwBytesPerLine*i + j;

				// 去掉边缘的 y0 行 x0 列
				if((int)i<y0 || (int)i>=dwHeight-y0 || (int)j<x0 || (int)j>=dwWidth-x0)
				{
					*lpDest = byFill;
					continue;
				}

				double dValue = 0;
				for(int m=0;m<h;m++)
				{
					for(int n=0;n<w;n++)
					{
						//LPBYTE lpSrc = lpDIBits_bak + dwBytesPerLine * (dwHeight-1-i+y0-m) + (j-x0+n);
						LPBYTE lpSrc = lpDIBits_bak + dwBytesPerLine * (i-y0+m) + (j-x0+n);
						// 计算加权平均
						dValue += *(lpSrc) * pTemplate[m*w+n];
					}
				}
				dValue = fabs(dValue * coef + 0.5);

				if(dValue>255)	*lpDest = (BYTE)255;
				else			*lpDest = (BYTE)(WORD)dValue;

			}//for(j)
		}//for(i)

		_deletea(lpDIBits_bak);

		return TRUE;
	}




	///============================================================================
	///  _private                         下面是私有成员函数
	///============================================================================

	// 查找中值
	BYTE KDib::MedianValue(LPBYTE lpBuf,int len)
	{
		int i,j;

		BYTE byMedianValue = 0;

		// 冒泡排序
		for(j=0;j<len;j++)
		{
			for(i=0;i<len-j-1;i++)
			{
				if(lpBuf[i]>lpBuf[i+1])
				{// 降序
					byMedianValue = lpBuf[i];
					lpBuf[i] = lpBuf[i+1];
					lpBuf[i+1] = byMedianValue;
				}
			}
		}

		if((len & 1)>0)
		{// 奇数
			byMedianValue = lpBuf[(len+1)/2];
		}
		else
		{// 偶数
			byMedianValue = (lpBuf[len/2] + lpBuf[len/2+1]) / 2;
		}

		return byMedianValue;
	}

//---------------------------------------------------------
}// namespace Images - end
//---------------------------------------------------------

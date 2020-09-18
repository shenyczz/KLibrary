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
	///  _public                         �����Ǳ�����Ա����
	///============================================================================

	//---------------------------------------------------------
	// ��    ��: Empty
	// ��    ��: public
	// ��    ��: ���DIB,�ͷ��Ѿ�������ڴ�,�ر�ӳ���ļ�
	// ��    ��:
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
	//---------------------------------------------------------
	void KDib::Empty()
	{
		DetachMapFile();	//�ر��ڴ�ӳ���ļ�

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
	// ��    ��: Load
	// ��    ��: public
	// ��    ��: ��ȡ�ļ�����
	// ��    ��:
	//			 [i] lpszPathName - DIB �ļ���
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
	//---------------------------------------------------------
	BOOL KDib::Load(LPCTSTR lpszPathName)
	{
		Empty();

		FILE* fp = _tfopen(lpszPathName,_T("rb"));
		if(fp==NULL) return FALSE;

		// ��ȡ�ļ�ͷ
		BITMAPFILEHEADER bmfh;
		if(fread(&bmfh,sizeof(BITMAPFILEHEADER),1,fp)<1)
		{
			fclose(fp);
			return FALSE;
		}

		// �Ƿ� BMP �ļ�
		if(bmfh.bfType != 0x4d42)
		{
			fclose(fp);
			return FALSE;
		}

		// ��ȡ BITMAPINFOHEADER
		BITMAPINFOHEADER bmif;
		if(fread(&bmif,sizeof(BITMAPINFOHEADER),1,fp)<1)
		{
			fclose(fp);
			return FALSE;
		}
		// ����ָ��
		fseek(fp,(int)sizeof(BITMAPINFOHEADER) * -1,SEEK_CUR);

		// ����ͼ�����ݴ�С
		bmif.biSizeImage = DIBWIDTHBYTES(bmif.biWidth * bmif.biBitCount) * bmif.biHeight;

		// �����ɫ���������
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
	// ��    ��: Save
	// ��    ��: public
	// ��    ��: ��DIB����д���ļ���
	// ��    ��:
	//			 [i] iRChannel - ��ɫͨ��
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
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
			// д�ļ�ͷ
			fwrite(&bmfh,sizeof(BITMAPFILEHEADER),1,fp);
			// д DIB ͷ����ɫ��
			fwrite(m_lpBitmapInfoHeader,sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_dwColorTableEntries,1,fp);
			// д����
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
	// ��    ��: AttachMemory
	// ��    ��: public
	// ��    ��: �����ڴ���Դ
	// ��    ��:
	//			 [i] lpvMem - �ڴ��ַָ��
	//			 [i] bMustDelete - �Ƿ�������ڴ�, Ĭ�� = FALSE
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
	//---------------------------------------------------------
	BOOL KDib::AttachMemory(LPVOID lpvMem,BOOL bMustDelete /*=FALSE*/)
	{
		if(lpvMem == NULL)
			return FALSE;

		Empty();

		// �Ƿ����ͷ��ڴ�
		m_bMustDelete = bMustDelete;

		// DIB ����ָ��
		m_lpBitmapInfoHeader = (LPBITMAPINFOHEADER)lpvMem;

		// ������ɫ���ַ
		m_lpvColorTable = (LPBYTE)m_lpBitmapInfoHeader + sizeof(BITMAPINFOHEADER);

		// ����DIB���ݳߴ�
		if(!ComputeMetrics(m_lpBitmapInfoHeader))
			return FALSE;

		// �����ɫ��ߴ�(��ɫ������� m_dwColorTableEntries)
		if(!ComputePaletteSize(m_lpBitmapInfoHeader))
			return FALSE;

		// ���ɵ�ɫ�� m_hPalette
		MakePalette();

		// DIB ����ָ��
		m_lpImageData = (LPBYTE)m_lpvColorTable + sizeof(RGBQUAD) * m_dwColorTableEntries;

		// ���
		m_hBitmap = this->DIB2DDB();

		m_bEmpty = FALSE;

		return TRUE;
	}

	//---------------------------------------------------------
	// ��    ��: AttachFile
	// ��    ��: public
	// ��    ��: ����ͼ���ļ�
	// ��    ��:
	//			 [i] lpszPathName - DIB �ļ�
	//			 [i] bMustDelete - �Ƿ�������ڴ�, Ĭ�� = FALSE
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
	//---------------------------------------------------------
	BOOL KDib::AttachFile(LPCTSTR lpszPathName,BOOL bMapfile /*= FALSE*/)
	{
		if(bMapfile) 
			return AttachMapFile(lpszPathName);
		else
			return Load(lpszPathName);
	}

	//---------------------------------------------------------
	// ��    ��: AttachFile
	// ��    ��: public
	// ��    ��: HBITMAP
	// ��    ��:
	//			 [i] hBitmap - λͼ���
	//			 [i] hPalette - ��ɫ��ָ��
	//			 [i] dwCompression - ѹ����ʽ
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
	//---------------------------------------------------------
	BOOL KDib::AttachHBITMAP(HBITMAP hBitmap,HPALETTE hPalette /*=NULL*/,DWORD dwCompression /*=BI_RGB*/)
	{
		return DDB2DIB(hBitmap,hPalette,dwCompression) != NULL;
	}

	//---------------------------------------------------------
	// ��    ��: Detch
	// ��    ��: public
	// ��    ��: ���� DIB
	// ��    ��:
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: ������಻�����ͷ��ڴ�
	//---------------------------------------------------------
	HGLOBAL KDib::Detch()
	{ 
		m_bMustDelete = FALSE;
		return (HGLOBAL)m_lpBitmapInfoHeader;
	}

	//---------------------------------------------------------
	// ��    ��: GetHBITMAP
	// ��    ��: public
	// ��    ��: ȡ��λͼ���
	// ��    ��:
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: ������಻�����ͷ��ڴ�
	//---------------------------------------------------------
	const HBITMAP KDib::GetHBITMAP( )
	{
		return  this->DIB2DDB();
	}

	//---------------------------------------------------------
	// ��    ��: DIBits
	// ��    ��: public
	// ��    ��: ȡ�� DIB ͨ������
	// ��    ��:
	//			 [i] iChannel - ͨ��
	// ��    ��: 
	//			 LPBYTE - ͨ�����ݻ�����ָ��
	// ע    ��: 
	//---------------------------------------------------------
	LPBYTE KDib::DIBits(int iChannel)
	{
		DWORD i,j;

		_deletea(m_lpBytesBuf);

		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return NULL;
		}

		// ͼ������ָ��
		LPBYTE lpDIBits = DIBits();

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// 8 λ
		if(wBitCount == 8)
		{
			return lpDIBits;
		}

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// ��ʱ������
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
	// ��    ��: UsePalette
	// ��    ��: public
	//
	// ��    ��: Ӧ�õ�ɫ��
	//
	// ��    ��:
	//			 [i] hDC - DC ���
	//			 [i] bBackground - ����Ӧ��
	//
	// ��    ��: ��ɫ����ɫ��
	//
	// ע    ��: 
	//
	//---------------------------------------------------------
	UINT KDib::UsePalette(HDC hDC, BOOL bBackground /*= FALSE*/)
	{
		if(m_hPalette == NULL) return 0;

		::SelectPalette(hDC, m_hPalette, bBackground);

		return ::RealizePalette(hDC);
	}

	//---------------------------------------------------------
	// ��    ��: Capture
	// ��    ��: public
	// ��    ��: ��׽�ͻ�������������ͼ��
	// ��    ��:
	//			 [i] hDC - �豸����ָ��
	//			 [i] rect - ץȡ�������
	//			 [i] hPalette - ��ɫ��,Ĭ�� = NULL
	//			 [i] bForce256 - 256ɫ,Ĭ�� = FALSE
	//			 [i] pData - �� bForce256 = TRUE ʱ,����Ϊ NULL
	// ��    ��: 
	// ע    ��: 
	//---------------------------------------------------------
	void KDib::Capture(HDC hDC, LPCRECT lpRect, HPALETTE hPalette /*= NULL*/, BOOL bForce256 /*= FALSE*/,int* pData /*= NULL*/)
	{
		if(bForce256 && pData == NULL)
			throw;

		Empty();

		RECT rect = *lpRect;

		//����ץȡ�����С
		int nWidth = abs(rect.right - rect.left);
		int nHeight = abs(rect.bottom - rect.top);

		//HDC hScreenDC=::CreateDC(_T("DISPLAY"),NULL,NULL,NULL);	//����DC
		//HDC hCaptureDC	= hScreenDC;
		HDC hCaptureDC	= hDC;
		HDC hMemDC		= ::CreateCompatibleDC(hCaptureDC);
		HBITMAP hBitmap	= ::CreateCompatibleBitmap(hCaptureDC,nWidth,nHeight);

		::SelectObject(hMemDC,hBitmap);
	
		//����Ļ���ݿ������հ�λͼ
		::BitBlt(hMemDC,0,0,nWidth,nHeight,hDC,rect.left,rect.top,SRCCOPY);

		if(!hPalette)
		{
			//��ϵͳ��ɫ�屣�浽λͼ��ɫ����
			//����һ���㹻��ĵ�ɫ��
			//���ݵ�ǰϵͳȡ����ɫ��
			int nBITSPIXEL	= ::GetDeviceCaps(hCaptureDC,BITSPIXEL);
			int nPLANES		= ::GetDeviceCaps(hCaptureDC,PLANES);
			int nColors=(1<<nBITSPIXEL*nPLANES);

			//��ʼ���հ��߼���ɫ�� ��ϵͳ��ɫ�����հ׵�ɫ��
			LOGPALETTE* pLogPal		= (LOGPALETTE*)new BYTE[sizeof(LOGPALETTE)+nColors*sizeof(PALETTEENTRY)];
			pLogPal->palVersion		= PALVERSION;
			pLogPal->palNumEntries	= nColors;
			nColors					= ::GetSystemPaletteEntries(hCaptureDC,0,nColors,(LPPALETTEENTRY)(pLogPal->palPalEntry));

			//�����µĵ�ɫ��
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
	// ��    ��: TranslucentBlt
	// ��    ��: public
	// ��    ��: ���ư�͸��ͼ��
	// ��    ��:
	//			 [i] hDC - DC ���
	//			 [i] x - hBitmap �����Ͻ�x����
	//			 [i] y - hBitmap �����Ͻ�y����
	//			 [i] hBitmap - λͼ���
	//			 [i] nTranslucent - ͸������ 0 - 100
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: ��ɫ����͸��
	//---------------------------------------------------------
	BOOL KDib::TranslucentBlt(HDC hDC, int x, int y, HBITMAP hBitmap,int nTranslucent)
	{
		if(nTranslucent>100) nTranslucent = 100;

		// ����Ҫ��͸����ʾλͼ�ߴ�
		BITMAP bm;
		::GetObject(hBitmap,sizeof(BITMAP),&bm);
		int nWidth = bm.bmWidth;
		int nHeight = bm.bmHeight;

		// �����ڴ� DC �Ա�ѡ��ͼ��
		HDC hMemDC = ::CreateCompatibleDC(hDC);
		HBITMAP hOldMemBitmap = (HBITMAP)::SelectObject(hMemDC,hBitmap);

		// ������ģ DC
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

		// ����
		::SelectObject(hMemDC,hOldMemBitmap);
		::DeleteDC(hMemDC);

		::SelectObject(hMaskDC,hOldMaskBitmap);
		::DeleteObject(hMaskBitmap);
		::DeleteDC(hMaskDC);

		return TRUE;
	}
	//---------------------------------------------------------
	// ��    ��: TranslucentBlt
	// ��    ��: public
	// ��    ��: ���ư�͸��ͼ��
	// ��    ��:
	//			 [i] x - hBitmap �����Ͻ�x����
	//			 [i] y - hBitmap �����Ͻ�y����
	//			 [i] hBitmap - λͼ���
	//			 [i] nTranslucent - ͸������ 0 - 100
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: ��ɫ����͸��
	//---------------------------------------------------------
	BOOL KDib::TranslucentBlt(int x, int y, HBITMAP hBitmap,int nTranslucent)
	{
		if(this->IsEmpty())
		{
			return FALSE;
		}

		if(nTranslucent>100) nTranslucent = 100;

		HDC hDC = ::GetDC(NULL);

		// �Ȱѱ���ѡ�� DC
		HDC hDestDC = ::CreateCompatibleDC(hDC);
		HBITMAP hDestBitmap = this->DIB2DDB();
		HBITMAP hOldDestBitmap = (HBITMAP)::SelectObject(hDestDC,hDestBitmap);

		// ����Ҫ��͸����ʾλͼ�ߴ�
		BITMAP bm;
		::GetObject(hBitmap,sizeof(BITMAP),&bm);
		int nWidth = bm.bmWidth;
		int nHeight = bm.bmHeight;

		// �����ڴ� DC �Ա�ѡ��ͼ��
		HDC hMemDC = ::CreateCompatibleDC(hDC);
		HBITMAP hOldMemBitmap = (HBITMAP)::SelectObject(hMemDC,hBitmap);

		// ������ģ DC
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

		// ����
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
	// ��    ��: TransparentBlt
	// ��    ��: public
	// ��    ��: ���ư�͸��ͼ��
	// ��    ��:
	//			 [i] hDC - DC ���
	//			 [i] x - hBitmap �����Ͻ�x����
	//			 [i] y - hBitmap �����Ͻ�y����
	//			 [i] hBitmap - λͼ���
	//			 [i] clrTransparent - ͸��ɫ
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
	//---------------------------------------------------------
	BOOL KDib::TransparentBlt(HDC hDC, int x, int y, HBITMAP hBitmap, UINT clrTransparent)
	{
		// ����Ҫ��͸����ʾλͼ�ߴ�
		BITMAP bm;
		::GetObject(hBitmap,sizeof(BITMAP),&bm);
		int nWidth = bm.bmWidth;
		int nHeight = bm.bmHeight;

		// �����ڴ� DC �Ա�ѡ��ͼ��
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
	// ��    ��: TransparentBlt
	// ��    ��: public
	// ��    ��: ���ư�͸��ͼ��
	// ��    ��:
	//			 [i] x - hBitmap �����Ͻ�x����
	//			 [i] y - hBitmap �����Ͻ�y����
	//			 [i] hBitmap - λͼ���
	//			 [i] clrTransparent - ͸��ɫ
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
	//---------------------------------------------------------
	BOOL KDib::TransparentBlt(int x, int y, HBITMAP hBitmap, UINT clrTransparent)
	{
		if(this->IsEmpty())
		{
			return FALSE;
		}

		KDib dibtmp;
	

		HDC hDC = ::GetDC(NULL);

		// �Ȱѱ���ѡ�� DC
		HDC hDestDC = ::CreateCompatibleDC(hDC);
		HBITMAP hDestBitmap = this->DIB2DDB();
		HBITMAP hOldDestBitmap = (HBITMAP)::SelectObject(hDestDC,hDestBitmap);

		// ����Ҫ͸����ʾλͼ�ߴ�
		BITMAP bm;
		::GetObject(hBitmap,sizeof(BITMAP),&bm);
		int nWidth = bm.bmWidth;
		int nHeight = bm.bmHeight;

		// �����ڴ� DC �Ա�ѡ��ͼ��
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
	// ��    ��: Clone
	// ��    ��: public
	// ��    ��: ��¡
	// ��    ��:
	// ��    ��: 
	// ע    ��: 
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
	// ��    ��: Print
	// ��    ��: public
	// ��    ��: ��ӡ DIB
	// ��    ��:
	//			 [i] hDC - DC ���
	//			 [i] hDIB - �ⲿ���� DIB ���,Ĭ�� = NULL
	// ��    ��: 
	// ע    ��: 
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
		::SetMapMode(hDC,MM_LOENGLISH);		//ӳ��ģʽ MM_LOENGLISH

		//LPBITMAPINFOHEADER lpBitmapInfoHeader = m_lpBitmapInfoHeader;
		LPBITMAPINFOHEADER lpBitmapInfoHeader = this->BitmapInfoHeader();

		DWORD dwWidth = m_dwWidth;		// DIB ���
		DWORD dwHeight = m_dwHeight;	// DIB �߶�

		// DIBͷ�ߴ�
		DWORD dwDIBHeadSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_dwColorTableEntries;

		//����ָ��
		LPSTR lpDIBBits=(LPSTR)m_lpImageData;	//only image data

		//����ɴ�ӡ����,Ҫ�������1/8Ӣ���϶ 1 inch=25.4mm
		//��MM_LOENGLISH��
		int cxBorder = ::GetDeviceCaps(hDC,LOGPIXELSX)/8;
		int cyBorder = ::GetDeviceCaps(hDC,LOGPIXELSY)/8;
		//TRACE("cxBorder=%d cyborder=%d\n",cxBorder,cyBorder);

		POINT point;
		//ȡ�ô�ӡ����Ŀ�Ⱥ͸߶�(����)	A4=210*297mm
		point.x = ::GetDeviceCaps(hDC,HORZRES);
		point.y = ::GetDeviceCaps(hDC,VERTRES);
		//TRACE("Pixel cxPage=%d cyPage=%d\n",size.cx,size.cy);	//1488*2104
		::DPtoLP(hDC,&point,1);
		//TRACE("LP cxPage=%d cyPage=%d\n",size.cx,size.cy);		//827*1169

		int cxPage = abs(point.x);	// ҳ��
		int cyPage = abs(point.y);	// ҳ��

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

		//��չλͼ�Գ�����ӡֽ
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
	// ��    ��: ReplaceColorTable
	// ��    ��: public
	// ��    ��: �滻��ɫ��
	// ��    ��:
	// ��    ��: 
	// ע    ��: 
	//---------------------------------------------------------
	BOOL KDib::ReplaceColorTable(LPRGBQUAD lpRrgbQuad)
	{
		// 8λͼ������ɫ��
		if(BitCount() != 8 || lpRrgbQuad==NULL)
		{
			return FALSE;
		}

		LPVOID lpColorTable = ColorTable();
		memcpy(lpColorTable,lpRrgbQuad,sizeof(RGBQUAD)*256);

		return TRUE;
	}

	//---------------------------------------------------------
	// ��    ��: Color2Gray
	// ��    ��: public
	// ��    ��: ��ɫͼ��任�ɻҶ�ͼ��
	// ��    ��:
	//			 [i] lpRrgbQuad - ��ɫ��ָ��
	// ��    ��: 
	// ע    ��: 
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
		pBitmapInfo->bmiHeader.biSizeImage		= dwSizeImage;	//��BI_RGB��ʽʱ���Ϊ0
		pBitmapInfo->bmiHeader.biXPelsPerMeter	= 0;
		pBitmapInfo->bmiHeader.biYPelsPerMeter	= 0;
		pBitmapInfo->bmiHeader.biClrUsed		= 0;
		pBitmapInfo->bmiHeader.biClrImportant	= 0;

		// ���ɻҶ�ͼ����ɫ��
		LPRGBQUAD lpRgbQuad = (LPRGBQUAD)(lpDib + sizeof(BITMAPINFOHEADER));
		for(int i=0; i<256; i++)
		{
			lpRgbQuad[i].rgbRed			= i;
			lpRgbQuad[i].rgbGreen		= i;
			lpRgbQuad[i].rgbBlue		= i;
			lpRgbQuad[i].rgbReserved	= 0;
		}

		DWORD dwDIBWidthSource = DIBWIDTHBYTES(dwWidth*8*3);
		// Դ����ָ��
		LPBYTE lpDIBitsSource = DIBits();


		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);
		// DIB ͼ�����ݵ�ַ
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
	// ��    ��: Gray2Color
	// ��    ��: public
	// ��    ��: �Ҷ�ͼ��任�ɲ�ɫͼ��
	// ��    ��:
	//			 [i] lpRrgbQuad - ��ɫ��ָ��
	// ��    ��: 
	// ע    ��: 
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
		pBitmapInfo->bmiHeader.biSizeImage		= dwSizeImage;	//��BI_RGB��ʽʱ���Ϊ0
		pBitmapInfo->bmiHeader.biXPelsPerMeter	= 0;
		pBitmapInfo->bmiHeader.biYPelsPerMeter	= 0;
		pBitmapInfo->bmiHeader.biClrUsed		= 0;
		pBitmapInfo->bmiHeader.biClrImportant	= 0;


		// Դ����ָ��
		LPBYTE lpDIBitsSource = DIBits();
		DWORD dwDIBWidthSource = DIBWIDTHBYTES(dwWidth*8);

		// DIB ͼ�����ݵ�ַ
		LPBYTE lpDIBits = lpDib + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 0;
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*3);

		// �����ػҶ�ֵΪ��ɫ��ʽ��ÿ��ͨ����ֵ������ͨ�����
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
	// ��    ��: Reverse
	// ��    ��: public
	// ��    ��: ����
	// ��    ��:
	// ��    ��: 
	// ע    ��: 
	//---------------------------------------------------------
	BOOL KDib::Reverse()
	{
		// ��ɫλ��(ֻ����8λ��24λλͼ����)
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ��ɫ�������
		DWORD dwColorTableEntries = ColorTableEntries();

		DWORD dwSizeImage = DIBWIDTHBYTES(dwWidth*8*wBitCount/8) * dwHeight;
		DWORD dwDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * dwColorTableEntries + dwSizeImage;

		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*wBitCount/8);

		// ͼ������ָ��
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
	// ��    ��: Reverse
	// ��    ��: public
	// ��    ��: ��ֵ��
	// ��    ��:
	//			 [i] iThreshold - ��ֵ
	// ��    ��: 
	// ע    ��: 
	//			 С�� iThreshold ���Ǳ���,���ڵ��� iThreshold ����Ŀ��ֵ
	//---------------------------------------------------------
	BOOL KDib::Binary(int iThreshold)
	{
		if(iThreshold<0 || iThreshold>255)
		{
			return FALSE;
		}

		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		if(wBitCount==24)
		{// �ȻҶȻ�
			if(!Color2Gray()) return FALSE;
		}

		// ����ȡ����ɫλ��
		wBitCount = BitCount();
		if(wBitCount!=8)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ��ɫ�������(=256)
		DWORD dwColorTableEntries = ColorTableEntries();

		DWORD dwSizeImage = DIBWIDTHBYTES(dwWidth*8*wBitCount/8) * dwHeight;
		DWORD dwDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * dwColorTableEntries + dwSizeImage;

		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*wBitCount/8);

		// ͼ������ָ��
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
	// ��    ��: LinerTransform
	// ��    ��: public
	// ��    ��: ���Ա任
	// ��    ��:
	//			 [i] a - ���Ա任ϵ��A(б��),�����Աȶ�
	//			 [i] b - ���Ա任ϵ��B(�ؾ�),��������
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - �ɹ�
	// ע    ��: 
	//			���Ա任��ʽ f(x) = a*x + b
	//				(1) a=1; b=0 --> ͼ�񲻱�
	//				(2) a=1; b!=0 --> �������� b=[-255,+255]
	//				(3) a<0 --> ��
	//				(4) a<-1;b=255 --> ����
	//				(5) a>1, b=0 --> ���ӶԱȶ� a=(1,4]
	//				(6) a<1, b=0 --> ��С�Աȶ� a=(0.1,1]
	//---------------------------------------------------------
	BOOL KDib::LinerTransform(float a,float b)
	{
		DWORD i,j;

		KDib* pDib = this;

		if(!pDib || pDib->IsEmpty())
		{
			return TRUE;
		}

		// ��ɫλ��
		WORD wBitCount = pDib->BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		// ����ͨ����
		int ch = wBitCount / 8;

		DWORD dwWidth = pDib->Width();
		DWORD dwHeight = pDib->Height();

		// ÿ�����ص��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*ch);

		// ͼ������ָ��
		LPBYTE lpDIBits = pDib->DIBits();

		PBYTE lpSrc=NULL;
		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				for(int k=0;k<ch;k++)
				{
					//ָ��DIB��(i,j)���ص�ָ��
					PBYTE lpPixel = lpDIBits + i*dwBytesPerLine + j*ch + k;

					//���Ա任
					float fTemp = (*lpPixel)*a + b;

					if(fTemp>255)		fTemp = 255;
					else if(fTemp<0)	fTemp = 0;
					else				fTemp = fTemp+0.5f;	//��������

					*lpPixel = (BYTE)(fTemp);
				}//k
			}//j
		}//i

		return TRUE;
	}

	//---------------------------------------------------------
	// ��    ��: LinearStrech
	// ��    ��: public
	// ��    ��: �ֶλҶ���������
	// ��    ��:
	//			 [i] x[2] - 
	//			 [i] y[2] - 
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - �ɹ�
	// ע    ��: 
	//---------------------------------------------------------
	BOOL KDib::LinearStrech(int x[2],int y[2])
	{
		DWORD i,j;

		// ��ɫλ��
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

		// ͼ������ָ��
		LPBYTE lpDIBits = DIBits();

		if(wBitCount == 8)
		{
			return LinearStrech_SingleChannel(lpDIBits,dwWidth,dwHeight,xx,yy);
		}

		// ����ǲ�ɫͼ��,���ÿ��ͨ�������ݷ��벢��������,
		// Ȼ���ٺϳɲ�ɫͼ��

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// ��ʱ������
		LPBYTE lpBuf = new BYTE[dwBytesPerLineSingleChannel * dwHeight];

		for(int k=0;k<3;k++)
		{// ��ͨ��
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
	// ��    ��: LogarithmStrech
	// ��    ��: public
	// ��    ��: ��������
	// ��    ��:
	//			 [i] a - 
	//			 [i] b - 
	//			 [i] c - 
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - �ɹ�
	// ע    ��: 
	//			 y = a + ln(x+1)/(b*lnc)
	//			 b!=0 c!=1
	//---------------------------------------------------------
	BOOL KDib::LogarithmStrech(double a,double b,double c)
	{
		DWORD i,j;

		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		// ����Ҫ��
		if(b==0 || c==1)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ͼ������ָ��
		LPBYTE lpDIBits = DIBits();

		if(wBitCount == 8)
		{
			return LogarithmStrech_SingleChannel(lpDIBits,dwWidth,dwHeight,a,b,c);
		}

		// ����ǲ�ɫͼ��,���ÿ��ͨ�������ݷ��벢��������,
		// Ȼ���ٺϳɲ�ɫͼ��

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// ��ʱ������
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
	// ��    ��: ExponentStrech
	// ��    ��: public
	// ��    ��: ָ������
	// ��    ��:
	//			 [i] a - 
	//			 [i] b - 
	//			 [i] c - 
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - �ɹ�
	// ע    ��: 
	//			 y = b^[c*(x-a)] - 1
	//---------------------------------------------------------
	BOOL KDib::ExponentStrech(double a,double b,double c)
	{
		DWORD i,j;

		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ͼ������ָ��
		LPBYTE lpDIBits = DIBits();

		if(wBitCount == 8)
		{
			return ExponentStrech_SingleChannel(lpDIBits,dwWidth,dwHeight,a,b,c);
		}

		// ����ǲ�ɫͼ��,���ÿ��ͨ�������ݷ��벢��������,
		// Ȼ���ٺϳɲ�ɫͼ��

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// ��ʱ������
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
	// ��    ��: ComputeHistogram
	// ��    ��: public
	// ��    ��: ͼ��ֱ��ͼͳ��
	// ��    ��:
	//			 [o] histogram - ���δ�Ÿ�ͨ���ĻҶ�ͳ������
	//			     histogram[3] - ����ͨ��ͳ������
	//			     histogram[2] - ��ɫͨ��ͳ������
	//			     histogram[1] - ��ɫͨ��ͳ������
	//			     histogram[0] - ��ɫͨ��ͳ������ / �Ҷ�ͼ�ĻҶ�ͳ������
	//
	//			 [o] average - ���δ�Ÿ�ͨ���ľ�ֵ
	//				 average[3] - ����ͨ����ֵ
	//				 average[2] - ��ɫͨ����ֵ
	//				 average[1] - ��ɫͨ����ֵ
	//				 average[0] - ��ɫͨ����ֵ / �Ҷ�ͼ�ĻҶȾ�ֵ
	//
	//			 [o] deviation - ���δ�Ÿ�ͨ���ķ���
	//				 deviation[3] - ����ͨ������
	//				 deviation[2] - ��ɫͨ������
	//				 deviation[1] - ��ɫͨ������
	//				 deviation[0] - ��ɫͨ������ / �Ҷ�ͼ�ĻҶȷ���
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
	//---------------------------------------------------------
	BOOL KDib::HistogramCompute(int histogram[4][256],double average[4] ,double deviation[4])
	{
		// ��ɫλ��
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

	// ͼ��ֱ��ͼ����
	BOOL KDib::HistogramAverage(BOOL bRed /*= TRUE*/,BOOL bGreen /*= TRUE*/,BOOL bBlue /*= TRUE*/)
	{
		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		if(wBitCount==24 && !bRed && !bGreen && !bBlue)
		{// ��ɫͼ��������һ��ͨ��
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
	// ��    ��: Smooth_Neighbour
	// ��    ��: public
	// ��    ��: ͼ��ƽ�� - ����ƽ����
	// ��    ��:
	//			 [i] iDimOfTemplate - ģ��ά��,Ĭ�� = 3 (3 5 7 9)
	//			 [i] byFill - ��Ե�����ֵ,Ĭ�� = 0
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: Ĭ��ģ��������ʾ
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

		// ����ģ��
		DWORD dwSizeTmplatte = iDimOfTemplate * iDimOfTemplate;
		double* pTemplate = new double[dwSizeTmplatte];
		for(i=0;i<dwSizeTmplatte;i++)
		{
			*(pTemplate+i) = 1.0;
		}

		int w = iDimOfTemplate;			// ģ����
		int h = iDimOfTemplate;			// ģ��߶�

		int x0 = iDimOfTemplate / 2;	// ģ������Ԫ�� X ����
		int y0 = iDimOfTemplate / 2;	// ģ������Ԫ�� Y ����

		// ģ��ϵ��
		double coef = 1.0 / (dwSizeTmplatte);

		// ��ɫλ��
		WORD wBitCount = pDib->BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = pDib->Width();
		DWORD dwHeight = pDib->Height();

		//// ����ͨ����
		int ch = wBitCount / 8;

		// ÿ�����ص��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*ch);

		// ��ͨ��ÿ�����ص��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth*8);

		// ͼ������ָ��
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
	// ��    ��: Smooth_power
	// ��    ��: public
	// ��    ��: ͼ��ƽ�� - ��Ȩƽ����
	// ��    ��:
	//			 [i] iTemplate - ģ��ά��,Ĭ�� = 5 (5 10 16 48)
	//			 [i] byFill - ��Ե�����ֵ,Ĭ�� = 0
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	//
	// ע    ��: Ĭ��ģ��������ʾ
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

		// ����ģ��
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

		// ��ɫλ��
		WORD wBitCount = pDib->BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = pDib->Width();
		DWORD dwHeight = pDib->Height();

		//// ����ͨ����
		int ch = wBitCount / 8;

		// ÿ�����ص��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*ch);

		// ��ͨ��ÿ�����ص��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth*8);

		// ͼ������ָ��
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

	// ͼ��ƽ�� - ����Ӧ�ֲ�ƽ��
	BOOL KDib::Smooth_Auto(BYTE byFill /*= 0*/)
	{
		DWORD i,j;

		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ����ͨ����
		int ch = wBitCount / 8;

		// ÿ�����ص��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*ch);

		// ��ͨ��ÿ�����ص��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth*8);

		// ͼ������ָ��
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

	// ͼ��ƽ�� - ��ֵ�˲�
	BOOL KDib::Smooth_MedianFilter(int iTemplate /*= 3*/,BYTE byFill /*= 0*/)
	{
		DWORD i,j;

		// ��ɫλ��
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

		// ����ͨ����
		int ch = wBitCount / 8;

		// ÿ�����ص��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*ch);

		// ��ͨ��ÿ�����ص��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth*8);

		// ͼ������ָ��
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

	// �� - �ݶ���
	BOOL KDib::Sharp_Grad(BYTE byThreshold /*= 0*/)
	{
		DWORD i,j;

		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ����ͨ����
		int ch = wBitCount / 8;

		// ÿ�����ص��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*ch);

		// ��ͨ��ÿ�����ص��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth*8);

		// ͼ������ָ��
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

	// ͼ���� - ������˹ģ����
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
		//����ģ��0
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

		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=8 && wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ����ͨ����
		int ch = wBitCount / 8;

		// ÿ�����ص��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8*ch);

		// ��ͨ��ÿ�����ص��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth*8);

		// ͼ������ָ��
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
	// ��    ��: AdjustColorRgb
	// ��    ��: public
	// ��    ��: ����DIB��ɫ - RGB
	// ��    ��:
	//			 [i] dRScale - ��ɫ�������ű���[-1,1]
	//			 [i] dGScale - ��ɫ�������ű���[-1,1]
	//			 [i] dBScale - ��ɫ�������ű���[-1,1]
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
	//---------------------------------------------------------
	BOOL KDib::AdjustColorRgb(double dRScale, double dGScale, double dBScale)
	{
		DWORD i,j;

		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=24)
		{
			return FALSE;
		}

		// ͼ�����ݳߴ�
		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ͨ������
		DWORD dwChannel = wBitCount / 8;

		// ͼ������ָ��
		LPBYTE lpDIBits = DIBits();

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * dwChannel * 8);

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// ��ʱ������(һ��ͨ��)
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

			// ����ͨ������
			AdjustColorRgb_SingleChannel(lpBuf,dwWidth,dwHeight,scale);

			// ����
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
	///  _protected                         �����Ǳ�����Ա����
	///============================================================================

	//---------------------------------------------------------
	// ��    ��: DIB2DDB
	// ��    ��: protected
	// ��    ��: DIB ת��Ϊ DDB
	// ��    ��:
	// ��    ��: HBITMAP ���
	// ע    ��: 
	//---------------------------------------------------------
	HBITMAP KDib::DIB2DDB()
	{
		if (m_dwSizeImage == 0) return NULL;	//û������

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
	// ��    ��: DDB2DIB
	// ��    ��: public
	// ��    ��: DDB ת��Ϊ DIB
	// ��    ��:
	//			 [i] hBitmap - λͼ���
	//			 [i] hPalette - ��ɫ����
	//			 [i] dwCompression - ѹ����ʽ
	// ��    ��: HBITMAP ���
	// ע    ��: 
	//---------------------------------------------------------
	HANDLE KDib::DDB2DIB(HBITMAP hBitmap,HPALETTE hPalette /*= NULL*/,DWORD dwCompression /*= BI_RGB*/)
	{
		if(hBitmap == NULL || dwCompression == BI_BITFIELDS)
			return NULL;

		Empty();

		// ȡ������ DC
		HDC hDC=::GetDC(NULL);

		// ��ɫ��
		HPALETTE hOldPal = NULL;
		HPALETTE hPal = hPalette;
		if(hPal==NULL)
		{
			//hPal=(HPALETTE)::GetStockObject(DEFAULT_PALETTE);
			hPal = ::CreateHalftonePalette(hDC);	//��ɫ��ɫ��
		}

		// Ӧ�õ�ɫ��
		hOldPal=::SelectPalette(hDC,hPal,FALSE);
		UINT uEntries = ::RealizePalette(hDC);	//���ص�ɫ���������

		//DDB - ȡ�� HBITMAP ��Ϣ
		BITMAP bm;
		::GetObject(hBitmap,sizeof(BITMAP),(LPSTR)&bm);

		//DIB - ��ʼ�� BitmapInfoHead
		BITMAPINFOHEADER bi;

		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = bm.bmWidth;
		bi.biHeight = bm.bmHeight;
		bi.biPlanes = 1;
		bi.biBitCount = bm.bmPlanes*bm.bmBitsPixel;
		bi.biCompression = dwCompression;			//BI_RGB
		bi.biSizeImage = 0;							//��GetDIBits()����
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;

		// ������ɫ�������
		int nColorEntrys = (bi.biBitCount > 8) ? 0 : (1 << bi.biBitCount);

		// ���� DIB ͷ�ߴ�
		DWORD dwDIBHeaderSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColorEntrys;

		// ���� DIB ͷ�ڴ�,����������
		PBYTE pByte = new BYTE[dwDIBHeaderSize];
		if(!pByte)
		{
			::SelectPalette(hDC,hOldPal,FALSE);
			::ReleaseDC(NULL,hDC);
			return NULL;
		}

		LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)pByte;
		*lpbi = bi;

		// ���� GetDIBits() ����
		// ��� lpBits = NULL, GetDIBits() ���Զ����� biSizeImage
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

		// fill the biSizeImage field �õ�biSizeImage
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

		// DIB �ߴ� = DIBͷ + DIB����
		DWORD dwDIBSize = dwDIBHeaderSize + bi.biSizeImage;

		// ���� DIB �ڴ�
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

		// �����ڴ�
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

		AttachMemory(pDib,TRUE);	// TRUE ��ʾ�������������ڴ� pDib

		return (HANDLE)pDib;
	}

	//---------------------------------------------------------
	// ��    ��: DDB2DIB
	// ��    ��: public
	// ��    ��: DDB ת��Ϊ DIB
	// ��    ��:
	//			 [i] hBitmap - λͼ���
	//			 [i] bForce256 - ǿ��ת��Ϊ256ɫλͼ
	//			 [i] pbmData - ����ָ��
	// ��    ��: HBITMAP ���
	// ע    ��: 
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

		// �����ڴ� header + color table + picture data
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
	// ��    ��: AttachMapFile
	// ��    ��: protected
	// ��    ��: ����ͼ���ļ�
	// ��    ��:
	//			 [i] lpszPathName - ͼ���ļ�
	//			 [i] bShare - �Ƿ����,Ĭ��ΪFALSE
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: �����ڴ�ӳ���ļ�
	//---------------------------------------------------------
	BOOL KDib::AttachMapFile(LPCTSTR lpszPathName)
	{
		// �ȷ����ڴ�ӳ���ļ�
		DetachMapFile();
		//--------------------------------------------
		// ��3������ӳ��һ���ڴ�ӳ���ļ�
		// 1. �������һ���ļ��ں˶���,�ö����ʾ��Ҫ�����ڴ�ӳ���ļ��Ĵ����ļ�
		HANDLE hFile = ::CreateFile(lpszPathName,					// FileName
									GENERIC_WRITE | GENERIC_READ,	// Desire Access
									//bShare ? FILE_SHARE_READ : 0,	// Share mode
									0,	// Share mode
									NULL,							// SECURITY_ATTRIBUTES Point
									OPEN_EXISTING,					// How to create
									FILE_ATTRIBUTE_NORMAL,			// File attributes
									NULL);							// Specifies a handle with GENERIC_READ access to a template file

		if(hFile == INVALID_HANDLE_VALUE) return FALSE;

		// 2. ����һ���ļ�ӳ���ں˶���������ϵͳ���ļ��Ĵ�С�Լ���η���
		HANDLE hMap = ::CreateFileMapping(hFile,			// Ҫӳ����ļ����
										  NULL,				// ��ȫ����
										  PAGE_READWRITE,	// ���ļ�ӳ��ı���
										  0,				// high-order 32 bits of object size
										  0,				// low-order 32 bits of object size
										  NULL);			// if NULL, the mapping object is created without a name

		if(hMap == NULL) return FALSE;

		// 3. ӳ�������ļ������ý��̵ĵ�ַ�ռ�
		LPVOID lpvMapFile = ::MapViewOfFile(hMap,			// file-mapping object 
											FILE_MAP_WRITE,	// access mode 
											0,				// high-order 32 bits of object size
											0,				// low-order 32 bits of object size
											0);				// number of bytes to map,if zero, the entire file is mapped

		if(lpvMapFile == NULL) return FALSE;
		//--------------------------------------------
		if(((LPBITMAPFILEHEADER)lpvMapFile)->bfType != BMP_HEADER_MARKER)
		{// ����BMP�ļ�
			DetachMapFile();
			return FALSE;
		}
		//--------------------------------------------
		// ʹ���ڴ�ӳ���ļ������ͷ��ڴ�
		if(!AttachMemory((LPBYTE) lpvMapFile + sizeof(BITMAPFILEHEADER),FALSE))
		{// ���� BITMAPFILEHEADER, ���� DIB ����
			return FALSE;
		}
		//--------------------------------------------
		m_hFile = hFile;			// �ļ����
		m_hMap = hMap;				// �ڴ�ӳ���ļ����
		m_lpvMapFile = lpvMapFile;	// �ڴ�ӳ���ļ�ָ��

		m_bEmpty = FALSE;			// �ǿ�
		//--------------------------------------------
		return TRUE;
	}

	//---------------------------------------------------------
	// ��    ��: DetachFile
	// ��    ��: protected
	// ��    ��: �����ڴ�ӳ���ļ�
	// ��    ��:
	// ��    ��: 
	// ע    ��: 
	//---------------------------------------------------------
	void KDib::DetachMapFile()
	{
		if(m_hFile == NULL) return;
		//--------------------------------------------
		// �����ڴ�ӳ���ļ��� 3 ����
		// 1. ȡ�����ļ�ӳ���ں˶����ӳ��
		::UnmapViewOfFile(m_lpvMapFile);

		// 2. �ر��ļ�ӳ���ں˶���
		::CloseHandle(m_hMap);

		// 3. �ر��ļ��ں˶���
		::CloseHandle(m_hFile);
		//--------------------------------------------
		m_hFile = NULL;
	}

	//---------------------------------------------------------
	// ��    ��: ComputeMetrics
	// ��    ��: protected
	// ��    ��: ����DIB���ݳߴ�
	// ��    ��:
	//			 [i] lpBitmapInfoHeader - LPBITMAPINFOHEADER
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	//
	// ע    ��: ���� 
	//			 m_dwWidth - ͼ����
	//			 m_dwHeight - ͼ��߶�
	//			 m_nBitCount - ͼ��λ��
	//			 m_dwSizeImage - DIB �����ֽ���
	//---------------------------------------------------------
	BOOL KDib::ComputeMetrics(LPBITMAPINFOHEADER lpBitmapInfoHeader)
	{
		if(lpBitmapInfoHeader->biSize != sizeof(BITMAPINFOHEADER))
		{// ���� BITMAPINFOHEADER �ṹ
			return FALSE;
		}

		// ͼ���Ⱥ͸߶�
		m_dwWidth = (DWORD)lpBitmapInfoHeader->biWidth;
		m_dwHeight = (DWORD)lpBitmapInfoHeader->biHeight;

		// ͼ��λ��
		m_nBitCount = lpBitmapInfoHeader->biBitCount;

		// ÿ�������ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(m_dwWidth * m_nBitCount);
		DWORD dwSizeImage = dwBytesPerLine * m_dwHeight;

		// DIB �����ֽ���
		m_dwSizeImage = lpBitmapInfoHeader->biSizeImage;
		if(m_dwSizeImage==0)
		{// ���� DIB �����ֽ���
			lpBitmapInfoHeader->biSizeImage = dwSizeImage;
			m_dwSizeImage = dwSizeImage;
		}

		return TRUE;
	}

	//---------------------------------------------------------
	// ��    ��: ComputePaletteSize
	// ��    ��: protected
	// ��    ��: �����ɫ��ߴ�(��ɫ�������)
	// ��    ��:
	//			 [i] iRChannel - ��ɫͨ��
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
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
	// ��    ��: MakePalette
	// ��    ��: protected
	// ��    ��: �� DIB ��ɫ�����ɵ�ɫ��
	// ��    ��:
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
	//---------------------------------------------------------
	HPALETTE KDib::MakePalette()
	{
		if(m_hPalette != NULL)
		{
			::DeleteObject(m_hPalette);
			m_hPalette = NULL;
		}

		// û�е�ɫ��
		if(m_dwColorTableEntries == 0) return NULL;

		// ��ɫ��ָ��
		LPRGBQUAD pDibQuad = (LPRGBQUAD)m_lpvColorTable;

		// �߼���ɫ��
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
		//���ɵ�ɫ��
		m_hPalette = ::CreatePalette(pLogPal);

		delete []pLogPal;

		return m_hPalette;
	}

	//---------------------------------------------------------
	// ��    ��: LinearStrech_SingleChannel
	// ��    ��: protected
	// ��    ��: ��ͨ���ֶλҶ���������
	// ��    ��:
	//			 [i] lpDIBits - 
	//			 [i] dwWidth - 
	//			 [i] dwHeight - 
	//			 [i] dwHeight - 
	//			 [i] x[2] - 
	//			 [i] y[2] - 
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
	//---------------------------------------------------------
	BOOL KDib::LinearStrech_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,BYTE x[2],BYTE y[2])
	{
		DWORD i,j;

		if(lpDIBits==NULL || dwWidth==0	|| dwHeight==0) 
			return FALSE;

		// x ����Ϊ 0 �� 255 ���� x[0] != x[1]
		if(x[0]>=255 || x[1]>=255 || x[0]==0 || x[1]==0 || x[0]==x[1])
			return FALSE;

		if(y[0]>255 || y[1]>255)
			return FALSE;

		// ÿ�����ص��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);

		// (x1,y1) �� (x2,y2) Ϊ�����ֶε�����
		double x1 = x[0];
		double y1 = y[0];

		double x2 = x[1];
		double y2 = y[1];

		// �����ֶε�ֱ��б��
		double slop1 = y1/x1;
		double slop2 = (y2-y1)/(x2-x1);
		double slop3 = (255.0-y2)/(255.0-x2);

		// ����ֱ�������б��
		double dis1 = 0;
		double dis2 = y1 - slop2 * x1;
		double dis3 = y2 - slop3 * x2;

		// ӳ���
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
	// ��    ��: LogarithmStrech_SingleChannel
	// ��    ��: protected
	// ��    ��: ��ͨ�������任
	// ��    ��:
	//			 [i] lpDIBits - 
	//			 [i] dwWidth - 
	//			 [i] dwHeight - 
	//			 [i] dwHeight - 
	//			 [i] x[2] - 
	//			 [i] y[2] - 
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
	//			 y = a + ln(x+1)/(b*lnc)
	//			 b!=0 c!=1
	//---------------------------------------------------------
	BOOL KDib::LogarithmStrech_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double a,double b,double c)
	{
		DWORD i,j;

		// ÿ�����ص��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwBytesPerLine;j++)
			{
				// �任�Ҷ�
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
	// ��    ��: ExponentStrech_SingleChannel
	// ��    ��: protected
	// ��    ��: ��ͨ��ָ������
	// ��    ��:
	//			 [i] lpDIBits - 
	//			 [i] dwWidth - 
	//			 [i] dwHeight - 
	//			 [i] dwHeight - 
	//			 [i] x[2] - 
	//			 [i] y[2] - 
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
	//			 y = b^[c*(x-a)] - 1
	//---------------------------------------------------------
	BOOL KDib::ExponentStrech_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double a,double b,double c)
	{
		DWORD i,j;

		// ÿ�����ص��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwBytesPerLine;j++)
			{
				// �任�Ҷ�
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
	// ��    ��: HistogramCompute_SingleChannel
	// ��    ��: protected
	// ��    ��: ͼ��ͨ��ֱ��ͼͳ��
	// ��    ��:
	//			 [i] lpDIBits - 
	//			 [i] dwWidth - 
	//			 [i] dwHeight - 
	//			 [i] dwHeight - 
	//			 [i] x[2] - 
	//			 [i] y[2] - 
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
	//---------------------------------------------------------
	BOOL KDib::HistogramCompute_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,int histogram[256],double& average ,double& deviation)
	{
		DWORD i,j;

		// ֱ��ͼ��������
		for(i=0;i<256;i++)
		{
			histogram[i] = 0;
		}

		// ÿ�����ص��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8);

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				histogram[*(lpDIBits + i*dwBytesPerLine + j)]++;
			}
		}

		// ƽ��ֵ
		average = 0;
		for(i=0;i<256;i++)
		{
			average += i*histogram[i];
		}
		average /= dwWidth*dwHeight;

		// ����
		deviation = 0;
		for(i=0;i<256;i++)
		{
			deviation += (i-average) * (i-average)*histogram[i];
		}
		deviation /= dwWidth*dwHeight;
		deviation = sqrt(deviation);

		return TRUE;
	}

	// ͼ��Ҷ�ֱ��ͼͳ��
	BOOL KDib::HistogramCompute_Gray(int histogram[256],double& average ,double& deviation)
	{
		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=8)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ͼ������ָ��
		LPBYTE lpDIBits = DIBits();

		return HistogramCompute_SingleChannel(lpDIBits,dwWidth,dwHeight,histogram,average,deviation);
	}

	// ͼ������ֱ��ͼͳ��
	BOOL KDib::HistogramCompute_Bright(int histogram[256],double& average ,double& deviation)
	{
		DWORD i,j;

		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ͼ������ָ��
		LPBYTE lpDIBits = DIBits();

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// ��ʱ������
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

	// ͼ���ɫֱ��ͼͳ��
	BOOL KDib::HistogramCompute_Red(int histogram[256],double& average ,double& deviation)
	{
		DWORD i,j;

		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ͼ������ָ��
		LPBYTE lpDIBits = DIBits();

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// ��ʱ������
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

	// ͼ����ɫֱ��ͼͳ��
	BOOL KDib::HistogramCompute_Green(int histogram[256],double& average ,double& deviation)
	{
		DWORD i,j;

		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ͼ������ָ��
		LPBYTE lpDIBits = DIBits();

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// ��ʱ������
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

	// ͼ����ɫֱ��ͼͳ��
	BOOL KDib::HistogramCompute_Blue(int histogram[256],double& average ,double& deviation)
	{
		DWORD i,j;

		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ͼ������ָ��
		LPBYTE lpDIBits = DIBits();

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// ��ʱ������
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

	// ͼ��ͨ��ֱ��ͼ����
	BOOL KDib::HistogramAverage_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight)
	{
		DWORD i,j;

		int hist[256];
		double avg,dev;

		if(!HistogramCompute_SingleChannel(lpDIBits,dwWidth,dwHeight,hist,avg,dev))
		{
			return FALSE;
		}

		// ÿ�����ص��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8);

		// ӳ���
		double map[256];

		// ����ӳ���
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
	// ͼ��Ҷ�ֱ��ͼ����
	BOOL KDib::HistogramAverage_Gray()
	{
		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=8)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ͼ������ָ��
		LPBYTE lpDIBits = DIBits();

		return HistogramAverage_SingleChannel(lpDIBits,dwWidth,dwHeight);
	}

	// ͼ��ͨ��ֱ��ͼ���� - Red
	BOOL KDib::HistogramAverage_Red()
	{
		DWORD i,j;

		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ͼ������ָ��
		LPBYTE lpDIBits = DIBits();

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// ��ʱ������
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

	// ͼ��ͨ��ֱ��ͼ���� - Green
	BOOL KDib::HistogramAverage_Green()
	{
		DWORD i,j;

		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ͼ������ָ��
		LPBYTE lpDIBits = DIBits();

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// ��ʱ������
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

	// ͼ��ͨ��ֱ��ͼ���� - Blue
	BOOL KDib::HistogramAverage_Blue()
	{
		DWORD i,j;

		// ��ɫλ��
		WORD wBitCount = BitCount();
		if(wBitCount!=24)
		{
			return FALSE;
		}

		DWORD dwWidth = Width();
		DWORD dwHeight = Height();

		// ͼ������ָ��
		LPBYTE lpDIBits = DIBits();

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth * 8 * 3);

		// ��ͨ��ÿ������ռ�õ��ֽ���
		DWORD dwBytesPerLineSingleChannel = DIBWIDTHBYTES(dwWidth * 8);

		// ��ʱ������
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
	// ��    ��: AdjustColorRgb_SingleChannel
	// ��    ��: protected
	// ��    ��: ��ͨ����ɫ����
	// ��    ��:
	//			 [i] lpDIBits - 
	//			 [i] dwWidth - 
	//			 [i] dwHeight - 
	//			 [i] dScale - �任����[0,1]
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
	//---------------------------------------------------------
	BOOL KDib::AdjustColorRgb_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double dScale)
	{
		DWORD i,j;

		// ÿ�����ص��ֽ���
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
	// ��    ��: Smooth_Template_SingleChannel
	// ��    ��: protected
	// ��    ��: ��ͨ��ͼ��ƽ�� - ģ�巨
	// ��    ��:
	//			 [i] lpDIBits - DIB ����ָ��
	//			 [i] dwWidth - DIB ���ݿ��
	//			 [i] dwHeight - DIB ���ݸ߶�
	//
	//			 [i] pTemplate - ģ��ָ��
	//			 [i] w - ģ����
	//			 [i] h - ģ��߶�
	//			 [i] x0 - ģ������Ԫ�� X ����
	//			 [i] y0 - ģ������Ԫ�� Y ����
	//			 [i] coef - ģ��ϵ��
	//
	//			 [i] byFill - ��Ե�����ֵ
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - ʧ��
	// ע    ��: 
	//---------------------------------------------------------
	BOOL KDib::Smooth_Template_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double* pTemplate,int w,int h,int x0,int y0,double coef,BYTE byFill)
	{
		DWORD i,j;

		// ÿ�����ص��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);

		// ����ͨ������
		LPBYTE lpDIBits_bak = new BYTE[dwBytesPerLine*dwHeight];
		memcpy(lpDIBits_bak,lpDIBits,sizeof(BYTE)*dwBytesPerLine*dwHeight);

		for(i=0; i<dwHeight; i++)
		{
			for(j=0; j<dwWidth; j++)
			{
				LPBYTE lpDest = lpDIBits + dwBytesPerLine*(dwHeight-1-i) + j;

				// ȥ����Ե�� y0 �� x0 ��
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

						// �����Ȩƽ��
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

	// ��ͨ��ͼ��ƽ�� - ����Ӧ�ֲ�ƽ��
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

		// ÿ�����ص��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);

		LPBYTE lpDIBits_bak = new BYTE[dwBytesPerLine*dwHeight];
		memcpy(lpDIBits_bak,lpDIBits,sizeof(BYTE)*dwBytesPerLine*dwHeight);

		double mean[9];		// ��ֵ
		double bunsan[9];	// ��ֵ�ķ���

		double dtmp[9];
		for(row=0; row<dwHeight;row++)
		{
			for(col=0; col<dwWidth; col++)
			{
				// ȥ����Ե�� y0 �� x0 ��
				if((int)row<2 || (int)row>=dwHeight-2 || (int)col<2 || (int)col>=dwWidth-2)
				{
					lpDIBits[ dwBytesPerLine * row + col ] = byFill;
					continue;
				}

				///�� 9 �ֽ�����ľ�ֵ�ͷ���
				//------------------------------------
				// �� 0 ��
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

				// ��ֵ
				mean[k] = 0;
				for(i=0;i<9;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 9;

				// ��ֵ�ķ���
				bunsan[k] = 0;
				for(i=0;i<9;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// �� 1 ��
				k = 1;
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col-2) ];
				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col-1) ];

				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col-2) ];
				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col-1) ];
				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];

				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col-2) ];
				dtmp[6] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col-1) ];

				// ��ֵ
				mean[k] = 0;
				for(i=0;i<7;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 7;

				// ��ֵ�ķ���
				bunsan[k] = 0;
				for(i=0;i<7;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// �� 2 ��
				k = 2;
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row-2) + (col-1) ];
				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row-2) + (col+0) ];
				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row-2) + (col+1) ];

				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col-1) ];
				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+0) ];
				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+1) ];

				dtmp[6] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];

				// ��ֵ
				mean[k] = 0;
				for(i=0;i<7;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 7;

				// ��ֵ�ķ���
				bunsan[k] = 0;
				for(i=0;i<7;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// �� 3 ��
				k = 3;
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+1) ];
				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+2) ];

				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];
				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+1) ];
				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+2) ];

				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+1) ];
				dtmp[6] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+2) ];

				// ��ֵ
				mean[k] = 0;
				for(i=0;i<7;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 7;

				// ��ֵ�ķ���
				bunsan[k] = 0;
				for(i=0;i<7;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// �� 4 ��
				k = 4;
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];

				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col-1) ];
				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+0) ];
				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+1) ];

				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row+2) + (col-1) ];
				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row+2) + (col+0) ];
				dtmp[6] = lpDIBits_bak[ dwBytesPerLine * (row+2) + (col+1) ];

				// ��ֵ
				mean[k] = 0;
				for(i=0;i<7;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 7;

				// ��ֵ�ķ���
				bunsan[k] = 0;
				for(i=0;i<7;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// �� 5 ��
				k = 5;
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row-2) + (col-2) ];
				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row-2) + (col-1) ];

				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col-2) ];
				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col-1) ];
				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+0) ];

				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col-1) ];
				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];

				// ��ֵ
				mean[k] = 0;
				for(i=0;i<7;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 7;

				// ��ֵ�ķ���
				bunsan[k] = 0;
				for(i=0;i<7;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// �� 6 ��
				k = 6;
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];
				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+1) ];

				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+0) ];
				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+1) ];
				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row-1) + (col+2) ];

				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row-2) + (col+1) ];
				dtmp[6] = lpDIBits_bak[ dwBytesPerLine * (row-2) + (col+2) ];

				// ��ֵ
				mean[k] = 0;
				for(i=0;i<7;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 7;

				// ��ֵ�ķ���
				bunsan[k] = 0;
				for(i=0;i<7;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// �� 7 ��
				k = 7;
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];
				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+1) ];

				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+0) ];
				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+1) ];
				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+2) ];

				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row+2) + (col+1) ];
				dtmp[6] = lpDIBits_bak[ dwBytesPerLine * (row+2) + (col+2) ];

				// ��ֵ
				mean[k] = 0;
				for(i=0;i<7;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 7;

				// ��ֵ�ķ���
				bunsan[k] = 0;
				for(i=0;i<7;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// �� 8 ��
				k = 8;
				dtmp[0] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col+0) ];
				dtmp[1] = lpDIBits_bak[ dwBytesPerLine * (row+0) + (col-1) ];

				dtmp[2] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col+0) ];
				dtmp[3] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col-1) ];
				dtmp[4] = lpDIBits_bak[ dwBytesPerLine * (row+1) + (col-2) ];

				dtmp[5] = lpDIBits_bak[ dwBytesPerLine * (row+2) + (col-1) ];
				dtmp[6] = lpDIBits_bak[ dwBytesPerLine * (row+2) + (col-2) ];

				// ��ֵ
				mean[k] = 0;
				for(i=0;i<7;i++)
				{
					mean[k] += dtmp[i];
				}
				mean[k] /= 7;

				// ��ֵ�ķ���
				bunsan[k] = 0;
				for(i=0;i<7;i++)
				{
					bunsan[k] += dtmp[i]*dtmp[i] - mean[k]*mean[k];
				}
				//------------------------------------
				// �󷽲���С�Ľ�����
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

	// ͼ��ƽ�� - ��ֵ�˲�
	BOOL KDib::Smooth_MedianFilter_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,int w,int h,int x0,int y0,BYTE byFill)
	{
		DWORD i,j;

		// ÿ�����ص��ֽ���
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
				// ȥ����Ե�� y0 �� x0 ��
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

	// ��ͨ��ͼ���� - �ݶ���
	BOOL KDib::Sharp_Grad_SingleChanne(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,BYTE byThreshold)
	{
		DWORD i,j;

		// ÿ�����ص��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);

		// ����ͨ������
		LPBYTE lpDIBits_bak = new BYTE[dwBytesPerLine*dwHeight];
		memcpy(lpDIBits_bak,lpDIBits,sizeof(BYTE)*dwBytesPerLine*dwHeight);

		for(i=0;i<dwHeight;i++)
		{
			for(j=0;j<dwWidth;j++)
			{
				LPBYTE lpDst = lpDIBits + dwBytesPerLine * i +j;

				// �����ݶ�����
				// i �� j ��
				LPBYTE lpSrc	= lpDIBits_bak + dwBytesPerLine * (i+0) + (j+0);
				// i+1 �� j ��
				LPBYTE lpSrc1	= lpDIBits_bak + dwBytesPerLine * (i+1) + (j+0);
				// i �� j+1 ��
				LPBYTE lpSrc2	= lpDIBits_bak + dwBytesPerLine * (i+0) + (j+1);

				// �ݶ�
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

	// ��ͨ��ͼ���� - ������˹ģ����
	BOOL KDib::Sharp_LapTemplate_SingleChanne(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double* pTemplate,int w,int h,int x0,int y0,double coef,BYTE byFill)
	{
		DWORD i,j;

		// ÿ�����ص��ֽ���
		DWORD dwBytesPerLine = DIBWIDTHBYTES(dwWidth*8);

		// ����ͨ������
		LPBYTE lpDIBits_bak = new BYTE[dwBytesPerLine*dwHeight];
		memcpy(lpDIBits_bak,lpDIBits,sizeof(BYTE)*dwBytesPerLine*dwHeight);

		for(i=0; i<dwHeight; i++)
		{
			for(j=0; j<dwWidth; j++)
			{
				//LPBYTE lpDest = lpDIBits + dwBytesPerLine*(dwHeight-1-i) + j;
				LPBYTE lpDest = lpDIBits + dwBytesPerLine*i + j;

				// ȥ����Ե�� y0 �� x0 ��
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
						// �����Ȩƽ��
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
	///  _private                         ������˽�г�Ա����
	///============================================================================

	// ������ֵ
	BYTE KDib::MedianValue(LPBYTE lpBuf,int len)
	{
		int i,j;

		BYTE byMedianValue = 0;

		// ð������
		for(j=0;j<len;j++)
		{
			for(i=0;i<len-j-1;i++)
			{
				if(lpBuf[i]>lpBuf[i+1])
				{// ����
					byMedianValue = lpBuf[i];
					lpBuf[i] = lpBuf[i+1];
					lpBuf[i+1] = byMedianValue;
				}
			}
		}

		if((len & 1)>0)
		{// ����
			byMedianValue = lpBuf[(len+1)/2];
		}
		else
		{// ż��
			byMedianValue = (lpBuf[len/2] + lpBuf[len/2+1]) / 2;
		}

		return byMedianValue;
	}

//---------------------------------------------------------
}// namespace Images - end
//---------------------------------------------------------

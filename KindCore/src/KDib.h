/******************************************************************************
 ** KImage.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KImage
 **  Version: 
 ** Function: 
 ** Explain: λͼ�ļ�ȥ�� BITMAPFILEHEADER ����һ��DIB
 **
 **		λͼ�ļ�ͷ	--> BITMAPFILEHEADER	-----------------|
 **											                 |
 **		λͼ��Ϣͷ	--> BITMAPINFOHEADER	--------|		 |
 **													|		 | bmp �ļ�
 **		��ɫ��		--> RGBQUAD                     | DIB	 |
 **                                                 |		 |
 ** 	����        --> IMAGE DATA			--------|--------|
 **
 **   Author: ������.֣�� (shenyczz@163.com)
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
			eBChannel = 0,	// ��ɫͨ��
			eGChannel,		// ��ɫͨ��
			eRChannel,		// ��ɫͨ��

			eYChannel,		// ����ͨ��
			eIChannel,		// ����ͨ��
			eQChannel,		// ���Ͷ�ͨ��
		};

		enum DibTransform : DWORD
		{
			DibTransform_Reverse			= 0x0001,	// ����
			DibTransform_Bright				= 0x0002,	// ����
			DibTransform_Contrast			= 0x0004,	// �Աȶ�
			DibTransform_ToGray				= 0x0008,	// ת��Ϊ�Ҷ�ͼ��
			DibTransform_Smooth				= 0x0010,	// ƽ��
			DibTransform_HistogramAverage	= 0x0020,	// ֱ��ͼ����
			DibTransform_Adjust_Color_Rgb	= 0x0100,	// ��ɫ���� - RGB
			DibTransform_Adjust_Color_Cmy	= 0x0200,	// ��ɫ���� - CMY
			DibTransform_Adjust_Color_Hsi	= 0x0400,	// ��ɫ���� - HSI
			DibTransform_Adjust_Color_Hls	= 0x0800,	// ��ɫ���� - HLS
		};

	public:
		KDib(void);
		virtual ~KDib(void);

	///========================================================
	///��Ա���� - Beg
	///---------------
	public:
		// �Ƿ�Ϊ��
		BOOL IsEmpty() { return m_bEmpty; }

		// �ǻҶ�ͼ��
		BOOL IsGray() { return m_nBitCount == 8; }

		// ��24λ��ɫͼ��
		BOOL IsColor24bits() { return m_nBitCount == 24; }

		// ��ȡ�ߴ�
		DWORD Width() { return m_dwWidth; }
		DWORD Height() { return m_dwHeight; }

		// DIB ͼ�����ݴ�С
		DWORD ImageSize() { return m_dwSizeImage; }

		// BITMAPINFOHEADER �ṹָ��
		LPBITMAPINFOHEADER BitmapInfoHeader() { return m_lpBitmapInfoHeader;}

		// ͼ������ָ��
		LPBYTE ImageData() { return m_lpImageData; }
		LPBYTE DIBits() { return m_lpImageData; }

		// ͼ��λ��
		WORD BitCount() { return m_nBitCount; }

		// ��ɫ�������
		DWORD ColorTableEntries() { return m_dwColorTableEntries; }
		LPVOID ColorTable() { return m_lpvColorTable; }

		// λͼ���
		HBITMAP Bitmap() { return m_hBitmap; }
		//HBITMAP GetBitmapHandel() { return m_hBitmap; }

		// ��ɫ����
		HPALETTE Palette() { return m_hPalette; }

		HANDLE GetSafeHandle() { return (HANDLE)m_lpBitmapInfoHeader; }

		// ����������
		operator HBITMAP() { return m_hBitmap; }

	public:/// ��������

		// ���DIB,�ͷ��Ѿ�������ڴ�,��Ҫʱ��ر�ӳ���ļ�
		void Empty();

		// ��ȡ�ļ�����
		BOOL Load(LPCTSTR lpszPathName);

		// д�ļ�
		BOOL Save(LPCTSTR lpszFile);

		// �����ڴ���Դ(����ע���Ƿ���Ҫ���ฺ���ڴ����)
		BOOL AttachMemory(LPVOID lpvMem,BOOL bMustDelete = FALSE);

		// ����ͼ���ļ�
		BOOL AttachFile(LPCTSTR lpszPathName,BOOL bMapfile = FALSE);

		// ���� HBITMAP
		BOOL AttachHBITMAP(HBITMAP hBitmap,HPALETTE hPalette = NULL,DWORD dwCompression = BI_RGB);

		// ���� DIB
		HGLOBAL Detch();

		// DIB ת��Ϊ DDB
		const HBITMAP GetHBITMAP();

		// ȡ�� DIB ͨ������
		LPBYTE DIBits(int iChannel);

		// Ӧ�õ�ɫ��
		UINT UsePalette(HDC hDC, BOOL bBackground  = FALSE);

		// ��׽�ͻ�������������ͼ��
		void Capture(HDC hDC, LPCRECT lpRect, HPALETTE hPalette = NULL, BOOL bForce256 = FALSE,int* pData = NULL);

		// ���ư�͸��ͼ��
		BOOL TranslucentBlt(HDC hDC, int x, int y, HBITMAP hBitmap,int nTranslucent);
		BOOL TranslucentBlt(int x, int y, HBITMAP hBitmap,int nTranslucent);

		// ����͸��ͼ��
		BOOL TransparentBlt(HDC hDC, int x, int y, HBITMAP hBitmap, UINT clrTransparent);
		BOOL TransparentBlt(int x, int y, HBITMAP hBitmap, UINT clrTransparent);

		// ��¡
		KDib* Clone();

		// ��ӡ
		void Print(HDC hDC,HANDLE hDIB = NULL,BOOL bMustDelete = FALSE);

	public: /// �Ҷȱ仯

		// �滻��ɫ��
		BOOL ReplaceColorTable(LPRGBQUAD lpRrgbQuad);

		// ��ɫͼ��任�ɻҶ�ͼ��
		BOOL Color2Gray(LPRGBQUAD lpRrgbQuad = NULL);

		// �Ҷ�ͼ��任�ɲ�ɫͼ��
		BOOL Gray2Color(LPRGBQUAD lpRrgbQuad = NULL);

		// ����
		BOOL Reverse();

		// ��ֵ��
		BOOL Binary(int iThreshold);

		// �Ҷ����Ա任
		BOOL LinerTransform(float a,float b);

		// �ֶλҶ���������
		BOOL LinearStrech(int x[2],int y[2]);

		// ��������
		BOOL LogarithmStrech(double a,double b,double c);

		// ָ������
		BOOL ExponentStrech(double a,double b,double c);

		// ͼ��ֱ��ͼͳ��
		BOOL HistogramCompute(int histogram[4][256],double average[4] ,double deviation[4]);

		// ͼ��ֱ��ͼ����
		BOOL HistogramAverage(BOOL bRed = TRUE,BOOL bGreen = TRUE,BOOL bBlue = TRUE);


	public: /// ��ɫ����

		// ��ɫ���� - RGB
		BOOL AdjustColorRgb(double dRScale, double dGScale, double dBScale);
		BOOL AdjustColorCmy();
		BOOL AdjustColorHsi();
		BOOL AdjustColorHls();


	public: /// ͼ����ǿ

		// ͼ��ƽ�� - ����ƽ����
		BOOL Smooth_Neighbour(int iDimOfTemplate =3,BYTE byFill = 0);

		// ͼ��ƽ�� - ��Ȩƽ����
		BOOL Smooth_Power(int iTemplate = 16,BYTE byFill = 0);

		// ͼ��ƽ�� - ����Ӧ�ֲ�ƽ��
		BOOL Smooth_Auto(BYTE byFill = 0);

		// ͼ��ƽ�� - ��ֵ�˲�
		BOOL Smooth_MedianFilter(int iTemplate = 3,BYTE byFill = 0);

		// ͼ���� - �ݶ���
		BOOL Sharp_Grad(BYTE byThreshold = 0);

		// ͼ���� - ������˹ģ����
		BOOL Sharp_LapTemplate(int iNumberOfTemplate = 0,BYTE byFill = 0);



	protected: /// ��������

		// ����ͼ���ļ�
		BOOL AttachMapFile(LPCTSTR lpszPathName);
		// �����ڴ�ӳ���ļ�
		void DetachMapFile();

		// ����DIB���ݳߴ�
		BOOL ComputeMetrics(LPBITMAPINFOHEADER lpBitmapInfoHeader);

		// �����ɫ��ߴ�(��ɫ�������)
		BOOL ComputePaletteSize(LPBITMAPINFOHEADER lpBitmapInfoHeader);

		// �� DIB ��ɫ�����ɵ�ɫ��
		HPALETTE MakePalette();

		// DIB ת��Ϊ DDB
		HBITMAP DIB2DDB();

		// DDB ת��Ϊ DIB
		HANDLE DDB2DIB(HBITMAP hBitmap,HPALETTE hPalette = NULL,DWORD dwCompression = BI_RGB);
		HANDLE DDB2DIB(HBITMAP hBitmap,BOOL bForce256,int *pbmData);

	protected: /// �Ҷȱ仯

		// ��ͨ���ֶλҶ���������
		BOOL LinearStrech_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,BYTE x[2],BYTE y[2]);

		// ��ͨ�������任
		BOOL LogarithmStrech_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double a,double b,double c);

		// ��ͨ��ָ������
		BOOL ExponentStrech_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double a,double b,double c);

		// ��ͨ��ͼ��ֱ��ͼͳ��
		BOOL HistogramCompute_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,int histogram[256],double& average ,double& deviation);
		// ͼ��Ҷ�ֱ��ͼͳ��
		BOOL HistogramCompute_Gray(int histogram[256],double& average ,double& deviation);
		// ͼ������ֱ��ͼͳ��
		BOOL HistogramCompute_Bright(int histogram[256],double& average ,double& deviation);
		// ͼ���ɫֱ��ͼͳ��
		BOOL HistogramCompute_Red(int histogram[256],double& average ,double& deviation);
		// ͼ����ɫֱ��ͼͳ��
		BOOL HistogramCompute_Green(int histogram[256],double& average ,double& deviation);
		// ͼ����ɫֱ��ͼͳ��
		BOOL HistogramCompute_Blue(int histogram[256],double& average ,double& deviation);

		// ��ͨ��ͼ��ֱ��ͼ����
		BOOL HistogramAverage_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight);
		// ͼ��Ҷ�ֱ��ͼ����
		BOOL HistogramAverage_Gray();
		// ��ͨ��ͼ��ֱ��ͼ���� - ��ɫ
		BOOL HistogramAverage_Red();
		// ��ͨ��ͼ��ֱ��ͼ���� - ��ɫ
		BOOL HistogramAverage_Green();
		// ��ͨ��ͼ��ֱ��ͼ���� - ��ɫ
		BOOL HistogramAverage_Blue();

	protected: /// ��ɫ����

		// ��ͨ����ɫ����
		BOOL AdjustColorRgb_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double dScale);


	protected: /// ͼ����ǿ

		// ��ͨ��ͼ��ƽ�� - ģ�巨
		BOOL Smooth_Template_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double* pTemplate,int w,int h,int x0,int y0,double coef,BYTE byFill);

		// ��ͨ��ͼ��ƽ�� - ����Ӧ�ֲ�ƽ��
		BOOL Smooth_Auto_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,BYTE byFill);

		// ��ͨ��ͼ��ƽ�� - ��ֵ�˲�
		BOOL Smooth_MedianFilter_SingleChannel(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,int w,int h,int x0,int y0,BYTE byFill);

		// ��ͨ��ͼ���� - �ݶ���
		BOOL Sharp_Grad_SingleChanne(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,BYTE byThreshold);

		// ��ͨ��ͼ���� - ������˹ģ����
		BOOL Sharp_LapTemplate_SingleChanne(LPBYTE lpDIBits,DWORD dwWidth,DWORD dwHeight,double* pTemplate,int w,int h,int x0,int y0,double coef,BYTE byFill);

	private:
		// ������ֵ
		BYTE MedianValue(LPBYTE lpBuf,int len);

	///---------------
	///��Ա���� - End
	///========================================================

	///========================================================
	///��Ա���� - Beg
	///---------------

	protected:
		BOOL				m_bEmpty;				// �Ƿ�Ϊ��DIB

		HANDLE				m_hFile;				// �򿪵��ļ����
		HANDLE				m_hMap;					// �ڴ�ӳ���ļ����
		LPVOID				m_lpvMapFile;			// �ڴ�ӳ���ļ�ָ��

		HBITMAP				m_hBitmap;				// λͼ���
		HPALETTE			m_hPalette;				// ��ɫ����

		LPBITMAPINFOHEADER	m_lpBitmapInfoHeader;	// BITMAPINFOHEADER �ṹָ��
		LPVOID				m_lpvColorTable;		// ��ɫ���ַָ��
		LPBYTE				m_lpImageData;			// DIB���ݿ�ʼ��ַ

		DWORD				m_dwColorTableEntries;	// ��ɫ�������
		DWORD				m_dwSizeImage;			// DIB���ݴ�С

		DWORD				m_dwWidth;				// ͼ����
		DWORD				m_dwHeight;				// ͼ��߶�

		WORD				m_nBitCount;			// ͼ��λ��

		BOOL				m_bMustDelete;			// �ɱ����ͷ� m_lpBitmapInfoHeader ָ��

		LPBYTE				m_lpBytesBuf;			// ͨ�����ݵ���ʱ������

	///---------------
	///��Ա���� - End
	///========================================================

	};

//---------------------------------------------------------
}// namespace Images - end
//---------------------------------------------------------

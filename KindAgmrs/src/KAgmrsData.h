/******************************************************************************
 ** KAgmrsData.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I know nothing.

 **     Name: KAgmrsData - 农业气象遥感数据
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 

 ** Modifier: 
 ** DateTime: 
 **  Explain: 

 **    Usage: 

              //1. 
              use

******************************************************************************/

#pragma once

#include "KAxinData.h"
#include "KAgmrsDataProcessor.h"
#include "KAgmrsThreshold.h"

namespace Agmrs
{
	class KIND_EXT_CLASS KAgmrsData : public KAxinData
	{
	public:
		KAgmrsData(void);
		virtual ~KAgmrsData(void);

	public:
		// 保存数据
		virtual BOOL SaveData(LPCTSTR lpszFile);

	protected:
		// 装载数据
		virtual BOOL LoadData_bin(LPCTSTR lpszFile);

	public:
		// 内存清理
		virtual void Empty();

		// 设置/获取数据通道
		virtual void SetDataChannel(int iRChannel,int iGChannel,int iBChannel);
		virtual void GetDataChannel(int* iRChannel,int* iGChannel,int* iBChannel);

		// 设置/获取默认数据通道
		virtual void SetDataChannelDef(int iRChannel,int iGChannel,int iBChannel);
		virtual void GetDataChannelDef(int* iRChannel,int* iGChannel,int* iBChannel);

		// 取得产品数据
		virtual PFLOAT* GetProductData();

		// 数据转换为 DIB
		virtual BOOL DataToDIB(int iRChannel = 0,int iGChannel = 0,int iBChannel = 0);

		// 合并数据通道
		virtual BOOL ChannelCombine(ChannelCombineInfo* pChannelCombineInfo1,
			ChannelCombineInfo* pChannelCombineInfo2,
			LPCTSTR lpszOutputFile,
			KExtents* pExtents);

	public:
		// 是否空
		BOOL IsEmpty() { return m_bEmpty; }

		// 数据指针
		PWORD* GetData() { return m_ppwData; }
		// 数据的灰度指针
		PBYTE* GetDataGray() { return m_ppucDataGray; }

		// 取得文件头
		PVOID FileHeader() { return m_pFileHeader; }

		// 取得通道映射
		AgmrsDataChannelMap* GetAgmrsDataChannelMap() { return &m_AgmrsDataChannelMap; }

		// 取得指定点的全部通道数据
		ChannelInfoVector* GetChannelInfo(double lon, double lat); 

	public:
		// 检测农气遥感数据
		BOOL IsAgmrsData();
		BOOL IsAvhrrData()	{ return AgmrsDataSource::Avhrr	== this->GetAgmrsDataSource(); }
		BOOL IsModisData()	{ return AgmrsDataSource::Modis	== this->GetAgmrsDataSource(); }
		BOOL IsMersiData()	{ return AgmrsDataSource::Mersi	== this->GetAgmrsDataSource(); }
		BOOL IsVirrData()	{ return AgmrsDataSource::Virr	== this->GetAgmrsDataSource(); }

		// 取得遥感数据源
		int GetAgmrsDataSource();

	public:
		// 计算 ATI 时使用
		// 设置白天数据文件
		LPCTSTR GetDayFile()				{ return m_szDayFile; }
		void SetDayFile(LPCTSTR lpszFile)	{ _tcscpy(m_szDayFile,lpszFile); }

		// 设置晚上数据文件
		LPCTSTR GetNightFile()				{ return m_szNightFile; }
		void SetNightFile(LPCTSTR lpszFile)	{ _tcscpy(m_szNightFile,lpszFile); }

	public:
		// 经纬度转换为格点坐标
		virtual BOOL LonLat2xy(double dLon,double dLat,int& x,int& y, int flag = 0);
		// 格点坐标转换为经纬度坐标
		virtual BOOL xy2LonLat(int x,int y,double& lon,double& lat, int flag = 0);
		// 取得产品格点值
		virtual float GetProductValue(double lon, double lat);

	public:
		// 产品生成 - 全照率
		BOOL Abe(int iProductID);
		// 产品生成 - 地表温度
		BOOL Lst(int iMethod);
		// 产品生成 - 植被指数
		BOOL Vix(int iProductID);
		// 产品生成 - 干旱指数
		BOOL Dix(int iProductID);
		// 单项监测 - SIM (Single Item Monitor)
		BOOL Sim(int iProductID);

		// 取得像元类型
		int GetPixelType(int i,int j);
		// 是否晴空(无云)
		BOOL IsClearNight(int i,int j);

		// 计算TVDI
		BOOL Dix_Tvdi(KDataInfo* pDataInfo,float* pDataNdvi,float* pDataLst,float*& pDataTvdi);

		// 命名产品
		virtual LPCTSTR NameProduct(int iProductID);

	public:
		///------------------------------------------------
		///纯虚函数 - Beg
		///---------------
		// 填充数据信息
		virtual void FillDataInfo(LPVOID lpvData) = 0;
		// 映射通道(通道号和数据索引对应)
		virtual void MapChannel() = 0;
		// 根据通道代码取得通道数据索引
		virtual int GetDataIndex(int iChannelCode) = 0;
		// 通道名称转换为通道代码
		virtual int ChannelName2ChannelCode(LPCTSTR lpszChannelName) = 0;

		// 取得判别阈值
		virtual void* GetThreshold() = 0;
		// 设置默认的显示通道
		virtual void SetDefaultDisplayChannel() = 0;
		// 通道号转换为通道代码
		virtual int Channel2ChannelCode(int iChannel) = 0;
		// 通道代码转换为通道号
		virtual int ChannelCode2Channel(int iChannelCode) = 0;
		// 通道代码转换为通道名称
		virtual LPCTSTR ChannelCode2String(int iChannelCode) = 0;

		// 是白天数据
		virtual BOOL IsDay() = 0;
		// 是晚上数据
		virtual BOOL IsNight() = 0;
		///---------------
		///纯虚函数 - Beg
		///------------------------------------------------

	public:

	protected:
		BOOL SaveData_LD2(LPCTSTR lpszFile);
		BOOL SaveData_LD3(LPCTSTR lpszFile);

	protected:
		//-------------------------------------------------
		// 通道数据和产品数据
		PWORD* m_ppwData;			// 卫星通道数据
		PBYTE* m_ppucDataGray;		// 卫星通道数据灰度
		
		PVOID m_pFileHeader;		// 文件头
		//-------------------------------------------------
		int m_iRChannel;			//红色通道
		int m_iGChannel;			//绿色通道
		int m_iBChannel;			//蓝色通道

		int m_iRChannelDef;			//红色通道默认
		int m_iGChannelDef;			//绿色通道默认
		int m_iBChannelDef;			//蓝色通道默认
		//-------------------------------------------------
		// 通道名称
		TCHAR m_szChannel[MAX_PATH];

		// 白天文件
		TCHAR m_szDayFile[MAX_PATH];
		// 晚上文件
		TCHAR m_szNightFile[MAX_PATH];

		BOOL m_bEmpty;
		//-------------------------------------------------
		TCHAR m_szTemp[MAX_PATH];

	private:
		// 通道映射
		AgmrsDataChannelMap m_AgmrsDataChannelMap;
		// 通道信息数组
		ChannelInfoVector m_ChannelInfoVector;

#pragma region --静态成员--

	public:	// static function begin

		// 取得像元点面积(km^2)
		static double GetPixelArea(double lat, double resolution);

		static BOOL IsLd2File(LPCTSTR lpszFile);
		static BOOL IsLd3File(LPCTSTR lpszFile);

	public:
		static BOOL IsAgmrsData(LPCTSTR lpszFile);
		static int GetAgmrsDataSource(LPCTSTR lpszFile);
		static int GetAgmrsDataSource(WORD wSatelliteID);

		static BOOL IsAvhrrData(LPCTSTR lpszFile)	{ return AgmrsDataSource::Avhrr	== GetAgmrsDataSource(lpszFile); }
		static BOOL IsModisData(LPCTSTR lpszFile)	{ return AgmrsDataSource::Modis	== GetAgmrsDataSource(lpszFile); }
		static BOOL IsMersiData(LPCTSTR lpszFile)	{ return AgmrsDataSource::Mersi	== GetAgmrsDataSource(lpszFile); }
		static BOOL IsVirrData(LPCTSTR lpszFile)	{ return AgmrsDataSource::Virr	== GetAgmrsDataSource(lpszFile); }

	public:
		static KAvhrrThreshold s_AvhrrThreshold;
		static KAvhrrThreshold* GetAvhrrThreshold();
		static void SetAvhrrThreshold(KAvhrrThreshold* pAvhrrThreshold);

		static KModisThreshold s_ModisThreshold;
		static KModisThreshold* GetModisThreshold();
		static void SetModisThreshold(KModisThreshold* pModisThreshold);

		static KMersiThreshold s_MersiThreshold;
		static KMersiThreshold* GetMersiThreshold();
		static void SetMersiThreshold(KMersiThreshold* pMersiThreshold);

		static KVirrThreshold s_VirrThreshold;
		static KVirrThreshold* GetVirrThreshold();
		static void SetVirrThreshold(KVirrThreshold* pVirrThreshold);


	public:
		static KStdPalette		s_StdPalette;
		static KIdxPalette		s_IdxPalette;
		static KRbwPalette		s_RbwPalette;
		static KSegPalette		s_SegPalette;
		static KLinearPalette	s_LinearPalette;

		// 取得农气要干产品调色板
		static KPalette* GetAgmrsProductPalette(int iProductCode, PaletteType ePaletteType = PaletteType_Segment);

#pragma endregion

	};

//---------------------------------------------------------
}// namespace Agmrs - end
//---------------------------------------------------------

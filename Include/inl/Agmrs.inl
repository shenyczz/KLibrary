#ifndef __INCLUDED_AGMRS_INL__
#define __INCLUDED_AGMRS_INL__

namespace Agmrs
{
//---------------------------------------------------------
}// namespace Agmrs - end
//---------------------------------------------------------
using namespace Agmrs;

namespace Agmrs { namespace LDFile
{
//---------------------------------------------------------
}}// namespace Agmrs::LDFile - end
//---------------------------------------------------------
using namespace Agmrs::LDFile;

// 卫星遥感数据局地文件标识(FileID)
#define AGMRS_LOCAL_DATA_MARKER				((WORD)(('A'<<8)|'L'))		// 卫星中心局地文件标示 = LA

// 卫星遥感星载传感器识别码(SatelliteID)
#define AGMRS_SATELLITE_NOAA15_AVHRR		7			// NOAA15_AVHRR
#define AGMRS_SATELLITE_NOAA16_AVHRR		3			// NOAA16_AVHRR
#define AGMRS_SATELLITE_NOAA18_AVHRR		13			// NOAA18_AVHRR
#define AGMRS_SATELLITE_NOAA19_AVHRR		19			// NOAA19_AVHRR

#define AGMRS_SATELLITE_FY1D_AVHRR			114			// FY1D_AVHRR

#define AGMRS_SATELLITE_TERRA_MODIS			1001		// TERRA_MODIS
#define AGMRS_SATELLITE_AQUA_MODIS			1002		// AQUA_MODIS

#define AGMRS_SATELLITE_FY3A_MERSI			1100		// FY-3A_MERSI
#define AGMRS_SATELLITE_FY3B_MERSI			1101		// FY-3A_MERSI
#define AGMRS_SATELLITE_FY3C_MERSI			1102		// FY-3A_MERSI

#define AGMRS_SATELLITE_FY3A_VIRR			123			// FY-3A_VIRR
#define AGMRS_SATELLITE_FY3B_VIRR			124			// FY-3A_VIRR
#define AGMRS_SATELLITE_FY3C_VIRR			125			// FY-3A_VIRR

// 卫星遥感数据局地文件版本
#define AGMRS_LOCAL_DATA_2_VERSION			2002		// LD2文件版本号
#define AGMRS_LOCAL_DATA_3_VERSION			2006		// LD3文件版本号

// 
#define AGMRS_LOCAL_DATA_CHANNEL_NUM_MAX	40			// 局地数据通道数最大值

//#define AGMRS_DATA_LD2_CHANNEL_NUM_MAX		40				// LD2数据通道数最大值
//#define AGMRS_DATA_LD3_CHANNEL_NUM_MAX		1000			// LD3数据通道数最大值

// 遥感数据无效值
#define AGMRS_INVALID_DATA					AXIN_INVALID_DATA			// 卫星遥感无效数据(-9999)
#define AGMRS_PRODUCT_DATA_INVALID			AXIN_INVALID_DATA			// 产品无效值 -9999

// 遥感数据特殊值
#define AGMRS_PRODUCT_DATA_CLOUD			AXIN_SPECIAL_DATA_CLOUD		// 云	= 32770 (0x8002)
#define AGMRS_PRODUCT_DATA_FOG				AXIN_SPECIAL_DATA_FOG		// 雾	= 32771
#define AGMRS_PRODUCT_DATA_FIRING			AXIN_SPECIAL_DATA_FIRING	// 火点	= 32772
#define AGMRS_PRODUCT_DATA_FIRED			AXIN_SPECIAL_DATA_FIRED		// 火点	= 32773
#define AGMRS_PRODUCT_DATA_SNOW				AXIN_SPECIAL_DATA_SNOW		// 雪	= 32774
#define AGMRS_PRODUCT_DATA_ICE				AXIN_SPECIAL_DATA_ICE		// 冰	= 32775
#define AGMRS_PRODUCT_DATA_LAND				AXIN_SPECIAL_DATA_LAND		// 陆地	= 32776
#define AGMRS_PRODUCT_DATA_WATER			AXIN_SPECIAL_DATA_WATER		// 水体	= 32783 (0x800F)

//---------------------------------------------------------
// 农业气象遥感反演产品代码常量定义 - Beg
//-------------------------------------
#define AGMRS_CROP_CODE_NONE			0		// 作物代码 - 未知
#define AGMRS_CROP_CODE_WHEAT			1		// 作物代码 - 小麦
#define AGMRS_CROP_CODE_CORN			2		// 作物代码 - 玉米
//-------------------------------------
// 农业气象遥感反演产品代码常量定义 - End
//---------------------------------------------------------

//---------------------------------------------------------
// 农业气象遥感反演产品代码常量定义 - Beg
//-------------------------------------
#define AGMRS_PRODUCT_CODE_NONE				0			// 农气遥感产品代码 - 未知

#define AGMRS_PRODUCT_CODE_BEG				6000		// 农气遥感产品代码 - BEG

///6001 ~ 6049
// 0.植被指数 - VIX (Vegetation Index)
#define AGMRS_PRODUCT_CODE_VIX_BEG			6000		// BEG - 植被指数
#define AGMRS_PRODUCT_CODE_VIX_NDVI			6001		// 归一化植被指数	- NDVI
#define AGMRS_PRODUCT_CODE_VIX_DVI			6002		// 差值植被指数		- DVI
#define AGMRS_PRODUCT_CODE_VIX_RVI			6003		// 比值植被指数		- RVI
#define AGMRS_PRODUCT_CODE_VIX_SAVI			6004		// 土壤调整植被指数 - SAVI
#define AGMRS_PRODUCT_CODE_VIX_EVI			6005		// 增强型植被指数	- EVI
#define AGMRS_PRODUCT_CODE_VIX_LAI			6030		// 叶面积指数		- LAI
#define AGMRS_PRODUCT_CODE_VIX_END			6050		// 叶面积指数		- LAI

///6051 ~ 6099
#define AGMRS_PRODUCT_CODE_DIF_VIX_BEG		6050		// 
#define AGMRS_PRODUCT_CODE_DIF_VIX_NDVI		6051		// 植被指数变化		- NDVI
#define AGMRS_PRODUCT_CODE_DIF_VIX_DVI		6052		// 植被指数变化		- DVI
#define AGMRS_PRODUCT_CODE_DIF_VIX_RVI		6053		// 植被指数变化		- RVI
#define AGMRS_PRODUCT_CODE_DIF_VIX_SAVI		6054		// 植被指数变化		- SAVI
#define AGMRS_PRODUCT_CODE_DIF_VIX_EVI		6055		// 植被指数变化		- EVI
#define AGMRS_PRODUCT_CODE_DIF_VIX_LAI		6080		// 植被指数变化		- LAI
#define AGMRS_PRODUCT_CODE_DIF_VIX_END		6100		// END - 植被指数

///6101 ~ 6149
// 1.干旱指数 - DIX (Drought Index)
#define AGMRS_PRODUCT_CODE_DIX_BEG			6100		// BEG - 干旱指数
#define AGMRS_PRODUCT_CODE_DIX_ATI			6101		// 热惯量指数			- ATI
#define AGMRS_PRODUCT_CODE_DIX_VSWI			6102		// 植被供水指数			- VSWI
#define AGMRS_PRODUCT_CODE_DIX_SWCI			6103		// 土壤含水量指数		- SWCI
#define AGMRS_PRODUCT_CODE_DIX_TVDI			6104		// 温度植被干旱指数		- TVDI
#define AGMRS_PRODUCT_CODE_DIX_CSMI			6105		// 耕作层土壤湿度指数	- CSMI
#define AGMRS_PRODUCT_CODE_DIX_PDI			6106		// 垂直干旱指数			- PDI
#define AGMRS_PRODUCT_CODE_DIX_MPDI			6107		// 修正垂直干旱指数		- MPDI
#define AGMRS_PRODUCT_CODE_DIX_END			6150		// END - 干旱指数

#define AGMRS_PRODUCT_CODE_DIF_DIX_BEG		6150		// 
#define AGMRS_PRODUCT_CODE_DIF_DIX_ATI		6151		// 干旱指数变化			- ATI	
#define AGMRS_PRODUCT_CODE_DIF_DIX_VSWI		6152		// 干旱指数变化			- VSWI
#define AGMRS_PRODUCT_CODE_DIF_DIX_SWCI		6153		// 干旱指数变化			- SWCI
#define AGMRS_PRODUCT_CODE_DIF_DIX_TVDI		6154		// 干旱指数变化			- TVDI
#define AGMRS_PRODUCT_CODE_DIF_DIX_CSMI		6155		// 干旱指数变化			- CSMI
#define AGMRS_PRODUCT_CODE_DIF_DIX_PDI		6156		// 干旱指数变化			- PDI	
#define AGMRS_PRODUCT_CODE_DIF_DIX_MPDI		6157		// 干旱指数变化			- MPDI
#define AGMRS_PRODUCT_CODE_DIF_DIX_END		6200		// END - 干旱指数

///6201 ~ 6299
// 2.土壤墒情 - SMC (Soil Moisture Content)
#define AGMRS_PRODUCT_CODE_SMC_BEG			6200		// BEG - 土壤墒情
#define AGMRS_PRODUCT_CODE_SMC_AH			6201		// 土壤 - 绝对湿度(Absolute Humidity)
#define AGMRS_PRODUCT_CODE_SMC_RH			6202		// 土壤 - 相对湿度(Relative Humidity)
#define AGMRS_PRODUCT_CODE_SMC_END			6299		// END - 土壤墒情

///6301 ~ 6399
// 3.灌溉 - IRG (Irrigation)
#define AGMRS_PRODUCT_CODE_IRG_BEG			6300		// BEG - 灌溉
#define AGMRS_PRODUCT_CODE_IRG_DEPTH		6301		// 灌溉深度
#define AGMRS_PRODUCT_CODE_IRG_AREA			6302		// 灌溉面积
#define AGMRS_PRODUCT_CODE_IRG_AMOUNT		6303		// 灌溉量
#define AGMRS_PRODUCT_CODE_IRG_END			6399		// END - 灌溉

///6401 ~ 6499
// 4.积雪指数、积雪深度、积雪覆盖
#define AGMRS_PRODUCT_CODE_SNOW_BEG			6400		// BEG - 积雪
#define AGMRS_PRODUCT_CODE_SNOW_NDSI		6401		// 积雪指数
#define AGMRS_PRODUCT_CODE_SNOW_DEPTH		6402		// 积雪深度
#define AGMRS_PRODUCT_CODE_SNOW_COVER		6403		// 积雪覆盖
#define AGMRS_PRODUCT_CODE_SNOW_END			6499		// END - 积雪

///6501 ~ 6599
// 5.
#define AGMRS_PRODUCT_CODE_6500_BEG			6500		// BEG - 
#define AGMRS_PRODUCT_CODE_6500_END			6599		// END - 

///6601 ~ 6699
// 6.产品评估
#define AGMRS_PRODUCT_CODE_EVALU_BEG		6600		// BEG - 产品评估
#define AGMRS_PRODUCT_CODE_EVALU_END		6699		// END - 产品评估

///6701 ~ 6799
// 7.
#define AGMRS_PRODUCT_CODE_6700_BEG			6700		// BEG -
#define AGMRS_PRODUCT_CODE_6799_END			6799		// END -

///6801 ~ 6899
// 8.等级 - LVL (Level)
#define AGMRS_PRODUCT_CODE_LVL_BEG			6800		// BEG - 等级
#define AGMRS_PRODUCT_CODE_LVL_MQ_WHEAT		6801		// 苗情等级 - 小麦
#define AGMRS_PRODUCT_CODE_LVL_MQ_CORN		6802		// 苗情等级 - 玉米
#define AGMRS_PRODUCT_CODE_LVL_6809			6809		// 
#define AGMRS_PRODUCT_CODE_LVL_6810			6810		// 
#define AGMRS_PRODUCT_CODE_LVL_DRY			6811		// 干旱等级
#define AGMRS_PRODUCT_CODE_LVL_6819			6819		// 
#define AGMRS_PRODUCT_CODE_LVL_6890			6890		// 
#define AGMRS_PRODUCT_CODE_LVL_LANDUSE		6898		// 土地利用
#define AGMRS_PRODUCT_CODE_LVL_END			6899		// END - 等级

///6901 ~ 6999
// 9.杂项 - MISC (Misce)
#define AGMRS_PRODUCT_CODE_MISC_BEG			6900		// BEG - 杂项
#define AGMRS_PRODUCT_CODE_MISC_CLOUD		6901		// 简单监测 - 云
#define AGMRS_PRODUCT_CODE_MISC_FOG			6902		// 简单监测 - 雾
#define AGMRS_PRODUCT_CODE_MISC_FIRE		6903		// 简单监测 - 火
#define AGMRS_PRODUCT_CODE_MISC_SNOW		6904		// 简单监测 - 雪
#define AGMRS_PRODUCT_CODE_MISC_ICE			6905		// 简单监测 - 冰
#define AGMRS_PRODUCT_CODE_MISC_LAND		6906		// 简单监测 - 陆
#define AGMRS_PRODUCT_CODE_MISC_WATER		6907		// 简单监测 - 水
#define AGMRS_PRODUCT_CODE_MISC_6909		6909		// 
#define AGMRS_PRODUCT_CODE_MISC_6910		6910		// 
#define AGMRS_PRODUCT_CODE_MISC_6911		6911		// 
#define AGMRS_PRODUCT_CODE_MISC_6919		6919		// 
#define AGMRS_PRODUCT_CODE_MISC_LST			6947		// 地表温度 - LST(K)
#define AGMRS_PRODUCT_CODE_MISC_ABE			6948		// 全反照率 - ABE

#define AGMRS_PRODUCT_CODE_DIF_MISC_6950	6950		// 
#define AGMRS_PRODUCT_CODE_DIF_MISC_CLOUD	6951		// 简单监测 - 云
#define AGMRS_PRODUCT_CODE_DIF_MISC_FOG		6952		// 简单监测 - 雾
#define AGMRS_PRODUCT_CODE_DIF_MISC_FIRE	6953		// 简单监测 - 火
#define AGMRS_PRODUCT_CODE_DIF_MISC_SNOW	6954		// 简单监测 - 雪
#define AGMRS_PRODUCT_CODE_DIF_MISC_ICE		6955		// 简单监测 - 冰
#define AGMRS_PRODUCT_CODE_DIF_MISC_LAND	6956		// 简单监测 - 陆
#define AGMRS_PRODUCT_CODE_DIF_MISC_WATER	6957		// 简单监测 - 水
#define AGMRS_PRODUCT_CODE_DIF_MISC_LST		6997		// 地表温度 - LST(K)
#define AGMRS_PRODUCT_CODE_DIF_MISC_ABE		6998		// 全反照率 - ABE
#define AGMRS_PRODUCT_CODE_MISC_END			6999		// END - 杂项

#define AGMRS_PRODUCT_CODE_END				6999		// 农气遥感产品代码 - END
//-------------------------------------
// 农业气象遥感反演产品代码常量定义 - End
//----------------------------------------------------------
namespace Agmrs
{
//#define AGMRS_DATA_SOURCE_VIRR 1

	// 农气遥感数据来源类型
	class AgmrsDataSource
	{
	public:
		enum
		{
			Unknown = 0,
			Avhrr,			// Noaa 卫星搭载
			Modis,			// Terra & Aqua 卫星搭载
			Mersi,			// Fy3a 卫星搭载
			Virr,			// Fy3a 卫星搭载
		};
	};

	// 中心波长(nm)
	enum CenterBound
	{
		CenterBound_0469,
	};

	// 火点类型
	class FireType
	{
	public:
		enum
		{
			None = 0,
			FireNaked,	// 明火 1
			FireEmber,	// 余火 2
			FireDark,	// 暗火 3
		};
	};

	// 农气遥感数据点类型
	class PixelType
	{
	public:
		enum
		{
			Unknown = 0,
			Cloud,		// 云
			Fog,			// 雾
			FireNaked,	// 火(明火)
			FireEmber,	// 火(余火)
			FireDark,	// 火(暗火)
			Snow,		// 雪
			Ice,			// 冰
			Land,		// 陆
			Water,		// 水
		};
	};

	// 地表温度反演方法
	class LstMethod
	{
	public:
		enum
		{
			Becker = 0,	// 地表温度 - Becker
			Sobrino,	// 地表温度 - Sobrino
			Qin,		// 地表温度 - Qin
			Regress,	// 地表温度 - 回归法
		};
	};

#pragma pack(1)

	// 通道数据转换为灰度数据时使用的参数
	struct Data2GrayParam
	{
		int iChannel;					// 通道号
		int iEnhanceType;				// 增强方式(0-直线，1-指数，2-对数 ...)
		WORD wMinValue;					// 最小值
		WORD wMaxValue;					// 最大值
		WORD wThreshold_min;			// 去掉一些小值后的极小值(通道数据下阈值)
		WORD wThreshold_max;			// 去掉一些大值后的极大值(通道数据上阈值)
		BYTE cMinGray;					// 最小灰度
		BYTE cMaxGray;					// 最大灰度
		BYTE cLessDownGray;				// 弱最小灰度
		BYTE cExceedUpGray;				// 超最大灰度
		BOOL bAntiPhase;				// 是否反相
	};

	// 通道映射使用的结构
	// 数据通道有38个 对应 1~36通道
	//          [ 内存数据通道 -> 数据通道(Base1) + 通道名称
	struct AgmrsDataChannel
	{
		TCHAR	szName[65];	// 通道名称
		int		iDataIndex;	// 数据索引(0 - 37)
		BOOL	bExist;		// 通道数据是否存在

		AgmrsDataChannel() 
		{
			_tcscpy(szName,_T(""));
			iDataIndex = -1;
			bExist = FALSE;
		}
	};
	typedef map<int,AgmrsDataChannel> AgmrsDataChannelMap;	// 通道映射

	// 遥感数据通道
	//struct RsDataChannel1
	//{
	//	int		iDataIndex;		// 数据索引(0 - 37)
	//	TCHAR	szName[65];		// 通道名称

	//	RsDataChannel1() 
	//	{
	//		iDataIndex = -1;
	//		_tcscpy(szName,_T(""));
	//	}
	//};
	//typedef map<int,RsDataChannel1> RsDataChannelMap;	// 通道映射

	// 点组
	struct GroupPoint
	{
		long	Row;			// 行
		long	Col;			// 列
		long	Group;			// 组
		WORD	Flag;			// 标记

		GroupPoint()
		{
			memset(this,0,sizeof(GroupPoint));
		}
	};

	// 火点
	struct FirePoint
	{
		//-------------------------------------------------
		long	Id;				// id
		short	Type;			// 见 FirePointType 枚举

		long	Row;			// 行
		long	Col;			// 列

		float	Lon;			// 经度
		float	Lat;			// 纬度

		float	Temperature;	// 温度
		float	Area;			// 面积

		long	Group;			// 组

		WORD	Flag;			// 标记
		//-------------------------------------------------
		// 组名称
		__declspec(property(get=GetGroupName,put=PutGroupName)) TCHAR* GroupName;
		TCHAR* GetGroupName() { return szGroupName; }
		void PutGroupName(LPCTSTR lpszGroupName)
		{
			for(int i=0;i<64;i++)
			{
				szGroupName[i] = _T('\0');
			}
			_tcsncpy(szGroupName,lpszGroupName,63);
			szGroupName[63] = 0;
		}
		//-------------------------------------------------
		FirePoint()
		{
			memset(this,0,sizeof(FirePoint));
		}
		//-------------------------------------------------

	private:
		TCHAR szGroupName[64];
	};

	// 通道信息
	struct ChannelInfo
	{
		double Value;

		__declspec(property(get=getChannel, put=putChannel)) TCHAR* Channel;
		TCHAR* getChannel() { return szChannel; }
		void putChannel(LPCTSTR lpszChannel) { _tcsncpy(szChannel,lpszChannel,126); }

		ChannelInfo()
		{
			memset(this,0,sizeof(ChannelInfo));
		}

	private:
		TCHAR szChannel[128];
	};
	typedef vector<ChannelInfo> ChannelInfoVector;

	// 通道合并信息
	struct ChannelCombineInfo
	{
		vector<int> Channels;

		__declspec(property(get=getFileName, put=putFileName)) TCHAR* FileName;
		TCHAR* getFileName() { return szFileName; }
		void putFileName(LPCTSTR lpszFileName) { _tcsncpy(szFileName,lpszFileName,MAX_PATH); }

		ChannelCombineInfo()
		{
			memset(this,0,sizeof(ChannelCombineInfo));
		}

	private:
		TCHAR szFileName[MAX_PATH];
	};

	// LD2、LD3文件头共有部分
	struct LDFHeaderCommon
	{
		WORD	wFileID;			// 文件标识
									// 局地文件为"LA" 

		WORD	wSatelliteID;		// 卫星标识

		WORD	wOrbitNo;			// 轨道号
		WORD	wUpOrDown;			// 升降轨标记, 1: 升轨, 0: 降轨

		WORD	wYear;				// 年
		WORD	wMonth;				// 月
		WORD	wDay;				// 日
		WORD	wHour;				// 时
		WORD	wMinute;			// 分

		WORD	wDayOrNight;		// 白天黑夜标识, 0: 白天, 1: 黑夜
		WORD	wChannelNums;		// 通道数 
										// LDF - 白天: 5, 黑夜: 3
										// LD2 - ?

		WORD	wProjectType;		// 投影方式
									// 0: 不投影
									// 1: 等角投影
									// 2: 麦卡托投影
									// 3: 兰布托投影
									// 4: 极射赤面投影
									// 5: 艾尔伯斯投影

		WORD	wWidth;				// 列数
		WORD	wHeight;			// 行数

		float	fLonResolution;		// 经度分辨率 -- 等角投影
									// x分辨率    -- 麦卡托、兰布托、极射赤面投影 
		float	fLatResolution;		// 纬度分辨率 -- 等角投影
									// y分辨率    -- 麦卡托、兰布托、极射赤面投影

		float	fStandardLat1;		// 标准纬度1 -- 麦卡托、兰布托、极射赤面投影有效
		float	fStandardLat2;		// 标准纬度2 -- 兰布托投影有效

		float	fRadiusOfEarth;		// 地球半径  -- 麦卡托、兰布托、极射赤面投影有效

		float	fLatMin;			// 最小纬度
		float	fLatMax;			// 最大纬度
		float	fLonMin;			// 最小经度
		float	fLonMax;			// 最大经度

		float	fStandardLon;		// 圆锥投影中心经线 (LAMBERT,POLAR,ALBERS)

		float	fCenterLon;			// 图象中心点的经度
		float	fCenterLat;			// 图象中心点的纬度

		LDFHeaderCommon()
		{
			memset(this,0,sizeof(LDFHeaderCommon));
		}

	};

	/**************************************************************
	 *														      *
	 *   卫星中心局地文件(LDF)包含有文件头(128个字节）+通道数据      *
	 *   通道数据的排放顺序为按1，2，3，....的通道顺序进行排放，     *
	 *   该结构在系统中作为中间结构进行处理                         *
	 *                                                            *
	***************************************************************/
									// 局地文件为"LA" 
									// 植被 NV -- NDVI
									// 植被 DV -- DVI
									// 植被 RV -- RVI
									// LST -- LS
									// SSY -- SS
	// 卫星中心局地文件(LDF)文件头
	// 128 bytes
	struct LDFHeader
	{
		WORD	wFileID;			// 文件标识
		WORD	wSatelliteID;		// 卫星标识

		WORD	wOrbitNo;			// 轨道号
		WORD	wUpOrDown;			// 升降轨标记, 1: 升轨, 0: 降轨

		WORD	wYear;				// 年
		WORD	wMonth;				// 月
		WORD	wDay;				// 日
		WORD	wHour;				// 时
		WORD	wMinute;			// 分

		WORD	wDayOrNight;		// 白天黑夜标识, 0: 白天, 1: 黑夜
		WORD	wChannelNums;		// 通道数 

		WORD	wProjectType;		// 投影方式
									// 0: 不投影
									// 1: 等角投影
									// 2: 麦卡托投影
									// 3: 兰布托投影
									// 4: 极射赤面投影
									// 5: 艾尔伯斯投影

		WORD	wWidth;				// 列数
		WORD	wHeight;			// 行数

		float	fLonResolution;		// 经度分辨率 -- 等角投影
									// x分辨率    -- 麦卡托、兰布托、极射赤面投影 
		float	fLatResolution;		// 纬度分辨率 -- 等角投影
									// y分辨率    -- 麦卡托、兰布托、极射赤面投影

		float	fStandardLat1;		// 标准纬度1 -- 麦卡托、兰布托、极射赤面投影有效
		float	fStandardLat2;		// 标准纬度2 -- 兰布托投影有效

		float	fRadiusOfEarth;		// 地球半径  -- 麦卡托、兰布托、极射赤面投影有效

		float	fLatMin;			// 最小纬度
		float	fLatMax;			// 最大纬度
		float	fLonMin;			// 最小经度
		float	fLonMax;			// 最大经度

		float	fSunAngleLB;		// 左下角太阳天顶角
		float	fSunAngleLT;		// 左上角太阳天顶角
		float	fSunAngleRB;		// 右下角太阳天顶角
		float	fSunAngleRT;		// 右上角太阳天顶角

		float	fSatAngleLB;		// 左下角卫星天顶角
		float	fSatAngleLT;		// 左上角卫星天顶角
		float	fSatAngleRB;		// 右下角卫星天顶角
		float	fSatAngleRT;		// 右上角卫星天顶角

		float	fSunSatAngleLB;		// 左下角太阳卫星天顶角
		float	fSunSatAngleLT;		// 左上角太阳卫星天顶角
		float	fSunSatAngleRB;		// 右下角太阳卫星天顶角
		float	fSunSatAngleRT;		// 右上角太阳卫星天顶角

		WORD	wMaxRow;			//
		WORD	wMinCol;			//

		WORD	wRevered;

		WORD	wRate;				// 存放插值放大的比率
		WORD	wBytes;				//
		WORD	wVersion;			// 版本号
		DWORD	dwSkipLength;		// 局地文件头记录的后面填充字段长度

		LDFHeader()
		{
			memset(this,0,sizeof(LDFHeader));
		}
	};

	/**************************************************************
	 *														     *
	 *   卫星中心局地文件(LD2)包含有文件头(128个字节）+通道数据  *
	 *   通道数据的排放顺序为按1，2，3，....的通道顺序进行排放， *
	 *   该结构在系统中作为中间结构进行处理                      *
	 *                                                           *
	***************************************************************/
	// 128 bytes
	struct LD2Header
	{
		WORD	wFileID;			// 文件标识
		WORD	wSatelliteID;		// 卫星标识

		WORD	wOrbitNo;			// 轨道号
		WORD	wUpOrDown;			// 升降轨标记, 1: 升轨, 0: 降轨

		WORD	wYear;				// 年
		WORD	wMonth;				// 月
		WORD	wDay;				// 日
		WORD	wHour;				// 时
		WORD	wMinute;			// 分

		WORD	wDayOrNight;		// 白天黑夜标识, 0: 白天, 1: 黑夜
		WORD	wChannelNums;		// 通道数 

		WORD	wProjectType;		// 投影方式
									// 0: 不投影
									// 1: 等角投影
									// 2: 麦卡托投影
									// 3: 兰布托投影
									// 4: 极射赤面投影
									// 5: 艾尔伯斯投影

		WORD	wWidth;				// 列数
		WORD	wHeight;			// 行数

		float	fLonResolution;		// 经度分辨率 -- 等角投影
									// x分辨率    -- 麦卡托、兰布托、极射赤面投影 
		float	fLatResolution;		// 纬度分辨率 -- 等角投影
									// y分辨率    -- 麦卡托、兰布托、极射赤面投影

		float	fStandardLat1;		// 标准纬度1 -- 麦卡托、兰布托、极射赤面投影有效
		float	fStandardLat2;		// 标准纬度2 -- 兰布托投影有效

		float	fRadiusOfEarth;		// 地球半径  -- 麦卡托、兰布托、极射赤面投影有效

		float	fLatMin;			// 最小纬度
		float	fLatMax;			// 最大纬度
		float	fLonMin;			// 最小经度
		float	fLonMax;			// 最大经度

		float	fStandardLon;		// 圆锥投影中心经线 (LAMBERT,POLAR,ALBERS)

		float	fCenterLon;			// 图象中心点的经度
		float	fCenterLat;			// 图象中心点的纬度

		BYTE	byChannelIndex[40];	// 通道索引
		WORD	wRate;				// 已合成天数
		WORD	wVersion;			// 版本号
		WORD	wBytes;				// 本结构字节数
		WORD	wResvere;			// 保留
		DWORD	dwSkipLength;		// 局地文件头记录的后面填充字段长度

		LD2Header()
		{
			memset(this,0,sizeof(LD2Header));
		}

	};

	/**************************************************************
	 *														     *
	 *   卫星中心局地文件(LD3)包含有文件头(4096个字节）+通道数据  *
	 *   通道数据的排放顺序为按1，2，3，....的通道顺序进行排放， *
	 *   该结构在系统中作为中间结构进行处理                      *
	 *                                                           *
	***************************************************************/
	struct LD3Header
	{
		WORD	wFileID;			// 文件标识
		WORD	wSatelliteID;		// 卫星标识

		WORD	wOrbitNo;			// 轨道号
		WORD	wUpOrDown;			// 升降轨标记, 1: 升轨, 0: 降轨

		WORD	wYear;				// 年
		WORD	wMonth;				// 月
		WORD	wDay;				// 日
		WORD	wHour;				// 时
		WORD	wMinute;			// 分

		WORD	wDayOrNight;		// 白天黑夜标识, 0: 白天, 1: 黑夜
		WORD	wChannelNums;		// 通道数 

		WORD	wProjectType;		// 投影方式
									// 0: 不投影
									// 1: 等角投影
									// 2: 麦卡托投影
									// 3: 兰布托投影
									// 4: 极射赤面投影
									// 5: 艾尔伯斯投影

		WORD	wWidth;				// 列数
		WORD	wHeight;			// 行数

		float	fLonResolution;		// 经度分辨率 -- 等角投影
									// x分辨率    -- 麦卡托、兰布托、极射赤面投影 
		float	fLatResolution;		// 纬度分辨率 -- 等角投影
									// y分辨率    -- 麦卡托、兰布托、极射赤面投影

		float	fStandardLat1;		// 标准纬度1 -- 麦卡托、兰布托、极射赤面投影有效
		float	fStandardLat2;		// 标准纬度2 -- 兰布托投影有效

		float	fRadiusOfEarth;		// 地球半径  -- 麦卡托、兰布托、极射赤面投影有效

		float	fLatMin;			// 最小纬度
		float	fLatMax;			// 最大纬度
		float	fLonMin;			// 最小经度
		float	fLonMax;			// 最大经度

		float	fStandardLon;		// 圆锥投影中心经线 (LAMBERT,POLAR,ALBERS)

		float	fCenterLon;			// 图象中心点的经度
		float	fCenterLat;			// 图象中心点的纬度
		//------------------------------
		WORD	wChIndex[1000];		// 通道索引

		WORD	wChannelNumsOfAncillary;	// 辅助通道数 
		WORD	wAncillaryChIndex[100];		// 辅助通道索引

		WORD	wComposeDays;		// 已合成天数
		WORD	wVersion;			// 版本号
		UCHAR	NDVIOffset;			// ??
		bool	bCloudMask;			// WORD	wBytes;				// 本结构字节数
		DWORD	dwSkipLength;		// 局地文件头记录的后面填充字段长度
		//------------------------------
		// UCHAR	ucFlag1;
		// 目前由于头文件过短，使用这个字符标识是否包含太阳天顶角、太阳方位角、海拔高度、卫星天顶角、卫星方位角、地表特征数据
		// 第1位标识太阳天顶角
		// 第2位标识太阳方位角
		// 第3位标识海拔高度
		// 第4位标识卫星天顶角
		// 第5位标识卫星方位角
		// 第6位标识地表特征数据
		// UCHAR	ucFlag2;
		//------------------------------
		UCHAR	ucNoaach3Flag;			// 0-3A  1-3B
		UCHAR	ucFlag3[1807];
		//------------------------------

		LD3Header()
		{
			memset(this,0,sizeof(LD3Header));
		}

	};

#pragma pack()

//---------------------------------------------------------
}// namespace Agmrs - end
//---------------------------------------------------------

#include "Avhrr.inl"
#include "Modis.inl"
#include "Mersi.inl"
#include "Virr.inl"

#endif	//#ifndef __INCLUDED_AGMRS_INL__

#ifndef __INCLUDED_AXIN_INL__
#define __INCLUDED_AXIN_INL__

#include "Lightning.inl"	// 雷电

namespace Axin
{
//---------------------------------------------------------
}// namespace Axin - end
//---------------------------------------------------------
using namespace Axin;

namespace Axin
{

//---------------------------------------------------------
//                     宏
//=========================================================

// Axin 一般宏定义
//
#define AXIN_FID_TXT_MARK				_T("AXIN")			// Axin 文本文件标识
#define AXIN_FID_BIN_MARK				('A' | 'X' << 8)	// Axin 二进制文件标识

#define AXIN_COMMENT_LENGTH				64		// 注释信息长度

// 无效数据
//
#define AXIN_INVALID_DATA				-9999				// 无效数据

// 特殊处理数据(根据海拔高度定义)
//
#define AXIN_SPECIAL_DATA_BEG			0x8000				// 开始		= 32768
#define AXIN_SPECIAL_DATA_8001			0x8001				// 
#define AXIN_SPECIAL_DATA_CLOUD			0x8002				// 云		= 32770
#define AXIN_SPECIAL_DATA_FOG			0x8003				// 雾		= 32771
#define AXIN_SPECIAL_DATA_FIRING		0x8004				// 明火点	= 32772
#define AXIN_SPECIAL_DATA_FIRED			0x8005				// 过火点	= 32773
#define AXIN_SPECIAL_DATA_SNOW			0x8006				// 雪		= 32774
#define AXIN_SPECIAL_DATA_ICE			0x8007				// 冰		= 32775
#define AXIN_SPECIAL_DATA_LAND			0x8008				// 陆地		= 32776
#define AXIN_SPECIAL_DATA_8009			0x8009				// 
#define AXIN_SPECIAL_DATA_800A			0x800A				// 
#define AXIN_SPECIAL_DATA_800B			0x800B				// 
#define AXIN_SPECIAL_DATA_800C			0x800C				// 
#define AXIN_SPECIAL_DATA_800D			0x800D				// 
#define AXIN_SPECIAL_DATA_800E			0x800E				// 
#define AXIN_SPECIAL_DATA_WATER			0x800F				// 水体		= 32783
#define AXIN_SPECIAL_DATA_END			0x8010				// 结束		= 32784

// Axin 文件格式代码(0~99)
// 
#define AXIN_FMT_CODE_00				0		// 
#define AXIN_FMT_CODE_01				1		// 
#define AXIN_FMT_CODE_02				2		// 
#define AXIN_FMT_CODE_03				3		// 
#define AXIN_FMT_CODE_04				4		// 
#define AXIN_FMT_CODE_05				5		// 
#define AXIN_FMT_CODE_06				6		// 
#define AXIN_FMT_CODE_07				7		// 
#define AXIN_FMT_CODE_08				8		// 
#define AXIN_FMT_CODE_09				9		// 

#define AXIN_FMT_CODE_10				10		// 
#define AXIN_FMT_CODE_11				11		// 
#define AXIN_FMT_CODE_12				12		// 
#define AXIN_FMT_CODE_13				13		// 
#define AXIN_FMT_CODE_14				14		// 
#define AXIN_FMT_CODE_15				15		// 
#define AXIN_FMT_CODE_16				16		// 
#define AXIN_FMT_CODE_17				17		// 
#define AXIN_FMT_CODE_18				18		// 
#define AXIN_FMT_CODE_19				19		// 

#define AXIN_FMT_CODE_20				20		// 
#define AXIN_FMT_CODE_21				21		// 
#define AXIN_FMT_CODE_22				22		// 
#define AXIN_FMT_CODE_23				23		// 
#define AXIN_FMT_CODE_24				24		// 
#define AXIN_FMT_CODE_25				25		// 
#define AXIN_FMT_CODE_26				26		// 
#define AXIN_FMT_CODE_27				27		// 
#define AXIN_FMT_CODE_28				28		// 
#define AXIN_FMT_CODE_29				29		// 

#define AXIN_FMT_CODE_TIN				30		// 离散数据
#define AXIN_FMT_CODE_31				31		// 
#define AXIN_FMT_CODE_32				32		// 
#define AXIN_FMT_CODE_33				33		// 
#define AXIN_FMT_CODE_34				34		// 
#define AXIN_FMT_CODE_35				35		// 
#define AXIN_FMT_CODE_36				36		// 
#define AXIN_FMT_CODE_37				37		// 
#define AXIN_FMT_CODE_38				38		// 
#define AXIN_FMT_CODE_39				39		// 

#define AXIN_FMT_CODE_GRID				40		// 格点数据
#define AXIN_FMT_CODE_GRID_CONTOUR		40		// 格点数据(用于生成等值线)
#define AXIN_FMT_CODE_GRID_IMAGE		41		// 格点数据(用于生成图像)
#define AXIN_FMT_CODE_GRID_42			42		// 
#define AXIN_FMT_CODE_43				43		// 
#define AXIN_FMT_CODE_44				44		// 
#define AXIN_FMT_CODE_45				45		// 
#define AXIN_FMT_CODE_46				46		// 
#define AXIN_FMT_CODE_47				47		// 
#define AXIN_FMT_CODE_48				48		// 
#define AXIN_FMT_CODE_GRID_WIND_VECTOR	49		// 格点数据(用于风场矢量)

#define AXIN_FMT_CODE_50				50		// 
#define AXIN_FMT_CODE_51				51		// 
#define AXIN_FMT_CODE_52				52		// 
#define AXIN_FMT_CODE_53				53		// 
#define AXIN_FMT_CODE_54				54		// 
#define AXIN_FMT_CODE_55				55		// 
#define AXIN_FMT_CODE_56				56		// 
#define AXIN_FMT_CODE_57				57		// 
#define AXIN_FMT_CODE_58				58		// 
#define AXIN_FMT_CODE_59				59		// 

#define AXIN_FMT_CODE_COLOR_TABLE		60		// 颜色表文件
#define AXIN_FMT_CODE_61				61		// 
#define AXIN_FMT_CODE_62				62		// 
#define AXIN_FMT_CODE_63				63		// 
#define AXIN_FMT_CODE_64				64		// 
#define AXIN_FMT_CODE_65				65		// 
#define AXIN_FMT_CODE_66				66		// 
#define AXIN_FMT_CODE_67				67		// 
#define AXIN_FMT_CODE_68				68		// 
#define AXIN_FMT_CODE_69				69		// 

#define AXIN_FMT_CODE_IMAGE_CTRL_INFO	70		// 图像控制信息
#define AXIN_FMT_CODE_71				71		// 
#define AXIN_FMT_CODE_72				72		// 
#define AXIN_FMT_CODE_73				73		// 
#define AXIN_FMT_CODE_74				74		// 
#define AXIN_FMT_CODE_75				75		// 
#define AXIN_FMT_CODE_76				76		// 
#define AXIN_FMT_CODE_77				77		// 
#define AXIN_FMT_CODE_78				78		// 
#define AXIN_FMT_CODE_79				79		// 

#define AXIN_FMT_CODE_80				80		// 
#define AXIN_FMT_CODE_81				81		// 
#define AXIN_FMT_CODE_82				82		// 
#define AXIN_FMT_CODE_83				83		// 
#define AXIN_FMT_CODE_84				84		// 
#define AXIN_FMT_CODE_85				85		// 
#define AXIN_FMT_CODE_86				86		// 
#define AXIN_FMT_CODE_87				87		// 
#define AXIN_FMT_CODE_88				88		// 
#define AXIN_FMT_CODE_89				89		// 

#define AXIN_FMT_CODE_90				90		// 
#define AXIN_FMT_CODE_91				91		// 
#define AXIN_FMT_CODE_92				92		// 
#define AXIN_FMT_CODE_93				93		// 
#define AXIN_FMT_CODE_94				94		// 
#define AXIN_FMT_CODE_95				95		// 
#define AXIN_FMT_CODE_96				96		// 
#define AXIN_FMT_CODE_97				97		// 
#define AXIN_FMT_CODE_98				98		// 
#define AXIN_FMT_CODE_99				99		// 

#define AXIN_FMT_CVID_MIN				1		// 等值线数量最小值
#define AXIN_FMT_CVID_MAX				99		// 等值线数量最大值
#define AXIN_FMT_CVID_AUTO				8888	// 等值线数量 = 8888 的含义见说明书
#define AXIN_FMT_CVID_CONTOUR			9999	// 等值线数量 = 9999 的含义见说明书
#define AXIN_FMT_CLIP_AREA_BOX			9999	// 剪切区盒子

//---------------------------------------------------------
//                     常量
//=========================================================
const int Const_Color_Table_Packet_Code		= 0x80;		// 颜色表数据包代码

//---------------------------------------------------------
//                     宏
//=========================================================
/*
#define KD_BLOCK_DIVIDER			0xFFFF				// 块分隔符
#define KD_BLOCK_ID_DHB				0xF0F0				// DHB块ID
#define _KD_BLOCK_ID_DDB			1					// DDB块ID
#define KD_BLOCK_ID_DSB				2					// DSB块ID

#define KD_BLOCK_ID_DMB				0xF1F1				// DMB块ID
#define KD_BLOCK_ID_DDB				0xD1D1				// DDB块ID
#define KD_BLOCK_ID_EDB				0xE1E1				// EDB块ID
#define KD_BLOCK_ID_CDB				0xC1C1				// CDB块ID
#define KD_BLOCK_ID_SDB				1					// SDB块ID
#define KD_BLOCK_ID_LDB				2					// LDB块ID
#define KD_BLOCK_ID_TDB				3					// TDB块ID

#define KD_BLOCK_LENGTH_DMB			32					// DMB块长度
#define KD_BLOCK_LENGTH_EMB			32					// EMB块长度
#define KD_BLOCK_LENGTH_CDB			64					// CDB块长度
#define KD_BLOCK_LENGTH_DDB			128					// DDB块长度

#define KD_BLOCK_OFFSET_DMB			0					// DMB偏移地址
#define KD_BLOCK_OFFSET_EMB			32					// EMB偏移地址
#define KD_BLOCK_OFFSET_CDB			64					// CDB偏移地址
#define KD_BLOCK_OFFSET_DDB			128					// DDB偏移地址

*/


//---------------------------------------------------------
//                     结构
//=========================================================
#pragma pack(1)

// 256 字节
// 
struct AxinGridImageHeader
{
	//-----------------------------------------------------Partion_1 - 128字节
	WORD	wFileID;			// 1		文件标识 'A' | 'X'<<8
	WORD	wFormatCode;		// 2		格式代码
	WORD	wProductCode;		// 3		产品代码
	WORD	wDataSource;		// 4		数据来源

	BYTE	cDataType;			// 5.1		数据类型 1-BYTE  2-SHORT/WORD  4-LONG/DWORD 40-FLOAT 80-DOUBLE
	BYTE	cLayerNums;			// 5.2		层数(最多16层)
	BYTE	cColorTableFlag;	// 6.1		颜色表标记
	BYTE	cProjectType;		// 6.2		投影类型: 0-不投影,1-等角投影,2-麦卡托投影,3-兰布托投影,4-极射赤面投影,5-艾尔伯斯投影
	WORD	wElementCode;		// 7		要素代码
	WORD	wTimePeriod;		// 8		时效

	WORD	wYear;				// 9		年
	WORD	wMonth;				// 10		月
	WORD	wDay;				// 11		日
	WORD	wHour;				// 12		时
	WORD	wMinute;			// 13		分
	WORD	wSecond;			// 14		秒
	DWORD	dwMillisecond;		// 15-16	毫秒

	union
	{
		WORD	xNum;			// 17		列数
		WORD	wWidth;			// 17		列数
	};
	union
	{
		WORD	yNum;			// 18		行数
		WORD	wHeight;		// 18		行数
	};

	union
	{
		float	xMin;			// 19-20	最小经度
		float	fLonMin;		// 19-20	最小经度
	};
	union
	{
		float	yMin;			// 21-22	最小纬度
		float	fLatMin;		// 21-22	最小纬度
	};

	union
	{
		float	xMax;			// 23-24	最大经度
		float	fLonMax;		// 23-24	最大经度
	};
	union
	{
		float	yMax;			// 25-26	最大纬度
		float	fLatMax;		// 25-26	最大纬度
	};

	union
	{
		float	xInterval;			// 27-28	经度分辨率 -- 等角投影		x分辨率 -- 麦卡托、兰布托、极射赤面投影 
		float	fLonResolution;		// 27-28	经度分辨率 -- 等角投影		x分辨率 -- 麦卡托、兰布托、极射赤面投影 
	};
	union
	{
		float	yInterval;			// 29-30	纬度分辨率 -- 等角投影		y分辨率 -- 麦卡托、兰布托、极射赤面投影
		float	fLatResolution;		// 29-30	纬度分辨率 -- 等角投影		y分辨率 -- 麦卡托、兰布托、极射赤面投影
	};

	float	fStandardLon;		// 31-32	圆锥投影中心经线 (LAMBERT,POLAR,ALBERS)

	float	fStandardLat1;		// 33-34	标准纬度1 -- 麦卡托、兰布托、极射赤面投影有效
	float	fStandardLat2;		// 35-36	标准纬度2 -- 兰布托投影有效

	float	fCenterLon;			// 37-38	中心点的经度
	float	fCenterLat;			// 39-40	中心点的纬度

	LONG	dwInvalidData;		// 41-42	无效数据值
	float	fBaseValue;			// 43-44	基准值
	float	fScale;				// 45-46	比例因子

	WORD	wComposeDays;		// 47		已合成天数

	union
	{
		BYTE	cVersion[2];	// 48		版本(255.255)
		WORD	wVersion;		// 48		版本号
	};

	WORD	wLayerCode[16];		// 49-64	层代码
	//-----------------------------------------------------Partion_2 - 64 bytes
	WORD	wReserved2[24];		//			可作为层代码扩展(扩展到40层)

	WORD	wYear2;				// 年
	WORD	wMonth2;			// 月
	WORD	wDay2;				// 日
	WORD	wHour2;				// 时
	WORD	wMinute2;			// 分
	WORD	wSecond2;			// 秒
	DWORD	dwMillisecond2;		// 毫秒
	//-----------------------------------------------------Partion_3 - 64 bytes(用于注释)
	BYTE	cComment[64];		// 注释
	//-----------------------------------------------------

	AxinGridImageHeader()
	{
		memset(this,0,sizeof(AxinGridImageHeader));
		cVersion[0] = 1;
		cVersion[1] = 0;
	}

};

// 颜色表头
// 32 字节
struct ColorTablePacketHeader
{
	WORD	wPacketCode;			// 01		包代码 = 0x80
	DWORD	dwPacketLength;			// 02-03	包长度
	WORD	wType;					// 04		类型
	WORD	wColorEntries;			// 05		颜色入口数
	WORD	wSpecificColorNumbers;	// 06		特定颜色数量
	WORD	wHasComment;			// 07		是否有对应注释
	WORD	wHasTransparentColor;	// 08		使用透明色

	union							// 09-10	透明颜色
	{
		DWORD	dwTransparentColor;
		struct
		{
			BYTE TB;
			BYTE TG;
			BYTE TR;
			BYTE TA;
		};
	};

	union							// 11-12	默认颜色
	{
		DWORD	dwDefaultColor;
		struct
		{
			BYTE DB;
			BYTE DG;
			BYTE DR;
			BYTE DA;
		};
	};

	DWORD	dwSpecialDataMin;		// 13-14	特定最小数据
	DWORD	dwSpecialDataMax;		// 15-16	特定最大数据

	// 颜色数据 ...
	// 注释数据 ...

	ColorTablePacketHeader()
	{
		memset(this,0,sizeof(ColorTablePacketHeader));

		wPacketCode		= Const_Color_Table_Packet_Code;
		dwPacketLength	= sizeof(ColorTablePacketHeader);

		wColorEntries	= 0;

		TA				= 255;
		DA				= 255;

		dwSpecialDataMin = 0x8000;
		dwSpecialDataMin = 0x800F;
	}

};

#pragma pack()

//---------------------------------------------------------
}// namespace Axin - end
//---------------------------------------------------------

#endif	//#ifndef __INCLUDED_AXIN_INL__

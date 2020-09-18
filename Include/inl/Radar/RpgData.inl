#ifndef __INCLUDED_WSR98D_RPG_DATA_INL__
#define __INCLUDED_WSR98D_RPG_DATA_INL__

namespace Wsr98d { namespace Rpg
{
//---------------------------------------------------------
}}// namespace Wsr98d::Rpg - end
//---------------------------------------------------------
using namespace Wsr98d::Rpg;

namespace Wsr98d { namespace Rpg { namespace Helper
{
//---------------------------------------------------------
}}}// namespace Wsr98d::Rpg::Helper - end
//---------------------------------------------------------
using namespace Wsr98d::Rpg::Helper;

#pragma pack(1)

#define RPG_DATA_NO_COLOR_LEVEL							-1				// 无颜色级别

#define RPG_DATA_BLOCK_DIVIDER							-1				// 产品块分隔符
#define RPG_DATA_LAYER_DIVIDER							-1				// 产品层分隔符

#define RPG_DATA_PSB_BLOCK_ID							1				// Product Symbology Block ID
#define RPG_DATA_GAB_BLOCK_ID							2				// Graphic Alphanumeric Block ID
#define RPG_DATA_TAB_BLOCK_ID							3				// Tabular Alphanumeric Block ID

#define RPG_DATA_PRODUCT_CODE_MIN						16				// 产品代码最小值
#define RPG_DATA_PRODUCT_CODE_MAX						131				// 产品代码最大值

#define RPG_DATA_INVALID_VALUE							9999			// 无效的雷达回波值

///========================================================
///WSR-98D SA/SB 数据包代码常量 - beg
///===================================
const int const_PacketCode_Mono_Text							= 1;		// [P3-108] 单色文本
const int const_PacketCode_Special_Symbol						= 2;		// [P3-110] 特殊符号
const int const_PacketCode_Mesocyclone_3						= 3;		// [P3-129] 中气旋-3 (指直径16公里以下的旋风)
const int const_PacketCode_Wind_Barb							= 4;		// [P3-128] 风钩
const int const_PacketCode_Vector_Arrow							= 5;		// [P3-127] 风矢
const int const_PacketCode_Mono_Linked_Vector					= 6;		// [P3- 97] 单色连接矢量
const int const_PacketCode_Mono_Unlinked_Vector					= 7;		// [P3-101] 单色非连接矢量
const int const_PacketCode_Color_Text							= 8;		// [P3-109] 彩色文本
const int const_PacketCode_Color_Linked_Vector					= 9;		// [P3- 98]	彩色连接矢量
const int const_PacketCode_Color_Unlinked_Vector				= 10;		// [P3-102] 彩色非连接矢量
const int const_PacketCode_Mesocyclone_11						= 11;		// [P3-129] 中气旋-11 (指直径16公里以下的旋风)
const int const_PacketCode_Tornado_Vortex_Signature				= 12;		// [P3-129] 龙卷涡旋特征 TVS
const int const_PacketCode_Hail_Positive						= 13;		// [P3-129] 冰雹实况
const int const_PacketCode_Hail_Probable						= 14;		// [P3-129] 冰雹概率
const int const_PacketCode_Storm_Id								= 15;		// [P3-130] 风暴ID
const int const_PacketCode_Digital_Radial_Data_Array			= 16;		// [P3-125] 数字径向数据阵列
const int const_PacketCode_Digital_Precipitation_Data_Array		= 17;		// [P3-121] 数字降水数据阵列
const int const_PacketCode_Precipitation_Rate_Data_Array		= 18;		// [P3-123] 降水率数据阵列
const int const_PacketCode_Hda_Hail								= 19;		// [P3-130] ?
const int const_PacketCode_Cell_Trend							= 21;		// [P3-132] 雷暴单体趋势(Build 10.0)
const int const_PacketCode_Cell_Trend_Volume_Scan_Time			= 22;		// [P3-135] 雷暴单体趋势体扫时间(Build 10.0)
const int const_PacketCode_Scit_Past_Data						= 23;		// [P3-130] 风暴中心信息路径实况数据
const int const_PacketCode_Scit_Forecast_Data					= 24;		// [P3-130] 风暴中心信息路径预报数据
const int const_PacketCode_Sti_Circle							= 25;		// [P3-130] 风暴路径信息圈
const int const_PacketCode_Tornado_Vortex_Signature_Ex			= 26;		// [P3-129] 龙卷涡旋特征 ETVS(Build 10.0)
const int const_PacketCode_Set_Color_Level						= 0x0802;	// [P3-105] 颜色设置级别
const int const_PacketCode_Linked_Contour_Vector				= 0x0e03;	// [P3-105] 连接等值线矢量
const int const_PacketCode_Unlinked_Contour_Vector				= 0x3501;	// [P3-105] 非连接等值线矢量
const int const_PacketCode_Radial_Data							= 0xaf1f;	// [P3-117] 径向数据
const int const_PacketCode_Raster_Data							= 0xba07;	// [P3-119] 栅格数据
const int const_PacketCode_Raster_Data_Normal					= 0xba07;	// [P3-119] 栅格数据
const int const_PacketCode_Raster_Data_Section					= 0xba07;	// [P3-119] 栅格数据 - 剖面
const int const_PacketCode_Raster_Data_Slice					= 0xba0f;	// [P3-119] 栅格数据 - 切片
//------------------------------------------------
const int const_Color_Level_Present_Indicator					= 0x0002;	// 指示器 - 包中存在颜色级别 [P3-105]
const int const_Initial_Point_Indicator							= 0x8000;	// 指示器 - 包中存在初始点 [P3-105]

const int const_Raster_Data_Pack_Descriptor						= 2;		// 栅格数据包描述符 [P3-119]

const int const_Raster_Data_Operate_Flag_1						= 0x8000;	// [P3-119]
const int const_Raster_Data_Operate_Flag_2						= 0x00c0;	// [P3-119]

const int const_Raster_Data_Normal_Operate_Flag_1				= 0x8000;	// [P3-119]
const int const_Raster_Data_Normal_Operate_Flag_2				= 0x00c0;	// [P3-119]

const int const_Raster_Data_Section_Operate_Flag_1				= 0x8000;	// [P3-119]
const int const_Raster_Data_Section_Operate_Flag_2				= 0x00c0;	// [P3-119]

const int const_Raster_Data_Slice_Operate_Flag_1				= 0x8000;	// [P3-119]
const int const_Raster_Data_Slice_Operate_Flag_2				= 0x00c0;	// [P3-119]

// 0x0c23;	//[P3-114]
// 0x0e23;	//[P3-114]
// 0x3521;	//[P3-114]
// 0x4e00;	//[P3-114]
// 0x4e01;	//[P3-114]
///===================================
///WSR-98D SA/SB 数据包代码常量 - end
///========================================================

// 产品类型
enum ProductType
{
	ProductType_None	= 0,
	ProductType_RadialImage,				// 1
	ProductType_RasterImage,				// 2
	ProductType_LinkedContourVectorImage,	// 3
	ProductType_GeographicAlphaImage,		// 4
	ProductType_NonGeographicAlphaImage,	// 5
	ProductType_AlphanumericImage,			// 6

	ProductType_RasterImageSection,			// 7 [2-1]
	ProductType_RasterImageSlice,			// 8 [2-2]
};

///--------------------------------------------------------
///1 - 公用结构、自定义结构 - beg
///------------------------------

// 点集合
typedef vector <POINT> PointVector;

// 距离库的行程编码结构 (P3-117)
struct RunColor
{
public:
	RunColor()
	{
		iRun = 0;
		iValue = 0;
		iColorIndex = 0;
	}
	virtual ~RunColor() {}

public:
	virtual void Attach(char cRunColor)
	{
		iRun = (cRunColor & 0xF0) >> 4;
		iColorIndex	= cRunColor & 0x0F;
	}

	virtual void Attach(sfx_uint16 iRunColor)
	{
		iRun = (iRunColor & 0xFF00) >> 8;
		iColorIndex	= iRunColor & 0xFF;
	}

public:
	sfx_uint16		iRun;			//	距离库号
	sfx_uint16		iValue;			//	要素值
	sfx_uint16		iColorIndex;	//	颜色索引

};

// 1.0.1 位置结构
struct Position
{
	sfx_int16	I;	//I坐标
	sfx_int16	J;	//J坐标
};

// 1.1.1 数据包头结构(1,2,3,4,5,6,9,7,8,10,11,12,13,14,15,19,23,24,25,26)
struct DataPacketHeader
{
	sfx_uint16	iPacketCode;			//	数据包代码(1 to ...)
	sfx_uint16	iLengthOfDataBlock;		//	数据块长度(1 to 32767 字节)
};

// 1.1.2 数据层头结构
struct DataLayerHeader
{
	sfx_int16	iLayerDivider;		//	层分隔标记(-1)
	sfx_uint32	lLengthOfDataLayer;	//	数据层长度(字节: 1 to 80000  不包含本字段和层分隔标记字段)
};

// 1.1.3 每页数据头结构
struct PageHeader
{
	sfx_uint16	iCurrentPageNumber;		//	当前页号(1 to 48)
	sfx_uint16	iLengthOfPage;			//	当前页长度(字节: 4 to 1360 : in Text Packet 1 through Text Packet N)
};

// 1.1.4 每条径向数据头结构 (P3-117)
struct EachRadialHeader
{
	sfx_uint16	iNumbersOfRLECodeInRadial;		//	Number of Run Length Encoded 16-bit halfwords per radial
												//	每条射线 RLECODE 的半字数量(单位:HWORD 1 to 230 精度:1)实际要 *2
	sfx_uint16	iRadialStartAngle;				//	射线开始角度(0 to 3599 精度:0.1)
	sfx_uint16	iDeltaAngle;					//	角度间隔(0 to 20 精度:0.1)
	//One Radial Data Block
};

// 1.1.5 栅格数据每行数据头结构 (P3-119)
struct EachRowHeader
{
	sfx_uint16	iNumberOfBytesInThisRow;	//本行数据字节数(2-920)
	// Data ...
};
///------------------------------
///1 - 公用结构、自定义结构 - end
///--------------------------------------------------------

///--------------------------------------------------------
///2 - 产品文件数据块(5大块) - beg
///-------------------------------
// 2.1 Message Head Block (P3-37)
typedef struct MessageHeaderBlock
{
	sfx_int16	iMsgCode;		//00	信息代码(0 to 211, -131 to -16)
	sfx_uint16	iMsgDate;		//01	日期(Julian Date: 0-32767)
	sfx_uint32	lMsgTime;		//02-03	时间(GMT秒: 0-86399) GMT - Greenwich Mean Time 格林尼治标准时间
	sfx_uint32	lMsgLength;		//04-05 信息长度(包含信息头在内的字节数: 18-409856)
	sfx_uint16	iSID;			//06	发送ID(0-999)
	sfx_uint16	iDID;			//07	接收ID(0-999)
	sfx_uint16	iBlockNumber;	//08	信息中头块加上产品描述块的块数
}MHB;

// 2.2 Product Description Block (P3-54)
typedef struct ProductDescriptionBlock
{
	sfx_int16	iBlockDivider;				//09	块分隔标记(-1)
	sfx_int32	lLat;						//10-11	纬度(度: -90 to +90 精度: 0.001)
	sfx_int32	lLon;						//12-13	经度(度: -180 to +180 精度: 0.001)
	sfx_int16	iAlt;						//14	海拔(英尺: -100 to +11000 精度: 1)
	sfx_uint16	iProductCode;				//15	产品代码(16 to 131)
	sfx_uint16	iOPMode;					//16	操作模式(0 to 2) 0=维护模式 1=晴空模式 2=降雨/强天气模式
	sfx_uint16	iVCP;						//17	体扫模式VCP(Volume Coverage Pattern, 1 to 767)
	sfx_int16	iSequenceNumber;			//18	序列号(-13,0 to 32767)
	sfx_uint16	iVScanNumber;				//19	体扫号(Volume Scan Number, 1 to 80)
	sfx_uint16	iVScanDate;					//20	体扫儒略日(Julian Date: 1 to 32767)
	sfx_uint32	lVScanStartTime;			//21-22	体扫开始时间(GMT秒: 0 to 86399)
	sfx_uint16	iPGDate;					//23	产品生成儒略日(Julian Date: 1 to 32767)
	sfx_uint32	lPGTime;					//24-25	产品生成时间(GMT秒: 0 to 86399)
	sfx_uint16	iPD01;						//26	产品依赖的字定义01
	sfx_uint16	iPD02;						//27	产品依赖的字定义02
	sfx_uint16	iElevationNumber;			//28	体扫仰角号(1 to 20)
	sfx_int16	iPD03;						//29	产品依赖的字定义03
	sfx_uint16	iDataLevelThreshold[16];	//30-45 数据级别(Data Level Threshold)
	sfx_int16	iPD04;						//46	产品依赖的字定义04
	sfx_int16	iPD05;						//47	产品依赖的字定义05
	sfx_uint16	iPD06;						//48	产品依赖的字定义06
	sfx_uint16	iPD07;						//49	产品依赖的字定义07
	sfx_uint16	iPD08;						//50	产品依赖的字定义08
	sfx_uint16	iPD09;						//51	产品依赖的字定义09
	sfx_uint16	iPD10;						//52	产品依赖的字定义10
	union
	{
	sfx_uint16	iMapNumber;					//53	如果是地图为地图号(0 to 17)
	sfx_uint8	cVer_SPTB[2];				//53	如果是产品为版本和白点(Spot Blank)起止标记(0 to 255/0 to 1)
	};
	union
	{
	sfx_uint32	lOffsetSymbology;			//54-55	Symbology 偏移(HWord: 0 to 80000)
	sfx_uint16	iOffsetSymbology[2];		//54-55	Symbology 偏移(HWord: 0 to 80000)
	};
	sfx_uint32	lOffsetGraphic;				//56-57	Graphic	  偏移(HWord: 0 to 80000)
	sfx_uint32	lOffsetTabular;				//58-59	Tabular	  偏移(HWord: 0 to 80000)
}PDB;

// 2.3 Product Symbology Block (P3-57)
typedef struct ProductSymbologyBlock
{
	sfx_int16	iBlockDivider;		//	块分隔符(-1)
	sfx_uint16	iBlockID;			//	块ID(=1)
	sfx_uint32	lBlockLength;		//	块长度,从块分割符到块尾的字节数(字节: 1 to 80000)
	sfx_uint16	iTotalLayers;		//	数据层数(1 to 15)
}PSB;

// 2.4 Graphic Alphanumeric Block (P3-58)
typedef struct GraphicAlphanumericBlock
{
	sfx_int16	iBlockDivider;		//	块分隔标记(-1)
	sfx_uint16	iBlockID;			//	块ID(=2)
	sfx_uint32	lBlockLength;		//	块长度(1 to 65536)
	sfx_uint16	iNumberOfPages;		//	总页数(1 to 48)
}GAB;

// 2.5 Tabular Alphanumeric Block (P3-59)
typedef struct TabularAlphanumericBlock
{
	sfx_int16		iBlockDivider;			//	块分隔标记(-1)
	sfx_uint16		iBlockID;				//	块ID(=3)
	sfx_uint32		lBlockLength;			//	块长度(1 to 65536)
	MHB				tMsgHeader;				//	第二个信息头块
	PDB				tProductDescription;	//	第二个产品描述块
	sfx_int16		iBlockDivider2;			//	块分隔标记2(-1)
	sfx_uint16		iNumberOfPages;			//	总页数(1 to 48)
	//每页最大行数=17
}TAB;
///-------------------------------
///2 - 产品文件数据块(5大块) - end
///--------------------------------------------------------

///--------------------------------------------------------
///3 - 产品数据包头 - beg
///----------------------

// 3.01 单色文本数据包头 (P3-108)
//     Mono Text Data Packet
//     Packet Code = 1
typedef DataPacketHeader MonoTextPacketHeader;

// 3.02 特殊符号数据包头 (P3-110)
//     Special Symbol Packet
//     Packet Code = 2
typedef DataPacketHeader SpecilaSymbolPacketHeader;

// 3.03 中气旋数据包头 (P3-129)
//     Mesocyclone
//     Packet Code = 3/11
typedef DataPacketHeader MesocyclonePacketHeader;

// 3.04 风钩数据包头 (P3-128)
//     Wind Barb Data Packet
//     Packet Code = 4
typedef DataPacketHeader WindBarbDataPacketHeader;

// 3.05 风矢数据包头 (P3-127)
//     Vector Arrow Data Packet
//     Packet Code = 5
typedef DataPacketHeader VectorArrowDataPacketHeader;

// 3.06 单色连接矢量数据包头 (P3-97)
//     Linked Vector Packet
//     Packet Code = 6
typedef DataPacketHeader LinkedVectorPacketHeader;
typedef DataPacketHeader MonoLinkedVectorPacketHeader;

// 3.07 单色非连接矢量数据包头 (P3-101)
//     Unlinked Vector Packet
//     Packet Code = 7
typedef DataPacketHeader UnlinkedVectorPacketHeader;
typedef DataPacketHeader MonoUnlinkedVectorPacketHeader;

// 3.08 彩色文本数据包头 (P3-109)
//     Color Text Data Packet
//     Packet Code = 8
typedef DataPacketHeader ColorTextPacketHeader;

// 3.09 彩色连接矢量数据包头 (P3-98)
//     Linked Vector Packet
//     Packet Code = 9
typedef DataPacketHeader ColorLinkedVectorPacketHeader;

// 3.10 彩色非连接矢量数据包头 (P3-102)
//     Unlinked Vector Packet
//     Packet Code = 10
typedef DataPacketHeader ColorUnlinkedVectorPacketHeader;

// 3.11 中气旋数据包头 (P3-129)
//     Mesocyclone
//     Packet Code = 3/11
// 参看 3.03

// 3.12 龙卷蜗旋特征数据包头 (P3-129)
//     Tornado Vortex Signature
//     Packet Code = 12
typedef DataPacketHeader TvsPacketHeader,TornadoVortexSignaturePacketHeader;

// 3.13 冰雹实况数据包头 (P3-129)
//     Hail Positive
//     Packet Code = 13
typedef DataPacketHeader HailPositivePacketHeader;

// 3.14 冰雹概率数据包头 (P3-129)
//     Hail Probable
//     Packet Code = 14
typedef DataPacketHeader HailProbablePacketHeader;

// 3.15 风暴ID数据包头 (P3-130)
//     Storm ID
//     Packet Code = 15
typedef DataPacketHeader StormIdPacketHeader;

// 3.16 数字径向数据阵列数据包头 (P3-125)
//     Digital Radial Data Array Packet
//     Packet Code = 16
struct DigitalRadialDataArrayPacketHeader
{
	sfx_uint16	iPacketCode;					//	数据包代码(=16)
	sfx_uint16	iIndexOfFirstRangeBin;			//	第一个距离库的索引(0 to 230)
	sfx_uint16	iNumberOfRangeBinsPerRadial;	//	组成一条径向数据的距离库数(1 to 230)
	sfx_int16	iICenterOfSweep;				//	扫描中心I(单位:km/4 -2048 to 2047 )
	sfx_int16	iJCenterOfSweep;				//	扫描中心J(单位:km/4 -2048 to 2047 )
	sfx_uint16	iScaleFactor;					//	每个距离库包含的像素数(1 to 8000 精度:0.001)
	sfx_uint16	iNumberOfRadials;				//	射线总数量(1 to 400 精度:1)
	//Repeat For Each Radial Data
};

// 3.17 数字降水数据阵列数据包头 (P3-121)
//     Digital Precipatation Data Array Packet
//     Packet Code = 17
struct DigitalPrecipitationDataArrayPacketHeader
{
	sfx_uint16	iPacketCode;				// 17
	sfx_uint16	Spare[2];					// 未用
	sfx_int16	iNumberOfLFMBoxsInRow;		// 0 to 131 Number of boxes in each row
	sfx_int16	iNumberOfRows;				// 0 to 131 Total number of rows
	//Repeat For Each Row
};

// 3.18 降水率数据阵列数据包头 (P3-123)
//     Precipitation Rate Data Array Packet
//     Packet Code = 18
struct PrecipitationRateDataArrayPacketHeader
{
	sfx_uint16	iPacketCode;				// 18
	sfx_uint16	Spare[2];					// 未用
	sfx_int16	iNumberOfLFMBoxsInRow;		// Number of boxes in each row
	sfx_int16	iNumberOfRows;				// Total number of rows
	//Repeat For Each Row
};

// 3.19 冰雹概率数据包头 (P3-130)
//     HDA Hail
//     Packet Code = 19 (P62)
typedef DataPacketHeader HdaHailPacketHeader;

// 3.20 待定数据包
//     ?? Data Packet
//     Packet Code = 20

// 3.21 雷暴单体趋势(Build 10.0) (P3-132)
//     Cell Trend Data Packet
//     Packet Code = 21
struct CellTrendDataPacketHeader
{
	sfx_uint16	iPacketCode;			// 数据包代码(1 to ...)
	sfx_uint16	iLengthOfDataBlock;		// 数据块长度(1 to 32767 字节)
	sfx_uchar	cID1;					// Cell ID 的第一个字母
	sfx_uchar	cID2;					// Cell ID 的第二个字母
	Position	oPosition;				// Cell 位置 (km/8)
};

// 3.22 雷暴单体趋势体扫时间(Build 10.0)  (P3-135)
//     Cell Trend Volume Scan Times
//     Packet Code = 22
typedef DataPacketHeader CellTrendVolumeScanTimePacketHeader;

// 3.23 风暴中心信息路径实况数据 (P3-130) 
//     SCIT Past Data
//     Packet Code = 23
typedef DataPacketHeader ScitPastDataPacketHeader;

// 3.24 风暴中心信息路径预报数据 (P3-130)
//     SCIT Forecast Data
//     Packet Code = 24 (P62)
typedef DataPacketHeader ScitForecastDataPacketHeader;

// 3.25  风暴路径信息圈 (P3-130)
//      STI Circle
//      Packet Code = 25
typedef DataPacketHeader StiCirclePacketHeader;

// 3.26 龙卷蜗旋特征数据包头 (P3-129)
//     ETornado Vortex Signature
//     Packet Code = 26
typedef DataPacketHeader TvsPacketHeaderEx;

// 3.X0802 颜色设置级别数据包头 (P3-105)
//        Set Color Levels
//        Packet Code = 0x0802
struct SetColorLevelPacketHeader
{
	sfx_uint16 iPacketCode;				//  数据包代码(=0x0802)
	sfx_uint16 iColorValueIndicator;	//  颜色值指示器,其值=0x0002说明线条具有颜色
};

// 3.X0e03 连接等值线矢量数据包头  [P3-105]
//        Linked Contour Vectors Packet
//        Packet Code = 0x0e03
struct LinkedContourVectorPacketHeader
{
	sfx_uint16	iPacketCode;			//	包代码(0x0e03)
	sfx_uint16	iInitialPointIndicator;	//	初始位置存在本包中指示器(0x8000)
};

// 3.X3501 非连接等值线矢量数据包头 [P3-105]
//        Unlinked Contour Vectors Packet
//        Packet Code = 0x3501
struct UnlinkedContourVectorPacketHeader
{
	sfx_uint16	iPacketCode;			//	包代码(0x3501)
	sfx_uint16	iLengthOfVector;		//	矢量长度(字节: 8 to 32760, length = # of vectors * 8)
};

// 3.Xaf1f 径向数据包 [P3-117]
//        Radial DataPacket
//        Packet Code = 0xAF1F
struct RadialDataPacketHeader
{
	sfx_uint16	iPacketCode;					//	数据包代码(=0XAF1F)
	sfx_uint16	iIndexOfFirstRangeBin;			//	第一个距离库的索引(0 to 460)
	sfx_uint16	iNumberOfRangeBinsPerRadial;	//	组成一条径向数据的距离库数(1 to 460)
	sfx_int16	iICenterOfSweep;				//	扫描中心I(单位:km/4 -2048 to 2047 )
	sfx_int16	iJCenterOfSweep;				//	扫描中心J(单位:km/4 -2048 to 2047 )
	sfx_uint16	iScaleFactor;					//	每个距离库包含的像素数(1 to 8000 精度:0.001)
	sfx_uint16	iNumberOfRadials;				//	射线总数量(1 to 400 精度:1)
	//Repeat For Each Radial Data
};

// 3.Xba07 栅格数据包 [P3-119]
//        Raster Data Packet 
//        Packet Code1 = 0xBA07 Packet Code2 = 0x8000 Packet Code3 = 0x00C0] (P50)
//3.Xba0f 栅格数据包 [P3-119]
//        Raster Data Packet 
//        Packet Code1 = 0xBA0F Packet Code2 = 0x8000 Packet Code3 = 0x00C0] (P50)
struct RasterDataPacketHeader
{
	sfx_uint16	iPacketCode[3];		//	数据包代码 
									//  iPacketCode[0]=0xBA0F 或者 0xBA07
									//  iPacketCode[1]=0x8000
									//  iPacketCode[2]=0x00C0
	sfx_int16	iICoordinateStart;	//	I向数据的开始位置
	sfx_int16	iJCoordinateStart;	//	J向数据的开始位置
	sfx_uint16	iXScaleINT;			//	格点刻度要素( 1 to 67)
	sfx_uint16	iXScaleFractional;	//	保留给内部PUP使用
	sfx_uint16	iYScaleINT;			//	格点刻度要素( 1 to 67)
	sfx_uint16	iYScaleFractional;	//	保留给内部PUP使用
	sfx_uint16	iNumberOfRows;		//	行数(1 to 464)
	sfx_uint16	iPackDescriptor;	//	包描述符(=2),定义包的格式
	//Repeat For Each Row
};
///----------------------
///3 - 产品数据包头 - end
///--------------------------------------------------------

///--------------------------------------------------------
///4 - 产品数据包收集结构 - beg
///----------------------------
// 4.01 单色文本数据包 [P3-108]
//     Mono Text Data Packet
//     Packet Code = 1
struct WriteText
{
	sfx_int16	iColorLevel;	// 颜色等级(-1表示没有颜色等级)
	Position	oPosition;		// 文本起始位置
	char		szText[128];	// 字符串
};
typedef TPtrList <WriteText*> KWriteTextList;	// 字符串链表

typedef WriteText MonoText;
typedef KWriteTextList KMonoTextList;			// 单色文本链表

// 4.02 特殊符号数据包 [P3-110]
//     Special Symbol Packet
//     Packet Code = 2
typedef WriteText		SpecilaSymbol;
typedef KWriteTextList	KSpecilaSymbolList;	//特殊符号链表

// 4.03 中气旋数据包 [P3-129]
//     Mesocyclone
//     Packet Code = 3/11
struct Mesocyclone
{
	Position	oPosition;					//中气旋位置
	sfx_int16	iRadiusOfMesocyclone;		//中气旋半径
};
typedef TPtrList <Mesocyclone*> KMesocycloneList;	//中气旋链表

// 4.04 风钩数据包 [P3-128]
//     Wind Barb Data Packet
//     Packet Code = 4
struct WindBarb
{
	sfx_uint16	iColorLevelOfWindBarb;	//	风羽的颜色等级(字节: 1 to 15)
	sfx_int16	iXCoordinate;			//	风羽X位置(km/4 or Pixels)
	sfx_int16	iYCoordinate;			//	风羽Y位置(km/4 or Pixels)
	sfx_uint16	iWindDirection;			//	风向
	sfx_uint16	iWindSpeed;				//	风速
};
typedef TPtrList <WindBarb*> KWindBarbList;	//风钩链表

// 4.05 风矢数据包 [P3-127]
//     Vector Arrow Data Packet
//     Packet Code = 5
struct VectorArrow
{
	sfx_int16	iICoordinate;				//矢箭X位置(km/4 or Pixels)
	sfx_int16	iJCoordinate;				//矢箭Y位置(km/4 or Pixels)
	sfx_uint16	iDirectionOfArrow;			//矢箭方向
	sfx_uint16	iLengthOfArrow;				//矢箭长度
	sfx_uint16	iLengthOfArrowHead;			//矢箭头长度
};
typedef TPtrList <VectorArrow*> KVectorArrowList;	//矢箭链表

// 4.06 单色连接矢量数据包 [P3- 97]
//     Linked Vector Packet
//     Packet Code = 6
struct LinkedVector
{
	sfx_int16		iColorLevel;	//颜色等级(-1表示没有颜色等级)
	PointVector		oPointVector;	//连线(点集合) oLngPointArray
};
typedef TPtrList <LinkedVector*> KLinkedVectorList;	// 连接矢量数据

typedef LinkedVector MonoLinkedVector;
typedef KLinkedVectorList KMonoLinkedVectorList;	// 单色连接矢量数据

// 4.07 单色非连接矢量数据包 [P3-101]
//     Unlinked Vector Packet
//     Packet Code = 7
struct UnlinkedVector
{
	sfx_int16		iColorLevel;		//颜色等级(-1表示没有颜色等级)

	//vector<POINT>	oPointVector;	//连线(点集合) oLngPointArray
	PointVector		oPointVectorBeg;	//连线(点集合) oLngPointArray
	PointVector		oPointVectorEnd;	//连线(点集合) oLngPointArray
	//KLngPointArray	oLngPointArrayBeg;	//线段起点集合
	//KLngPointArray	oLngPointArrayEnd;	//线段终点集合
};
typedef TPtrList <UnlinkedVector*> KUnlinkedVectorList;

typedef UnlinkedVector MonoUnlinkedVector;
typedef KUnlinkedVectorList	KMonoUnlinkedVectorList;

// 4.08 彩色文本数据包 [P3-109]
//     Color Text Data Packet
//     Packet Code = 8
typedef WriteText		ColorText;
typedef KWriteTextList	KColorTextList;	//彩色文本链表

// 4.09 彩色连接矢量数据包 [P3- 98]
//     Linked Vector Packet
//     Packet Code = 9 
typedef LinkedVector		ColorLinkedVector;
typedef KLinkedVectorList	KColorLinkedVectorList;

// 4.10 彩色非连接矢量数据包 [P3-102]
//     Unlinked Vector Packet
//     Packet Code = 10
typedef UnlinkedVector		ColorUnlinkedVector;
typedef KUnlinkedVectorList	KColorUnlinkedVectorList;

// 4.11 中气旋数据包 [P3-129]
//     Mesocyclone
//     Packet Code = 3 / 11
// 参考 4.03

// 4.12 龙卷蜗旋特征数据包 [P3-129]
//     Tornado Vortex Signature
//     Packet Code = 12
typedef Position Tvs;
typedef Position TornadoVortexSignature;

// 4.13 冰雹实况数据包 [P3-129]
//     Hail Positive
//     Packet Code = 13
typedef Position HailPositive;

// 4.14 冰雹概率数据包 [P3-129]
//     Hail Probable
//     Packet Code = 14
typedef Position HailProbable;

//4.15 风暴ID数据包 [P3-130]
//     Storm ID
//     Packet Code = 15
typedef WriteText StormId;
typedef KWriteTextList KStormIdList;

// 4.16 数字径向数据阵列数据包 [P3-125]
//     Digital Radial Data Array Packet
//     Packet Code = 16
struct Radial
{
	EachRadialHeader	oEachRadialHeader;		//  每条径向数据射线头结构
	RunColor*			pRunColor;				//  距离库数组指针 (分配时 pRunColor = new TRunColor[iBinNum])

public:
	Radial()
	{
		memset(&oEachRadialHeader,0,sizeof(EachRadialHeader));
		pRunColor = 0;
	}

	virtual ~Radial()
	{
		if(pRunColor)
		{
			delete[] pRunColor;
			pRunColor = 0;
		}
	}

};
typedef TPtrList <Radial*> KRadialList;	// 用来保存一个平面上的径向射线数据

typedef Radial		DigitalRadialDataArray;
typedef KRadialList	KDigitalRadialDataArrayList;

// 4.17 数字降水数据阵列数据包 [P3-121]
//     Digital Precipatation Data Array Packet
//     Packet Code = 17
typedef struct RasterRow
{
	union
	{
		EachRowHeader	oEachRowHeader;
		sfx_uint16		iNumberOfBytesInThisRow;	//  本行数据字节数(2-920)
	};

	RunColor*			pRunColor;					//  距离库数组指针 (分配时 pRunColor = new TRunColor[??])

public:
	RasterRow()
	{
		iNumberOfBytesInThisRow = 0;
		pRunColor = NULL;
	}

	virtual ~RasterRow()
	{
		if(pRunColor)
		{
			delete[] pRunColor;
			pRunColor=NULL;
		}
	}
}RasterRow;
typedef TPtrList <RasterRow*> KRasterRowList;	// 用来保存一个平面上的栅格图像数据

typedef RasterRow		DigitalPrecipitationDataArray;
typedef KRasterRowList	KDigitalPrecipitationDataArrayList;

// 4.18 降水率数据阵列数据包 [P3-123]
//     Precipitation Rate Data Array Packet
//     Packet Code = 18
typedef RasterRow		PrecipitationRateDataArray;
typedef KRasterRowList	KPrecipitationRateDataArrayList;

// 4.19 冰雹概率数据包 [P3-130]
//     HDA Hail
//     Packet Code = 19
struct HdaHail
{
	Position	oPosition;					// 位置(km/4)
	sfx_int16	iProbabilityOfHail;			// 冰雹概率
	sfx_int16	iProbabilityOfSevereHail;	// 强冰雹概率
	sfx_uint16	iMaxHailSize;				// 最大冰雹尺寸(预期)
};
typedef TPtrList <HdaHail*> KHdaHailList;	//冰雹概率链表

//4.20 待定数据包

//4.21 待定数据包

//4.22 待定数据包

//4.25 风暴路径信息圈数据包  [P3-130]
//     STI Circle
//     Packet Code = 25
struct StiCircle
{
	Position	oPosition;				//位置
	sfx_int16	iRadiusOfCircle;		//半径
};
typedef TPtrList <StiCircle*> KStiCircleList;

// 4.23 风暴中心信息路径实况数据包 [P3-130]
//     SCIT Past Data
//     Packet Code = 23
struct Scit
{
	KSpecilaSymbolList		oSpecilaSymbolList;		// Packet Code = 2
	KMonoLinkedVectorList	oMonoLinkedVectorList;	// Packet Code = 6
	KStiCircleList			oStiCircleList;			// Packet Code = 25
};
typedef TPtrList <Scit*> KScitList;

typedef Scit ScitPast;
typedef KScitList KScitPastList;

// 4.24 风暴中心路径信息预报数据包 [P3-130]
//     SCIT Forecast Data
//     Packet Code = 24
typedef Scit ScitForecast;
typedef KScitList KScitForecastList;

// 4.26 龙卷蜗旋特征数据包 [P3-129]
//     ETornado Vortex Signature
//     Packet Code = 26
typedef Position TvsEx;
typedef Position TornadoVortexSignatureEx;

// 4.x0802 颜色级别数据包 [P3-105]
//        Set Color Levels
//        Packet Code = 0x0802

// 4.x0E03 连接等值线矢量数据包 [P3-105]
//        Linked Contour Vectors Packet
//        Packet Code = 0x0E03
typedef LinkedVector		LinkedContourVector;
typedef KLinkedVectorList	KLinkedContourVectorList;

// 4.x3501 非连接等值线矢量数据包 [P3-105]
//        UnLinked Contour Vectors Packet
//        Packet Code = 0x3501
typedef UnlinkedVector		UnlinkedContourVector;
typedef KUnlinkedVectorList	KUnlinkedContourVectorList;

// 4.xAF1F 径向数据包 [P3-117]
//        Radial Data Packet
//        Packet Code = 0xAF1F
//typedef struct Radial TRadial;
//typedef TxList <TRadial*> KRadialList;	// 用来保存一个平面上的径向射线数据

// 4.xBA07 栅格数据包 [P3-119]
//        Raster Data Packet
//        Packet Code = 0xBA07
//typedef struct RasterRow TRasterRow;
//typedef TxList <TRasterRow*> KRasterRowList;	// 用来保存一个平面上的栅格图像数据
///----------------------------
///4 - 产品数据包收集结构 - end
///--------------------------------------------------------

///--------------------------------------------------------
///9 - 自定义辅助结构 - beg
///------------------------
// 9.1 产品数据图例
struct Legend
{
	double		dValue;			// 要素值
	COLORREF	dwColor;		// 对应的颜色
	TCHAR		szText[64];		// 注释
};

// 9.2 产品信息名称
struct ProductNameInfo
{
	TCHAR	szEnName[64];		// 雷达产品英文名字
	TCHAR	szCnName[128];		// 雷达产品中文名字
	TCHAR	szContraction[16];	// 雷达产品英文缩写
};

// 9.3 用经纬度坐标表示的距离库
struct RangeBin
{
	float		x[4];			// 库四边形顶点的x坐标
	float		y[4];			// 库四边形顶点的y坐标
	float		fValue;			// 要素值

	int			iColorIndex;	// 颜色索引
	COLORREF	clrBin;			// 库的颜色

public:
	RangeBin()
	{
		for (int i = 0; i < 4; i++)
		{
			x[i] = 0;
			y[i] = 0;
		}
		fValue = 0;
		iColorIndex = 0;
		clrBin = 0;
	}
	RangeBin(RangeBin& rb)
	{
		for (int i = 0; i < 4; i++)
		{
			x[i] = rb.x[i];
			y[i] = rb.y[i];
		}
		fValue = rb.fValue;
		iColorIndex = rb.iColorIndex;
		clrBin = rb.clrBin;
	}
};
typedef TPtrList <RangeBin*> KRangeBinList;	// 距离库链表
typedef TPtrList <KRangeBinList*> KRangeBinListCollection;

//9.3.1 每条径向射线上的距离库
// RangeBinPerRadial
struct RangeBinPerRadial
{
	INT16 iFlag;					//质控标记
	double dVariability;			//数据变率(表示数列平均变动程度的量)
	KRangeBinList* pRangeBinList;	//距离库链表

public:
	RangeBinPerRadial()
	{
		iFlag = 0;
		dVariability = 0;
		pRangeBinList = 0;
	}

	virtual ~RangeBinPerRadial()
	{
		if (pRangeBinList)
		{
			delete pRangeBinList;
			pRangeBinList = 0;
		}
	}


};
typedef TPtrList <RangeBinPerRadial*> KRangeBinPerRadialList;	// 每条径向射线上的距离库链表

// 数据级别
struct DataLevelThreshold
{
	WORD wDataLevel;		// 数据级别
	double dThreshold;		// 数据级别下限值
	TCHAR szLegendText[64];	// 图例文本
};

// 9.4 雷达信息
struct RadarInfo
{
	sfx_uint16	id;							// 雷达站ID
	TCHAR		szStationName[64];			// 雷达站名称
	double		dLon;						// 雷达站经度
	double		dLat;						// 雷达站纬度
	double		dAlt;						// 雷达站海拔(m)

	WORD		wYear;						// 年
	WORD		wMonth;						// 月
	WORD		wDay;						// 日
	WORD		wHour;						// 时
	WORD		wMinute;					// 分
	WORD		wSecond;					// 秒

	sfx_int16	iOPMode;					// 操作模式
	TCHAR		szCnOPModeName[MAX_PATH];	// 操作模式中文名称
	TCHAR		szEnOPModeName[MAX_PATH];	// 操作模式英文名称

	sfx_int16	iVCP;						// 体扫模式

	sfx_int16	iElevationNumber;			// 仰角号
	double		dElevation;					// 仰角

	// 产品部分
	//
	sfx_uint16	iProductCode;				// 产品代码(16-131)

	TCHAR		szUnit[16];					// 度量单位
	TCHAR		szCnName[128];				// 雷达产品中文名字
	TCHAR		szEnName[64];				// 雷达产品英文名字
	TCHAR		szContraction[16];			// 雷达产品英文缩写

	double		dRange;						// 覆盖范围(km)
	double		dResolution;				// 分辨率(km)

	double		dPolarRange;				// 距离圈距离(km)

	TCHAR		szSpecialInfo[10][64];		// 特殊信息(用于标注信息区)

	TCHAR		szComment[MAX_PATH];		// 雷达说明性文字
};

///------------------------
///9 - 自定义辅助结构 - end
///--------------------------------------------------------

#pragma pack()

#endif	//#ifndef __INCLUDED_WSR98D_RPG_DATA_INL__

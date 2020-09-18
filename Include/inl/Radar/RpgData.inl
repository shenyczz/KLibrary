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

#define RPG_DATA_NO_COLOR_LEVEL							-1				// ����ɫ����

#define RPG_DATA_BLOCK_DIVIDER							-1				// ��Ʒ��ָ���
#define RPG_DATA_LAYER_DIVIDER							-1				// ��Ʒ��ָ���

#define RPG_DATA_PSB_BLOCK_ID							1				// Product Symbology Block ID
#define RPG_DATA_GAB_BLOCK_ID							2				// Graphic Alphanumeric Block ID
#define RPG_DATA_TAB_BLOCK_ID							3				// Tabular Alphanumeric Block ID

#define RPG_DATA_PRODUCT_CODE_MIN						16				// ��Ʒ������Сֵ
#define RPG_DATA_PRODUCT_CODE_MAX						131				// ��Ʒ�������ֵ

#define RPG_DATA_INVALID_VALUE							9999			// ��Ч���״�ز�ֵ

///========================================================
///WSR-98D SA/SB ���ݰ����볣�� - beg
///===================================
const int const_PacketCode_Mono_Text							= 1;		// [P3-108] ��ɫ�ı�
const int const_PacketCode_Special_Symbol						= 2;		// [P3-110] �������
const int const_PacketCode_Mesocyclone_3						= 3;		// [P3-129] ������-3 (ֱָ��16�������µ�����)
const int const_PacketCode_Wind_Barb							= 4;		// [P3-128] �繳
const int const_PacketCode_Vector_Arrow							= 5;		// [P3-127] ��ʸ
const int const_PacketCode_Mono_Linked_Vector					= 6;		// [P3- 97] ��ɫ����ʸ��
const int const_PacketCode_Mono_Unlinked_Vector					= 7;		// [P3-101] ��ɫ������ʸ��
const int const_PacketCode_Color_Text							= 8;		// [P3-109] ��ɫ�ı�
const int const_PacketCode_Color_Linked_Vector					= 9;		// [P3- 98]	��ɫ����ʸ��
const int const_PacketCode_Color_Unlinked_Vector				= 10;		// [P3-102] ��ɫ������ʸ��
const int const_PacketCode_Mesocyclone_11						= 11;		// [P3-129] ������-11 (ֱָ��16�������µ�����)
const int const_PacketCode_Tornado_Vortex_Signature				= 12;		// [P3-129] ������������ TVS
const int const_PacketCode_Hail_Positive						= 13;		// [P3-129] ����ʵ��
const int const_PacketCode_Hail_Probable						= 14;		// [P3-129] ��������
const int const_PacketCode_Storm_Id								= 15;		// [P3-130] �籩ID
const int const_PacketCode_Digital_Radial_Data_Array			= 16;		// [P3-125] ���־�����������
const int const_PacketCode_Digital_Precipitation_Data_Array		= 17;		// [P3-121] ���ֽ�ˮ��������
const int const_PacketCode_Precipitation_Rate_Data_Array		= 18;		// [P3-123] ��ˮ����������
const int const_PacketCode_Hda_Hail								= 19;		// [P3-130] ?
const int const_PacketCode_Cell_Trend							= 21;		// [P3-132] �ױ���������(Build 10.0)
const int const_PacketCode_Cell_Trend_Volume_Scan_Time			= 22;		// [P3-135] �ױ�����������ɨʱ��(Build 10.0)
const int const_PacketCode_Scit_Past_Data						= 23;		// [P3-130] �籩������Ϣ·��ʵ������
const int const_PacketCode_Scit_Forecast_Data					= 24;		// [P3-130] �籩������Ϣ·��Ԥ������
const int const_PacketCode_Sti_Circle							= 25;		// [P3-130] �籩·����ϢȦ
const int const_PacketCode_Tornado_Vortex_Signature_Ex			= 26;		// [P3-129] ������������ ETVS(Build 10.0)
const int const_PacketCode_Set_Color_Level						= 0x0802;	// [P3-105] ��ɫ���ü���
const int const_PacketCode_Linked_Contour_Vector				= 0x0e03;	// [P3-105] ���ӵ�ֵ��ʸ��
const int const_PacketCode_Unlinked_Contour_Vector				= 0x3501;	// [P3-105] �����ӵ�ֵ��ʸ��
const int const_PacketCode_Radial_Data							= 0xaf1f;	// [P3-117] ��������
const int const_PacketCode_Raster_Data							= 0xba07;	// [P3-119] դ������
const int const_PacketCode_Raster_Data_Normal					= 0xba07;	// [P3-119] դ������
const int const_PacketCode_Raster_Data_Section					= 0xba07;	// [P3-119] դ������ - ����
const int const_PacketCode_Raster_Data_Slice					= 0xba0f;	// [P3-119] դ������ - ��Ƭ
//------------------------------------------------
const int const_Color_Level_Present_Indicator					= 0x0002;	// ָʾ�� - ���д�����ɫ���� [P3-105]
const int const_Initial_Point_Indicator							= 0x8000;	// ָʾ�� - ���д��ڳ�ʼ�� [P3-105]

const int const_Raster_Data_Pack_Descriptor						= 2;		// դ�����ݰ������� [P3-119]

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
///WSR-98D SA/SB ���ݰ����볣�� - end
///========================================================

// ��Ʒ����
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
///1 - ���ýṹ���Զ���ṹ - beg
///------------------------------

// �㼯��
typedef vector <POINT> PointVector;

// �������г̱���ṹ (P3-117)
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
	sfx_uint16		iRun;			//	������
	sfx_uint16		iValue;			//	Ҫ��ֵ
	sfx_uint16		iColorIndex;	//	��ɫ����

};

// 1.0.1 λ�ýṹ
struct Position
{
	sfx_int16	I;	//I����
	sfx_int16	J;	//J����
};

// 1.1.1 ���ݰ�ͷ�ṹ(1,2,3,4,5,6,9,7,8,10,11,12,13,14,15,19,23,24,25,26)
struct DataPacketHeader
{
	sfx_uint16	iPacketCode;			//	���ݰ�����(1 to ...)
	sfx_uint16	iLengthOfDataBlock;		//	���ݿ鳤��(1 to 32767 �ֽ�)
};

// 1.1.2 ���ݲ�ͷ�ṹ
struct DataLayerHeader
{
	sfx_int16	iLayerDivider;		//	��ָ����(-1)
	sfx_uint32	lLengthOfDataLayer;	//	���ݲ㳤��(�ֽ�: 1 to 80000  ���������ֶκͲ�ָ�����ֶ�)
};

// 1.1.3 ÿҳ����ͷ�ṹ
struct PageHeader
{
	sfx_uint16	iCurrentPageNumber;		//	��ǰҳ��(1 to 48)
	sfx_uint16	iLengthOfPage;			//	��ǰҳ����(�ֽ�: 4 to 1360 : in Text Packet 1 through Text Packet N)
};

// 1.1.4 ÿ����������ͷ�ṹ (P3-117)
struct EachRadialHeader
{
	sfx_uint16	iNumbersOfRLECodeInRadial;		//	Number of Run Length Encoded 16-bit halfwords per radial
												//	ÿ������ RLECODE �İ�������(��λ:HWORD 1 to 230 ����:1)ʵ��Ҫ *2
	sfx_uint16	iRadialStartAngle;				//	���߿�ʼ�Ƕ�(0 to 3599 ����:0.1)
	sfx_uint16	iDeltaAngle;					//	�Ƕȼ��(0 to 20 ����:0.1)
	//One Radial Data Block
};

// 1.1.5 դ������ÿ������ͷ�ṹ (P3-119)
struct EachRowHeader
{
	sfx_uint16	iNumberOfBytesInThisRow;	//���������ֽ���(2-920)
	// Data ...
};
///------------------------------
///1 - ���ýṹ���Զ���ṹ - end
///--------------------------------------------------------

///--------------------------------------------------------
///2 - ��Ʒ�ļ����ݿ�(5���) - beg
///-------------------------------
// 2.1 Message Head Block (P3-37)
typedef struct MessageHeaderBlock
{
	sfx_int16	iMsgCode;		//00	��Ϣ����(0 to 211, -131 to -16)
	sfx_uint16	iMsgDate;		//01	����(Julian Date: 0-32767)
	sfx_uint32	lMsgTime;		//02-03	ʱ��(GMT��: 0-86399) GMT - Greenwich Mean Time �������α�׼ʱ��
	sfx_uint32	lMsgLength;		//04-05 ��Ϣ����(������Ϣͷ���ڵ��ֽ���: 18-409856)
	sfx_uint16	iSID;			//06	����ID(0-999)
	sfx_uint16	iDID;			//07	����ID(0-999)
	sfx_uint16	iBlockNumber;	//08	��Ϣ��ͷ����ϲ�Ʒ������Ŀ���
}MHB;

// 2.2 Product Description Block (P3-54)
typedef struct ProductDescriptionBlock
{
	sfx_int16	iBlockDivider;				//09	��ָ����(-1)
	sfx_int32	lLat;						//10-11	γ��(��: -90 to +90 ����: 0.001)
	sfx_int32	lLon;						//12-13	����(��: -180 to +180 ����: 0.001)
	sfx_int16	iAlt;						//14	����(Ӣ��: -100 to +11000 ����: 1)
	sfx_uint16	iProductCode;				//15	��Ʒ����(16 to 131)
	sfx_uint16	iOPMode;					//16	����ģʽ(0 to 2) 0=ά��ģʽ 1=���ģʽ 2=����/ǿ����ģʽ
	sfx_uint16	iVCP;						//17	��ɨģʽVCP(Volume Coverage Pattern, 1 to 767)
	sfx_int16	iSequenceNumber;			//18	���к�(-13,0 to 32767)
	sfx_uint16	iVScanNumber;				//19	��ɨ��(Volume Scan Number, 1 to 80)
	sfx_uint16	iVScanDate;					//20	��ɨ������(Julian Date: 1 to 32767)
	sfx_uint32	lVScanStartTime;			//21-22	��ɨ��ʼʱ��(GMT��: 0 to 86399)
	sfx_uint16	iPGDate;					//23	��Ʒ����������(Julian Date: 1 to 32767)
	sfx_uint32	lPGTime;					//24-25	��Ʒ����ʱ��(GMT��: 0 to 86399)
	sfx_uint16	iPD01;						//26	��Ʒ�������ֶ���01
	sfx_uint16	iPD02;						//27	��Ʒ�������ֶ���02
	sfx_uint16	iElevationNumber;			//28	��ɨ���Ǻ�(1 to 20)
	sfx_int16	iPD03;						//29	��Ʒ�������ֶ���03
	sfx_uint16	iDataLevelThreshold[16];	//30-45 ���ݼ���(Data Level Threshold)
	sfx_int16	iPD04;						//46	��Ʒ�������ֶ���04
	sfx_int16	iPD05;						//47	��Ʒ�������ֶ���05
	sfx_uint16	iPD06;						//48	��Ʒ�������ֶ���06
	sfx_uint16	iPD07;						//49	��Ʒ�������ֶ���07
	sfx_uint16	iPD08;						//50	��Ʒ�������ֶ���08
	sfx_uint16	iPD09;						//51	��Ʒ�������ֶ���09
	sfx_uint16	iPD10;						//52	��Ʒ�������ֶ���10
	union
	{
	sfx_uint16	iMapNumber;					//53	����ǵ�ͼΪ��ͼ��(0 to 17)
	sfx_uint8	cVer_SPTB[2];				//53	����ǲ�ƷΪ�汾�Ͱ׵�(Spot Blank)��ֹ���(0 to 255/0 to 1)
	};
	union
	{
	sfx_uint32	lOffsetSymbology;			//54-55	Symbology ƫ��(HWord: 0 to 80000)
	sfx_uint16	iOffsetSymbology[2];		//54-55	Symbology ƫ��(HWord: 0 to 80000)
	};
	sfx_uint32	lOffsetGraphic;				//56-57	Graphic	  ƫ��(HWord: 0 to 80000)
	sfx_uint32	lOffsetTabular;				//58-59	Tabular	  ƫ��(HWord: 0 to 80000)
}PDB;

// 2.3 Product Symbology Block (P3-57)
typedef struct ProductSymbologyBlock
{
	sfx_int16	iBlockDivider;		//	��ָ���(-1)
	sfx_uint16	iBlockID;			//	��ID(=1)
	sfx_uint32	lBlockLength;		//	�鳤��,�ӿ�ָ������β���ֽ���(�ֽ�: 1 to 80000)
	sfx_uint16	iTotalLayers;		//	���ݲ���(1 to 15)
}PSB;

// 2.4 Graphic Alphanumeric Block (P3-58)
typedef struct GraphicAlphanumericBlock
{
	sfx_int16	iBlockDivider;		//	��ָ����(-1)
	sfx_uint16	iBlockID;			//	��ID(=2)
	sfx_uint32	lBlockLength;		//	�鳤��(1 to 65536)
	sfx_uint16	iNumberOfPages;		//	��ҳ��(1 to 48)
}GAB;

// 2.5 Tabular Alphanumeric Block (P3-59)
typedef struct TabularAlphanumericBlock
{
	sfx_int16		iBlockDivider;			//	��ָ����(-1)
	sfx_uint16		iBlockID;				//	��ID(=3)
	sfx_uint32		lBlockLength;			//	�鳤��(1 to 65536)
	MHB				tMsgHeader;				//	�ڶ�����Ϣͷ��
	PDB				tProductDescription;	//	�ڶ�����Ʒ������
	sfx_int16		iBlockDivider2;			//	��ָ����2(-1)
	sfx_uint16		iNumberOfPages;			//	��ҳ��(1 to 48)
	//ÿҳ�������=17
}TAB;
///-------------------------------
///2 - ��Ʒ�ļ����ݿ�(5���) - end
///--------------------------------------------------------

///--------------------------------------------------------
///3 - ��Ʒ���ݰ�ͷ - beg
///----------------------

// 3.01 ��ɫ�ı����ݰ�ͷ (P3-108)
//     Mono Text Data Packet
//     Packet Code = 1
typedef DataPacketHeader MonoTextPacketHeader;

// 3.02 ����������ݰ�ͷ (P3-110)
//     Special Symbol Packet
//     Packet Code = 2
typedef DataPacketHeader SpecilaSymbolPacketHeader;

// 3.03 ���������ݰ�ͷ (P3-129)
//     Mesocyclone
//     Packet Code = 3/11
typedef DataPacketHeader MesocyclonePacketHeader;

// 3.04 �繳���ݰ�ͷ (P3-128)
//     Wind Barb Data Packet
//     Packet Code = 4
typedef DataPacketHeader WindBarbDataPacketHeader;

// 3.05 ��ʸ���ݰ�ͷ (P3-127)
//     Vector Arrow Data Packet
//     Packet Code = 5
typedef DataPacketHeader VectorArrowDataPacketHeader;

// 3.06 ��ɫ����ʸ�����ݰ�ͷ (P3-97)
//     Linked Vector Packet
//     Packet Code = 6
typedef DataPacketHeader LinkedVectorPacketHeader;
typedef DataPacketHeader MonoLinkedVectorPacketHeader;

// 3.07 ��ɫ������ʸ�����ݰ�ͷ (P3-101)
//     Unlinked Vector Packet
//     Packet Code = 7
typedef DataPacketHeader UnlinkedVectorPacketHeader;
typedef DataPacketHeader MonoUnlinkedVectorPacketHeader;

// 3.08 ��ɫ�ı����ݰ�ͷ (P3-109)
//     Color Text Data Packet
//     Packet Code = 8
typedef DataPacketHeader ColorTextPacketHeader;

// 3.09 ��ɫ����ʸ�����ݰ�ͷ (P3-98)
//     Linked Vector Packet
//     Packet Code = 9
typedef DataPacketHeader ColorLinkedVectorPacketHeader;

// 3.10 ��ɫ������ʸ�����ݰ�ͷ (P3-102)
//     Unlinked Vector Packet
//     Packet Code = 10
typedef DataPacketHeader ColorUnlinkedVectorPacketHeader;

// 3.11 ���������ݰ�ͷ (P3-129)
//     Mesocyclone
//     Packet Code = 3/11
// �ο� 3.03

// 3.12 ���������������ݰ�ͷ (P3-129)
//     Tornado Vortex Signature
//     Packet Code = 12
typedef DataPacketHeader TvsPacketHeader,TornadoVortexSignaturePacketHeader;

// 3.13 ����ʵ�����ݰ�ͷ (P3-129)
//     Hail Positive
//     Packet Code = 13
typedef DataPacketHeader HailPositivePacketHeader;

// 3.14 �����������ݰ�ͷ (P3-129)
//     Hail Probable
//     Packet Code = 14
typedef DataPacketHeader HailProbablePacketHeader;

// 3.15 �籩ID���ݰ�ͷ (P3-130)
//     Storm ID
//     Packet Code = 15
typedef DataPacketHeader StormIdPacketHeader;

// 3.16 ���־��������������ݰ�ͷ (P3-125)
//     Digital Radial Data Array Packet
//     Packet Code = 16
struct DigitalRadialDataArrayPacketHeader
{
	sfx_uint16	iPacketCode;					//	���ݰ�����(=16)
	sfx_uint16	iIndexOfFirstRangeBin;			//	��һ������������(0 to 230)
	sfx_uint16	iNumberOfRangeBinsPerRadial;	//	���һ���������ݵľ������(1 to 230)
	sfx_int16	iICenterOfSweep;				//	ɨ������I(��λ:km/4 -2048 to 2047 )
	sfx_int16	iJCenterOfSweep;				//	ɨ������J(��λ:km/4 -2048 to 2047 )
	sfx_uint16	iScaleFactor;					//	ÿ������������������(1 to 8000 ����:0.001)
	sfx_uint16	iNumberOfRadials;				//	����������(1 to 400 ����:1)
	//Repeat For Each Radial Data
};

// 3.17 ���ֽ�ˮ�����������ݰ�ͷ (P3-121)
//     Digital Precipatation Data Array Packet
//     Packet Code = 17
struct DigitalPrecipitationDataArrayPacketHeader
{
	sfx_uint16	iPacketCode;				// 17
	sfx_uint16	Spare[2];					// δ��
	sfx_int16	iNumberOfLFMBoxsInRow;		// 0 to 131 Number of boxes in each row
	sfx_int16	iNumberOfRows;				// 0 to 131 Total number of rows
	//Repeat For Each Row
};

// 3.18 ��ˮ�������������ݰ�ͷ (P3-123)
//     Precipitation Rate Data Array Packet
//     Packet Code = 18
struct PrecipitationRateDataArrayPacketHeader
{
	sfx_uint16	iPacketCode;				// 18
	sfx_uint16	Spare[2];					// δ��
	sfx_int16	iNumberOfLFMBoxsInRow;		// Number of boxes in each row
	sfx_int16	iNumberOfRows;				// Total number of rows
	//Repeat For Each Row
};

// 3.19 �����������ݰ�ͷ (P3-130)
//     HDA Hail
//     Packet Code = 19 (P62)
typedef DataPacketHeader HdaHailPacketHeader;

// 3.20 �������ݰ�
//     ?? Data Packet
//     Packet Code = 20

// 3.21 �ױ���������(Build 10.0) (P3-132)
//     Cell Trend Data Packet
//     Packet Code = 21
struct CellTrendDataPacketHeader
{
	sfx_uint16	iPacketCode;			// ���ݰ�����(1 to ...)
	sfx_uint16	iLengthOfDataBlock;		// ���ݿ鳤��(1 to 32767 �ֽ�)
	sfx_uchar	cID1;					// Cell ID �ĵ�һ����ĸ
	sfx_uchar	cID2;					// Cell ID �ĵڶ�����ĸ
	Position	oPosition;				// Cell λ�� (km/8)
};

// 3.22 �ױ�����������ɨʱ��(Build 10.0)  (P3-135)
//     Cell Trend Volume Scan Times
//     Packet Code = 22
typedef DataPacketHeader CellTrendVolumeScanTimePacketHeader;

// 3.23 �籩������Ϣ·��ʵ������ (P3-130) 
//     SCIT Past Data
//     Packet Code = 23
typedef DataPacketHeader ScitPastDataPacketHeader;

// 3.24 �籩������Ϣ·��Ԥ������ (P3-130)
//     SCIT Forecast Data
//     Packet Code = 24 (P62)
typedef DataPacketHeader ScitForecastDataPacketHeader;

// 3.25  �籩·����ϢȦ (P3-130)
//      STI Circle
//      Packet Code = 25
typedef DataPacketHeader StiCirclePacketHeader;

// 3.26 ���������������ݰ�ͷ (P3-129)
//     ETornado Vortex Signature
//     Packet Code = 26
typedef DataPacketHeader TvsPacketHeaderEx;

// 3.X0802 ��ɫ���ü������ݰ�ͷ (P3-105)
//        Set Color Levels
//        Packet Code = 0x0802
struct SetColorLevelPacketHeader
{
	sfx_uint16 iPacketCode;				//  ���ݰ�����(=0x0802)
	sfx_uint16 iColorValueIndicator;	//  ��ɫֵָʾ��,��ֵ=0x0002˵������������ɫ
};

// 3.X0e03 ���ӵ�ֵ��ʸ�����ݰ�ͷ  [P3-105]
//        Linked Contour Vectors Packet
//        Packet Code = 0x0e03
struct LinkedContourVectorPacketHeader
{
	sfx_uint16	iPacketCode;			//	������(0x0e03)
	sfx_uint16	iInitialPointIndicator;	//	��ʼλ�ô��ڱ�����ָʾ��(0x8000)
};

// 3.X3501 �����ӵ�ֵ��ʸ�����ݰ�ͷ [P3-105]
//        Unlinked Contour Vectors Packet
//        Packet Code = 0x3501
struct UnlinkedContourVectorPacketHeader
{
	sfx_uint16	iPacketCode;			//	������(0x3501)
	sfx_uint16	iLengthOfVector;		//	ʸ������(�ֽ�: 8 to 32760, length = # of vectors * 8)
};

// 3.Xaf1f �������ݰ� [P3-117]
//        Radial DataPacket
//        Packet Code = 0xAF1F
struct RadialDataPacketHeader
{
	sfx_uint16	iPacketCode;					//	���ݰ�����(=0XAF1F)
	sfx_uint16	iIndexOfFirstRangeBin;			//	��һ������������(0 to 460)
	sfx_uint16	iNumberOfRangeBinsPerRadial;	//	���һ���������ݵľ������(1 to 460)
	sfx_int16	iICenterOfSweep;				//	ɨ������I(��λ:km/4 -2048 to 2047 )
	sfx_int16	iJCenterOfSweep;				//	ɨ������J(��λ:km/4 -2048 to 2047 )
	sfx_uint16	iScaleFactor;					//	ÿ������������������(1 to 8000 ����:0.001)
	sfx_uint16	iNumberOfRadials;				//	����������(1 to 400 ����:1)
	//Repeat For Each Radial Data
};

// 3.Xba07 դ�����ݰ� [P3-119]
//        Raster Data Packet 
//        Packet Code1 = 0xBA07 Packet Code2 = 0x8000 Packet Code3 = 0x00C0] (P50)
//3.Xba0f դ�����ݰ� [P3-119]
//        Raster Data Packet 
//        Packet Code1 = 0xBA0F Packet Code2 = 0x8000 Packet Code3 = 0x00C0] (P50)
struct RasterDataPacketHeader
{
	sfx_uint16	iPacketCode[3];		//	���ݰ����� 
									//  iPacketCode[0]=0xBA0F ���� 0xBA07
									//  iPacketCode[1]=0x8000
									//  iPacketCode[2]=0x00C0
	sfx_int16	iICoordinateStart;	//	I�����ݵĿ�ʼλ��
	sfx_int16	iJCoordinateStart;	//	J�����ݵĿ�ʼλ��
	sfx_uint16	iXScaleINT;			//	���̶�Ҫ��( 1 to 67)
	sfx_uint16	iXScaleFractional;	//	�������ڲ�PUPʹ��
	sfx_uint16	iYScaleINT;			//	���̶�Ҫ��( 1 to 67)
	sfx_uint16	iYScaleFractional;	//	�������ڲ�PUPʹ��
	sfx_uint16	iNumberOfRows;		//	����(1 to 464)
	sfx_uint16	iPackDescriptor;	//	��������(=2),������ĸ�ʽ
	//Repeat For Each Row
};
///----------------------
///3 - ��Ʒ���ݰ�ͷ - end
///--------------------------------------------------------

///--------------------------------------------------------
///4 - ��Ʒ���ݰ��ռ��ṹ - beg
///----------------------------
// 4.01 ��ɫ�ı����ݰ� [P3-108]
//     Mono Text Data Packet
//     Packet Code = 1
struct WriteText
{
	sfx_int16	iColorLevel;	// ��ɫ�ȼ�(-1��ʾû����ɫ�ȼ�)
	Position	oPosition;		// �ı���ʼλ��
	char		szText[128];	// �ַ���
};
typedef TPtrList <WriteText*> KWriteTextList;	// �ַ�������

typedef WriteText MonoText;
typedef KWriteTextList KMonoTextList;			// ��ɫ�ı�����

// 4.02 ����������ݰ� [P3-110]
//     Special Symbol Packet
//     Packet Code = 2
typedef WriteText		SpecilaSymbol;
typedef KWriteTextList	KSpecilaSymbolList;	//�����������

// 4.03 ���������ݰ� [P3-129]
//     Mesocyclone
//     Packet Code = 3/11
struct Mesocyclone
{
	Position	oPosition;					//������λ��
	sfx_int16	iRadiusOfMesocyclone;		//�������뾶
};
typedef TPtrList <Mesocyclone*> KMesocycloneList;	//����������

// 4.04 �繳���ݰ� [P3-128]
//     Wind Barb Data Packet
//     Packet Code = 4
struct WindBarb
{
	sfx_uint16	iColorLevelOfWindBarb;	//	�������ɫ�ȼ�(�ֽ�: 1 to 15)
	sfx_int16	iXCoordinate;			//	����Xλ��(km/4 or Pixels)
	sfx_int16	iYCoordinate;			//	����Yλ��(km/4 or Pixels)
	sfx_uint16	iWindDirection;			//	����
	sfx_uint16	iWindSpeed;				//	����
};
typedef TPtrList <WindBarb*> KWindBarbList;	//�繳����

// 4.05 ��ʸ���ݰ� [P3-127]
//     Vector Arrow Data Packet
//     Packet Code = 5
struct VectorArrow
{
	sfx_int16	iICoordinate;				//ʸ��Xλ��(km/4 or Pixels)
	sfx_int16	iJCoordinate;				//ʸ��Yλ��(km/4 or Pixels)
	sfx_uint16	iDirectionOfArrow;			//ʸ������
	sfx_uint16	iLengthOfArrow;				//ʸ������
	sfx_uint16	iLengthOfArrowHead;			//ʸ��ͷ����
};
typedef TPtrList <VectorArrow*> KVectorArrowList;	//ʸ������

// 4.06 ��ɫ����ʸ�����ݰ� [P3- 97]
//     Linked Vector Packet
//     Packet Code = 6
struct LinkedVector
{
	sfx_int16		iColorLevel;	//��ɫ�ȼ�(-1��ʾû����ɫ�ȼ�)
	PointVector		oPointVector;	//����(�㼯��) oLngPointArray
};
typedef TPtrList <LinkedVector*> KLinkedVectorList;	// ����ʸ������

typedef LinkedVector MonoLinkedVector;
typedef KLinkedVectorList KMonoLinkedVectorList;	// ��ɫ����ʸ������

// 4.07 ��ɫ������ʸ�����ݰ� [P3-101]
//     Unlinked Vector Packet
//     Packet Code = 7
struct UnlinkedVector
{
	sfx_int16		iColorLevel;		//��ɫ�ȼ�(-1��ʾû����ɫ�ȼ�)

	//vector<POINT>	oPointVector;	//����(�㼯��) oLngPointArray
	PointVector		oPointVectorBeg;	//����(�㼯��) oLngPointArray
	PointVector		oPointVectorEnd;	//����(�㼯��) oLngPointArray
	//KLngPointArray	oLngPointArrayBeg;	//�߶���㼯��
	//KLngPointArray	oLngPointArrayEnd;	//�߶��յ㼯��
};
typedef TPtrList <UnlinkedVector*> KUnlinkedVectorList;

typedef UnlinkedVector MonoUnlinkedVector;
typedef KUnlinkedVectorList	KMonoUnlinkedVectorList;

// 4.08 ��ɫ�ı����ݰ� [P3-109]
//     Color Text Data Packet
//     Packet Code = 8
typedef WriteText		ColorText;
typedef KWriteTextList	KColorTextList;	//��ɫ�ı�����

// 4.09 ��ɫ����ʸ�����ݰ� [P3- 98]
//     Linked Vector Packet
//     Packet Code = 9 
typedef LinkedVector		ColorLinkedVector;
typedef KLinkedVectorList	KColorLinkedVectorList;

// 4.10 ��ɫ������ʸ�����ݰ� [P3-102]
//     Unlinked Vector Packet
//     Packet Code = 10
typedef UnlinkedVector		ColorUnlinkedVector;
typedef KUnlinkedVectorList	KColorUnlinkedVectorList;

// 4.11 ���������ݰ� [P3-129]
//     Mesocyclone
//     Packet Code = 3 / 11
// �ο� 4.03

// 4.12 ���������������ݰ� [P3-129]
//     Tornado Vortex Signature
//     Packet Code = 12
typedef Position Tvs;
typedef Position TornadoVortexSignature;

// 4.13 ����ʵ�����ݰ� [P3-129]
//     Hail Positive
//     Packet Code = 13
typedef Position HailPositive;

// 4.14 �����������ݰ� [P3-129]
//     Hail Probable
//     Packet Code = 14
typedef Position HailProbable;

//4.15 �籩ID���ݰ� [P3-130]
//     Storm ID
//     Packet Code = 15
typedef WriteText StormId;
typedef KWriteTextList KStormIdList;

// 4.16 ���־��������������ݰ� [P3-125]
//     Digital Radial Data Array Packet
//     Packet Code = 16
struct Radial
{
	EachRadialHeader	oEachRadialHeader;		//  ÿ��������������ͷ�ṹ
	RunColor*			pRunColor;				//  ���������ָ�� (����ʱ pRunColor = new TRunColor[iBinNum])

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
typedef TPtrList <Radial*> KRadialList;	// ��������һ��ƽ���ϵľ�����������

typedef Radial		DigitalRadialDataArray;
typedef KRadialList	KDigitalRadialDataArrayList;

// 4.17 ���ֽ�ˮ�����������ݰ� [P3-121]
//     Digital Precipatation Data Array Packet
//     Packet Code = 17
typedef struct RasterRow
{
	union
	{
		EachRowHeader	oEachRowHeader;
		sfx_uint16		iNumberOfBytesInThisRow;	//  ���������ֽ���(2-920)
	};

	RunColor*			pRunColor;					//  ���������ָ�� (����ʱ pRunColor = new TRunColor[??])

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
typedef TPtrList <RasterRow*> KRasterRowList;	// ��������һ��ƽ���ϵ�դ��ͼ������

typedef RasterRow		DigitalPrecipitationDataArray;
typedef KRasterRowList	KDigitalPrecipitationDataArrayList;

// 4.18 ��ˮ�������������ݰ� [P3-123]
//     Precipitation Rate Data Array Packet
//     Packet Code = 18
typedef RasterRow		PrecipitationRateDataArray;
typedef KRasterRowList	KPrecipitationRateDataArrayList;

// 4.19 �����������ݰ� [P3-130]
//     HDA Hail
//     Packet Code = 19
struct HdaHail
{
	Position	oPosition;					// λ��(km/4)
	sfx_int16	iProbabilityOfHail;			// ��������
	sfx_int16	iProbabilityOfSevereHail;	// ǿ��������
	sfx_uint16	iMaxHailSize;				// �������ߴ�(Ԥ��)
};
typedef TPtrList <HdaHail*> KHdaHailList;	//������������

//4.20 �������ݰ�

//4.21 �������ݰ�

//4.22 �������ݰ�

//4.25 �籩·����ϢȦ���ݰ�  [P3-130]
//     STI Circle
//     Packet Code = 25
struct StiCircle
{
	Position	oPosition;				//λ��
	sfx_int16	iRadiusOfCircle;		//�뾶
};
typedef TPtrList <StiCircle*> KStiCircleList;

// 4.23 �籩������Ϣ·��ʵ�����ݰ� [P3-130]
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

// 4.24 �籩����·����ϢԤ�����ݰ� [P3-130]
//     SCIT Forecast Data
//     Packet Code = 24
typedef Scit ScitForecast;
typedef KScitList KScitForecastList;

// 4.26 ���������������ݰ� [P3-129]
//     ETornado Vortex Signature
//     Packet Code = 26
typedef Position TvsEx;
typedef Position TornadoVortexSignatureEx;

// 4.x0802 ��ɫ�������ݰ� [P3-105]
//        Set Color Levels
//        Packet Code = 0x0802

// 4.x0E03 ���ӵ�ֵ��ʸ�����ݰ� [P3-105]
//        Linked Contour Vectors Packet
//        Packet Code = 0x0E03
typedef LinkedVector		LinkedContourVector;
typedef KLinkedVectorList	KLinkedContourVectorList;

// 4.x3501 �����ӵ�ֵ��ʸ�����ݰ� [P3-105]
//        UnLinked Contour Vectors Packet
//        Packet Code = 0x3501
typedef UnlinkedVector		UnlinkedContourVector;
typedef KUnlinkedVectorList	KUnlinkedContourVectorList;

// 4.xAF1F �������ݰ� [P3-117]
//        Radial Data Packet
//        Packet Code = 0xAF1F
//typedef struct Radial TRadial;
//typedef TxList <TRadial*> KRadialList;	// ��������һ��ƽ���ϵľ�����������

// 4.xBA07 դ�����ݰ� [P3-119]
//        Raster Data Packet
//        Packet Code = 0xBA07
//typedef struct RasterRow TRasterRow;
//typedef TxList <TRasterRow*> KRasterRowList;	// ��������һ��ƽ���ϵ�դ��ͼ������
///----------------------------
///4 - ��Ʒ���ݰ��ռ��ṹ - end
///--------------------------------------------------------

///--------------------------------------------------------
///9 - �Զ��帨���ṹ - beg
///------------------------
// 9.1 ��Ʒ����ͼ��
struct Legend
{
	double		dValue;			// Ҫ��ֵ
	COLORREF	dwColor;		// ��Ӧ����ɫ
	TCHAR		szText[64];		// ע��
};

// 9.2 ��Ʒ��Ϣ����
struct ProductNameInfo
{
	TCHAR	szEnName[64];		// �״��ƷӢ������
	TCHAR	szCnName[128];		// �״��Ʒ��������
	TCHAR	szContraction[16];	// �״��ƷӢ����д
};

// 9.3 �þ�γ�������ʾ�ľ����
struct RangeBin
{
	float		x[4];			// ���ı��ζ����x����
	float		y[4];			// ���ı��ζ����y����
	float		fValue;			// Ҫ��ֵ

	int			iColorIndex;	// ��ɫ����
	COLORREF	clrBin;			// �����ɫ

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
typedef TPtrList <RangeBin*> KRangeBinList;	// ���������
typedef TPtrList <KRangeBinList*> KRangeBinListCollection;

//9.3.1 ÿ�����������ϵľ����
// RangeBinPerRadial
struct RangeBinPerRadial
{
	INT16 iFlag;					//�ʿر��
	double dVariability;			//���ݱ���(��ʾ����ƽ���䶯�̶ȵ���)
	KRangeBinList* pRangeBinList;	//���������

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
typedef TPtrList <RangeBinPerRadial*> KRangeBinPerRadialList;	// ÿ�����������ϵľ��������

// ���ݼ���
struct DataLevelThreshold
{
	WORD wDataLevel;		// ���ݼ���
	double dThreshold;		// ���ݼ�������ֵ
	TCHAR szLegendText[64];	// ͼ���ı�
};

// 9.4 �״���Ϣ
struct RadarInfo
{
	sfx_uint16	id;							// �״�վID
	TCHAR		szStationName[64];			// �״�վ����
	double		dLon;						// �״�վ����
	double		dLat;						// �״�վγ��
	double		dAlt;						// �״�վ����(m)

	WORD		wYear;						// ��
	WORD		wMonth;						// ��
	WORD		wDay;						// ��
	WORD		wHour;						// ʱ
	WORD		wMinute;					// ��
	WORD		wSecond;					// ��

	sfx_int16	iOPMode;					// ����ģʽ
	TCHAR		szCnOPModeName[MAX_PATH];	// ����ģʽ��������
	TCHAR		szEnOPModeName[MAX_PATH];	// ����ģʽӢ������

	sfx_int16	iVCP;						// ��ɨģʽ

	sfx_int16	iElevationNumber;			// ���Ǻ�
	double		dElevation;					// ����

	// ��Ʒ����
	//
	sfx_uint16	iProductCode;				// ��Ʒ����(16-131)

	TCHAR		szUnit[16];					// ������λ
	TCHAR		szCnName[128];				// �״��Ʒ��������
	TCHAR		szEnName[64];				// �״��ƷӢ������
	TCHAR		szContraction[16];			// �״��ƷӢ����д

	double		dRange;						// ���Ƿ�Χ(km)
	double		dResolution;				// �ֱ���(km)

	double		dPolarRange;				// ����Ȧ����(km)

	TCHAR		szSpecialInfo[10][64];		// ������Ϣ(���ڱ�ע��Ϣ��)

	TCHAR		szComment[MAX_PATH];		// �״�˵��������
};

///------------------------
///9 - �Զ��帨���ṹ - end
///--------------------------------------------------------

#pragma pack()

#endif	//#ifndef __INCLUDED_WSR98D_RPG_DATA_INL__

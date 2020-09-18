#ifndef __INCLUDED_AXIN_INL__
#define __INCLUDED_AXIN_INL__

#include "Lightning.inl"	// �׵�

namespace Axin
{
//---------------------------------------------------------
}// namespace Axin - end
//---------------------------------------------------------
using namespace Axin;

namespace Axin
{

//---------------------------------------------------------
//                     ��
//=========================================================

// Axin һ��궨��
//
#define AXIN_FID_TXT_MARK				_T("AXIN")			// Axin �ı��ļ���ʶ
#define AXIN_FID_BIN_MARK				('A' | 'X' << 8)	// Axin �������ļ���ʶ

#define AXIN_COMMENT_LENGTH				64		// ע����Ϣ����

// ��Ч����
//
#define AXIN_INVALID_DATA				-9999				// ��Ч����

// ���⴦������(���ݺ��θ߶ȶ���)
//
#define AXIN_SPECIAL_DATA_BEG			0x8000				// ��ʼ		= 32768
#define AXIN_SPECIAL_DATA_8001			0x8001				// 
#define AXIN_SPECIAL_DATA_CLOUD			0x8002				// ��		= 32770
#define AXIN_SPECIAL_DATA_FOG			0x8003				// ��		= 32771
#define AXIN_SPECIAL_DATA_FIRING		0x8004				// �����	= 32772
#define AXIN_SPECIAL_DATA_FIRED			0x8005				// �����	= 32773
#define AXIN_SPECIAL_DATA_SNOW			0x8006				// ѩ		= 32774
#define AXIN_SPECIAL_DATA_ICE			0x8007				// ��		= 32775
#define AXIN_SPECIAL_DATA_LAND			0x8008				// ½��		= 32776
#define AXIN_SPECIAL_DATA_8009			0x8009				// 
#define AXIN_SPECIAL_DATA_800A			0x800A				// 
#define AXIN_SPECIAL_DATA_800B			0x800B				// 
#define AXIN_SPECIAL_DATA_800C			0x800C				// 
#define AXIN_SPECIAL_DATA_800D			0x800D				// 
#define AXIN_SPECIAL_DATA_800E			0x800E				// 
#define AXIN_SPECIAL_DATA_WATER			0x800F				// ˮ��		= 32783
#define AXIN_SPECIAL_DATA_END			0x8010				// ����		= 32784

// Axin �ļ���ʽ����(0~99)
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

#define AXIN_FMT_CODE_TIN				30		// ��ɢ����
#define AXIN_FMT_CODE_31				31		// 
#define AXIN_FMT_CODE_32				32		// 
#define AXIN_FMT_CODE_33				33		// 
#define AXIN_FMT_CODE_34				34		// 
#define AXIN_FMT_CODE_35				35		// 
#define AXIN_FMT_CODE_36				36		// 
#define AXIN_FMT_CODE_37				37		// 
#define AXIN_FMT_CODE_38				38		// 
#define AXIN_FMT_CODE_39				39		// 

#define AXIN_FMT_CODE_GRID				40		// �������
#define AXIN_FMT_CODE_GRID_CONTOUR		40		// �������(�������ɵ�ֵ��)
#define AXIN_FMT_CODE_GRID_IMAGE		41		// �������(��������ͼ��)
#define AXIN_FMT_CODE_GRID_42			42		// 
#define AXIN_FMT_CODE_43				43		// 
#define AXIN_FMT_CODE_44				44		// 
#define AXIN_FMT_CODE_45				45		// 
#define AXIN_FMT_CODE_46				46		// 
#define AXIN_FMT_CODE_47				47		// 
#define AXIN_FMT_CODE_48				48		// 
#define AXIN_FMT_CODE_GRID_WIND_VECTOR	49		// �������(���ڷ糡ʸ��)

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

#define AXIN_FMT_CODE_COLOR_TABLE		60		// ��ɫ���ļ�
#define AXIN_FMT_CODE_61				61		// 
#define AXIN_FMT_CODE_62				62		// 
#define AXIN_FMT_CODE_63				63		// 
#define AXIN_FMT_CODE_64				64		// 
#define AXIN_FMT_CODE_65				65		// 
#define AXIN_FMT_CODE_66				66		// 
#define AXIN_FMT_CODE_67				67		// 
#define AXIN_FMT_CODE_68				68		// 
#define AXIN_FMT_CODE_69				69		// 

#define AXIN_FMT_CODE_IMAGE_CTRL_INFO	70		// ͼ�������Ϣ
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

#define AXIN_FMT_CVID_MIN				1		// ��ֵ��������Сֵ
#define AXIN_FMT_CVID_MAX				99		// ��ֵ���������ֵ
#define AXIN_FMT_CVID_AUTO				8888	// ��ֵ������ = 8888 �ĺ����˵����
#define AXIN_FMT_CVID_CONTOUR			9999	// ��ֵ������ = 9999 �ĺ����˵����
#define AXIN_FMT_CLIP_AREA_BOX			9999	// ����������

//---------------------------------------------------------
//                     ����
//=========================================================
const int Const_Color_Table_Packet_Code		= 0x80;		// ��ɫ�����ݰ�����

//---------------------------------------------------------
//                     ��
//=========================================================
/*
#define KD_BLOCK_DIVIDER			0xFFFF				// ��ָ���
#define KD_BLOCK_ID_DHB				0xF0F0				// DHB��ID
#define _KD_BLOCK_ID_DDB			1					// DDB��ID
#define KD_BLOCK_ID_DSB				2					// DSB��ID

#define KD_BLOCK_ID_DMB				0xF1F1				// DMB��ID
#define KD_BLOCK_ID_DDB				0xD1D1				// DDB��ID
#define KD_BLOCK_ID_EDB				0xE1E1				// EDB��ID
#define KD_BLOCK_ID_CDB				0xC1C1				// CDB��ID
#define KD_BLOCK_ID_SDB				1					// SDB��ID
#define KD_BLOCK_ID_LDB				2					// LDB��ID
#define KD_BLOCK_ID_TDB				3					// TDB��ID

#define KD_BLOCK_LENGTH_DMB			32					// DMB�鳤��
#define KD_BLOCK_LENGTH_EMB			32					// EMB�鳤��
#define KD_BLOCK_LENGTH_CDB			64					// CDB�鳤��
#define KD_BLOCK_LENGTH_DDB			128					// DDB�鳤��

#define KD_BLOCK_OFFSET_DMB			0					// DMBƫ�Ƶ�ַ
#define KD_BLOCK_OFFSET_EMB			32					// EMBƫ�Ƶ�ַ
#define KD_BLOCK_OFFSET_CDB			64					// CDBƫ�Ƶ�ַ
#define KD_BLOCK_OFFSET_DDB			128					// DDBƫ�Ƶ�ַ

*/


//---------------------------------------------------------
//                     �ṹ
//=========================================================
#pragma pack(1)

// 256 �ֽ�
// 
struct AxinGridImageHeader
{
	//-----------------------------------------------------Partion_1 - 128�ֽ�
	WORD	wFileID;			// 1		�ļ���ʶ 'A' | 'X'<<8
	WORD	wFormatCode;		// 2		��ʽ����
	WORD	wProductCode;		// 3		��Ʒ����
	WORD	wDataSource;		// 4		������Դ

	BYTE	cDataType;			// 5.1		�������� 1-BYTE  2-SHORT/WORD  4-LONG/DWORD 40-FLOAT 80-DOUBLE
	BYTE	cLayerNums;			// 5.2		����(���16��)
	BYTE	cColorTableFlag;	// 6.1		��ɫ����
	BYTE	cProjectType;		// 6.2		ͶӰ����: 0-��ͶӰ,1-�Ƚ�ͶӰ,2-����ͶӰ,3-������ͶӰ,4-�������ͶӰ,5-������˹ͶӰ
	WORD	wElementCode;		// 7		Ҫ�ش���
	WORD	wTimePeriod;		// 8		ʱЧ

	WORD	wYear;				// 9		��
	WORD	wMonth;				// 10		��
	WORD	wDay;				// 11		��
	WORD	wHour;				// 12		ʱ
	WORD	wMinute;			// 13		��
	WORD	wSecond;			// 14		��
	DWORD	dwMillisecond;		// 15-16	����

	union
	{
		WORD	xNum;			// 17		����
		WORD	wWidth;			// 17		����
	};
	union
	{
		WORD	yNum;			// 18		����
		WORD	wHeight;		// 18		����
	};

	union
	{
		float	xMin;			// 19-20	��С����
		float	fLonMin;		// 19-20	��С����
	};
	union
	{
		float	yMin;			// 21-22	��Сγ��
		float	fLatMin;		// 21-22	��Сγ��
	};

	union
	{
		float	xMax;			// 23-24	��󾭶�
		float	fLonMax;		// 23-24	��󾭶�
	};
	union
	{
		float	yMax;			// 25-26	���γ��
		float	fLatMax;		// 25-26	���γ��
	};

	union
	{
		float	xInterval;			// 27-28	���ȷֱ��� -- �Ƚ�ͶӰ		x�ֱ��� -- ���С������С��������ͶӰ 
		float	fLonResolution;		// 27-28	���ȷֱ��� -- �Ƚ�ͶӰ		x�ֱ��� -- ���С������С��������ͶӰ 
	};
	union
	{
		float	yInterval;			// 29-30	γ�ȷֱ��� -- �Ƚ�ͶӰ		y�ֱ��� -- ���С������С��������ͶӰ
		float	fLatResolution;		// 29-30	γ�ȷֱ��� -- �Ƚ�ͶӰ		y�ֱ��� -- ���С������С��������ͶӰ
	};

	float	fStandardLon;		// 31-32	Բ׶ͶӰ���ľ��� (LAMBERT,POLAR,ALBERS)

	float	fStandardLat1;		// 33-34	��׼γ��1 -- ���С������С��������ͶӰ��Ч
	float	fStandardLat2;		// 35-36	��׼γ��2 -- ������ͶӰ��Ч

	float	fCenterLon;			// 37-38	���ĵ�ľ���
	float	fCenterLat;			// 39-40	���ĵ��γ��

	LONG	dwInvalidData;		// 41-42	��Ч����ֵ
	float	fBaseValue;			// 43-44	��׼ֵ
	float	fScale;				// 45-46	��������

	WORD	wComposeDays;		// 47		�Ѻϳ�����

	union
	{
		BYTE	cVersion[2];	// 48		�汾(255.255)
		WORD	wVersion;		// 48		�汾��
	};

	WORD	wLayerCode[16];		// 49-64	�����
	//-----------------------------------------------------Partion_2 - 64 bytes
	WORD	wReserved2[24];		//			����Ϊ�������չ(��չ��40��)

	WORD	wYear2;				// ��
	WORD	wMonth2;			// ��
	WORD	wDay2;				// ��
	WORD	wHour2;				// ʱ
	WORD	wMinute2;			// ��
	WORD	wSecond2;			// ��
	DWORD	dwMillisecond2;		// ����
	//-----------------------------------------------------Partion_3 - 64 bytes(����ע��)
	BYTE	cComment[64];		// ע��
	//-----------------------------------------------------

	AxinGridImageHeader()
	{
		memset(this,0,sizeof(AxinGridImageHeader));
		cVersion[0] = 1;
		cVersion[1] = 0;
	}

};

// ��ɫ��ͷ
// 32 �ֽ�
struct ColorTablePacketHeader
{
	WORD	wPacketCode;			// 01		������ = 0x80
	DWORD	dwPacketLength;			// 02-03	������
	WORD	wType;					// 04		����
	WORD	wColorEntries;			// 05		��ɫ�����
	WORD	wSpecificColorNumbers;	// 06		�ض���ɫ����
	WORD	wHasComment;			// 07		�Ƿ��ж�Ӧע��
	WORD	wHasTransparentColor;	// 08		ʹ��͸��ɫ

	union							// 09-10	͸����ɫ
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

	union							// 11-12	Ĭ����ɫ
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

	DWORD	dwSpecialDataMin;		// 13-14	�ض���С����
	DWORD	dwSpecialDataMax;		// 15-16	�ض��������

	// ��ɫ���� ...
	// ע������ ...

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

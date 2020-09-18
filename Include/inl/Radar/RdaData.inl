#ifndef __INCLUDED_WSR98D_RDA_DATA_INL__
#define __INCLUDED_WSR98D_RDA_DATA_INL__

namespace Wsr98d { namespace Rda
{
//--------------------------------------------------------
}}// namespace Wsr98d::Rda - end
//--------------------------------------------------------
using namespace Wsr98d::Rda;

#pragma pack(1)

//#define CB_RADAR

#ifdef CB_RADAR
	//CB�״�������
	#define RDA_DATA_RGATES			800		//�����ʾ������
	#define RDA_DATA_VGATES			1600	//�ٶȾ������
	#define RDA_DATA_WGATES			1600	//�׿�������
#else
	//SA�״�������
	#define RDA_DATA_RGATES			460		//�����ʾ������
	#define RDA_DATA_VGATES			920		//�ٶȾ������
	#define RDA_DATA_WGATES			920		//�׿�������
#endif

#define RDA_DATA_MAX_CUTS			20		//������ǲ���
#define RDA_DATA_MAX_RADIALS		360		//ÿ�������ϵķ�λ��,ÿ�ȱ���һ������

#define RDA_DATA_CODE_INVALID		0		//����ֵ�е�������,��ʾ��Ч�۲�����
#define RDA_DATA_CODE_RANFOLD		1		//����ֵ�е�������,��ʾ�о���ģ��

#define RDA_DATA_VALUE_INVALID		-999.	//ʵ��ֵ�е�������,��ʾ��Ч�۲�����
#define RDA_DATA_VALUE_RANFOLD		+999.	//ʵ��ֵ�е�������,��ʾ�о���ģ��

#define RDA_DATA_RES_POINT_FIVE		2		//�ٶȾ�������,��ʾ����Ϊ0.5m/s
#define RDA_DATA_RES_ONE_POINT		4		//�ٶȾ�������,��ʾ����Ϊ1.0m/s

//����λ�ò���
#define RDA_DATA_ELV_BEG			0		//���ǿ�ʼ״̬��־(�����ǵĵ�һ����������)
#define RDA_DATA_ELV_MID			1		//�����м����ݱ��(�������м�ľ�������)
#define RDA_DATA_ELV_END			2		//���ǽ���״̬��־(�����ǵ����һ����������)
#define RDA_DATA_VOL_BEG			3		//��ɨ��ʼ״̬��־(��ɨ��ʼ�ĵ�һ����������)
#define RDA_DATA_VOL_END			4		//��ɨ����״̬��־(��ɨ���������һ����������)

///========================================================
///�ṹ���� - beg
///===============

//1. CTM��Ϣ - Channel Terminal Manager
//��0��11�ֽڣ����ڴ�RDA��RPG�������������������У�飬
//������ɨ��������˵�����Ǻ���Ҫ����ˣ���ȡʱ�������������迼��
typedef struct ChannelTerminalManager
{
	sfx_uchar	cTemp[11];	//0~11 �ֽ�

}CTM;

//2. ��¼��Ϣͷ�ṹ - Message Header
//��12��27�ֽڣ�����ȷ�����ں���������ǻ������ݻ����������13��֮һ
//��¼��Ϣ�ṹ��MessageType˵���˼�¼�������ͣ��������£�
//	 1=DIGITAL RADAR DATA, This message may contain a combination of either reflectivity, liased velocity, or spectrum width.
//	 2=RDA STATUS DATA.
//	 3=PERFORMANCE/MAINTENANCE DATA.
//	 4=CONSOLE MESSAGE �C RDA TO RPG.
//	 5=MAINTENANCE LOG DATA.
//	 6=RDA CONTROL COMMANDS.
//	 7=VOLUME COVERAGE PATTERN.
//	 8=CLUTTER CENSOR ZONES.
//	 9=REQUEST FOR DATA.
//	10=CONSOLE MESSAGE �C RPG TO RDA.
//	11=LOOP BACK TEST �C RDA TO RPG.
//	12=LOOP BACK TEST �C RPG TO RDA.
//	13=CLUTTER FILTER BYPASS MAP �C RDA to RPG.
//	14=EDITED CLUTTER FILTER BYPASS MAP �C RGP to RDA.
typedef struct RdaMessageHead
{
	sfx_uint16	iMessageSize;				//12~13 ��ʾ��¼���ȣ�˫�ֽڵ�λ��ʾ
	sfx_uchar	cChannelID;					//14    �źŴ���ͨ����(=0,1,2)
    sfx_uchar	cMessageType;				//15    ��¼��������
											//			1=�״�����
    sfx_int16	iSequenceID;				//16~17 ��¼˳��ţ�0~0x7FFF֮��ѭ����ʾ
	sfx_uint16	iMessageGenerationDate;		//18~19 ��¼��������,��1/1/1970������ Julian���ڱ�ʾ
	sfx_uint32	lMessageGenerationTime;		//20~23 ��¼������GMTʱ��(����)
    sfx_int16	iMessageSegmentsNumber;		//24~25 ��¼����ֶζ���
    sfx_int16	iMessageSegmentNumber;		//26~27 ��¼����κ�
}RMH;

//3. �״�����ͷ��Digital Radar Data Header���ṹ
//��28��127�ֽڣ����������������ݵ����ڡ�ʱ�䡢��λ�����Ǻ��������͵���Ϣ
typedef struct RdaDataHead
{
	sfx_uint32	lRadialCollectionTime;		//28~31	�������ݲɼ���GMTʱ��(����)
	sfx_uint16	iRadialCollectionDate;		//32~33	�������ϲɼ�����,��1/1/1970����������(Julian)���ڱ�ʾ��ʾ

	sfx_uint16	iUnambiguousRange;			//34~35	��ģ������(��λ: 0.1 km)

	sfx_uint16	iAzimuthAngle;				//36~37	��λ�Ƕ� (ʵ��ֵ=(��ֵ>>3) * 180.0 / 4096.0 ��)

	sfx_uint16	iRadialNumber;				//38~39	��ǰ�����ھ������߱��
	sfx_uint16	iRadialStatus;				//40~41	����λ�ò���
												//		0: �����ǵĵ�һ����������(���ǿ�ʼ״̬��־)
												//		1: �������м�ľ�������
												//		2: �����ǵ����һ����������(���ǽ���״̬��־)
												//		3: ��ɨ��ʼ�ĵ�һ����������(��ɨ��ʼ״̬��־)
												//		4: ��ɨ���������һ����������(��ɨ����״̬��־)

	sfx_uint16	iElevationAngle;			//42~43	���� (ʵ��ֵ=(��ֵ>>3) * 180.0 / 4096.0 ��)
	sfx_uint16	iElevationNumber;			//44~45	��ɨ�ڵ����Ǳ��

	sfx_int16	iFirstGateRangeOfRef;		//46~47	���������ݵĵ�һ�������ľ���(m)
	sfx_int16	iFirstGateRangeOfDoppler;	//48~49	����������(�ٶ�/�׿�)�ĵ�һ�������ľ���(m)

	sfx_uint16	iGateSizeOfRef;				//50~51	���������ݵľ���ⳤ(m)
	sfx_uint16	iGateSizeOfDoppler;			//52~53	����������(�ٶ�/�׿�)�ľ���ⳤ(m)

	sfx_uint16	iGateNumbersOfRef;			//54~55	���������ݵľ������
	sfx_uint16	iGateNumbersOfDoppler;		//56~57	����������(�ٶ�/�׿�)�ľ������

	sfx_uint16	iSectorNumber;				//58~59	cut�ڵ��������
	sfx_uint32	lCalibrationConst;			//60~63	ϵͳ����궨���� (dB)

	sfx_uint16	iOffsetOfRef;				//64~65	ǿ������ƫ�Ƶ�ַ (ƫ��cEchoData[0]���ֽ���)
	sfx_uint16	iOffsetOfVel;				//66~67	�ٶ�����ƫ�Ƶ�ַ (ƫ��cEchoData[0]���ֽ���)
	sfx_uint16	iOffsetOfSpw;				//68~69	�׿�����ƫ�Ƶ�ַ (ƫ��cEchoData[0]���ֽ���)

	sfx_uint16	iVelResolution;				//70~71	�������ٶȷֱ���(2:��ʾ0.5m/s, 4:��ʾ1.0m/s)

	sfx_uint16	iVCP;						//72~73	��ɨ(VCP)ģʽ
												//		11: ��ˮģʽ,16������
												//		21: ��ˮģʽ,14������
												//		31: ���ģʽ, 8������
												//		32: ���ģʽ, 7������

	sfx_uint16	iUnused[4];					//74~81	����

	sfx_uint16	iRefPlaybackPointer;		//82~83	���ڻطŵ�ǿ������ָ��,ͬ64
	sfx_uint16	iVelPlaybackPointer;		//84~85	���ڻطŵ��ٶ�����ָ��,ͬ66
	sfx_uint16	iSpwPlaybackPointer;		//86~87	���ڻطŵ��׿�����ָ��,ͬ68

	sfx_uint16	iNyquistVel;				//88~89	���˹���ٶ�(0.01 m/s)

	sfx_int16	iAAF;						//90~91 Atmospheric Attenuation Factor(0.001 dB/Km)
	sfx_int16	iTOVER;						//92~93 û�о����۵������ز�����֮����С��ֵ(0.1��)

	sfx_uint16	iTemp4[1];					//94~95	����

	sfx_uint16	iCircleTotal;				//96~97	??

	sfx_uint16	iTemp5[15];					//98~127 ����
}RDADH;

//4. �������ݣ�Radar Base Data��
//��128��2427�ֽڣ����������ʡ��ٶȺ��ٶ��׿����ֻ������ݡ����ݰ��ֽڱ�����,�������ݲɼ��ķ�����ȷ�����μ�¼�����������Ƿ���ڡ�
//��128~587�ֽ�Ϊ���������ݣ�����0~460����ʵ��dBZ=(Value-2)/2.0-32.0��
//��588~1507�ֽ�Ϊ�ٶ����ݣ�����0~920������ģ����Χ230Km�� �ٶȷֱ���Ϊ0.5m/sʱ��ʵ��ֵV=(Value-2)/2-63.5��Ϊ1.0m/sʱ��ʵ��ֵV=(Value-2)-127.0��
//��1508~2427�ֽ�Ϊ�ٶ��׿����ݣ�����0~920������ģ����Χ230Km��ʵ��ֵSW=(Value-2)/2-63.5��
//ע�⣺���л��������У�����0��ʾ�ز��źŵ�������ȵ����ޣ�����1��Ϊ������ģ���ġ�
typedef struct RdaEchoData
{
	sfx_uchar	cEchoDataRef[RDA_DATA_RGATES];
	sfx_uchar	cEchoDataVel[RDA_DATA_VGATES];
	sfx_uchar	cEchoDataSpw[RDA_DATA_WGATES];
}RBD;

//5. ֡�̱�����Ϣ (Frame Check Sequence)
//��2427~2431�ֽڣ�˵����¼���������������Ƿ���ڣ�������ĳ�����ݵĿ����Ƿ�����ʧ�������
//������Щ���ʱ����Ȼ��2432�ֽڴ��䣬�����4���ֽ�˵��
typedef struct FrameCheckSequence
{
	sfx_uchar		cUnused[4];
}FCS;

//�״�����ݰ��ṹ
typedef struct RdaRadialDataPacket
{
	//{������ͷ,��128(0-127)���ֽ�
	CTM					oCtm;				// CTM��Ϣ(Channel Terminal Manager)
	RdaMessageHead		oRdaMessageHead;	// ��Ϣͷ
	RdaDataHead			oRdaDataHead;		// ����ͷ
	RdaEchoData			oRdaEchoData;		// �ز�����
	FrameCheckSequence	oFcs;				// ֡�̱�����Ϣ
	//}�����ǻ�����ͷ,��128(0-127)���ֽ�

	//SA,SB �ز�����
	//�� CINRAD_RGATES+CINRAD_VGATES+CINRAD_WGATES = 460+920+920 = 2300 �ֽ�
	sfx_uchar			cEchoData[RDA_DATA_RGATES + RDA_DATA_VGATES + RDA_DATA_WGATES];

	//�����ֽ�
	sfx_char			cReserved[4];		//4���ֽ� ����
	//-----------------------------------------------------------
	//SA,SB �����ݹ� 128+2300+4 = 2432 �ֽ�
	//-----------------------------------------------------------
}RDDP;



//�Զ����״�ز��������

///===============
///�ṹ���� - end
///========================================================

#pragma pack()


#endif	//#ifndef __INCLUDED_WSR98D_RDA_DATA_INL__

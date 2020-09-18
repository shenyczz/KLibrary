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
	//CB雷达距离库数
	#define RDA_DATA_RGATES			800		//反射率距离库数
	#define RDA_DATA_VGATES			1600	//速度距离库数
	#define RDA_DATA_WGATES			1600	//谱宽距离库数
#else
	//SA雷达距离库数
	#define RDA_DATA_RGATES			460		//反射率距离库数
	#define RDA_DATA_VGATES			920		//速度距离库数
	#define RDA_DATA_WGATES			920		//谱宽距离库数
#endif

#define RDA_DATA_MAX_CUTS			20		//最大仰角层数
#define RDA_DATA_MAX_RADIALS		360		//每层仰角上的方位数,每度保留一个径向

#define RDA_DATA_CODE_INVALID		0		//编码值中的特殊标记,表示无效观测数据
#define RDA_DATA_CODE_RANFOLD		1		//编码值中的特殊标记,表示有距离模糊

#define RDA_DATA_VALUE_INVALID		-999.	//实际值中的特殊标记,表示无效观测数据
#define RDA_DATA_VALUE_RANFOLD		+999.	//实际值中的特殊标记,表示有距离模糊

#define RDA_DATA_RES_POINT_FIVE		2		//速度精度类型,表示精度为0.5m/s
#define RDA_DATA_RES_ONE_POINT		4		//速度精度类型,表示精度为1.0m/s

//径向位置参数
#define RDA_DATA_ELV_BEG			0		//仰角开始状态标志(该仰角的第一条径向数据)
#define RDA_DATA_ELV_MID			1		//仰角中间数据标记(该仰角中间的径向数据)
#define RDA_DATA_ELV_END			2		//仰角结束状态标志(该仰角的最后一条径向数据)
#define RDA_DATA_VOL_BEG			3		//体扫开始状态标志(体扫开始的第一条径向数据)
#define RDA_DATA_VOL_END			4		//体扫结束状态标志(体扫结束的最后一条径向数据)

///========================================================
///结构定义 - beg
///===============

//1. CTM信息 - Channel Terminal Manager
//第0～11字节，用于从RDA到RPG传输过程中数据完整性校验，
//对于体扫基数据来说并不是很重要，因此，读取时可以跳过，不予考虑
typedef struct ChannelTerminalManager
{
	sfx_uchar	cTemp[11];	//0~11 字节

}CTM;

//2. 记录信息头结构 - Message Header
//第12～27字节，用于确定跟在后面的数据是基本数据或者是另外的13类之一
//记录信息结构中MessageType说明了记录数据类型，定义如下：
//	 1=DIGITAL RADAR DATA, This message may contain a combination of either reflectivity, liased velocity, or spectrum width.
//	 2=RDA STATUS DATA.
//	 3=PERFORMANCE/MAINTENANCE DATA.
//	 4=CONSOLE MESSAGE – RDA TO RPG.
//	 5=MAINTENANCE LOG DATA.
//	 6=RDA CONTROL COMMANDS.
//	 7=VOLUME COVERAGE PATTERN.
//	 8=CLUTTER CENSOR ZONES.
//	 9=REQUEST FOR DATA.
//	10=CONSOLE MESSAGE – RPG TO RDA.
//	11=LOOP BACK TEST – RDA TO RPG.
//	12=LOOP BACK TEST – RPG TO RDA.
//	13=CLUTTER FILTER BYPASS MAP – RDA to RPG.
//	14=EDITED CLUTTER FILTER BYPASS MAP – RGP to RDA.
typedef struct RdaMessageHead
{
	sfx_uint16	iMessageSize;				//12~13 表示记录长度，双字节单位表示
	sfx_uchar	cChannelID;					//14    信号传输通道号(=0,1,2)
    sfx_uchar	cMessageType;				//15    记录数据类型
											//			1=雷达数据
    sfx_int16	iSequenceID;				//16~17 记录顺序号，0~0x7FFF之间循环表示
	sfx_uint16	iMessageGenerationDate;		//18~19 记录产生日期,以1/1/1970以来的 Julian日期表示
	sfx_uint32	lMessageGenerationTime;		//20~23 记录产生的GMT时间(毫秒)
    sfx_int16	iMessageSegmentsNumber;		//24~25 记录传输分段段数
    sfx_int16	iMessageSegmentNumber;		//26~27 记录传输段号
}RMH;

//3. 雷达数据头（Digital Radar Data Header）结构
//第28～127字节，用于描述径向数据的日期、时间、方位、仰角和数据类型等信息
typedef struct RdaDataHead
{
	sfx_uint32	lRadialCollectionTime;		//28~31	径向数据采集的GMT时间(毫秒)
	sfx_uint16	iRadialCollectionDate;		//32~33	径向资料采集日期,以1/1/1970以来的儒略(Julian)日期表示表示

	sfx_uint16	iUnambiguousRange;			//34~35	不模糊距离(单位: 0.1 km)

	sfx_uint16	iAzimuthAngle;				//36~37	方位角度 (实际值=(数值>>3) * 180.0 / 4096.0 度)

	sfx_uint16	iRadialNumber;				//38~39	当前仰角内径向射线编号
	sfx_uint16	iRadialStatus;				//40~41	径向位置参数
												//		0: 该仰角的第一条径向数据(仰角开始状态标志)
												//		1: 该仰角中间的径向数据
												//		2: 该仰角的最后一条径向数据(仰角结束状态标志)
												//		3: 体扫开始的第一条径向数据(体扫开始状态标志)
												//		4: 体扫结束的最后一条径向数据(体扫结束状态标志)

	sfx_uint16	iElevationAngle;			//42~43	仰角 (实际值=(数值>>3) * 180.0 / 4096.0 度)
	sfx_uint16	iElevationNumber;			//44~45	体扫内的仰角编号

	sfx_int16	iFirstGateRangeOfRef;		//46~47	反射率数据的第一个距离库的距离(m)
	sfx_int16	iFirstGateRangeOfDoppler;	//48~49	多普勒数据(速度/谱宽)的第一个距离库的距离(m)

	sfx_uint16	iGateSizeOfRef;				//50~51	反射率数据的距离库长(m)
	sfx_uint16	iGateSizeOfDoppler;			//52~53	多普勒数据(速度/谱宽)的距离库长(m)

	sfx_uint16	iGateNumbersOfRef;			//54~55	反射率数据的距离库数
	sfx_uint16	iGateNumbersOfDoppler;		//56~57	多普勒数据(速度/谱宽)的距离库数

	sfx_uint16	iSectorNumber;				//58~59	cut内的扇区编号
	sfx_uint32	lCalibrationConst;			//60~63	系统增益标定常数 (dB)

	sfx_uint16	iOffsetOfRef;				//64~65	强度数据偏移地址 (偏离cEchoData[0]的字节数)
	sfx_uint16	iOffsetOfVel;				//66~67	速度数据偏移地址 (偏离cEchoData[0]的字节数)
	sfx_uint16	iOffsetOfSpw;				//68~69	谱宽数据偏移地址 (偏离cEchoData[0]的字节数)

	sfx_uint16	iVelResolution;				//70~71	多普勒速度分辨率(2:表示0.5m/s, 4:表示1.0m/s)

	sfx_uint16	iVCP;						//72~73	体扫(VCP)模式
												//		11: 降水模式,16层仰角
												//		21: 降水模式,14层仰角
												//		31: 晴空模式, 8层仰角
												//		32: 晴空模式, 7层仰角

	sfx_uint16	iUnused[4];					//74~81	保留

	sfx_uint16	iRefPlaybackPointer;		//82~83	用于回放的强度数据指针,同64
	sfx_uint16	iVelPlaybackPointer;		//84~85	用于回放的速度数据指针,同66
	sfx_uint16	iSpwPlaybackPointer;		//86~87	用于回放的谱宽数据指针,同68

	sfx_uint16	iNyquistVel;				//88~89	尼奎斯特速度(0.01 m/s)

	sfx_int16	iAAF;						//90~91 Atmospheric Attenuation Factor(0.001 dB/Km)
	sfx_int16	iTOVER;						//92~93 没有距离折叠两个回波功率之间最小差值(0.1瓦)

	sfx_uint16	iTemp4[1];					//94~95	保留

	sfx_uint16	iCircleTotal;				//96~97	??

	sfx_uint16	iTemp5[15];					//98~127 保留
}RDADH;

//4. 基本数据（Radar Base Data）
//第128～2427字节，包括反射率、速度和速度谱宽三种基本数据。数据按字节编码存放,根据数据采集的方法来确定本段记录中三种数据是否存在。
//第128~587字节为反射率数据（库数0~460），实际dBZ=(Value-2)/2.0-32.0。
//第588~1507字节为速度数据（库数0~920），退模糊范围230Km。 速度分辨率为0.5m/s时，实际值V=(Value-2)/2-63.5；为1.0m/s时，实际值V=(Value-2)-127.0。
//第1508~2427字节为速度谱宽数据（库数0~920），退模糊范围230Km，实际值SW=(Value-2)/2-63.5。
//注意：所有基本数据中，编码0表示回波信号低于信噪比的门限，编码1认为数据是模糊的。
typedef struct RdaEchoData
{
	sfx_uchar	cEchoDataRef[RDA_DATA_RGATES];
	sfx_uchar	cEchoDataVel[RDA_DATA_VGATES];
	sfx_uchar	cEchoDataSpw[RDA_DATA_WGATES];
}RBD;

//5. 帧继编码信息 (Frame Check Sequence)
//第2427~2431字节，说明记录中三个基本数据是否存在，或者是某个数据的库数是否有损失等情况。
//出现这些情况时，仍然按2432字节传输，由最后4个字节说明
typedef struct FrameCheckSequence
{
	sfx_uchar		cUnused[4];
}FCS;

//雷达基数据包结构
typedef struct RdaRadialDataPacket
{
	//{基数据头,共128(0-127)个字节
	CTM					oCtm;				// CTM信息(Channel Terminal Manager)
	RdaMessageHead		oRdaMessageHead;	// 信息头
	RdaDataHead			oRdaDataHead;		// 数据头
	RdaEchoData			oRdaEchoData;		// 回波数据
	FrameCheckSequence	oFcs;				// 帧继编码信息
	//}以上是基数据头,共128(0-127)个字节

	//SA,SB 回波数据
	//共 CINRAD_RGATES+CINRAD_VGATES+CINRAD_WGATES = 460+920+920 = 2300 字节
	sfx_uchar			cEchoData[RDA_DATA_RGATES + RDA_DATA_VGATES + RDA_DATA_WGATES];

	//保留字节
	sfx_char			cReserved[4];		//4个字节 保留
	//-----------------------------------------------------------
	//SA,SB 基数据共 128+2300+4 = 2432 字节
	//-----------------------------------------------------------
}RDDP;



//自定义雷达回波格点数据

///===============
///结构定义 - end
///========================================================

#pragma pack()


#endif	//#ifndef __INCLUDED_WSR98D_RDA_DATA_INL__

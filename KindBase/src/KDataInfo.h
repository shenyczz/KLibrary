/******************************************************************************
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KDataInfo - ������Ϣ
 **  Version: 
 ** Function: 
 **  Explain: 
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

#include "KExtents.h"
#include "KString.h"

class KIND_EXT_CLASS KDataInfo
{
public:
	KDataInfo(void);
	virtual ~KDataInfo(void);

public:
	// �ļ���ʶ
	WORD& FileID()				{ return this->wFileID; }
	// ��ʽ����
	WORD& FormatCode()			{ return this->wFormatCode; }
	// �ļ��洢����
	WORD& FileStorageType()		{ return this->wFileStorageType; }

	// ������Դ
	WORD& DataSource()			{ return this->wDataSource; }
	// ��������
	WORD& DataCategory()		{ return this->wDataCategory; }
	// �ֽ���
	WORD& BytesOrder()			{ return this->wBytesOrder; }
	// ��������
	WORD& DataType()			{ return this->wDataType; }
	// ��Ʒ����
	WORD& ProductCode()			{ return this->wProductCode; }
	// Ҫ�ش��� (��ʶ���ݵ���������: ���������ȡ�ʱ�䡢�¶ȡ�ѹ����ʪ�ȡ��ٶȵ�)
	WORD& ElementCode()			{ return this->wElementCode; }
	// ��־
	DWORD& Flag()				{ return this->dwFlag; }

	// ʱ��
	WORD&  Year()				{ return this->wYear; }
	WORD&  Month()				{ return this->wMonth; }
	WORD&  Day()				{ return this->wDay; }
	WORD&  Hour()				{ return this->wHour; }
	WORD&  Minute()				{ return this->wMinute; }
	WORD&  Second()				{ return this->wSecond; }
	DWORD& Millisecond()		{ return this->dwMillisecond; }

	WORD&  Year2()				{ return this->wYear2; }
	WORD&  Month2()				{ return this->wMonth2; }
	WORD&  Day2()				{ return this->wDay2; }
	WORD&  Hour2()				{ return this->wHour2; }
	WORD&  Minute2()			{ return this->wMinute2; }
	WORD&  Second2()			{ return this->wSecond2; }
	DWORD& Millisecond2()		{ return this->dwMillisecond2; }

	DATETIME& DateTime()		{ return this->datmBeg; }
	DATETIME& BeginDateTime()	{ return this->datmBeg; }
	DATETIME& EndDateTime()		{ return this->datmEnd; }
	DATETIME& DateTimeBegin()	{ return this->datmBeg; }
	DATETIME& DateTimeEnd()		{ return this->datmEnd; }

	// ʱЧ
	WORD& TimePeriod()			{ return this->wTimePeriod; }

	// ���
	WORD& Layer()				{ return this->wLayer; }

	// ����
	WORD& LayerNumbers()		{ return this->wLayerNums; }
	// �����
	WORD& LayerCode(int index)	{ return this->wLayerCode[index]; }

	// ͨ����
	WORD& ChannelNumbers()			{ return this->wChannelNums; }
	// ͨ������
	short& ChannelCode(int index)	{ return this->iChannelCode[index]; }

	// ����ͨ����
	WORD& AncillaryChannelNums()			{ return wAncillaryChannelNums; }
	// ����ͨ������
	short& AncillaryChannelCode(int index)	{ return this->iAncillaryChannelCode[index]; }

	// ��ǰ������
	int& CurrentLayerIndex()	{ return this->iCurrentLayerIndex; }
	// ��ǰͨ������
	int& CurrentChannelIndex()	{ return this->iCurrentChannelIndex; }

	// ͶӰ����
	WORD& ProjectType()			{ return this->wProjectType; }
	// ��ɫ������
	WORD& PaletteType()			{ return this->wPaletteType; }

	// �ܵ���ɢ������(��վ���� ������)
	WORD& TotalNum()			{ return this->wTotalNum; }
	// ��վ��ͼҪ�ظ���(���8��)
	WORD& ElementNumbers()		{ return this->wElementNum; }

	// �ض�������Сֵ
	DWORD& SpecificDataMin()	{ return this->dwSpecificDataMin; }
	// �ض��������ֵ
	DWORD& SpecificDataMax()	{ return this->dwSpecificDataMax; }
	// �ض���������
	WORD& SpecificColorNumbers(){ return this->wSpecificDataNumbers; }

	// ��Ч����
	LONG& InvalidData()			{ return (LONG&)this->dwInvalidData; }
	// ͸��ɫ
	DWORD& TransparentColor()	{ return this->dwTransparentColor; }

	// SDB ƫ�Ƶ�ַ
	DWORD& SdbOffset()			{ return this->dwSdbOffset; }
	// LDB ƫ�Ƶ�ַ
	DWORD& LdbOffset()			{ return this->dwLdbOffset; }
	// TDB ƫ�Ƶ�ַ
	DWORD& TdbOffset()			{ return this->dwTdbOffset; }

	// �Ƿ���ע��
	BOOL& HasComment()			{ return this->bHasComment; }
	// �Ƿ�����ɫ��
	BOOL& HasColorTable()		{ return this->bHasColorTable; }
	// �Ƿ���͸��ɫ
	BOOL& HasTransparentColor()	{ return this->bHasTransparentColor; }

	// ��׼ֵ
	double& BaseValue()			{ return this->dBaseValue; }
	// ����ϵ��
	double& Scale()				{ return this->dScale; }

	// �Ӵֵĵ�ֵ��ֵ
	double& BoldContourValue()	{ return this->dBoldContourValue; }
	// ƽ��ϵ��
	double& SmoothCoefficient()	{ return this->dSmoothCoefficient; }
	//-----------------------------------------------------
	// ���
	int& Width()				{ return this->xNum; }
	// �߶�
	int& Height()				{ return this->yNum; }
	// ��С����
	double& MinLon()			{ return this->xMin; }
	// ��Сγ��
	double& MinLat()			{ return this->yMin; }
	// ��󾭶�
	double& MaxLon()			{ return this->xMax; }
	// ���γ��
	double& MaxLat()			{ return this->yMax; }
	// ���ȼ��
	double& LonInterval()		{ return this->xInterval; }
	double& IntervalLon()		{ return this->xInterval; }
	// γ�ȼ��
	double& LatInterval()		{ return this->yInterval; }
	double& IntervalLat()		{ return this->yInterval; }
	// ��׼����
	double& StandardLon()		{ return this->dStandardLon; }
	// ��׼γ��1
	double& StandardLat1()		{ return this->dStandardLat1; }
	// ��׼γ��2
	double& StandardLat2()		{ return this->dStandardLat2; }
	// ���ľ���
	double& CenterLon()			{ return this->dCenterLon; }
	// ����γ��
	double& CenterLat()			{ return this->dCenterLat; }
	//-----------------------------------------------------
	// X������
	int& XNum()					{ return this->xNum; }
	// ��СX 
	double& XMin()				{ return this->xMin; }
	// ���X
	double& XMax()				{ return this->xMax; }
	// X���
	double& XInterval()			{ return this->xInterval; }

	// Y������
	int& YNum()					{ return this->yNum; }
	// ��СY
	double& YMin()				{ return this->yMin; }
	// ���Y
	double& YMax()				{ return this->yMax; }
	// Y���
	double& YInterval()			{ return this->yInterval; }

	// X������
	int& NumX()					{ return this->xNum; }
	// ��СX 
	double& MinX()				{ return this->xMin; }
	// ���X
	double& MaxX()				{ return this->xMax; }
	// X���
	double& IntervalX()			{ return this->xInterval; }

	// Y������
	int& NumY()					{ return this->yNum; }
	// ��СY
	double& MinY()				{ return this->yMin; }
	// ���Y
	double& MaxY()				{ return this->yMax; }
	// Y���
	double& IntervalY()			{ return this->yInterval; }

	// Z������
	int& ZNum()					{ return this->zNum; }
	// Z������Сֵ
	double& ZMin()				{ return this->zMin; }
	// Z�������ֵ
	double& ZMax()				{ return this->zMax; }
	// Z������ֵ
	double& ZInterval()			{ return this->zInterval; }

	// Z������
	int& NumZ()					{ return this->zNum; }
	// Z������Сֵ
	double& MinZ()				{ return this->zMin; }
	// Z�������ֵ
	double& MaxZ()				{ return this->zMax; }
	// Z������ֵ
	double& IntervalZ()			{ return this->zInterval; }

	// U���������
	int& UNum()					{ return this->uNum; }
	// U������Сֵ
	double& UMin()				{ return this->uMin; }
	// U�������ֵ
	double& UMax()				{ return this->uMax; }
	// U����ƽ��ֵ
	double& UAvg()				{ return this->uAvg; }
	// U�������ֵ
	double& UInterval()			{ return this->uInterval; }
	// U������Сֵ
	double& UExtremumMin()		{ return this->uExtremumMin; }
	// U��������ֵ
	double& UExtremumMax()		{ return this->uExtremumMax; }

	// V���������
	int& VNum()					{ return this->vNum; }
	// V������Сֵ
	double& VMin()				{ return this->vMin; }
	// V�������ֵ
	double& VMax()				{ return this->vMax; }
	// V����ƽ��ֵ
	double& VAvg()				{ return this->vAvg; }
	// V�������ֵ
	double& VInterval()			{ return this->vInterval; }
	// V������Сֵ
	double& VExtremumMin()		{ return this->vExtremumMin; }
	// V��������ֵ
	double& VExtremumMax()		{ return this->vExtremumMax; }

	// w���������
	int& WNum()					{ return this->wNum; }
	// w������Сֵ
	double& WMin()				{ return this->wMin; }
	// w�������ֵ
	double& WMax()				{ return this->wMax; }
	// w����ƽ��ֵ
	double& WAvg()				{ return this->wAvg; }
	// w�������ֵ
	double& WInterval()			{ return this->wInterval; }
	// w������Сֵ
	double& WExtremumMin()		{ return this->wExtremumMin; }
	// w��������ֵ
	double& WExtremumMax()		{ return this->wExtremumMax; }
	//-----------------------------------------------------
	// ƽ��ֵ
	double& AverageValue()		{ return this->vAvg; }
	// ��Сֵ
	double& ExtremumValueMin()	{ return this->vExtremumMin; }
	// ����ֵ
	double& ExtremumValueMax()	{ return this->vExtremumMax; }
	//-----------------------------------------------------
	// ��ֵ��������
	WORD& ContourNumbers()		{ return this->wContourNums; }
	// ��ֵ��ֵ
	double* ContourValue()		{ return &this->dContourValue[0]; }
	double& ContourValue(int index)		{ return this->dContourValue[index]; }
	// Ҫ��׷�ٵĵ�ֵ����Сֵ
	double& ContourMin()		{ return this->dContourMin; }
	// Ҫ��׷�ٵĵ�ֵ�����ֵ
	double& ContourMax()		{ return this->dContourMax; }
	// Ҫ��׷�ٵĵ�ֵ�߼��ֵ
	double& ContourInterval()	{ return this->dContourInterval; }
	// ��ֵ�߷���
	WORD& ContourPortion()		{ return this->wContourPortion; }

	// ������
	WORD& ClipArea()		{ return this->wClipArea; }
	// ��������Ե���ϵ�X����(���100����)
	double* XClip()				{ return &this->xClip[0]; }
	double& XClip(int index)	{ return this->xClip[index]; }
	// ��������Ե���ϵ�Y����(���100����)
	double* YClip()				{ return &this->yClip[0]; }
	double& YClip(int index)	{ return this->yClip[index]; }

	// ��������Χ
	double& XClipMin()			{ return this->xClip[0]; }
	double& YClipMin()			{ return this->yClip[0]; }
	double& XClipMax()			{ return this->xClip[1]; }
	double& YClipMax()			{ return this->yClip[1]; }

	// ע��
	void SetComment(LPCTSTR lpszComment)
	{ strComment = lpszComment; }
	KString GetComment()		{ return strComment; }
	KString& Comment()			{ return strComment; }

	// �Ƿ���Ч����
	BOOL IsInvalidData(double value);
	//-----------------------------------------------------

public:
	TCHAR		szFileID[16];			// �ı������ļ���ʶ
	WORD		wFileID;				// �ļ���ʶ
	WORD		wDataSource;			// ������Դ
	union
	{
		WORD		wFormatCode;		// ��ʽ����
		WORD		wDataCategory;		// ��������
	};
	WORD		wBytesOrder;			// �ֽ���
	WORD		wDataType;				// ��������
	WORD		wElementCode;			// Ҫ�ش��� (��ʶ���ݵ���������: ���������ȡ�ʱ�䡢�¶ȡ�ѹ����ʪ�ȡ��ٶȵ�)
	WORD		wProductCode;			// ��Ʒ����
	WORD		wFileStorageType;		// �ļ��洢����

	// ʱ��
	union
	{
		DATETIME	datmBeg;			// ��ʼʱ��
		struct
		{
			WORD		wYear;			// ��
			WORD		wMonth;			// ��
			WORD		wDay;			// ��
			WORD		wHour;			// ʱ
			WORD		wMinute;		// ��
			WORD		wSecond;		// ��
			DWORD		dwMillisecond;	// ����
		};
	};
	union
	{
		DATETIME	datmEnd;			// ����ʱ��
		struct
		{
			WORD		wYear2;			// ��
			WORD		wMonth2;		// ��
			WORD		wDay2;			// ��
			WORD		wHour2;			// ʱ
			WORD		wMinute2;		// ��
			WORD		wSecond2;		// ��
			DWORD		dwMillisecond2;	// ����
		};
	};

	WORD		wTimePeriod ;			// ʱЧ

	union
	{
		WORD	wLayer;					// ���
		WORD	wLayerNums;				// ����
		WORD	wChannelNums;			// ͨ����
	};
	union
	{
		WORD	wLayerCode[1000];		// �����
		short	iChannelCode[1000];		// ͨ������
	};
	union
	{
		int		iCurrentLayerIndex;		// ��ǰ������
		int		iCurrentChannelIndex;	// ��ǰͨ������
	};

	WORD		wAncillaryChannelNums;			// ����ͨ������
	short		iAncillaryChannelCode[100];		// ����ͨ������

	WORD		wProjectType;			// ͶӰ����
	WORD		wPaletteType;			// ��ɫ������

	WORD		wTotalNum;				// �ܵ���ɢ������(��վ���� ������),Ҫ�����0
	WORD		wElementNum;			// ��վ��ͼҪ�ظ���

	WORD		wSpecificDataNumbers;	// �ض���������
	DWORD		dwSpecificDataMin;		// �ض�������Сֵ
	DWORD		dwSpecificDataMax;		// �ض��������ֵ

	DWORD		dwInvalidData;			// ��Ч����
	DWORD		dwTransparentColor;		// ͸��ɫ

	DWORD		dwSdbOffset;			// SDB ƫ�Ƶ�ַ
	DWORD		dwLdbOffset;			// LDB ƫ�Ƶ�ַ
	DWORD		dwTdbOffset;			// TDB ƫ�Ƶ�ַ

	BOOL		bHasComment;			// �Ƿ���ע��
	BOOL		bHasColorTable;			// �Ƿ�����ɫ��
	BOOL		bHasTransparentColor;	// �Ƿ���͸��ɫ

	double		dBaseValue;				// ��׼ֵ
	double		dScale;					// ����ϵ��

	double		dBoldContourValue;		// �Ӵֵĵ�ֵ��ֵ
	double		dSmoothCoefficient;		// ƽ��ϵ��

	// x ����
	int			xNum;					// X��������(����)
	double		xMin;					// X������Сֵ
	double		xMax;					// X�������ֵ
	double		xInterval;				// X������ֵ

	// y ����
	int			yNum;					// Y��������(����)
	double		yMin;					// Y������Сֵ
	double		yMax;					// Y�������ֵ
	double		yInterval;				// Y������ֵ

	// z ����
	int			zNum;					// Z��������(����)
	double		zMin;					// Z������Сֵ
	double		zMax;					// Z�������ֵ
	double		zInterval;				// Z������ֵ

	// u ����
	int			uNum;					// U���������
	double		uMin;					// U������Сֵ
	double		uMax;					// U�������ֵ
	double		uAvg;					// U����ƽ��ֵ
	double		uInterval;				// U�������ֵ
	double		uExtremumMin;			// U������Сֵ
	double		uExtremumMax;			// U��������ֵ

	// v ���� / ����ֵ
	int			vNum;					// V���������
	double		vMin;					// V������Сֵ
	double		vMax;					// V�������ֵ
	double		vAvg;					// V����ƽ��ֵ
	double		vInterval;				// V�������ֵ
	double		vExtremumMin;			// V������Сֵ
	double		vExtremumMax;			// V��������ֵ

	// w ���� / ����ֵ
	int			wNum;					// W���������
	double		wMin;					// W������Сֵ
	double		wMax;					// W�������ֵ
	double		wAvg;					// W����ƽ��ֵ
	double		wInterval;				// W�������ֵ
	double		wExtremumMin;			// W������Сֵ
	double		wExtremumMax;			// W��������ֵ

	double		dStandardLon;			// ��׼����(Բ׶ͶӰ���ľ��� LAMBERT,POLAR,ALBERS)

	double		dStandardLat1;			// ��׼γ��1 -- ���С������С��������ͶӰ��Ч
	double		dStandardLat2;			// ��׼γ��2 -- ������ͶӰ��Ч

	double		dCenterLon;				// ���ĵ�ľ���
	double		dCenterLat;				// ���ĵ��γ��

	// ��ֵ��
	WORD		wContourNums;		// ��ֵ��������(һ����ֵ��Ӧһ����ֵ����)
	// 0 - ��ʾ�������û��Ҫ��׷�ٵĵ�ֵ��ֵ,(����������ͼ)
	// 1~99 - ��ʾ��������������Ҫ��׷�ٵĵ�ֵ��ֵ,����Ϊ wContourNums
	// 9999 - ��ʾ����������������ʾ ��ֵ����Сֵ ��ֵ�����ֵ ��ֵ�߼��ֵ
	union
	{
		double dContourValue[100];		// wContourNums >0 �� ������ 9999, 
		// �����洢Ҫ��׷�ٵĵ�ֵ��ֵ
		struct
		{
			double dContourInterval;	// Ҫ��׷�ٵĵ�ֵ�߼��ֵ
			double dContourMin;			// Ҫ��׷�ٵĵ�ֵ����Сֵ
			double dContourMax;			// Ҫ��׷�ٵĵ�ֵ�����ֵ
		};
	};

	// ����ֵ�߱��Ϊ8888ʱ����ֵ�ߵ�����.Ĭ��=10
	WORD wContourPortion;					// 

	// ���������
	WORD wClipArea;
	// 0		- ��ʾû�м�����
	// [3,99]	- �������߽������
	// 9999		- ��������ĸ����ֱ���X��Y����Χ Xmin  Ymin  Xmax  Ymax
	double	xClip[100];					// ��������Ե���ϵ� X ����(���100����)
	double	yClip[100];					// ��������Ե���ϵ� Y ����(���100����)

	//-----------------------------------------------------
	// ��������������Ϣ
	WORD	wOrbitNo;	// �����
	WORD& OrbitNo()	{ return this->wOrbitNo; }

	WORD	wUpOrDown;	// ������
	WORD& UpOrDown()	{ return this->wUpOrDown; }

	WORD	wDayOrNight;	// �����ҹ���
	WORD& DayOrNight()	{ return this->wDayOrNight; }

	double dRadiusOfEarth;	// ����뾶
	double& RadiusOfEarth()	{ return this->dRadiusOfEarth; }

	DWORD dwVersion;		// �汾
	DWORD& Version()		{ return this->dwVersion; }

	//-----------------------------------------------------

	DWORD	dwFlag;						// ��־

	KString strComment;					// ע����Ϣ

	// ����ƫ�Ƶ�ַ
	public:	__declspec(property(get=GetDataOffset, put=SetDataOffset)) DWORD DataOffset;
	public:	DWORD GetDataOffset() { return m_dwDataOffset; }
	public:	VOID SetDataOffset(DWORD offset) { m_dwDataOffset = offset; }
	private:DWORD m_dwDataOffset;


	// ���ݴ洢���� 0 - δ֪ Ascii - �ı� Binary - ������
	public:	__declspec(property(get=GetDataStoreType, put=SetDataStoreType)) int DataStoreType;
	public:	int GetDataStoreType() { return m_iDataStoreType; }
	public:	VOID SetDataStoreType(int type) { m_iDataStoreType = type; }
	private:int m_iDataStoreType;
};

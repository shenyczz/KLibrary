/******************************************************************************
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KDataInfo - 数据信息
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: 申永辰.郑州 (shenyczz@163.com)
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
	// 文件标识
	WORD& FileID()				{ return this->wFileID; }
	// 格式代码
	WORD& FormatCode()			{ return this->wFormatCode; }
	// 文件存储类型
	WORD& FileStorageType()		{ return this->wFileStorageType; }

	// 数据来源
	WORD& DataSource()			{ return this->wDataSource; }
	// 数据种类
	WORD& DataCategory()		{ return this->wDataCategory; }
	// 字节序
	WORD& BytesOrder()			{ return this->wBytesOrder; }
	// 数据类型
	WORD& DataType()			{ return this->wDataType; }
	// 产品代码
	WORD& ProductCode()			{ return this->wProductCode; }
	// 要素代码 (标识数据的物理属性: 重量、长度、时间、温度、压力、湿度、速度等)
	WORD& ElementCode()			{ return this->wElementCode; }
	// 标志
	DWORD& Flag()				{ return this->dwFlag; }

	// 时间
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

	// 时效
	WORD& TimePeriod()			{ return this->wTimePeriod; }

	// 层次
	WORD& Layer()				{ return this->wLayer; }

	// 层数
	WORD& LayerNumbers()		{ return this->wLayerNums; }
	// 层代码
	WORD& LayerCode(int index)	{ return this->wLayerCode[index]; }

	// 通道数
	WORD& ChannelNumbers()			{ return this->wChannelNums; }
	// 通道代码
	short& ChannelCode(int index)	{ return this->iChannelCode[index]; }

	// 辅助通道数
	WORD& AncillaryChannelNums()			{ return wAncillaryChannelNums; }
	// 辅助通道代码
	short& AncillaryChannelCode(int index)	{ return this->iAncillaryChannelCode[index]; }

	// 当前层索引
	int& CurrentLayerIndex()	{ return this->iCurrentLayerIndex; }
	// 当前通道索引
	int& CurrentChannelIndex()	{ return this->iCurrentChannelIndex; }

	// 投影类型
	WORD& ProjectType()			{ return this->wProjectType; }
	// 调色板类型
	WORD& PaletteType()			{ return this->wPaletteType; }

	// 总的离散点数量(总站点数 总项数)
	WORD& TotalNum()			{ return this->wTotalNum; }
	// 单站填图要素个数(最多8个)
	WORD& ElementNumbers()		{ return this->wElementNum; }

	// 特定数据最小值
	DWORD& SpecificDataMin()	{ return this->dwSpecificDataMin; }
	// 特定数据最大值
	DWORD& SpecificDataMax()	{ return this->dwSpecificDataMax; }
	// 特定数据数量
	WORD& SpecificColorNumbers(){ return this->wSpecificDataNumbers; }

	// 无效数据
	LONG& InvalidData()			{ return (LONG&)this->dwInvalidData; }
	// 透明色
	DWORD& TransparentColor()	{ return this->dwTransparentColor; }

	// SDB 偏移地址
	DWORD& SdbOffset()			{ return this->dwSdbOffset; }
	// LDB 偏移地址
	DWORD& LdbOffset()			{ return this->dwLdbOffset; }
	// TDB 偏移地址
	DWORD& TdbOffset()			{ return this->dwTdbOffset; }

	// 是否有注释
	BOOL& HasComment()			{ return this->bHasComment; }
	// 是否有颜色表
	BOOL& HasColorTable()		{ return this->bHasColorTable; }
	// 是否有透明色
	BOOL& HasTransparentColor()	{ return this->bHasTransparentColor; }

	// 基准值
	double& BaseValue()			{ return this->dBaseValue; }
	// 比例系数
	double& Scale()				{ return this->dScale; }

	// 加粗的等值线值
	double& BoldContourValue()	{ return this->dBoldContourValue; }
	// 平滑系数
	double& SmoothCoefficient()	{ return this->dSmoothCoefficient; }
	//-----------------------------------------------------
	// 宽度
	int& Width()				{ return this->xNum; }
	// 高度
	int& Height()				{ return this->yNum; }
	// 最小经度
	double& MinLon()			{ return this->xMin; }
	// 最小纬度
	double& MinLat()			{ return this->yMin; }
	// 最大经度
	double& MaxLon()			{ return this->xMax; }
	// 最大纬度
	double& MaxLat()			{ return this->yMax; }
	// 经度间隔
	double& LonInterval()		{ return this->xInterval; }
	double& IntervalLon()		{ return this->xInterval; }
	// 纬度间隔
	double& LatInterval()		{ return this->yInterval; }
	double& IntervalLat()		{ return this->yInterval; }
	// 标准经度
	double& StandardLon()		{ return this->dStandardLon; }
	// 标准纬度1
	double& StandardLat1()		{ return this->dStandardLat1; }
	// 标准纬度2
	double& StandardLat2()		{ return this->dStandardLat2; }
	// 中心经度
	double& CenterLon()			{ return this->dCenterLon; }
	// 中心纬度
	double& CenterLat()			{ return this->dCenterLat; }
	//-----------------------------------------------------
	// X向格点数
	int& XNum()					{ return this->xNum; }
	// 最小X 
	double& XMin()				{ return this->xMin; }
	// 最大X
	double& XMax()				{ return this->xMax; }
	// X间隔
	double& XInterval()			{ return this->xInterval; }

	// Y向格点数
	int& YNum()					{ return this->yNum; }
	// 最小Y
	double& YMin()				{ return this->yMin; }
	// 最大Y
	double& YMax()				{ return this->yMax; }
	// Y间隔
	double& YInterval()			{ return this->yInterval; }

	// X向格点数
	int& NumX()					{ return this->xNum; }
	// 最小X 
	double& MinX()				{ return this->xMin; }
	// 最大X
	double& MaxX()				{ return this->xMax; }
	// X间隔
	double& IntervalX()			{ return this->xInterval; }

	// Y向格点数
	int& NumY()					{ return this->yNum; }
	// 最小Y
	double& MinY()				{ return this->yMin; }
	// 最大Y
	double& MaxY()				{ return this->yMax; }
	// Y间隔
	double& IntervalY()			{ return this->yInterval; }

	// Z向格点数
	int& ZNum()					{ return this->zNum; }
	// Z坐标最小值
	double& ZMin()				{ return this->zMin; }
	// Z坐标最大值
	double& ZMax()				{ return this->zMax; }
	// Z坐标间隔值
	double& ZInterval()			{ return this->zInterval; }

	// Z向格点数
	int& NumZ()					{ return this->zNum; }
	// Z坐标最小值
	double& MinZ()				{ return this->zMin; }
	// Z坐标最大值
	double& MaxZ()				{ return this->zMax; }
	// Z坐标间隔值
	double& IntervalZ()			{ return this->zInterval; }

	// U分量格点数
	int& UNum()					{ return this->uNum; }
	// U分量最小值
	double& UMin()				{ return this->uMin; }
	// U分量最大值
	double& UMax()				{ return this->uMax; }
	// U分量平均值
	double& UAvg()				{ return this->uAvg; }
	// U分量间隔值
	double& UInterval()			{ return this->uInterval; }
	// U分量极小值
	double& UExtremumMin()		{ return this->uExtremumMin; }
	// U分量极大值
	double& UExtremumMax()		{ return this->uExtremumMax; }

	// V分量格点数
	int& VNum()					{ return this->vNum; }
	// V分量最小值
	double& VMin()				{ return this->vMin; }
	// V分量最大值
	double& VMax()				{ return this->vMax; }
	// V分量平均值
	double& VAvg()				{ return this->vAvg; }
	// V分量间隔值
	double& VInterval()			{ return this->vInterval; }
	// V分量极小值
	double& VExtremumMin()		{ return this->vExtremumMin; }
	// V分量极大值
	double& VExtremumMax()		{ return this->vExtremumMax; }

	// w分量格点数
	int& WNum()					{ return this->wNum; }
	// w分量最小值
	double& WMin()				{ return this->wMin; }
	// w分量最大值
	double& WMax()				{ return this->wMax; }
	// w分量平均值
	double& WAvg()				{ return this->wAvg; }
	// w分量间隔值
	double& WInterval()			{ return this->wInterval; }
	// w分量极小值
	double& WExtremumMin()		{ return this->wExtremumMin; }
	// w分量极大值
	double& WExtremumMax()		{ return this->wExtremumMax; }
	//-----------------------------------------------------
	// 平均值
	double& AverageValue()		{ return this->vAvg; }
	// 极小值
	double& ExtremumValueMin()	{ return this->vExtremumMin; }
	// 极大值
	double& ExtremumValueMax()	{ return this->vExtremumMax; }
	//-----------------------------------------------------
	// 等值线组数量
	WORD& ContourNumbers()		{ return this->wContourNums; }
	// 等值线值
	double* ContourValue()		{ return &this->dContourValue[0]; }
	double& ContourValue(int index)		{ return this->dContourValue[index]; }
	// 要求追踪的等值线最小值
	double& ContourMin()		{ return this->dContourMin; }
	// 要求追踪的等值线最大值
	double& ContourMax()		{ return this->dContourMax; }
	// 要求追踪的等值线间隔值
	double& ContourInterval()	{ return this->dContourInterval; }
	// 等值线份数
	WORD& ContourPortion()		{ return this->wContourPortion; }

	// 剪切区
	WORD& ClipArea()		{ return this->wClipArea; }
	// 剪切区边缘线上点X坐标(最多100个点)
	double* XClip()				{ return &this->xClip[0]; }
	double& XClip(int index)	{ return this->xClip[index]; }
	// 剪切区边缘线上点Y坐标(最多100个点)
	double* YClip()				{ return &this->yClip[0]; }
	double& YClip(int index)	{ return this->yClip[index]; }

	// 剪切区范围
	double& XClipMin()			{ return this->xClip[0]; }
	double& YClipMin()			{ return this->yClip[0]; }
	double& XClipMax()			{ return this->xClip[1]; }
	double& YClipMax()			{ return this->yClip[1]; }

	// 注释
	void SetComment(LPCTSTR lpszComment)
	{ strComment = lpszComment; }
	KString GetComment()		{ return strComment; }
	KString& Comment()			{ return strComment; }

	// 是否无效数据
	BOOL IsInvalidData(double value);
	//-----------------------------------------------------

public:
	TCHAR		szFileID[16];			// 文本数据文件标识
	WORD		wFileID;				// 文件标识
	WORD		wDataSource;			// 数据来源
	union
	{
		WORD		wFormatCode;		// 格式代码
		WORD		wDataCategory;		// 数据种类
	};
	WORD		wBytesOrder;			// 字节序
	WORD		wDataType;				// 数据类型
	WORD		wElementCode;			// 要素代码 (标识数据的物理属性: 重量、长度、时间、温度、压力、湿度、速度等)
	WORD		wProductCode;			// 产品代码
	WORD		wFileStorageType;		// 文件存储类型

	// 时间
	union
	{
		DATETIME	datmBeg;			// 开始时间
		struct
		{
			WORD		wYear;			// 年
			WORD		wMonth;			// 月
			WORD		wDay;			// 日
			WORD		wHour;			// 时
			WORD		wMinute;		// 分
			WORD		wSecond;		// 秒
			DWORD		dwMillisecond;	// 毫秒
		};
	};
	union
	{
		DATETIME	datmEnd;			// 结束时间
		struct
		{
			WORD		wYear2;			// 年
			WORD		wMonth2;		// 月
			WORD		wDay2;			// 日
			WORD		wHour2;			// 时
			WORD		wMinute2;		// 分
			WORD		wSecond2;		// 秒
			DWORD		dwMillisecond2;	// 毫秒
		};
	};

	WORD		wTimePeriod ;			// 时效

	union
	{
		WORD	wLayer;					// 层次
		WORD	wLayerNums;				// 层数
		WORD	wChannelNums;			// 通道数
	};
	union
	{
		WORD	wLayerCode[1000];		// 层代码
		short	iChannelCode[1000];		// 通道代码
	};
	union
	{
		int		iCurrentLayerIndex;		// 当前层索引
		int		iCurrentChannelIndex;	// 当前通道索引
	};

	WORD		wAncillaryChannelNums;			// 辅助通道数量
	short		iAncillaryChannelCode[100];		// 辅助通道代码

	WORD		wProjectType;			// 投影类型
	WORD		wPaletteType;			// 调色板类型

	WORD		wTotalNum;				// 总的离散点数量(总站点数 总项数),要求大于0
	WORD		wElementNum;			// 单站填图要素个数

	WORD		wSpecificDataNumbers;	// 特定数据数量
	DWORD		dwSpecificDataMin;		// 特定数据最小值
	DWORD		dwSpecificDataMax;		// 特定数据最大值

	DWORD		dwInvalidData;			// 无效数据
	DWORD		dwTransparentColor;		// 透明色

	DWORD		dwSdbOffset;			// SDB 偏移地址
	DWORD		dwLdbOffset;			// LDB 偏移地址
	DWORD		dwTdbOffset;			// TDB 偏移地址

	BOOL		bHasComment;			// 是否有注释
	BOOL		bHasColorTable;			// 是否有颜色表
	BOOL		bHasTransparentColor;	// 是否有透明色

	double		dBaseValue;				// 基准值
	double		dScale;					// 比例系数

	double		dBoldContourValue;		// 加粗的等值线值
	double		dSmoothCoefficient;		// 平滑系数

	// x 分量
	int			xNum;					// X方向格点数(列数)
	double		xMin;					// X坐标最小值
	double		xMax;					// X坐标最大值
	double		xInterval;				// X坐标间隔值

	// y 分量
	int			yNum;					// Y方向格点数(行数)
	double		yMin;					// Y坐标最小值
	double		yMax;					// Y坐标最大值
	double		yInterval;				// Y坐标间隔值

	// z 分量
	int			zNum;					// Z方向格点数(层数)
	double		zMin;					// Z坐标最小值
	double		zMax;					// Z坐标最大值
	double		zInterval;				// Z坐标间隔值

	// u 分量
	int			uNum;					// U分量格点数
	double		uMin;					// U分量最小值
	double		uMax;					// U分量最大值
	double		uAvg;					// U分量平均值
	double		uInterval;				// U分量间隔值
	double		uExtremumMin;			// U分量极小值
	double		uExtremumMax;			// U分量极大值

	// v 分量 / 数据值
	int			vNum;					// V分量格点数
	double		vMin;					// V分量最小值
	double		vMax;					// V分量最大值
	double		vAvg;					// V分量平均值
	double		vInterval;				// V分量间隔值
	double		vExtremumMin;			// V分量极小值
	double		vExtremumMax;			// V分量极大值

	// w 分量 / 数据值
	int			wNum;					// W分量格点数
	double		wMin;					// W分量最小值
	double		wMax;					// W分量最大值
	double		wAvg;					// W分量平均值
	double		wInterval;				// W分量间隔值
	double		wExtremumMin;			// W分量极小值
	double		wExtremumMax;			// W分量极大值

	double		dStandardLon;			// 标准经度(圆锥投影中心经线 LAMBERT,POLAR,ALBERS)

	double		dStandardLat1;			// 标准纬度1 -- 麦卡托、兰布托、极射赤面投影有效
	double		dStandardLat2;			// 标准纬度2 -- 兰布托投影有效

	double		dCenterLon;				// 中心点的经度
	double		dCenterLat;				// 中心点的纬度

	// 等值线
	WORD		wContourNums;		// 等值线组数量(一个数值对应一个等值线组)
	// 0 - 表示紧随其后没有要求追踪的等值线值,(数据用于填图)
	// 1~99 - 表示紧随其后的数据是要求追踪的等值线值,数量为 wContourNums
	// 9999 - 表示紧随其后的三个数表示 等值线最小值 等值线最大值 等值线间隔值
	union
	{
		double dContourValue[100];		// wContourNums >0 且 不等于 9999, 
		// 用来存储要求追踪的等值线值
		struct
		{
			double dContourInterval;	// 要求追踪的等值线间隔值
			double dContourMin;			// 要求追踪的等值线最小值
			double dContourMax;			// 要求追踪的等值线最大值
		};
	};

	// 当等值线标记为8888时，等值线的数量.默认=10
	WORD wContourPortion;					// 

	// 剪切区标记
	WORD wClipArea;
	// 0		- 表示没有剪切区
	// [3,99]	- 剪切区边界点数量
	// 9999		- 紧随其后四个数分别是X、Y方向范围 Xmin  Ymin  Xmax  Ymax
	double	xClip[100];					// 剪切区边缘线上点 X 坐标(最多100个点)
	double	yClip[100];					// 剪切区边缘线上点 Y 坐标(最多100个点)

	//-----------------------------------------------------
	// 卫星特有数据信息
	WORD	wOrbitNo;	// 轨道号
	WORD& OrbitNo()	{ return this->wOrbitNo; }

	WORD	wUpOrDown;	// 升降轨
	WORD& UpOrDown()	{ return this->wUpOrDown; }

	WORD	wDayOrNight;	// 白天黑夜标记
	WORD& DayOrNight()	{ return this->wDayOrNight; }

	double dRadiusOfEarth;	// 地球半径
	double& RadiusOfEarth()	{ return this->dRadiusOfEarth; }

	DWORD dwVersion;		// 版本
	DWORD& Version()		{ return this->dwVersion; }

	//-----------------------------------------------------

	DWORD	dwFlag;						// 标志

	KString strComment;					// 注释信息

	// 数据偏移地址
	public:	__declspec(property(get=GetDataOffset, put=SetDataOffset)) DWORD DataOffset;
	public:	DWORD GetDataOffset() { return m_dwDataOffset; }
	public:	VOID SetDataOffset(DWORD offset) { m_dwDataOffset = offset; }
	private:DWORD m_dwDataOffset;


	// 数据存储类型 0 - 未知 Ascii - 文本 Binary - 二进制
	public:	__declspec(property(get=GetDataStoreType, put=SetDataStoreType)) int DataStoreType;
	public:	int GetDataStoreType() { return m_iDataStoreType; }
	public:	VOID SetDataStoreType(int type) { m_iDataStoreType = type; }
	private:int m_iDataStoreType;
};

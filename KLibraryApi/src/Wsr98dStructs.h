#pragma once

class Wsr98dStructs
{
public:
	Wsr98dStructs();
	~Wsr98dStructs();
};

#pragma pack(1)

struct RadarGridInfo
{
	//x分量
	INT16		xNum;			// X方向格点数(列数)
	DOUBLE		xMin;			// X坐标最小值
	DOUBLE		xMax;			// X坐标最大值
	DOUBLE		xInterval;		// X坐标间隔值

	//y分量
	INT16		yNum;			// Y方向格点数(行数)
	DOUBLE		yMin;			// Y坐标最小值
	DOUBLE		yMax;			// Y坐标最大值
	DOUBLE		yInterval;		// Y坐标间隔值

	//时间
	UINT16		wYear;			// 年
	UINT16		wMonth;			// 月
	UINT16		wDay;			// 日
	UINT16		wHour;			// 时
	UINT16		wMinute;		// 分
	UINT16		wSecond;		// 秒

};

#pragma pack()

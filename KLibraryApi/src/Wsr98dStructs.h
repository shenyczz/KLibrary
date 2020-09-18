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
	//x����
	INT16		xNum;			// X��������(����)
	DOUBLE		xMin;			// X������Сֵ
	DOUBLE		xMax;			// X�������ֵ
	DOUBLE		xInterval;		// X������ֵ

	//y����
	INT16		yNum;			// Y��������(����)
	DOUBLE		yMin;			// Y������Сֵ
	DOUBLE		yMax;			// Y�������ֵ
	DOUBLE		yInterval;		// Y������ֵ

	//ʱ��
	UINT16		wYear;			// ��
	UINT16		wMonth;			// ��
	UINT16		wDay;			// ��
	UINT16		wHour;			// ʱ
	UINT16		wMinute;		// ��
	UINT16		wSecond;		// ��

};

#pragma pack()

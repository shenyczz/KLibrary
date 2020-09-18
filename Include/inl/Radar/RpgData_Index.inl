//{====================================================
#ifndef __INCLUDED____WSR98D_RPGDATA_INDEX_INL__
#define __INCLUDED____WSR98D_RPGDATA_INDEX_INL__

//{---Begin Include------------------------------------
//}---End Include--------------------------------------

#pragma pack(1)
//{---Begin Define-------------------------------------
//}---End Define---------------------------------------

//{---Begin Define Structure---------------------------
typedef struct _tagProductIndex
{
	short	iStationID;			// 雷达站点号
	short	iProductID;			// 产品号
	long	lLevel;				// 级数
	short	iResolution;		// 分辨率
	short	iElevation;			// 仰角(精度: 0.1)
	short	iParam1;			// 参数1
	short	iParam2;			// 参数2
	short	iYear;				// 年
	short	iMonth;				// 月
	short	iDay;				// 日
	short	iHour;				// 时
	short	iMinute;			// 分
	short	iSecond;			// 秒
	long	lSseconds;			// 与1970.1.1相差的秒数减去8小时=世界时
	char	szFileName[26];		// 用字符表示年、月、日、时、分、秒、层数、产品号及站点ID
								// 如"20030920.013245.01.19.371"
	short	iSpares[13];		// 备用
}TProductIndex;

typedef CArray <TProductIndex,TProductIndex> TProductIndexArray;
typedef CList  <TProductIndex*,TProductIndex*> TProductIndexList;
//}---End Define Structure-----------------------------

//{---Begin Define Collect Class-----------------------
//}---End Define Collect Class-------------------------

//{---Begin Define enum--------------------------------
//}---End Define enum----------------------------------
#pragma pack()

#endif	//#ifndef __INCLUDED____WSR98D_RPGDATA_INDEX_INL__

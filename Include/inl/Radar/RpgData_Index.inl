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
	short	iStationID;			// �״�վ���
	short	iProductID;			// ��Ʒ��
	long	lLevel;				// ����
	short	iResolution;		// �ֱ���
	short	iElevation;			// ����(����: 0.1)
	short	iParam1;			// ����1
	short	iParam2;			// ����2
	short	iYear;				// ��
	short	iMonth;				// ��
	short	iDay;				// ��
	short	iHour;				// ʱ
	short	iMinute;			// ��
	short	iSecond;			// ��
	long	lSseconds;			// ��1970.1.1����������ȥ8Сʱ=����ʱ
	char	szFileName[26];		// ���ַ���ʾ�ꡢ�¡��ա�ʱ���֡��롢��������Ʒ�ż�վ��ID
								// ��"20030920.013245.01.19.371"
	short	iSpares[13];		// ����
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

#ifndef __INCLUDED_WSR98D_INL__
#define __INCLUDED_WSR98D_INL__

namespace Wsr98d
{
//---------------------------------------------------------
}// namespace Wsr98d - end
//---------------------------------------------------------
using namespace Wsr98d;

namespace Wsr98d
{
	// Wsr98d ��Ʒ����
	enum Wsr98dProductCode : int
	{
		R16		= 16, R17, R18, R19, R20, R21,
		V22		= 22, V23, V24, V25, V26, V27,
		SW28	= 28, SW29, SW30,
		USP31	= 31,
		DHSR32	= 32,
		HSR33	= 33,
		CFC34	= 34,
		CR35	= 35, CR36, CR37, CR38,
		CRC39	= 39, CRC40,
		ET41	= 41,
		ETC42	= 42,
		SWR43	= 43,
		SWV44	= 44,
		SWW45	= 45,
		SWS46	= 46,
		SWP47	= 47,
		VWP48	= 48,
		CM49	= 49,
		RCS50	= 50,
		VCS51	= 51,
		SCS52	= 52,
		WER53	= 53,
		Spare54	= 54,
		SRR55	= 55,
		SRM56	= 56,
		VIL57	= 57,
		STI58	= 58,
		HI59	= 59,
		M60		= 60,
		TVS61	= 61,
		SS62	= 62,
		LRA63	= 63, LRA64,
		LRM65	= 65, LRM66,
		APR67	= 67,
		Spare68	= 68, Spare69, Spare70, Spare71, Spare72,
		UAM73	= 73, UAM74, UAM75,
		Spare76	= 76,
		FTM77	= 77,
		OHP78	= 78,
		THP79	= 79,
		STP80	= 80,
		HDP81	= 81,
		SPD82	= 82,
		RCM83	= 83,
		VAD84	= 84,
		RCS85	= 85,
		VCS86	= 86,
		CS87	= 87,
		CSC88	= 88,
		LRA89	= 89,
		LRM90	= 90,
		CAR110	= 110,
		CAV113	= 113,
		CAS115	= 115,
	};

//---------------------------------------------------------
}// namespace Wsr98d - end
//---------------------------------------------------------


#pragma pack(1)

// �籩
struct Storm
{
	TCHAR	StormId[65];	// �籩ID
	FLOAT	Azimuth;		// ��λ��(deg)
	FLOAT	Range;			// ��Χ(km)
	FLOAT	Fcst;			// Ԥ���ƶ�����(deg)
	FLOAT	Movement;		// �ƶ��ٶ�(mps)
	FLOAT	Error;			// ���
	FLOAT	Mean;			// ƽ�����
	FLOAT	dBZMax;			// ��� dBZ
	FLOAT	Height;			// ��� dBZ �߶�(km)
	FLOAT	Lon;			// ����
	FLOAT	Lat;			// γ��

	Storm()
	{
		memset(this,0,sizeof(Storm));
	}

	Storm(LPCTSTR stormid)
	{
		memset(this,0,sizeof(Storm));
		int len = _tcslen(stormid);
		len = len>64 ? 64 : len;
		_tcsncpy(StormId,stormid,len);
		StormId[len] = _T('\0');
	}
};

typedef vector<Storm> StormSequence;

// ����ָ��
struct HailIndex
{
	TCHAR	StormId[65];	// �籩ID
	FLOAT	Azimuth;		// ��λ��(deg)
	FLOAT	Range;			// ��Χ(km)
	FLOAT	Poh;			// ��������(%) ProbabilityOfHail
	FLOAT	Posh;			// ǿ��������(%) ProbabilityOfSevereHail
	FLOAT	HailSize;		// �����ߴ�(mm)
	FLOAT	Alt_0C;			// 0�Ȳ�߶�(km)
	FLOAT	Alt_n20C;		// -20�Ȳ�߶�(km)
	FLOAT	Lon;			// ����
	FLOAT	Lat;			// γ��

	HailIndex()
	{
		memset(this,0,sizeof(HailIndex));
	}

	HailIndex(LPCTSTR stormid)
	{
		memset(this,0,sizeof(HailIndex));
		int len = _tcslen(stormid);
		len = len>64 ? 64 : len;
		_tcsncpy(StormId,stormid,len);
		StormId[len] = _T('\0');
	}
};

typedef vector<HailIndex> HailIndexSequence;


#pragma pack()

#include ".\RdaData.inl"
#include ".\RpgData.inl"


#endif	//#ifndef __INCLUDED_WSR98D_INL__

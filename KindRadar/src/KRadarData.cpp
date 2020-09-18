#include "StdAfx.h"
#include "KRadarData.h"

#include "KWsr98Data.h"

KRadarData::KRadarData(void)
{
	m_hBitmap = NULL;
	m_eRadarType = eRadarType_None;

	m_pRadarDataOperator = NULL;
	m_bEmpty = TRUE;

	m_dwWidth = 512;
	m_dwHeight = 512;

	m_clrBack = RGB(0,0,0);
}

KRadarData::~KRadarData(void)
{
	Empty();
}

// 清空数据
void KRadarData::Empty()
{
	this->SetFileName(_T(""));

	::DeleteObject(m_hBitmap);
	m_hBitmap = NULL;

	m_bEmpty = TRUE;
}

// 装载数据
BOOL KRadarData::LoadData(LPCTSTR lpszFile, LPVOID lpvData)
{
	this->SetFileName(lpszFile);

	return TRUE; 
}

// 保存图像
BOOL KRadarData::SaveImage(LPCTSTR lpszFile)
{
	KDib dib;
	if(!dib.AttachHBITMAP(m_hBitmap))
		return FALSE;

	return dib.Save(lpszFile);
}

// 初始化颜色表
void KRadarData::InitColorTable(int iCode)
{
	for(int i=0;i<256;i++)
	{
		m_ColorTable[i] = 0;
	}
}


// 日期
void KRadarData::SetDate(sfx_uint16 year,sfx_uint16 month,sfx_uint16 day)
{
	m_iYear		= year;
	m_iMonth	= month;
	m_iDay		= day;
}
void KRadarData::GetDate(sfx_uint16 &year,sfx_uint16 &month,sfx_uint16 &day)
{
	year	= m_iYear;
	month	= m_iMonth;
	day		= m_iDay;
}

// 时间
void KRadarData::SetTime(sfx_uint16 hour,sfx_uint16 minute,sfx_uint16 second)
{
	m_iHour		= hour;
	m_iMinute	= minute;
	m_iSecond	= second;
}
void KRadarData::GetTime(sfx_uint16 &hour,sfx_uint16 &minute,sfx_uint16 &second)
{
	hour	= m_iHour;
	minute	= m_iMinute;
	second	= m_iSecond;
}

// 日期 时间
void KRadarData::SetDateTime(sfx_uint16 year,sfx_uint16 month,sfx_uint16 day,sfx_uint16 hour,sfx_uint16 minute,sfx_uint16 second)
{
	m_iYear		= year;
	m_iMonth	= month;
	m_iDay		= day;

	m_iHour		= hour;
	m_iMinute	= minute;
	m_iSecond	= second;
}
void KRadarData::GetDateTime(sfx_uint16 &year,sfx_uint16 &month,sfx_uint16 &day,sfx_uint16 &hour,sfx_uint16 &minute,sfx_uint16 &second)
{
	year	= m_iYear;
	month	= m_iMonth;
	day		= m_iDay;

	hour	= m_iHour;
	minute	= m_iMinute;
	second	= m_iSecond;
}

// 取得雷达数据操作器
//KRadarDataOperator* KRadarData::GetRadarDataOperator(RadarDataOperatorType eRadarDataOperatorType)
//{
//	if(eRadarDataOperatorType != eRadarDataOperator_Auto)
//	{
//		switch(eRadarDataOperatorType)
//		{
//		case eRadarDataOperator_Reader:
//			return m_pReader;
//			break;
//
//		case eRadarDataOperator_Writer:
//			return m_pWriter;
//			break;
//
//		case eRadarDataOperator_Render:
//			return m_pRender;
//			break;
//
//		default:
//			return NULL;
//		}
//	}
//	else
//	{
//		if(m_pReader)		return m_pReader;
//		else if(m_pWriter)	return m_pWriter;
//		else if(m_pRender)	return m_pRender;
//		else				return NULL;
//	}
//}

// 位图句柄
void KRadarData::SetImageHandle(HBITMAP hBitmap)
{
	::DeleteObject(m_hBitmap);
	m_hBitmap = hBitmap;
}

// 填写雷达信息
void KRadarData::FillRadarInfo(RadarInfo* pRadarInfo)
{
	memset(pRadarInfo,0,sizeof(RadarInfo));	//清除旧信息
}


//=========================================================
// 静态函数 - beg
//---------------
//static 
bool KRadarData::IsRadarData(LPCTSTR lpszFile)
{
	return KWsr98Data::IsWsr98Data(lpszFile);
}
//---------------
// 静态函数 - end
//=========================================================

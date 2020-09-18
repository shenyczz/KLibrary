/******************************************************************************
 ** KSti58gab.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KSti58gab -  STI58 的 GAB 数据
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 

 ** Modifier: 
 ** DateTime: 
 **  Explain: 

 **    Usage: 

              //1. 
              use

******************************************************************************/

#pragma once

#include "KGab.h"

/*
 // STI58GAB 文件格式

 FILENAME  20090603.130323.00.58.371
 LON_LAT   113.697   34.704

 STORM ID      P5        R5        X5        D6        M4        E6             
 AZ/RAN    161/147   178/128   167/145   174/171   181/ 99   184/118            
 FCST MVT  279/  7   136/  1   169/  6      NEW    183/  5      NEW             
 ERR/MEAN  7.7/ 4.1  4.9/ 4.4  0.3/ 0.3  0.0/ 0.0  7.3/ 2.0  0.0/ 0.0           
 DBZM HGT   44  2.7   43  4.4   40  2.6   36  3.4   36  3.2   35  1.9           

 STORM ID      F6        G6        H6                                           
 AZ/RAN    147/ 49   177/111   181/ 54                                          
 FCST MVT     NEW       NEW       NEW                                           
 ERR/MEAN  0.0/ 0.0  0.0/ 0.0  0.0/ 0.0                                         
 DBZM HGT   44  3.1   35  3.7   43  2.5                                         

 */

namespace Wsr98d { namespace Rpg { namespace Helper
{

	class KIND_EXT_CLASS KSti58gab : public KGab
	{
	public:
		KSti58gab(void);
		virtual ~KSti58gab(void);

	public:
		StormSequence* GetStormSequence() { return &m_StormSequence; }

	public:
		BOOL LoadData(LPCTSTR lpszFile);

	private:
		StormSequence m_StormSequence;

	};

//---------------------------------------------------------
}}}// namespace Wsr98d::Rpg::Helper - end
//---------------------------------------------------------

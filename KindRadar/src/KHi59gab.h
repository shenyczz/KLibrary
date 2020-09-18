#pragma once

#include "KGab.h"

/*
 FILENAME  20080502.210715.00.59.371
 LON_LAT   113.697   34.704

  STORM ID            C6       V4       A6       W5       T5       R4           
  AZ/RAN          45/165  193/151  159/140  191/140  165/ 41  171/ 51           
  POSH/POH        90/ 90   40/ 80   10/ 60   10/ 60   10/ 30    0/  0           
  MAX HAIL SIZE    38.10    19.05  < 12.70    12.70    12.70     0.00           
  0/-20 C TEMP ALT  3.2/ 6.1  KM(MSL) - LAST CHANGED 12:00 01/01/96             

  STORM ID            Y5       J5       U4       Z5       O5       P5           
  AZ/RAN         189/177  175/156  190/165  174/198  160/172  161/148           
  POSH/POH         0/  0    0/  0    0/  0    0/  0    0/  0    0/  0           
  MAX HAIL SIZE     0.00     0.00     0.00     0.00     0.00     0.00           
  0/-20 C TEMP ALT  3.2/ 6.1  KM(MSL) - LAST CHANGED 12:00 01/01/96             
*/

namespace Wsr98d { namespace Rpg { namespace Helper
{

	class KIND_EXT_CLASS KHi59gab : public KGab
	{
	public:
		KHi59gab(void);
		virtual ~KHi59gab(void);

	public:
		HailIndexSequence* GetHailIndexSequence() { return &m_HailIndexSequence; }

	public:
		BOOL LoadData(LPCTSTR lpszFile);

	private:
		HailIndexSequence m_HailIndexSequence;
	};

//---------------------------------------------------------
}}}// namespace Wsr98d::Rpg::Helper - end
//---------------------------------------------------------

#include "StdAfx.h"
#include "KHi59gab.h"

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

	KHi59gab::KHi59gab(void)
	{
		m_HailIndexSequence.clear();
	}

	KHi59gab::~KHi59gab(void)
	{
		m_HailIndexSequence.clear();
	}

	/*
	 FILENAME  20080502.210715.00.59.371
	 LON_LAT   113.697   34.704

	  STORM ID            C6       V4       A6       W5       T5       R4           
	  AZ/RAN          45/165  193/151  159/140  191/140  165/ 41  171/ 51           
	  POSH/POH        90/ 90   40/ 80   10/ 60   10/ 60   10/ 30   UNKNOWN           
	  MAX HAIL SIZE    38.10    19.05  < 12.70    12.70    12.70     0.00           
	  0/-20 C TEMP ALT  3.2/ 6.1  KM(MSL) - LAST CHANGED 12:00 01/01/96             
	*/
	BOOL KHi59gab::LoadData(LPCTSTR lpszFile)
	{
		int len;
		int curIndex = 0;
		int numStormCurGroup;
		BOOL bHasData = FALSE;
		TCHAR szTemp[128] = _T("");

		// 冰雹指数序列
		HailIndexSequence* pHailIndexSequence = this->GetHailIndexSequence();
		pHailIndexSequence->clear();

		// 打开文件
		FILE* fp = _tfopen(lpszFile,_T("rt"));
		if(NULL == fp)
			return FALSE;

		// 首行 FILENAME  20080502.210715.00.59.371
		_fgetts(szTemp,128,fp);

		// 第二行 LON_LAT   113.697   34.704
		_fgetts(szTemp,128,fp);
		len = _tcslen(szTemp);
		szTemp[len-1] = _T('\0');	 // 最后一个字符是换行符 '\n',替换为 '\0'
		TCollection<KString> aLonLat = (KString(szTemp)).Split(_T( ' '));
		if(aLonLat.Count()<3)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// 取得雷达经纬度,用于计算风暴单体经纬度
		float lon0 = (float)_tstof(aLonLat[1]);
		float lat0 = (float)_tstof(aLonLat[2]);

		// 读取一空行
		_fgetts(szTemp,128,fp);

		while(!feof(fp))
		{
			bHasData = TRUE;

			// 每个循环 5 行
			// 0:STORM ID		1:AZ/RAN
			// 2:POSH/POH		3:MAX HAIL SIZE		4:0/-20 C TEMP ALT
			LPCTSTR pszString = NULL;
			for(int i=0;i<5;i++)
			{
				_fgetts(szTemp,128,fp);
				len = _tcslen(szTemp);
				szTemp[len-1] = _T('\0');	 // 最后一个字符是换行符 '\n',替换为 '\0'
				len = _tcslen(szTemp);
				if(len==0)
				{
					bHasData = FALSE;
					break;
				}
				
				// 前4个从16开始是数据
				if(i<4)
				{
					pszString = (LPTSTR)KString::TrimString(&szTemp[16]);
				}
				// 第5个从19开始是数据
				else if(i==4)
				{
					pszString = (LPTSTR)KString::TrimString(&szTemp[19]);
				}

				//  STORM ID            C6       V4       A6       W5       T5       R4
				if(i==0)
				{
					// 以空格为分割符分割字符串
					TCollection<KString> aStormID = (KString(pszString)).Split(_T( ' '));
					// 本组数量
					numStormCurGroup = aStormID.Count();
					for(int index=0;index<numStormCurGroup;index++)
					{
						HailIndex hailIndex(aStormID[index]);
						pHailIndexSequence->push_back(hailIndex);
					}
				}
				// AZ/RAN          45/165  193/151  159/140  191/140  165/ 41  171/ 51           
				// POSH/POH        90/ 90   40/ 80   10/ 60   10/ 60   10/ 30   UNKNOWN           
				else if(i==1 || i==2)
				{
					KString strContent(pszString);
					strContent.Replace(_T("/"),_T(" "));
					strContent.Replace(_T("UNKNOWN"),_T("0 0"));

					// 以空格为分割符分割字符串
					TCollection<KString> aContent = strContent.Split(_T( ' '));

					// 数量应该是风暴核的2倍
					if(numStormCurGroup*2 != aContent.Count())
					{
						fclose(fp);
						fp = NULL;
						return FALSE;
					}

					int stormCount = pHailIndexSequence->size();
					for(int id=curIndex,index=0;id<stormCount,index<numStormCurGroup*2;id++,index+=2)
					{
						HailIndex* pHailIndex = &pHailIndexSequence->at(id);
						// AZ/RAN          45/165  193/151  159/140  191/140  165/ 41  171/ 51           
						if(i==1)
						{
							pHailIndex->Azimuth = (float)_tstof(aContent[index]);
							pHailIndex->Range = (float)_tstof(aContent[index+1]);

							pHailIndex->Lon = (float)this->ComputeLongitude(lon0,lat0,pHailIndex->Azimuth,pHailIndex->Range);
							pHailIndex->Lat = (float)this->ComputeLatitude(lon0,lat0,pHailIndex->Azimuth,pHailIndex->Range);
						}
						// POSH/POH        90/ 90   40/ 80   10/ 60   10/ 60   10/ 30   UNKNOWN           
						else if(i==2)
						{
							pHailIndex->Posh = (float)_tstof(aContent[index]);
							pHailIndex->Poh = (float)_tstof(aContent[index+1]);
						}
					}

				}// else if(i==1 || i==2)
				//MAX HAIL SIZE    38.10    19.05  < 12.70    12.70    12.70     UNKNOWN           
				else if(i==3)
				{
					KString strContent(pszString);
					strContent.Replace(_T("<"),_T(" "));
					strContent.Replace(_T("UNKNOWN"),_T("0.0"));
					// 以空格为分割符分割字符串
					TCollection<KString> aContent = strContent.Split(_T( ' '));
					if(numStormCurGroup != aContent.Count())
					{
						fclose(fp);
						fp = NULL;
						return FALSE;
					}

					int count = pHailIndexSequence->size();
					for(int id=curIndex,index=0;id<count,index<numStormCurGroup;id++,index++)
					{
						HailIndex* pHailIndex = &pHailIndexSequence->at(id);
						pHailIndex->HailSize = (float)_tstof(aContent[index]);
					}
				}
				// 0/-20 C TEMP ALT  3.2/ 6.1  KM(MSL) - LAST CHANGED 12:00 01/01/96             
				else	// i==4
				{
					KString strContent(pszString);
					int pos = strContent.Find(_T("KM"));
					pszString = strContent.Mid(0,pos);
					strContent = KString(pszString);
					strContent.Replace(_T("/"),_T(" "));
					// 以空格为分割符分割字符串
					TCollection<KString> aContent = strContent.Split(_T( ' '));

					int count = pHailIndexSequence->size();
					for(int id=curIndex;id<count;id++)
					{
						HailIndex* pHailIndex = &pHailIndexSequence->at(id);
						pHailIndex->Alt_0C = (float)_tstof(aContent[0]);
						pHailIndex->Alt_n20C = (float)_tstof(aContent[1]);
					}
				}

			}// for(i)

			if(bHasData)
				curIndex = pHailIndexSequence->size();

			// 读取一空行
			_fgetts(szTemp,128,fp);

		}// while(feof(fp))

		// 关闭文件
		fclose(fp);
		fp = NULL;

		return TRUE;

		//=================================================
		// 输出测试
		int size = pHailIndexSequence->size();
		fp = _tfopen(_T("e:\\temp\\hi.txt"),_T("wt"));
		for(int i=0;i<size;i++)
		{
			HailIndex* pHailIndex = &pHailIndexSequence->at(i);

			_ftprintf(fp,_T("%s %8.2f %8.2f %8.2f %8.2f %8.2f %8.2f %8.2f %8.3f %8.3f\n")
				,pHailIndex->StormId		// 风暴ID
				,pHailIndex->Azimuth		// 方位角(deg)
				,pHailIndex->Range			// 范围(km)
				,pHailIndex->Posh			// 强冰雹概率(%) ProbabilityOfSevereHail
				,pHailIndex->Poh			// 冰雹概率(%) ProbabilityOfHail
				,pHailIndex->HailSize		// 冰雹尺寸(mm)
				,pHailIndex->Alt_0C			// 0度层高度(km)
				,pHailIndex->Alt_n20C		// -20度层高度(km)
				,pHailIndex->Lon			// 经度
				,pHailIndex->Lat			// 纬度
				);
		}

		fclose(fp);

		return TRUE;
		//=================================================

	}

//---------------------------------------------------------
}}}// namespace Wsr98d::Rpg::Helper - end
//---------------------------------------------------------

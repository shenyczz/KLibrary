#include "StdAfx.h"
#include "KSti58gab.h"

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

	KSti58gab::KSti58gab(void)
	{
		m_StormSequence.clear();
	}

	KSti58gab::~KSti58gab(void)
	{
		m_StormSequence.clear();
	}

	/*
	 FILENAME  20090603.130323.00.58.371
	 LON_LAT   113.697   34.704

	 STORM ID      B6        V4        C6        T5        A6        W5             
	 AZ/RAN     57/276   193/151    45/165   165/ 41   159/140   191/140            
	 FCST MVT     NEW    191/  4      NEW    186/  6   161/  5   195/  9            
	 ERR/MEAN  0.0/ 0.0  0.5/ 1.6  0.0/ 0.0  1.0/ 0.8  0.6/ 0.6  1.4/ 1.4           
	 DBZM HGT   71  7.4   55  5.4   72  3.2   55  3.3   51  4.9   51  4.9           
	*/
	BOOL KSti58gab::LoadData(LPCTSTR lpszFile)
	{
		int len;
		int curIndex = 0;
		int numStormCurGroup;
		BOOL bHasData = FALSE;
		TCHAR szTemp[128] = _T("");

		StormSequence* pStormSequence = this->GetStormSequence();
		pStormSequence->clear();

		FILE* fp = _tfopen(lpszFile,_T("rt"));
		if(NULL == fp)
			return FALSE;

		// 首行 FILENAME  20090603.130323.00.58.371
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
			// 2:FCST MVT		3:ERR/MEAN		4:DBZM HGT
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

				// 第9个字符开始时内容
				LPTSTR pszString = (LPTSTR)KString::TrimString(&szTemp[9]);

				// STORM ID      B6        V4        C6        T5        A6        W5             
				if(i==0)
				{
					// 以空格为分割符分割字符串
					TCollection<KString> aStormID = (KString(pszString)).Split(_T( ' '));
					// 本组风暴数量
					numStormCurGroup = aStormID.Count();

					for(int index=0;index<numStormCurGroup;index++)
					{
						Storm storm(aStormID[index]);
						pStormSequence->push_back(storm);
					}
				}

				// DBZM HGT   71  7.4   55  5.4   72  3.2   55  3.3   51  4.9   51  4.9           
				else if(i==4)
				{
					// 以空格为分割符分割字符串(最大dbz及其高度)
					TCollection<KString> aDBZM_HGT = (KString(pszString)).Split(_T( ' '));
					if(numStormCurGroup*2 != aDBZM_HGT.Count())
					{
						fclose(fp);
						fp = NULL;
						return FALSE;
					}
					int count = pStormSequence->size();
					for(int id=curIndex,index=0;id<count,index<numStormCurGroup*2;id++,index+=2)
					{
						Storm* pStorm = &pStormSequence->at(id);
						pStorm->dBZMax = (float)_tstof(aDBZM_HGT[index]);
						pStorm->Height = (float)_tstof(aDBZM_HGT[index+1]);
					}
				}

				//AZ/RAN     57/276   193/151    45/165   165/ 41   159/140   191/140            
				//FCST MVT     NEW    191/  4      NEW    186/  6   161/  5   195/  9            
				//ERR/MEAN  0.0/ 0.0  0.5/ 1.6  0.0/ 0.0  1.0/ 0.8  0.6/ 0.6  1.4/ 1.4           
				else
				{
					KString strContent(pszString);
					strContent.Replace(_T("/"),_T(" "));
					strContent.Replace(_T("NEW"),_T("0 0"));

					// 以空格为分割符分割字符串(最大dbz及其高度)
					TCollection<KString> aContent = strContent.Split(_T( ' '));
					if(numStormCurGroup*2 != aContent.Count())
					{
						fclose(fp);
						fp = NULL;
						return FALSE;
					}

					int stormCount = pStormSequence->size();
					for(int id=curIndex,index=0;id<stormCount,index<numStormCurGroup*2;id++,index+=2)
					{
						Storm* pStorm = &pStormSequence->at(id);
						//AZ/RAN     57/276   193/151    45/165   165/ 41   159/140   191/140            
						if(i==1)
						{
							pStorm->Azimuth = (float)_tstof(aContent[index]);
							pStorm->Range = (float)_tstof(aContent[index+1]);

							pStorm->Lon = (float)this->ComputeLongitude(lon0,lat0,pStorm->Azimuth,pStorm->Range);
							pStorm->Lat = (float)this->ComputeLatitude(lon0,lat0,pStorm->Azimuth,pStorm->Range);
						}
						//FCST MVT     NEW    191/  4      NEW    186/  6   161/  5   195/  9            
						else if(i==2)
						{
							pStorm->Fcst = (float)_tstof(aContent[index]);
							pStorm->Movement = (float)_tstof(aContent[index+1]);
						}
						//ERR/MEAN  0.0/ 0.0  0.5/ 1.6  0.0/ 0.0  1.0/ 0.8  0.6/ 0.6  1.4/ 1.4           
						else if(i==3)
						{
							pStorm->Error = (float)_tstof(aContent[index]);
							pStorm->Mean = (float)_tstof(aContent[index+1]);
						}
						else
						{
							// no code
						}

					}// for(id...)

				}//else (1,2,3)

			} // for(i)

			if(bHasData)
				curIndex = pStormSequence->size();

			// 读取一空行
			_fgetts(szTemp,128,fp);

		}// while(feof(fp))

		fclose(fp);
		fp = NULL;

		return TRUE;

		//=================================================
		// 输出测试
		int size = pStormSequence->size();
		fp = _tfopen(_T("e:\\temp\\sti.txt"),_T("wt"));

		for(int i=0;i<size;i++)
		{
			Storm* pStorm = &pStormSequence->at(i);

			_ftprintf(fp,_T("%s %8.2f %8.2f %8.2f %8.2f %8.2f %8.2f %8.2f %8.2f %8.3f %8.3f\n")
				,pStorm->StormId	// 风暴ID
				,pStorm->Azimuth	// 方位角(deg)
				,pStorm->Range		// 范围(km)
				,pStorm->Fcst		// 预报移动方向(deg)
				,pStorm->Movement	// 移动速度(mps)
				,pStorm->Error		// 误差
				,pStorm->Mean		// 平均误差
				,pStorm->dBZMax		// 最大 dBZ
				,pStorm->Height		// 最大 dBZ 高度(km)
				,pStorm->Lon		// 经度
				,pStorm->Lat		// 纬度
				);
		}

		fclose(fp);
		return TRUE;
		//=================================================
	}


//---------------------------------------------------------
}}}// namespace Wsr98d::Rpg::Helper - end
//---------------------------------------------------------

#include "StdAfx.h"
#include "KRdaData.h"

namespace Wsr98d { namespace Rda
{

	KRdaData::KRdaData(void)
	{
		m_iElvIndexCur = 0;	// 当前仰角索引
		m_iAzmIndexCur = 0;	// 当前方位角索引

		m_bVolBeg = FALSE;	// 体扫开始
		m_bVolEnd = FALSE;	// 体扫结束

		m_pRefData = NULL;	// 仰角数据
		m_pVelData = NULL;	// 反射率数据
		m_pSpwData = NULL;	// 速度数据
		m_pElvData = NULL;	// 谱宽数据

		m_pRdaRadialDataPacket = NULL;
	
		for(int i=0;i<RDA_DATA_MAX_CUTS;i++)
		{
			//m_RadialListRef[i] = NULL;	// 强度径向数据集
			//m_RadialListVel[i] = NULL;	// 速度径向数据集
			//m_RadialListSpw[i] = NULL;	// 谱宽径向数据集
		}
	}

	KRdaData::~KRdaData(void)
	{
		this->FreeMemory();
	}

	BOOL KRdaData::LoadData(LPCTSTR lpszFile, LPVOID lpvData)
	{
		// 定义局部变量
		float fCurElv;	//当前仰角
		BOOL bRefData,bVelData,bSpwData;	//反射率、速度、谱宽数据标记

		//准备内存
		if(!PrepareMemory())
			return FALSE;

		// 径向数据
		RdaRadialDataPacket* pRdaRadialDataPacket = m_pRdaRadialDataPacket;
		// 信息头
		RdaMessageHead* pRdaMessageHead = &pRdaRadialDataPacket->oRdaMessageHead;
		// 数据头
		RdaDataHead* pRdaDataHead = &pRdaRadialDataPacket->oRdaDataHead;

		//打开文件
		FILE *fp = _tfopen(lpszFile,_T("rb"));
		if(fp==NULL)
			return FALSE;

		// 循环读取全部射线数据
		// 一次循环读取一条波束数据 RdaRadialDataPacket
		while(1)
		{
			// 初始化数据空间
			memset((char*)pRdaRadialDataPacket,0,sizeof(RdaRadialDataPacket));

			// 读取一条波束数据 RdaRadialDataPacket
			int iReadSize = (int)fread(pRdaRadialDataPacket,sizeof(RdaRadialDataPacket),1,fp);
			if(iReadSize < 1)
				break;

			// 根据雷达状态字段判断是否雷达数据
			if(pRdaMessageHead->cMessageType != 1)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// 根据射线状态确定体扫开始
			// 一个基数据文件包含一个体扫数据
			if(pRdaDataHead->iRadialStatus == RDA_DATA_VOL_BEG)	// 3 体扫开始
			{//本代码段一个体扫文件只可能执行一次

				//解码当前仰角
				fCurElv = (float)DecodeElv(pRdaDataHead->iElevationAngle);

				m_iElvIndexCur = 0;								//仰角索引置 0
				m_pElvData[m_iElvIndexCur] = fCurElv;			//保存当前仰角
			
				m_bVolBeg = TRUE;								//设置体扫开始标记
			}

			// 如果体扫没有开始
			// 进入下一个循环
			// 读入下一个 RdaRadialDataPacket 结构数据
			if(!m_bVolBeg)
				continue;

			///
			///如果体扫开始,进行下面的处理...
			///

			// 设置体扫结束标记
			m_bVolEnd = (pRdaDataHead->iRadialStatus == RDA_DATA_VOL_END);	// 4 体扫结束标记

			// 复位反射率、速度、谱宽数据标记
			bRefData = FALSE;
			bVelData = FALSE;
			bSpwData = FALSE;

			// 一个仰角层开始
			if(pRdaDataHead->iRadialStatus == RDA_DATA_ELV_BEG)	// 0 该仰角的第一条径向数据
			{
				// 解码当前仰角
				fCurElv = (float)DecodeElv(pRdaDataHead->iElevationAngle);

				// 仰角间隔大于 0.4 度视为是不同的仰角
				if(fCurElv - m_pElvData[m_iElvIndexCur] > 0.4)
				{
					m_iElvIndexCur++;						// 仰角索引递增
					m_pElvData[m_iElvIndexCur] = fCurElv;	// 保存当前仰角
				}
			}

			// 计算方位角
			float fCurAzm = (float)DecodeElv(pRdaDataHead->iAzimuthAngle);
			if(fCurAzm>360 || fabs(fCurAzm-360)<1.0e-12)	//fCurAz>=360
			{
				fCurAzm = fCurAzm - 360.0f;
			}
			// 计算方位角索引
			m_iAzmIndexCur = int(fCurAzm+0.5);	// 0-359

			// 判断本仰角层上的数据类型(反射率数据、速度数据、谱宽数据)
			// 有可能都是 TRUE 
			bRefData = pRdaDataHead->iOffsetOfRef != 0;
			bVelData = pRdaDataHead->iOffsetOfVel != 0;
			bSpwData = pRdaDataHead->iOffsetOfSpw != 0;

			if(bRefData)
			{
				// 保存反射率数据
				RefCollect();
			}
			if(bVelData)
			{
				// 保存速度数据
				VelCollect();
			}
			if(bSpwData)
			{
				// 保存谱宽数据
				SpwCollect();
			}

		}//while(1) - end


		// 关闭文件
		fclose(fp);
		fp = NULL;

		// 调用父类同名函数
		return KRadarData::LoadData(lpszFile);
	}
	void KRdaData::RefCollect()
	{
	}
	void KRdaData::VelCollect()
	{
	}
	void KRdaData::SpwCollect()
	{
	}

	// 准备内存
	BOOL KRdaData::PrepareMemory()
	{
		int i,iSize = 0;

		// 先释放内存 
		FreeMemory();

		//分配一条径向数据内存
		m_pRdaRadialDataPacket = new RdaRadialDataPacket;
		if(m_pRdaRadialDataPacket==NULL) { FreeMemory(); return FALSE; }
		memset((char*)m_pRdaRadialDataPacket,0,sizeof(RdaRadialDataPacket));

		///------------------------------------------------------------------------
		/// RDA_DATA_MAX_CUTS		// 每个体扫数据最大层数			= 20
		/// RDA_DATA_MAX_RADIALS	// 每个层最大径向数据数			= 360
		/// RDA_DATA_RGATES			// 每条径向数据的反射率距离库数	= 460
		///	RDA_DATA_VGATES			// 每条径向数据的  速度距离库数	= 920
		///	RDA_DATA_WGATES			// 每条径向数据的  谱宽距离库数	= 920
		///
		/// 最大容量 = 每条径向数据距离库数 * 每个层最大径向数据数 * 每个体扫数据最大层数
		///------------------------------------------------------------------------
		// 分配反射率内存
		iSize = RDA_DATA_RGATES * RDA_DATA_MAX_RADIALS * RDA_DATA_MAX_CUTS;	// 
		m_pRefData = new float[iSize];
		if(m_pRefData==NULL) { FreeMemory(); return FALSE; }
		for(i=0;i<iSize;i++)
		{
			*(m_pRefData+i) = RDA_DATA_VALUE_INVALID;	// -999 实际值中的特殊标记,表示无效观测数据
		}
		//分配速度内存
		iSize = RDA_DATA_VGATES * RDA_DATA_MAX_RADIALS * RDA_DATA_MAX_CUTS;
		m_pVelData = new float[iSize];
		if(m_pVelData==NULL) { FreeMemory(); return FALSE; }
		for(i=0;i<iSize;i++)
		{
			*(m_pVelData+i) = RDA_DATA_VALUE_INVALID;	// -999 实际值中的特殊标记,表示无效观测数据
		}
		//分配普宽内存
		iSize = RDA_DATA_WGATES * RDA_DATA_MAX_RADIALS * RDA_DATA_MAX_CUTS;
		m_pSpwData = new float[iSize];
		if(m_pSpwData==NULL) { FreeMemory(); return FALSE; }
		for(i=0;i<iSize;i++)
		{
			*(m_pSpwData+i) = RDA_DATA_VALUE_INVALID;	// -999 实际值中的特殊标记,表示无效观测数据
		}
		//分配仰角内存
		iSize = RDA_DATA_MAX_CUTS;
		m_pElvData = new float[iSize];
		if(m_pElvData==NULL) { FreeMemory(); return FALSE; }
		for(i=0;i<iSize;i++)
		{
			*(m_pElvData+i) = RDA_DATA_VALUE_INVALID;	// -999 实际值中的特殊标记,表示无效观测数据
		}

		//
		for(int i=0;i<RDA_DATA_MAX_CUTS;i++)
		{
			//m_RadialListRef[i] = new KRadialList;	// 强度径向数据集
			//m_RadialListVel[i] = new KRadialList;	// 速度径向数据集
			//m_RadialListSpw[i] = new KRadialList;	// 谱宽径向数据集
		}

		return TRUE;
	}
	// 释放内存
	void KRdaData::FreeMemory()
	{
		_delete(m_pRdaRadialDataPacket);

		_deletea(m_pRefData);
		_deletea(m_pVelData);
		_deletea(m_pSpwData);
		_deletea(m_pElvData);

		for(int i=0;i<RDA_DATA_MAX_CUTS;i++)
		{
			//_delete(m_RadialListRef[i]);
			//_delete(m_RadialListVel[i]);
			//_delete(m_RadialListSpw[i]);
		}
	}


	//---------------------------------------------------------
	// 名  称: DecodeRef
	// 功  能: 将读出的反射率数据解码
	// 调用者:  
	// 输  入:
	//		code      - 无符号的BYTE型，读出的反射率编码值
	//
	// 输  出: 
	// 返  回: 实型,返回解码后的反射率(dBZ)
	//---------------------------------------------------------
	double KRdaData::DecodeRef(unsigned char code)
	{
		if(code==RDA_DATA_CODE_INVALID)		return RDA_DATA_VALUE_INVALID;
		else if(code==RDA_DATA_CODE_RANFOLD)	return RDA_DATA_VALUE_RANFOLD;
		else								return (float((code-2.)/2.-32.5));
	}

	//---------------------------------------------------------
	// 名  称: DecodeVel
	// 功  能: 将读出的径向速度数据解码
	// 调用者:  
	// 输  入:
	//		code    - 无符号的BYTE型，读出的径向速度编码值
	//		ResType - 2字节整型，速度精度标记，精度不同，解码方式不同
	//		          ResType = 2: 精度为0.5 m/s
	//		          ResType = 4: 精度为1.0 m/s
	//
	// 输  出: 
	// 返  回: 实型,返回解码后的径向速度(m/s)
	//---------------------------------------------------------
	double KRdaData::DecodeVel(unsigned char code, short ResType)
	{
		if(code==RDA_DATA_CODE_INVALID)		return RDA_DATA_VALUE_INVALID;
		else if(code==RDA_DATA_CODE_RANFOLD)	return RDA_DATA_VALUE_RANFOLD;
		else      
		{
			if(ResType==RDA_DATA_RES_POINT_FIVE) //0.5 m/s 
				return (float((code-2.)/2.-63.5));
			else
				return (float((code-2)-127.));
		}
	}

	//---------------------------------------------------------
	// 名  称: DecodeSpw
	// 功  能: 将读出的谱宽数据解码
	// 调用者:  
	// 输  入:
	//		code    - 无符号的BYTE型，读出的径向速度编码值
	//
	// 输  出: 
	// 返  回: 实型,返回解码后的谱宽(m/s)
	//---------------------------------------------------------
	double KRdaData::DecodeSpw(unsigned char code)
	{
		if(code==RDA_DATA_CODE_INVALID)		return RDA_DATA_VALUE_INVALID;
		else if(code==RDA_DATA_CODE_RANFOLD)	return RDA_DATA_VALUE_RANFOLD;
		else								return ((double)(code-2) / 2) - 63.5;
	}

	//---------------------------------------------------------
	// 名    称: DecodeElv
	// 功    能: 仰角解码
	// 访    问: protected
	// 参    数:
	// 返    回: 
	// 注    释: (编码方式：[数值/8.]*[180./4096.]=度)
	//---------------------------------------------------------
	double KRdaData::DecodeElv(unsigned short code)
	{
		double fValue = 0.0f;
		fValue = ((double)code/8.0)*(180.0/4096.0);
		return fValue;
	}

	//=====================================================
	// 静态函数 - beg
	//---------------
	//static
	bool KRdaData::IsRdaData(LPCTSTR lpszFile)
	{
		return false;
	}
	//---------------
	// 静态函数 - end
	//=====================================================

//--------------------------------------------------------
}}// namespace Wsr98d::Rda - end
//--------------------------------------------------------


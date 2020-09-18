#include "StdAfx.h"
#include "KRdaData.h"

namespace Wsr98d { namespace Rda
{

	KRdaData::KRdaData(void)
	{
		m_iElvIndexCur = 0;	// ��ǰ��������
		m_iAzmIndexCur = 0;	// ��ǰ��λ������

		m_bVolBeg = FALSE;	// ��ɨ��ʼ
		m_bVolEnd = FALSE;	// ��ɨ����

		m_pRefData = NULL;	// ��������
		m_pVelData = NULL;	// ����������
		m_pSpwData = NULL;	// �ٶ�����
		m_pElvData = NULL;	// �׿�����

		m_pRdaRadialDataPacket = NULL;
	
		for(int i=0;i<RDA_DATA_MAX_CUTS;i++)
		{
			//m_RadialListRef[i] = NULL;	// ǿ�Ⱦ������ݼ�
			//m_RadialListVel[i] = NULL;	// �ٶȾ������ݼ�
			//m_RadialListSpw[i] = NULL;	// �׿������ݼ�
		}
	}

	KRdaData::~KRdaData(void)
	{
		this->FreeMemory();
	}

	BOOL KRdaData::LoadData(LPCTSTR lpszFile, LPVOID lpvData)
	{
		// ����ֲ�����
		float fCurElv;	//��ǰ����
		BOOL bRefData,bVelData,bSpwData;	//�����ʡ��ٶȡ��׿����ݱ��

		//׼���ڴ�
		if(!PrepareMemory())
			return FALSE;

		// ��������
		RdaRadialDataPacket* pRdaRadialDataPacket = m_pRdaRadialDataPacket;
		// ��Ϣͷ
		RdaMessageHead* pRdaMessageHead = &pRdaRadialDataPacket->oRdaMessageHead;
		// ����ͷ
		RdaDataHead* pRdaDataHead = &pRdaRadialDataPacket->oRdaDataHead;

		//���ļ�
		FILE *fp = _tfopen(lpszFile,_T("rb"));
		if(fp==NULL)
			return FALSE;

		// ѭ����ȡȫ����������
		// һ��ѭ����ȡһ���������� RdaRadialDataPacket
		while(1)
		{
			// ��ʼ�����ݿռ�
			memset((char*)pRdaRadialDataPacket,0,sizeof(RdaRadialDataPacket));

			// ��ȡһ���������� RdaRadialDataPacket
			int iReadSize = (int)fread(pRdaRadialDataPacket,sizeof(RdaRadialDataPacket),1,fp);
			if(iReadSize < 1)
				break;

			// �����״�״̬�ֶ��ж��Ƿ��״�����
			if(pRdaMessageHead->cMessageType != 1)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// ��������״̬ȷ����ɨ��ʼ
			// һ���������ļ�����һ����ɨ����
			if(pRdaDataHead->iRadialStatus == RDA_DATA_VOL_BEG)	// 3 ��ɨ��ʼ
			{//�������һ����ɨ�ļ�ֻ����ִ��һ��

				//���뵱ǰ����
				fCurElv = (float)DecodeElv(pRdaDataHead->iElevationAngle);

				m_iElvIndexCur = 0;								//���������� 0
				m_pElvData[m_iElvIndexCur] = fCurElv;			//���浱ǰ����
			
				m_bVolBeg = TRUE;								//������ɨ��ʼ���
			}

			// �����ɨû�п�ʼ
			// ������һ��ѭ��
			// ������һ�� RdaRadialDataPacket �ṹ����
			if(!m_bVolBeg)
				continue;

			///
			///�����ɨ��ʼ,��������Ĵ���...
			///

			// ������ɨ�������
			m_bVolEnd = (pRdaDataHead->iRadialStatus == RDA_DATA_VOL_END);	// 4 ��ɨ�������

			// ��λ�����ʡ��ٶȡ��׿����ݱ��
			bRefData = FALSE;
			bVelData = FALSE;
			bSpwData = FALSE;

			// һ�����ǲ㿪ʼ
			if(pRdaDataHead->iRadialStatus == RDA_DATA_ELV_BEG)	// 0 �����ǵĵ�һ����������
			{
				// ���뵱ǰ����
				fCurElv = (float)DecodeElv(pRdaDataHead->iElevationAngle);

				// ���Ǽ������ 0.4 ����Ϊ�ǲ�ͬ������
				if(fCurElv - m_pElvData[m_iElvIndexCur] > 0.4)
				{
					m_iElvIndexCur++;						// ������������
					m_pElvData[m_iElvIndexCur] = fCurElv;	// ���浱ǰ����
				}
			}

			// ���㷽λ��
			float fCurAzm = (float)DecodeElv(pRdaDataHead->iAzimuthAngle);
			if(fCurAzm>360 || fabs(fCurAzm-360)<1.0e-12)	//fCurAz>=360
			{
				fCurAzm = fCurAzm - 360.0f;
			}
			// ���㷽λ������
			m_iAzmIndexCur = int(fCurAzm+0.5);	// 0-359

			// �жϱ����ǲ��ϵ���������(���������ݡ��ٶ����ݡ��׿�����)
			// �п��ܶ��� TRUE 
			bRefData = pRdaDataHead->iOffsetOfRef != 0;
			bVelData = pRdaDataHead->iOffsetOfVel != 0;
			bSpwData = pRdaDataHead->iOffsetOfSpw != 0;

			if(bRefData)
			{
				// ���淴��������
				RefCollect();
			}
			if(bVelData)
			{
				// �����ٶ�����
				VelCollect();
			}
			if(bSpwData)
			{
				// �����׿�����
				SpwCollect();
			}

		}//while(1) - end


		// �ر��ļ�
		fclose(fp);
		fp = NULL;

		// ���ø���ͬ������
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

	// ׼���ڴ�
	BOOL KRdaData::PrepareMemory()
	{
		int i,iSize = 0;

		// ���ͷ��ڴ� 
		FreeMemory();

		//����һ�����������ڴ�
		m_pRdaRadialDataPacket = new RdaRadialDataPacket;
		if(m_pRdaRadialDataPacket==NULL) { FreeMemory(); return FALSE; }
		memset((char*)m_pRdaRadialDataPacket,0,sizeof(RdaRadialDataPacket));

		///------------------------------------------------------------------------
		/// RDA_DATA_MAX_CUTS		// ÿ����ɨ����������			= 20
		/// RDA_DATA_MAX_RADIALS	// ÿ���������������			= 360
		/// RDA_DATA_RGATES			// ÿ���������ݵķ����ʾ������	= 460
		///	RDA_DATA_VGATES			// ÿ���������ݵ�  �ٶȾ������	= 920
		///	RDA_DATA_WGATES			// ÿ���������ݵ�  �׿�������	= 920
		///
		/// ������� = ÿ���������ݾ������ * ÿ��������������� * ÿ����ɨ����������
		///------------------------------------------------------------------------
		// ���䷴�����ڴ�
		iSize = RDA_DATA_RGATES * RDA_DATA_MAX_RADIALS * RDA_DATA_MAX_CUTS;	// 
		m_pRefData = new float[iSize];
		if(m_pRefData==NULL) { FreeMemory(); return FALSE; }
		for(i=0;i<iSize;i++)
		{
			*(m_pRefData+i) = RDA_DATA_VALUE_INVALID;	// -999 ʵ��ֵ�е�������,��ʾ��Ч�۲�����
		}
		//�����ٶ��ڴ�
		iSize = RDA_DATA_VGATES * RDA_DATA_MAX_RADIALS * RDA_DATA_MAX_CUTS;
		m_pVelData = new float[iSize];
		if(m_pVelData==NULL) { FreeMemory(); return FALSE; }
		for(i=0;i<iSize;i++)
		{
			*(m_pVelData+i) = RDA_DATA_VALUE_INVALID;	// -999 ʵ��ֵ�е�������,��ʾ��Ч�۲�����
		}
		//�����տ��ڴ�
		iSize = RDA_DATA_WGATES * RDA_DATA_MAX_RADIALS * RDA_DATA_MAX_CUTS;
		m_pSpwData = new float[iSize];
		if(m_pSpwData==NULL) { FreeMemory(); return FALSE; }
		for(i=0;i<iSize;i++)
		{
			*(m_pSpwData+i) = RDA_DATA_VALUE_INVALID;	// -999 ʵ��ֵ�е�������,��ʾ��Ч�۲�����
		}
		//���������ڴ�
		iSize = RDA_DATA_MAX_CUTS;
		m_pElvData = new float[iSize];
		if(m_pElvData==NULL) { FreeMemory(); return FALSE; }
		for(i=0;i<iSize;i++)
		{
			*(m_pElvData+i) = RDA_DATA_VALUE_INVALID;	// -999 ʵ��ֵ�е�������,��ʾ��Ч�۲�����
		}

		//
		for(int i=0;i<RDA_DATA_MAX_CUTS;i++)
		{
			//m_RadialListRef[i] = new KRadialList;	// ǿ�Ⱦ������ݼ�
			//m_RadialListVel[i] = new KRadialList;	// �ٶȾ������ݼ�
			//m_RadialListSpw[i] = new KRadialList;	// �׿������ݼ�
		}

		return TRUE;
	}
	// �ͷ��ڴ�
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
	// ��  ��: DecodeRef
	// ��  ��: �������ķ��������ݽ���
	// ������:  
	// ��  ��:
	//		code      - �޷��ŵ�BYTE�ͣ������ķ����ʱ���ֵ
	//
	// ��  ��: 
	// ��  ��: ʵ��,���ؽ����ķ�����(dBZ)
	//---------------------------------------------------------
	double KRdaData::DecodeRef(unsigned char code)
	{
		if(code==RDA_DATA_CODE_INVALID)		return RDA_DATA_VALUE_INVALID;
		else if(code==RDA_DATA_CODE_RANFOLD)	return RDA_DATA_VALUE_RANFOLD;
		else								return (float((code-2.)/2.-32.5));
	}

	//---------------------------------------------------------
	// ��  ��: DecodeVel
	// ��  ��: �������ľ����ٶ����ݽ���
	// ������:  
	// ��  ��:
	//		code    - �޷��ŵ�BYTE�ͣ������ľ����ٶȱ���ֵ
	//		ResType - 2�ֽ����ͣ��ٶȾ��ȱ�ǣ����Ȳ�ͬ�����뷽ʽ��ͬ
	//		          ResType = 2: ����Ϊ0.5 m/s
	//		          ResType = 4: ����Ϊ1.0 m/s
	//
	// ��  ��: 
	// ��  ��: ʵ��,���ؽ����ľ����ٶ�(m/s)
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
	// ��  ��: DecodeSpw
	// ��  ��: ���������׿����ݽ���
	// ������:  
	// ��  ��:
	//		code    - �޷��ŵ�BYTE�ͣ������ľ����ٶȱ���ֵ
	//
	// ��  ��: 
	// ��  ��: ʵ��,���ؽ������׿�(m/s)
	//---------------------------------------------------------
	double KRdaData::DecodeSpw(unsigned char code)
	{
		if(code==RDA_DATA_CODE_INVALID)		return RDA_DATA_VALUE_INVALID;
		else if(code==RDA_DATA_CODE_RANFOLD)	return RDA_DATA_VALUE_RANFOLD;
		else								return ((double)(code-2) / 2) - 63.5;
	}

	//---------------------------------------------------------
	// ��    ��: DecodeElv
	// ��    ��: ���ǽ���
	// ��    ��: protected
	// ��    ��:
	// ��    ��: 
	// ע    ��: (���뷽ʽ��[��ֵ/8.]*[180./4096.]=��)
	//---------------------------------------------------------
	double KRdaData::DecodeElv(unsigned short code)
	{
		double fValue = 0.0f;
		fValue = ((double)code/8.0)*(180.0/4096.0);
		return fValue;
	}

	//=====================================================
	// ��̬���� - beg
	//---------------
	//static
	bool KRdaData::IsRdaData(LPCTSTR lpszFile)
	{
		return false;
	}
	//---------------
	// ��̬���� - end
	//=====================================================

//--------------------------------------------------------
}}// namespace Wsr98d::Rda - end
//--------------------------------------------------------


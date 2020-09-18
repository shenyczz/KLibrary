#include "StdAfx.h"
#include "KWsr98Data.h"
#include "KWsr98DataOperator.h"

#include "KRdaData.h"
#include "KRpgData.h"

namespace Wsr98d
{

	KWsr98Data::KWsr98Data(void)
	{
		m_eRadarType = eRadarType_Wsr98d_Sa;

		m_iThreshold_min = 0;
		m_iThreshold_max = 255;

		_tcscpy(m_szUnit,_T(""));

		m_pRangeBinList = NULL;
		m_pRangeBinPerRadialList = NULL;
	}

	KWsr98Data::~KWsr98Data(void)
	{
		_delete(m_pRangeBinList);
		_delete(m_pRangeBinPerRadialList);
	}

	// ��ն���(�����ص��麯��)
	void KWsr98Data::Empty()
	{
		if(m_pRangeBinList)
			m_pRangeBinList->Clear();

		// ���������ø���ͬ������
		KRadarData::Empty();
	}

	// װ������
	BOOL KWsr98Data::LoadData(LPCTSTR lpszFile, LPVOID lpvData)
	{
		//���ɵ���Empty(),Empty()����ֻ���������һ������

		// ���������ø���ͬ������
		return KRadarData::LoadData(lpszFile,lpvData);
	}

	// ȡ��ÿ��������
	double KWsr98Data::GetPixelPerKm()
	{
		//ÿ����������
		double dPixelPerKm;

		int iWidthImage = this->GetImageWidth();

		//ͼ����512*512Ϊ�ο�
		dPixelPerKm = 1.0 * iWidthImage / 512;	//ÿ����������

		return dPixelPerKm;
	}

	// ��ʼ����ɫ��
	// �״��Ʒʹ��16����ɫ
	void KWsr98Data::InitColorTable(int iProductCode)
	{
		// �ȵ��ø���ͬ������
		KRadarData::InitColorTable(iProductCode);

		switch (iProductCode)
		{
		//������������ɫ���ձ�
		case 19:	case 20:	case 33:	case 37:	case 38:
		case 39:	case 110:
			m_ColorTable[ 0] = RGB(0,0,0);			//������ɫ
			m_ColorTable[ 1] = RGB(0,172,164);		
			m_ColorTable[ 2] = RGB(192,192,254);		
			m_ColorTable[ 3] = RGB(122,114,238);		
			m_ColorTable[ 4] = RGB(30,30,208);		
			m_ColorTable[ 5] = RGB(166,252,168);		
			m_ColorTable[ 6] = RGB(0,234,0);			
			m_ColorTable[ 7] = RGB(16,146,26);		
			m_ColorTable[ 8] = RGB(252,244,100);		
			m_ColorTable[ 9] = RGB(200,200,2);		
			m_ColorTable[10] = RGB(140,140,0);		
			m_ColorTable[11] = RGB(254,172,172);		
			m_ColorTable[12] = RGB(254,100,92);		
			m_ColorTable[13] = RGB(238,2,48);		
			m_ColorTable[14] = RGB(212,142,254);		
			m_ColorTable[15] = RGB(170,36,250);		
			break;

		//�����ٶ���ɫ���ձ�
		case 27:	case 56:	case 113:
			m_ColorTable[ 0] = RGB(0,0,0);			//������ɫ
			m_ColorTable[ 1] = RGB(126,224,254);		
			m_ColorTable[ 2] = RGB(0,224,254);		
			m_ColorTable[ 3] = RGB(0,176,176);		
			m_ColorTable[ 4] = RGB(0,254,0);			
			m_ColorTable[ 5] = RGB(0,196,0);			
			m_ColorTable[ 6] = RGB(0,128,0);			
			m_ColorTable[ 7] = RGB(254,254,254);		
			m_ColorTable[ 8] = RGB(252,252,252);		
			m_ColorTable[ 9] = RGB(254,0,0);			
			m_ColorTable[10] = RGB(254,88,88);		
			m_ColorTable[11] = RGB(254,176,176);		
			m_ColorTable[12] = RGB(254,124,0);		
			m_ColorTable[13] = RGB(254,210,0);		
			m_ColorTable[14] = RGB(254,254,0);		
			m_ColorTable[15] = RGB(124,0,124);		
			break;

		//�����׿���ɫ���ձ�
		case 30:	case 115:
			m_ColorTable[ 0] = RGB(0,0,0);			//������ɫ
			m_ColorTable[ 1] = RGB(224,224,224);		
			m_ColorTable[ 2] = RGB(124,224,224);		
			m_ColorTable[ 3] = RGB(0,224,224);		
			m_ColorTable[ 4] = RGB(0,176,176);		
			m_ColorTable[ 5] = RGB(0,1254,254);		
			m_ColorTable[ 6] = RGB(0,196,0);			
			m_ColorTable[ 7] = RGB(0,128,0);			
			m_ColorTable[ 8] = RGB(254,254,0);		
			m_ColorTable[ 9] = RGB(254,210,0);		
			m_ColorTable[10] = RGB(254,124,0);		
			m_ColorTable[11] = RGB(254,176,176);		
			m_ColorTable[12] = RGB(254,88,88);		
			m_ColorTable[13] = RGB(254,0,0);			
			m_ColorTable[14] = RGB(254,254,254);		
			m_ColorTable[15] = RGB(124,0,124);		
			break;

		//��ˮ��ɫ���ձ�
		case 31:	case 78:	case 79:	case 80:
			m_ColorTable[ 0] = RGB(0,0,0);			//������ɫ
			m_ColorTable[ 1] = RGB(170,170,170);		
			m_ColorTable[ 2] = RGB(118,118,118);		
			m_ColorTable[ 3] = RGB(0,254,254);		
			m_ColorTable[ 4] = RGB(0,174,174);		
			m_ColorTable[ 5] = RGB(0,254,0);			
			m_ColorTable[ 6] = RGB(0,142,0);			
			m_ColorTable[ 7] = RGB(254,0,254);		
			m_ColorTable[ 8] = RGB(174,50,174);		
			m_ColorTable[ 9] = RGB(0,0,254);			
			m_ColorTable[10] = RGB(50,0,150);		
			m_ColorTable[11] = RGB(254,254,0);		
			m_ColorTable[12] = RGB(254,170,0);		
			m_ColorTable[13] = RGB(254,0,0);			
			m_ColorTable[14] = RGB(174,0,0);			
			m_ColorTable[15] = RGB(254,254,254);		
			break;

		//�߶���ɫ���ձ�
		case 41:	case 42:
			m_ColorTable[ 0] = RGB(0,0,0);			//������ɫ	
			m_ColorTable[ 1] = RGB(80,80,80);			
			m_ColorTable[ 2] = RGB(118,118,118);			
			m_ColorTable[ 3] = RGB(0,224,254);			
			m_ColorTable[ 4] = RGB(0,176,254);			
			m_ColorTable[ 5] = RGB(0,144,204);				
			m_ColorTable[ 6] = RGB(50,0,150);				
			m_ColorTable[ 7] = RGB(0,250,144);			
			m_ColorTable[ 8] = RGB(0,186,0);				
			m_ColorTable[ 9] = RGB(0,238,0);					
			m_ColorTable[10] = RGB(254,190,0);				
			m_ColorTable[11] = RGB(254,254,0);				
			m_ColorTable[12] = RGB(174,0,0);				
			m_ColorTable[13] = RGB(254,0,0);					
			m_ColorTable[14] = RGB(254,254,254);					
			m_ColorTable[15] = RGB(230,0,254);			
			break;

		//�繳
		case 48:
			m_ColorTable[ 0] = RGB(0,0,0);			//������ɫ
			m_ColorTable[ 1] = RGB(0,254,0);		
			m_ColorTable[ 2] = RGB(254,254,0);		
			m_ColorTable[ 3] = RGB(254,0,0);		
			m_ColorTable[ 4] = RGB(0,238,254);		
			m_ColorTable[ 5] = RGB(254,112,254);		
			m_ColorTable[ 6] = RGB(170,254,153);	//����1	
			m_ColorTable[ 7] = RGB(192,192,192);	//����2		
			m_ColorTable[ 8] = RGB(0,0,0);		
			m_ColorTable[ 9] = RGB(0,0,0);
			m_ColorTable[10] = RGB(0,0,0);
			m_ColorTable[11] = RGB(0,0,0);
			m_ColorTable[12] = RGB(0,0,0);
			m_ColorTable[13] = RGB(0,0,0);
			m_ColorTable[14] = RGB(0,0,0);
			m_ColorTable[15] = RGB(0,0,0);
			break;

		//��Ƭͼ��ɫ���ձ�
		case 53:
			m_ColorTable[ 0] = RGB(0,0,0);			//������ɫ
			m_ColorTable[ 1] = RGB(254,170,170);		
			m_ColorTable[ 2] = RGB(200,112,112);		
			m_ColorTable[ 3] = RGB(0,186,0);		
			m_ColorTable[ 4] = RGB(254,112,112);		
			m_ColorTable[ 5] = RGB(218,0,0);			
			m_ColorTable[ 6] = RGB(0,0,254);			
			m_ColorTable[ 7] = RGB(254,254,254);		
			m_ColorTable[ 8] = RGB(254,254,112);		
			m_ColorTable[ 9] = RGB(208,208,96);			
			m_ColorTable[10] = RGB(254,96,96);		
			m_ColorTable[11] = RGB(218,0,0);		
			m_ColorTable[12] = RGB(174,0,0);		
			m_ColorTable[13] = RGB(0,0,254);			
			m_ColorTable[14] = RGB(254,254,254);			
			m_ColorTable[15] = RGB(230,0,254);	
			break;

		//��ֱҺ̬ˮ������ɫ���ձ�
		case 57:
			m_ColorTable[ 0] = RGB(0,0,0);			//������ɫ
			m_ColorTable[ 1] = RGB(156,156,156);		
			m_ColorTable[ 2] = RGB(118,118,118);		
			m_ColorTable[ 3] = RGB(250,170,170);		
			m_ColorTable[ 4] = RGB(238,140,140);		
			m_ColorTable[ 5] = RGB(200,112,112);			
			m_ColorTable[ 6] = RGB(0,250,144);			
			m_ColorTable[ 7] = RGB(0,186,0);		
			m_ColorTable[ 8] = RGB(254,254,112);		
			m_ColorTable[ 9] = RGB(208,208,96);			
			m_ColorTable[10] = RGB(254,96,96);		
			m_ColorTable[11] = RGB(218,0,0);		
			m_ColorTable[12] = RGB(174,0,0);		
			m_ColorTable[13] = RGB(0,0,254);			
			m_ColorTable[14] = RGB(254,254,254);			
			m_ColorTable[15] = RGB(230,0,254);	
			break;

		case 67:	//APR67
			m_ColorTable[ 0] = RGB(0,0,0);			//������ɫ
			m_ColorTable[ 1] = RGB(118,118,118);		
			m_ColorTable[ 2] = RGB(156,156,156);		
			m_ColorTable[ 3] = RGB(0,186,0);		
			m_ColorTable[ 4] = RGB(254,0,0);		
			m_ColorTable[ 5] = RGB(208,112,0);			
			m_ColorTable[ 6] = RGB(254,254,0);			
			m_ColorTable[ 7] = RGB(118,0,124);		
			m_ColorTable[ 8] = RGB(0,0,0);		
			m_ColorTable[ 9] = RGB(0,0,0);
			m_ColorTable[10] = RGB(0,0,0);
			m_ColorTable[11] = RGB(0,0,0);
			m_ColorTable[12] = RGB(0,0,0);
			m_ColorTable[13] = RGB(0,0,0);
			m_ColorTable[14] = RGB(0,0,0);
			m_ColorTable[15] = RGB(0,0,0);
			break;

		//VAD
		case 84:
			m_ColorTable[ 0] = RGB(0,0,0);			//������ɫ
			m_ColorTable[ 1] = RGB(118,118,144);		
			m_ColorTable[ 2] = RGB(254,170,170);		
			m_ColorTable[ 3] = RGB(200,112,112);		
			m_ColorTable[ 4] = RGB(0,186,0);		
			m_ColorTable[ 5] = RGB(254,254,112);			
			m_ColorTable[ 6] = RGB(218,0,0);			
			m_ColorTable[ 7] = RGB(0,0,254);		
			m_ColorTable[ 8] = RGB(192,192,192);		
			m_ColorTable[ 9] = RGB(192,192,192);
			m_ColorTable[10] = RGB(254,128,64);
			m_ColorTable[11] = RGB(0,0,0);
			m_ColorTable[12] = RGB(0,0,0);
			m_ColorTable[13] = RGB(0,0,0);
			m_ColorTable[14] = RGB(0,0,0);
			m_ColorTable[15] = RGB(0,0,0);
			break;


		//�б���ɫ���ձ�
		case 87:
		case 88:
			m_ColorTable[ 0] = RGB(0,0,0);			//������ɫ
			m_ColorTable[ 1] = RGB(118,118,118);		
			m_ColorTable[ 2] = RGB(170,170,170);		
			m_ColorTable[ 3] = RGB(50,0,150);		
			m_ColorTable[ 4] = RGB(0,128,254);		
			m_ColorTable[ 5] = RGB(0,254,254);			
			m_ColorTable[ 6] = RGB(0,142,0);			
			m_ColorTable[ 7] = RGB(0,186,0);		
			m_ColorTable[ 8] = RGB(0,254,0);		
			m_ColorTable[ 9] = RGB(200,100,0);			
			m_ColorTable[10] = RGB(254,170,0);		
			m_ColorTable[11] = RGB(254,254,0);		
			m_ColorTable[12] = RGB(174,0,0);		
			m_ColorTable[13] = RGB(254,0,0);			
			m_ColorTable[14] = RGB(254,124,124);			
			m_ColorTable[15] = RGB(150,0,150);	
			break;

		//1. ������������ɫ���ձ�
		default:
			m_ColorTable[ 0] = RGB(0,0,0);			//������ɫ
			m_ColorTable[ 1] = RGB(0,172,164);		
			m_ColorTable[ 2] = RGB(192,192,254);		
			m_ColorTable[ 3] = RGB(122,114,238);		
			m_ColorTable[ 4] = RGB(30,30,208);		
			m_ColorTable[ 5] = RGB(166,252,168);		
			m_ColorTable[ 6] = RGB(0,234,0);			
			m_ColorTable[ 7] = RGB(16,146,26);		
			m_ColorTable[ 8] = RGB(252,244,100);		
			m_ColorTable[ 9] = RGB(200,200,2);		
			m_ColorTable[10] = RGB(140,140,0);		
			m_ColorTable[11] = RGB(254,172,172);		
			m_ColorTable[12] = RGB(254,100,92);		
			m_ColorTable[13] = RGB(238,2,48);		
			m_ColorTable[14] = RGB(212,142,254);		
			m_ColorTable[15] = RGB(170,36,250);		
			break;
		}

	}

	// ������ݼ�����ֵ(256������,һ��ʹ��16������)
	void KWsr98Data::MakeDataLevelThreshold(int iProductCode)
	{
		int i;

		for(i=0;i<256;i++)
		{
			m_DataLevelThreshold[i].wDataLevel = 0;
			m_DataLevelThreshold[i].dThreshold = 0;					// ���ݼ�������ֵ
			_tcscpy(m_DataLevelThreshold[i].szLegendText,_T(""));	// ͼ���ı�
		}


		// 256ɫ����
		if(iProductCode==32|| iProductCode==81)	// || iProducrCode==33 
		{
			// ��δ����
			//MakeDataLevelThreshold256(iProducrCode);
			//return;
		}

		union	// ���ݼ��� + ��ֵ
		{
			WORD wDataLevelThreshold;
			BYTE cDataLevelThreshold[2];
		};

		BYTE cDataLevel;	// ���ݼ���
		BYTE cThreshold;	// ��ֵ

		// ����
		int sign = 1;

		for(i=0;i<16;i++)
		{
			TCHAR* pLegendText = m_DataLevelThreshold[i].szLegendText;
			_tcscpy(pLegendText,_T(""));

			wDataLevelThreshold	= PDBPtr()->iDataLevelThreshold[i];

			cDataLevel	= cDataLevelThreshold[1];	// ���ݼ���(���ֽ�)
			cThreshold	= cDataLevelThreshold[0];	// ��ֵ(���ֽ�)

			// ���� ���ݼ��� + ��ֵ
			m_DataLevelThreshold[i].wDataLevel = cDataLevel;
			m_DataLevelThreshold[i].dThreshold = cThreshold;

			if(cDataLevel & 0x80)	// the MSB set to 1 -> ����
			{
				if(cDataLevel & 0x08)	_tcscat(pLegendText,_T(">"));	// bit 4
				if(cDataLevel & 0x04)	_tcscat(pLegendText,_T("<"));	// bit 5
				if(cDataLevel & 0x02)	_tcscat(pLegendText,_T("+"));	// bit 6
				if(cDataLevel & 0x01)	_tcscat(pLegendText,_T("-"));	// bit 7

				if(cThreshold		== 0)	_tcscat(pLegendText,_T(" "));
				else if(cThreshold	== 1)	_tcscat(pLegendText,_T("TH"));
				else if(cThreshold	== 2)	_tcscat(pLegendText,_T("ND"));
				else if(cThreshold	== 3)	_tcscat(pLegendText,_T("RF"));
			}
			else	// the MSB set to 0
			{
				if(cDataLevel & 0x08)	{ sign = 1; _tcscat(pLegendText,_T(">")); }	// bit 4
				if(cDataLevel & 0x04)	{ sign = 1; _tcscat(pLegendText,_T("<")); }	// bit 5
				if(cDataLevel & 0x02)	{ sign = 1; _tcscat(pLegendText,_T("")); }	// bit 6 "+"
				if(cDataLevel & 0x01)	{ sign =-1; _tcscat(pLegendText,_T("")); }	// bit 7 "-"

				if(cDataLevel & 0x20)		m_DataLevelThreshold[i].dThreshold = 0.05 * cThreshold * sign;	// bit 2 scaled by 20
				else if(cDataLevel & 0x10)	m_DataLevelThreshold[i].dThreshold = 0.10 * cThreshold * sign;	// bit 3 scaled by 10
				else						m_DataLevelThreshold[i].dThreshold = 1.00 * cThreshold * sign;	// scaled by 1

				// ���ݲ�Ʒ���������ֵ
				switch(iProductCode)
				{
				case 27:	case 30:	case 48:	case 56:
				case 113:	case 115:
					//ktn->mps: *1852.0/3600.0
					m_DataLevelThreshold[i].dThreshold = m_DataLevelThreshold[i].dThreshold * 1852.0f / 3600.0f;
					break;

				case 31:	case 78:	case 79:	case 80:
					//inch -> mm	1inch = 25.4mm
					m_DataLevelThreshold[i].dThreshold = m_DataLevelThreshold[i].dThreshold * 25.4f;
					break;

				case 41:	case 42:
					//feet - km: 1feet=0.305m
					m_DataLevelThreshold[i].dThreshold = m_DataLevelThreshold[i].dThreshold * 0.305f;
					break;
				}

				// ͼ���ı�
				TCHAR szTemp[16];
				_stprintf(szTemp,_T("%.2f"),m_DataLevelThreshold[i].dThreshold);
				_tcscat(pLegendText,szTemp);
			}

		}// for(i)
	}

	//�����״�վID�����״�վ����
	LPCTSTR KWsr98Data::GetRadarStationName(int id)
	{
		switch(id)
		{
		///-------------------------------------------
		// ���� = 6
		case 370:
			_tcscpy(m_szStationName,_T("����"));
			break;
		case 371:
			_tcscpy(m_szStationName,_T("֣��"));
			break;
		case 377:
			_tcscpy(m_szStationName, _T("����"));
			break;
		case 379:
			_tcscpy(m_szStationName, _T("����"));
			break;
		case 393:
			_tcscpy(m_szStationName,_T("���"));
			break;
		case 396:
			_tcscpy(m_szStationName,_T("פ���"));
			break;
		case 398:
			_tcscpy(m_szStationName,_T("����Ͽ"));
			break;
		///-------------------------------------------
		// ����
		case 558:
			_tcscpy(m_szStationName,_T("����"));
			break;
		///-------------------------------------------
		// ����
		case 773:
			_tcscpy(m_szStationName,_T("����"));
			break;
		///-------------------------------------------
		default:
			_tcscpy(m_szStationName,_T("Unknown"));
			break;
		}
		//---------------------------
		return m_szStationName;
	}

	// ȡ�ò�Ʒ����
	int KWsr98Data::GetProductCode()
	{
		return m_oPdb.iProductCode; 
	}

	// ȡ�ò�Ʒ���ݼ���
	int KWsr98Data::GetProductDataLevel(int iProductCode)
	{
		int iDataLevel = 0;

		switch(iProductCode)
		{
		case 16:
		case 17:	
		case 18:	
			iDataLevel = 8;
			break;

		case 19:
		case 20:
		case 21:
			iDataLevel = 16;
			break;

		case 22:
		case 23:
		case 24:
			iDataLevel = 8;
			break;

		case 25:
		case 26:
		case 27:
			iDataLevel = 16;
			break;

		case 28:
		case 29:
			iDataLevel = 8;
			break;

		case 30:
		case 31:
		case 32:
		case 33:
			iDataLevel = 16;
			break;

		case 34:
		case 35:
		case 36:
			iDataLevel = 8;
			break;

		case 37:
		case 38:
		case 39:
			iDataLevel = 16;
			break;

		case 40:
			iDataLevel = 0;
			break;

		case 41:
			iDataLevel = 16;
			break;

		case 42:
			iDataLevel = 12;
			break;

		case 43:
		case 44:
			iDataLevel = 16;
			break;

		case 45:
			iDataLevel = 8;
			break;

		case 46:
			iDataLevel = 16;
			break;

		case 47:
			iDataLevel = 0;
			break;

		case 48:
			iDataLevel = 6;
			break;

		case 49:
		case 50:
		case 51:
			iDataLevel = 16;
			break;

		case 52:
		case 53:
			iDataLevel = 8;
			break;

		case 54:	//No Used
			iDataLevel = 0;
			break;

		case 55:
		case 56:
		case 57:
			iDataLevel = 16;
			break;

		case 58:
		case 59:
		case 60:
		case 61:
		case 62:
			iDataLevel = 0;
			break;

		case 63:
		case 64:
		case 65:
		case 66:
		case 67:
		case 68:
		case 69:
		case 70:
		case 71:
		case 72:
			iDataLevel = 8;
			break;

		case 73:
			iDataLevel = 0;
			break;

		case 74:
			iDataLevel = 9;
			break;

		case 75:
		case 76:	//Reserved for internal PUP use
		case 77:
			iDataLevel = 0;
			break;

		case 78:
		case 79:
		case 80:
			iDataLevel = 16;
			break;

		case 81:
			iDataLevel = 8;	//256?
			break;

		case 82:
			iDataLevel = 0;
			break;

		case 83:
			iDataLevel = 9;
			break;

		case 84:
		case 85:
		case 86:
			iDataLevel = 8;
			break;

		case 87:
			iDataLevel = 16;
			break;

		case 88:
			iDataLevel = 6;
			break;

		case 89:
		case 90:
			iDataLevel = 8;
			break;
	
		case 91-109:
			break;

		case 110:
		case 111:
		case 112:
		case 113:
		case 114:
		case 115:
			iDataLevel = 16;
			break;

		default:
			iDataLevel = 0;
			break;
		}

		return iDataLevel;
	}

	// ȡ�ò�Ʒ����
	double KWsr98Data::GetProductElevation(int iProductCode)
	{
		double dElevation(0);
		//-------------------
		switch(iProductCode)
		{
		case 16:	case 17:	case 18:	case 19:	case 20:
		case 21:	case 22:	case 23:	case 24:	case 25:
		case 26:	case 27:	case 28:	case 29:	case 30:
		case 43:	case 44:	case 45:	case 46:	case 49:
		case 55:	case 56:	case 87:	case 88:
		case 110:	case 113:	case 115:
			dElevation = 0.1 * PDBPtr()->iPD03;
			break;

		case 53:
			//??
			break;

		case 84:
			dElevation = 0.1 * PDBPtr()->iPD06;
			break;

		default:
			dElevation = 0.0;
			break;
		}
		//-------------------
		return dElevation;
	}

	// ȡ�ò�Ʒ������Ϣ
	ProductNameInfo* KWsr98Data::GetProductNameInfo(int iProductCode)
	{
		memset(&m_ProductNameInfo,0,sizeof(ProductNameInfo));

		switch(iProductCode)
		{
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
			_stprintf(m_ProductNameInfo.szCnName,_T("����������"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Base Reflectivity"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("R"),iProductCode);
			break;

		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
			_stprintf(m_ProductNameInfo.szCnName,_T("�����ٶ�"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Base Velocity"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("V"),iProductCode);
			break;

		case 28:
		case 29:
		case 30:
			_stprintf(m_ProductNameInfo.szCnName,_T("�����׿�"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Base Spectrum Width"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("SW"),iProductCode);
			break;

		case 31:
			_stprintf(m_ProductNameInfo.szCnName,_T("�û���ѡ��ˮ"));
			_stprintf(m_ProductNameInfo.szEnName,_T("User Selectable Storm Total Precipitation"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("USP"),iProductCode);
			break;

		case 32:
			_stprintf(m_ProductNameInfo.szCnName,_T("���ֻ��ɨ�跴����"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Digital Hybrid Scan Reflectivity"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("DHR"),iProductCode);
			break;

		case 33:
			_stprintf(m_ProductNameInfo.szCnName,_T("���ɨ�跴����"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Hybrid Scan Reflectivity"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("HSR"),iProductCode);
			break;

		case 34:
			_stprintf(m_ProductNameInfo.szCnName,_T("�Ӳ��˲�����"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Clutter Filter Control"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("CFC"),iProductCode);
			break;

		case 35:
		case 36:
		case 37:
		case 38:
			_stprintf(m_ProductNameInfo.szCnName,_T("��Ϸ�����"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Composite Reflectivity"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("CR"),iProductCode);
			break;

		case 39:
		case 40:
			_stprintf(m_ProductNameInfo.szCnName,_T("��Ϸ����ʵ�ֵ��"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Composite Reflectivity Contour"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("CRC"),iProductCode);
			break;

		case 41:
			_stprintf(m_ProductNameInfo.szCnName,_T("�ز�����"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Echo Tops"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("ET"),iProductCode);
			break;

		case 42:
			_stprintf(m_ProductNameInfo.szCnName,_T("�ز����ߵ�ֵ��"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Echo Tops Contour"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("ETC"),iProductCode);
			break;

		case 43:
			_stprintf(m_ProductNameInfo.szCnName,_T("ǿ�������� - ������"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Severe Weather(Reflectivity)"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("SWR"),iProductCode);
			break;

		case 44:
			_stprintf(m_ProductNameInfo.szCnName,_T("ǿ��������(�ٶ�)"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Severe Weather(Velocity)"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("SWV"),iProductCode);
			break;

		case 45:
			_stprintf(m_ProductNameInfo.szCnName,_T("ǿ��������(�׿�)"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Severe Weather(Spectrum Width)"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("SWW"),iProductCode);
			break;

		case 46:
			_stprintf(m_ProductNameInfo.szCnName,_T("ǿ��������(�б�)"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Severe Weather(Shear)"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("SWS"),iProductCode);
			break;

		case 47:
			_stprintf(m_ProductNameInfo.szCnName,_T("ǿ��������(����)"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Severe Weather(Probability)"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("SWP"),iProductCode);
			break;

		case 48:
			_stprintf(m_ProductNameInfo.szCnName,_T("VAD ������"));
			_stprintf(m_ProductNameInfo.szEnName,_T("VAD Wind Profile"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("VWP"),iProductCode);
			break;

		case 49:
			_stprintf(m_ProductNameInfo.szCnName,_T("����׾�"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Combined Moment"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("CM"),iProductCode);
			break;
		  
		case 50:
			_stprintf(m_ProductNameInfo.szCnName,_T("��ֱ����(������)"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Cross Section(Reflectivity)"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("RCS"),iProductCode);
			break;

		case 51:
			_stprintf(m_ProductNameInfo.szCnName,_T("��ֱ����(�ٶ�)"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Cross Section(Velocity)"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("VCS"),iProductCode);
			break;

		case 52:
			_stprintf(m_ProductNameInfo.szCnName,_T("��ֱ����(�׿�)"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Cross Section(Spectrum Width)"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("SCS"),iProductCode);
			break;

		case 53:
			_stprintf(m_ProductNameInfo.szCnName,_T("���ز���"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Weak Echo Region"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("WER"),iProductCode);
			break;

		case 54:
			_stprintf(m_ProductNameInfo.szCnName,_T("����"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Reserved"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("RSV"),iProductCode);
			break;

		case 55:
			_stprintf(m_ProductNameInfo.szCnName,_T("�ֲ���Է籩�����ٶ�"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Storm Relative Mean Radial Velocity - Region"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("SRR"),iProductCode);
			break;

		case 56:
			_stprintf(m_ProductNameInfo.szCnName,_T("�籩��Ծ����ٶ�"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Storm Relative Mean Radial Velocity - Map"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("SRM"),iProductCode);
			break;

		case 57:
			_stprintf(m_ProductNameInfo.szCnName,_T("��ֱ�ۻ�Һ̬ˮ����"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Vertically Integrated Liquid"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("VIL"),iProductCode);
			break;

		case 58:
			_stprintf(m_ProductNameInfo.szCnName,_T("�籩·����Ϣ"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Storm Tarcking Information"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("STI"),iProductCode);
			break;

		case 59:
			_stprintf(m_ProductNameInfo.szCnName,_T("����ָ��"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Hail Index"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("HI"),iProductCode);
			break;

		case 60:
			_stprintf(m_ProductNameInfo.szCnName,_T("�г߶�����"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Mesocyclone"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("M"),iProductCode);
			break;

		case 61:
			_stprintf(m_ProductNameInfo.szCnName,_T("������������"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Tornado Vortex Signature"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("TVS"),iProductCode);
			break;

		case 62:
			_stprintf(m_ProductNameInfo.szCnName,_T("�籩�ṹ"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Storm Struct"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("SS"),iProductCode);
			break;

		case 63:
		case 64:
			_stprintf(m_ProductNameInfo.szCnName,_T("�ֲ���Ϸ�����ƽ��ֵ - L%d"),iProductCode-62);
			_stprintf(m_ProductNameInfo.szEnName,_T("Layer Composite Reflectivity Average - L%d"),iProductCode-62);
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("LRA"),iProductCode);
			break;

		case 65:
		case 66:
			_stprintf(m_ProductNameInfo.szCnName,_T("�ֲ���Ϸ��������ֵ - L%d"),iProductCode-64);
			_stprintf(m_ProductNameInfo.szEnName,_T("Layer Composite Reflectivity Maximum - L%d"),iProductCode-64);
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("LRM"),iProductCode);
			break;

		case 67:
			_stprintf(m_ProductNameInfo.szCnName,_T("�ֲ���Ϸ�����"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Layer Composite Reflectivity"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("APR"),iProductCode);
			break;

		case 68:
		case 69:
		case 70:
		case 71:
		case 72:
			_stprintf(m_ProductNameInfo.szCnName,_T("����"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Spare"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("SPARE"),iProductCode);
			break;

		case 73:
			_stprintf(m_ProductNameInfo.szCnName,_T("�û�������Ϣ"));
			_stprintf(m_ProductNameInfo.szEnName,_T("User Alert Message"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("UAM"),iProductCode);
			break;

		case 74:
			_stprintf(m_ProductNameInfo.szCnName,_T("�״������Ϣ"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Radar Code Message"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("RCM"),iProductCode);
			break;

		case 75:
			_stprintf(m_ProductNameInfo.szCnName,_T("�����ı���Ϣ"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Free Text Message"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("FTM"),iProductCode);
			break;

		case 76:
			_stprintf(m_ProductNameInfo.szCnName,_T("�������ڲ�PUPʹ��"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Reserved for internal PUP use"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("RFIU"),iProductCode);
			break;

		case 77:
			_stprintf(m_ProductNameInfo.szCnName,_T("PUP�����ı���Ϣ"));
			_stprintf(m_ProductNameInfo.szEnName,_T("PUP Free Text Message"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("PFTM"),iProductCode);
			break;

		case 78:
			_stprintf(m_ProductNameInfo.szCnName,_T("1Сʱ�ۻ���ˮ"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Surface Rainfall Accumulation(1 Hour)"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("OHP"),iProductCode);
			break;

		case 79:
			_stprintf(m_ProductNameInfo.szCnName,_T("3Сʱ�ۻ���ˮ"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Surface Rainfall Accumulation(3 Hour)"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("THP"),iProductCode);
			break;

		case 80:
			_stprintf(m_ProductNameInfo.szCnName,_T("�籩�ܽ�ˮ"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Storm Total Rainfall Accumulation"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("STP"),iProductCode);
			break;

		case 81:
			_stprintf(m_ProductNameInfo.szCnName,_T("1Сʱ���ֽ�ˮ����"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Hopurly Digital Precipitation Array"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("???"),iProductCode);
			break;

		case 82:
			_stprintf(m_ProductNameInfo.szCnName,_T("���併ˮ����"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Supplement Precipitation Data"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("SPD"),iProductCode);
			break;

		case 83:
			_stprintf(m_ProductNameInfo.szCnName,_T("�Ǳ༭�״������Ϣ"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Radar Code Message - Unedited"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("URCM"),iProductCode);
			break;

		case 84:
			_stprintf(m_ProductNameInfo.szCnName,_T("�ٶȷ�λ��ʾ"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Velocity Azimuth Display"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("VAD"),iProductCode);
			break;

		case 85:
			_stprintf(m_ProductNameInfo.szCnName,_T("�����ʴ�ֱ����"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Cross Section Reflectivity"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("RCS"),iProductCode);
			break;

		case 86:
			_stprintf(m_ProductNameInfo.szCnName,_T("�ٶȴ�ֱ����"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Cross Section Velocity"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("VCS"),iProductCode);
			break;

		case 87:
			_stprintf(m_ProductNameInfo.szCnName,_T("����б�"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Combined Shear"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("CS"),iProductCode);
			break;

		case 88:
			_stprintf(m_ProductNameInfo.szCnName,_T("����б��ֵ��"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Combined Shear Contour"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("CSC"),iProductCode);
			break;

		case 89:
			_stprintf(m_ProductNameInfo.szCnName,_T("�ֲ㸴�Ϸ�����ƽ��ֵ-L3"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Layer Composite Reflectivity - Layer 3 Average"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("LRA"),iProductCode);
			break;

		case 90:
			_stprintf(m_ProductNameInfo.szCnName,_T("�ֲ㸴�Ϸ��������ֵ-L3"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Layer Composite Reflectivity - Layer 3  Maximun"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("LRM"),iProductCode);
			break;

		case 91:
		case 92:
			_stprintf(m_ProductNameInfo.szCnName,_T("�������ڲ�PUP��RPGʹ��"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Reserved for internal PUP and RPG use"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("RFIPR"),iProductCode);
			break;

		case 93:
		case 94:
		case 95:
		case 96:
		case 97:
		case 98:
		case 99:
		case 100:
		case 101:
		case 102:
		case 103:
		case 104:
		case 105:
		case 106:
		case 107:
		case 108:
		case 109:
			_stprintf(m_ProductNameInfo.szCnName,_T("�����������Ĳ�Ʒʹ��"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Reserved for Future Product use"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("RFRU"),iProductCode);
			break;

		case 110:
			_stprintf(m_ProductNameInfo.szCnName,_T("������CAPPI"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Reflectivity CAPPI"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("CAR"),iProductCode);
			break;

		case 111:
		case 112:
		case 113:
			_stprintf(m_ProductNameInfo.szCnName,_T("�ٶ�CAPPI"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Velocity CAPPI"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("CAV"),iProductCode);
			break;

		case 114:
		case 115:
			_stprintf(m_ProductNameInfo.szCnName,_T("�׿�CAPPI"));
			_stprintf(m_ProductNameInfo.szEnName,_T("Spectrum Width CAPPI"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("CAS"),iProductCode);
			break;

		case 116:
		case 117:
		case 118:
		case 119:
			_stprintf(m_ProductNameInfo.szCnName,_T("����"));
			_stprintf(m_ProductNameInfo.szEnName,_T("NOUSE"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("NUS"),iProductCode);
			break;

		default:
			_stprintf(m_ProductNameInfo.szCnName,_T("����"));
			_stprintf(m_ProductNameInfo.szEnName,_T("NOUSE"));
			_stprintf(m_ProductNameInfo.szContraction,_T("%s%d"),_T("NUS"),iProductCode);
			break;
		}

		return &m_ProductNameInfo;
	}

	// ȡ�ø��Ƿ�Χ(km)
	double KWsr98Data::GetProductRange(int iProductCode)
	{
		RadarType eRadarType = GetRadarType();
		Assert(eRadarType!=eRadarType_None);

		double dRange = 0;
		//-----------------------------
		switch(iProductCode)
		{
		case 16:
			dRange = (eRadarType==eRadarType_Wsr98d_Sa || eRadarType==eRadarType_Wsr98d_Sb) ? 230 : -1;
			break;

		case 17:
		case 18:
			dRange = 460;
			break;

		case 19:
			dRange = 230;
			break;

		case 20:
		case 21:
			dRange = 460;
			break;

		case 22:
			dRange = 60;
			break;

		case 23:
			dRange = 115;
			break;

		case 24:
			dRange = 230;
			break;

		case 25:
			dRange = 60;
			break;

		case 26:
			dRange = 115;
			break;

		case 27:
			dRange = 230;
			break;

		case 28:
			dRange = 60;
			break;

		case 29:
			dRange = 115;
			break;

		case 30:
		case 31:
		case 32:
		case 33:
		case 34:
		case 35:
			dRange = 230;
			break;

		case 36:
			dRange = 460;
			break;

		case 37:
			dRange = 230;
			break;

		case 38:
			dRange = 460;
			break;

		case 39:
			dRange = 230;
			break;

		case 40:
			dRange = 460;
			break;

		case 41:
		case 42:
			dRange = 230;
			break;

		case 43:
		case 44:
		case 45:
		case 46:
			dRange = 50;
			break;

		case 47:
			dRange = 230;
			break;

		case 48:
			//dRange = N/A
			break;

		case 49:
			dRange = 25;
			break;

		case 50:
		case 51:	
		case 52:
			dRange = 230;
			break;

		case 53:
			dRange = 50;
			break;

		case 54:
			//dRange = Reserved
			break;

		case 55:
			dRange = 50;
			break;

		case 56:
		case 57:
			dRange = 230;
			break;

		case 58:
			dRange = 230;
			break;

		case 59:
		case 60:
		case 61:
			dRange = 230;
			break;

		case 62:
			dRange = 460;
			break;

		case 63:
		case 64:
		case 65:
		case 66:
		case 67:
			dRange = 230;
			break;

		case 68:
		case 69:
		case 70:
		case 71:
		case 72:
			dRange = 150;
			break;

		case 73:
			//dRange = N/A;
			break;

		case 74:
			dRange = 460;
			break;

		case 75:
			//dRange = N/A;
			break;

		case 76:
			//dRange = Reserved fot interal PUP user;
			break;

		case 77:
			//dRange = N/A;
			break;

		case 78:
		case 79:
		case 80:
		case 81:
			dRange = 230;
			break;

		case 82:
			//dRange = N/A;
			break;

		case 83:
			dRange = 460;
			break;

		case 84:
			//dRange = N/A;
			break;

		case 85:
		case 86:
			dRange = 230;
			break;

		case 87:
		case 88:
			dRange = 115;
			break;

		case 89:
		case 90:
			dRange = 230;
			break;

		//case 91 - 109:	//No Used

		case 110:
			dRange = 150;
			break;
		
		case 111:
			dRange = 75;
			break;

		case 112:
		case 113:
			dRange = 150;
			break;

		case 114:
			dRange = 75;
			break;

		case 115:
			dRange = 150;
			break;

		default:
			dRange = 0;
			break;
		}
		//-----------------------------
		return dRange;
	}

	// ȡ�ò�Ʒ����(һ�����ص��ʾ��km)
	double KWsr98Data::GetProductResolution(int iProductCode)
	{
		double dResolution(0);

		switch(iProductCode)
		{
		case 22:	case 25:	case 28:	case 44:	case 45:
			dResolution = 0.25;
			break;

		case 23:	case 26:	case 29:	case 46:	case 49:
		case 55:
			dResolution = 0.5;
			break;

		case 16:	case 19:	case 24:	case 27:	case 30:
		case 32:	case 35:	case 37:	case 39:	case 43:
		case 50:	case 51:	case 52:	case 53:	case 56:
		case 85:	case 86:	case 87:	case 110:	case 113:	case 115:
			dResolution = 1.0;
			break;

		case 17:	case 20:	case 31:	case 33:	case 78:
		case 79:	case 80:
			if(iProductCode==33) dResolution = 1.0;	// ˵����2,��ʾ��1
			else dResolution = 2.0;
			break;

		case 18:	case 21:	case 36:	case 38:	case 40:
		case 41:	case 42:	case 47:	case 57:	case 63:
		case 64:	case 65:	case 66:	case 67:	case 68:
		case 69:	case 70:	case 71:	case 72:	case 89:
		case 90:
			dResolution = 4.0;
			break;
		}

		return dResolution;
	}

	// ȡ�ò�Ʒ����
	ProductType KWsr98Data::GetProductType(int iProductCode)
	{
		ProductType eProductType = ProductType_None;
		//----------------------------------------------------
		switch(iProductCode)
		{
		case 16:	//R (Base Reflectivity)
		case 17:	//R (Base Reflectivity)
		case 18:	//R (Base Reflectivity)
		case 19:	//R (Base Reflectivity)
		case 20:	//R (Base Reflectivity)
		case 21:	//R (Base Reflectivity)
		case 22:	//V (Base Velocity)
		case 23:	//V (Base Velocity)
		case 24:	//V (Base Velocity)
		case 25:	//V (Base Velocity)
		case 26:	//V (Base Velocity)
		case 27:	//V (Base Velocity)
		case 28:	//SW (Base Spectrum Width)
		case 29:	//SW (Base Spectrum Width)
		case 30:	//SW (Base Spectrum Width)
		case 31:	//USP (User Selectable Storm Total Precipitation)
		case 32:	//DHR (Digital Hybrid Scan Reflectivity)
		case 33:	//HSR (Hybrid Scan Reflectivity)
		case 34:	//CFC (Clutter Filter Control)
		case 43:	//SWR (Severe Weather Reflectivity)
		case 44:	//SWV (Severe Weather Velocity)
		case 45:	//SWW (Severe Weather Spectrum Width)
		case 46:	//SWS (Severe Weather Shear)
		case 55:	//SRR (Storm Relative Mean Radial Velocity - Region)
		case 56:	//SRM (Storm Relative Mean Radial Velocity - Map)
		case 78:	//OHP (Surface Rainfall Accumulation - 1 hr) [PSB + TAB]
		case 79:	//THP (Surface Rainfall Accumulation - 3 hr) [PSB + TAB]
		case 80:	//STP (Storm Total Rainfall Accumulation) [PSB + TAB]
		case 110:	//CAR (CAPPI Reflectivity) [PSB]
		case 111:	//CAV (CAPPI Velocity) [PSB]
		case 112:	//CAV (CAPPI Velocity) [PSB]
		case 113:	//CAV (CAPPI Velocity) [PSB]
		case 114:	//CAS (CAPPI Spectrum Width) [PSB]
		case 115:	//CAS (CAPPI Spectrum Width) [PSB]
			eProductType = ProductType_RadialImage;				//1. Radial Image
			break;

		case 35:	//CR (Composite Reflectivity)
		case 36:	//CR (Composite Reflectivity)
		case 37:	//CR (Composite Reflectivity)
		case 38:	//CR (Composite Reflectivity)
		case 41:	//ET (Echo Tops)
		case 49:	//CM (Combined Moment)
		case 57:	//VIL (Vertically Integrated Liquid)
		case 63:	//LRA (Layer Composite Reflectivity - Layer 1 Average)
		case 64:	//LRA (Layer Composite Reflectivity - Layer 2 Average)
		case 65:	//LRM (Layer Composite Reflectivity - Layer 1 Maximum)
		case 66:	//LRM (Layer Composite Reflectivity - Layer 2 Maximum)
		case 67:	//APR
		case 68:	//??
		case 69:	//??
		case 70:	//??
		case 71:	//??
		case 72:	//??
		case 81:	//??? (Hourly Digital Precipitation Array) []
		case 83:	//??? (Radar Code Message - UnEdited) []
		case 87:	//CS (Combined Shear) [PSB + TAB]
		case 89:	//LRA (Layer Composite Reflectivity - Layer 3 Average) []
		case 90:	//LRM (Layer Composite Reflectivity - Layer 3 Maximum) []
			eProductType = ProductType_RasterImage;					//2. Raster Image
			break;

		case 39:	//CRC (Composite Reflectivity Contour)
		case 40:	//CRC (Composite Reflectivity Contour)
		case 42:	//ETC (Echo Tops Contour)
		case 88:	//CSC (Combined Shear Contour) [PSB + TAB]
			eProductType = ProductType_LinkedContourVectorImage;	//3. Linked Contour Vector Image
			break;

		case 47:	//SWP (Severe Weather Probability)
		case 58:	//STI (Storm Tracking Infomation)
		case 59:	//HI (Hail Index)
		case 60:	//M (Mesocyclone) [GAB + TAB]
		case 61:	//TVS (Tornado Vortex Signature) [GAB + TAB]
			eProductType = ProductType_GeographicAlphaImage;		//4. Geographic Alpha Image
			break;

		case 48:	//VWP (VAD Wind Profile)
		case 84:	//VAD (Velocity Azimuth Display) [PSB]
			eProductType = ProductType_NonGeographicAlphaImage;		//5. NonGeographic Alpha Image
			break;

		case 62:	//SS (Storm Struct) [PSB + GAB]
		case 82:	//SPD (Supplement Precipitation Data) [PSB]
			eProductType = ProductType_AlphanumericImage;			//6. Alphanumeric Image
			break;

		case 50:	//RCS (Cross Section Reflectivity)
		case 51:	//VCS (Cross Section Velocity)
		case 52:	//SCS (Cross Section Spectrum Width)
		case 85:	//RCS (Cross Section Reflectivity)
		case 86:	//VCS (Cross Section Velocity)
			eProductType = ProductType_RasterImageSection;			//7. Section Image
			break;

		case 53:	//WER (Weak Echo Region)
			eProductType = ProductType_RasterImageSlice;			//8. Slice Image
			break;

		case 54:	//no used
		case 73:	//UAM (User Alert Message)
		case 74:	//RCM (Radar Coded Message)
		case 75:	//FTM (Free Text Message)
		case 76:	//no used
		case 77:	//PTM (PUP Free Text Message)
		//case 91-100:
			eProductType = ProductType_None;						//?? Image(δ��)
			break;
		}
		//----------------------------------------------------
		return eProductType;
	}

	// ȡ�ò�Ʒ���ݵ�λ
	LPCTSTR KWsr98Data::GetProductUnit(int iProductCode)
	{
		_tcscpy(m_szUnit,_T(""));

		switch(iProductCode)
		{
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
			_tcscpy(m_szUnit,_T("dBZ"));
			break;
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
			_tcscpy(m_szUnit,_T("mps"));
			break;
		case 31:
			_tcscpy(m_szUnit,_T("mm"));
			break;
		case 32:
		case 33:
			_tcscpy(m_szUnit,_T("dBZ"));
			break;
		case 34:
			_tcscpy(m_szUnit,_T("Unknown"));
			break;
		case 35:
		case 36:
		case 37:
		case 38:
		case 39:
		case 40:
			_tcscpy(m_szUnit,_T("dBZ"));
			break;
		case 41:
		case 42:
			_tcscpy(m_szUnit,_T("km"));
			break;
		case 43:
			_tcscpy(m_szUnit,_T("dBZ"));
			break;
		case 44:
		case 45:
			_tcscpy(m_szUnit,_T("mps"));
			break;
		case 46:
			_tcscpy(m_szUnit,_T("Unknown"));
			break;
		case 47:
			_tcscpy(m_szUnit,_T("%"));
			break;
		case 48:
			_tcscpy(m_szUnit,_T("mps RMS"));
			break;
		case 49:
			_tcscpy(m_szUnit,_T("Unknown"));
			break;
		case 50:
			_tcscpy(m_szUnit,_T("dBZ"));
			break;
		case 51:
		case 52:
			_tcscpy(m_szUnit,_T("mps"));
			break;
		case 53:
			_tcscpy(m_szUnit,_T("Unknown"));
			break;
		case 54:
			_tcscpy(m_szUnit,_T("Unknown"));
			break;
		case 55:
		case 56:
			_tcscpy(m_szUnit,_T("mps"));
			break;
		case 57:
			_tcscpy(m_szUnit,_T("kg/m**2"));
			break;
		case 58:
		case 59:
		case 60:
		case 61:
		case 62:
			_tcscpy(m_szUnit,_T("---"));	// �޵�λ
			break;
		case 63:
		case 64:
		case 65:
		case 66:
			_tcscpy(m_szUnit,_T("dBZ"));
			break;
		case 67:
			_tcscpy(m_szUnit,_T("CM E2/3/s"));
			break;
		case 68:
		case 69:
		case 70:
		case 71:
		case 72:
			_tcscpy(m_szUnit,_T("Spare"));
			break;
		case 73:
		case 74:
		case 75:
		case 76:
		case 77:
			_tcscpy(m_szUnit,_T("---"));
			break;
		case 78:
		case 79:
		case 80:
		case 81:
		case 82:
			_tcscpy(m_szUnit,_T("mm"));
			break;
		case 83:
			_tcscpy(m_szUnit,_T("---"));
			break;
		case 84:
			_tcscpy(m_szUnit,_T("mps"));
			break;
		case 85:
			_tcscpy(m_szUnit,_T("dBZ"));
			break;
		case 86:
			_tcscpy(m_szUnit,_T("mps"));
			break;
		case 87:
		case 88:
			_tcscpy(m_szUnit,_T("E-4/s"));
			break;
		case 89:
		case 90:
			_tcscpy(m_szUnit,_T("dBZ"));
			break;

		case 110:
			_tcscpy(m_szUnit,_T("dBZ"));
			break;
		case 111:
		case 112:
		case 113:
		case 114:
		case 115:
			_tcscpy(m_szUnit,_T("mps"));
			break;
		}

		return m_szUnit;
	}

	// ����/��ȡ��ֵ
	void KWsr98Data::SetThreshold(int iThreshold_min,int iThreshold_max)
	{
		m_iThreshold_min = iThreshold_min;
		m_iThreshold_max = iThreshold_max;
	}
	void KWsr98Data::GetThreshold(int* iThreshold_min,int* iThreshold_max)
	{
		*iThreshold_min = m_iThreshold_min;
		*iThreshold_max = m_iThreshold_max;
	}

	// ����ͼ����Ϣ
	void KWsr98Data::MakeLegendInfo(int iProductCode)
	{
		// 1. ���ݲ�Ʒ�����ʼ����ɫ��
		InitColorTable(iProductCode);

		// 2. ���ݲ�Ʒ��Ʒ�����������PDB��ʼ�����ݼ�����ֵ
		MakeDataLevelThreshold(iProductCode);

		// 3. ����ͼ��
		for(int i=0;i<16;i++)
		{
			m_Legend[i].dwColor	= m_ColorTable[i];								// ��ɫ��
			m_Legend[i].dValue	= m_DataLevelThreshold[i].dThreshold;			// ���ݼ�������ֵ
			_tcscpy(m_Legend[i].szText,m_DataLevelThreshold[i].szLegendText);	// ͼ���ı�
		}

		/*
		// 1. ������Ϣͷ��MHB�Ͳ�Ʒ������PDB�����ɫ���ձ�
		//_StuffColorTable(iProductCode);

		//2. ������Ϣͷ��MHB�Ͳ�Ʒ������PDB������ݼ�����ֵ
		FillDataLevelThreshold(iProductCode);

		//3. ����ͼ��
		*/
	}

	// ��д�״���Ϣ
	void KWsr98Data::FillRadarInfo(RadarInfo* pRadarInfo)
	{
		// ���ø���ͬ������
		KRadarData::FillRadarInfo(pRadarInfo);

		MHB* mhbptr = this->MHBPtr();
		PDB* pdbptr = this->PDBPtr();

		// ��Ʒ����
		int iProductCode = this->GetProductCode();

		pRadarInfo->id = mhbptr->iSID;											// �״�վID
		_tcscpy(pRadarInfo->szStationName,this->GetRadarStationName(pRadarInfo->id));	// �״�վ����
		pRadarInfo->dLon = 0.001f * pdbptr->lLon;								// �״�վ����
		pRadarInfo->dLat = 0.001f * pdbptr->lLat;								// �״�վγ��
		pRadarInfo->dAlt = 0.305f * pdbptr->iAlt;								// �״�վ����(kft=>km)

		this->GetDateTime(pRadarInfo->wYear	// ��
			,pRadarInfo->wMonth			// ��
			,pRadarInfo->wDay			// ��
			,pRadarInfo->wHour			// ʱ
			,pRadarInfo->wMinute		// ��
			,pRadarInfo->wSecond		// ��
			);

		pRadarInfo->iOPMode = pdbptr->iOPMode;					// ����ģʽ
		if(pRadarInfo->iOPMode==0)
		{
			_tcscpy(pRadarInfo->szCnOPModeName,_T("ά��"));			// ����ģʽ��������
			_tcscpy(pRadarInfo->szEnOPModeName,_T("Maintenance"));	// ����ģʽӢ������
		}
		else if(pRadarInfo->iOPMode==1)
		{
			_tcscpy(pRadarInfo->szCnOPModeName,_T("���"));			// ����ģʽ��������
			_tcscpy(pRadarInfo->szEnOPModeName,_T("Clear Air"));	// ����ģʽӢ������
		}
		else if(pRadarInfo->iOPMode==2)
		{
			_tcscpy(pRadarInfo->szCnOPModeName,_T("��ˮ"));							// ����ģʽ��������
			_tcscpy(pRadarInfo->szEnOPModeName,_T("Precipitation/Severe Weather"));	// ����ģʽӢ������
		}

		pRadarInfo->iVCP = pdbptr->iVCP;	// ��ɨģʽ
		//cntr?? ���ı�ʾ 0km 0deg

		pRadarInfo->iElevationNumber = pdbptr->iElevationNumber;	//���Ǻ�
		pRadarInfo->dElevation = (float)this->GetProductElevation(iProductCode);	//����(��)

		pRadarInfo->iProductCode = iProductCode;	// ��Ʒ����(16-131)

		_tcscpy(pRadarInfo->szUnit,this->GetProductUnit(iProductCode));				// ������λ

		ProductNameInfo* pProductNameInfo = this->GetProductNameInfo(iProductCode);
		_tcscpy(pRadarInfo->szEnName,pProductNameInfo->szEnName);				// ��ƷӢ������
		_tcscpy(pRadarInfo->szCnName,pProductNameInfo->szCnName);				// ��Ʒ��������
		_tcscpy(pRadarInfo->szContraction,pProductNameInfo->szContraction);		// ��Ʒ��д

		pRadarInfo->dRange		= this->GetProductRange(iProductCode);		// ���Ƿ�Χ(km)
		pRadarInfo->dResolution	= this->GetProductResolution(iProductCode);	// �ֱ���(km)

		pRadarInfo->dPolarRange = 50.0 * pRadarInfo->dRange / 230;	// ����Ȧ����(km);

		// ����������Ϣ
		MakeProductSpecialInfo(pRadarInfo);

		// ������˵�������� - ����(��д)_yyyymmdd.hhMMss.Elevation.ProductCode.SID
		_stprintf(pRadarInfo->szComment,_T("%s(%s)_%4d%02d%02d.%02d%02d%02d.%02d.%d")
			,pRadarInfo->szCnName			// ����
			,pRadarInfo->szContraction		// ��д
			,pRadarInfo->wYear				// ��
			,pRadarInfo->wMonth				// ��
			,pRadarInfo->wDay				// ��
			,pRadarInfo->wHour				// ʱ
			,pRadarInfo->wMinute			// ��
			,pRadarInfo->wSecond			// ��
			,pRadarInfo->iElevationNumber	// ���Ǻ�
			,pRadarInfo->id					// �״�վID
			);

		return;
	}

	// ���������Ϣ
	void KWsr98Data::FillDataInfo(LPVOID lpvData)
	{
		KDataInfo* pDataInfo = (KDataInfo*)lpvData;

		//�״���Ϣ
		RadarInfo* pRadarInfo = this->GetRadarInfo();

		// ���ľ�γ��
		double dCenterX = pRadarInfo->dLon;
		double dCenterY = pRadarInfo->dLat;

		// ���Ƿ�Χ(km)
		double dRange = pRadarInfo->dRange;

		// 1.1����=0.008979��γ�� 1.1/111.37
		double dDegreePerKm = 0.0098769866211726676842955912723355;

		// ÿ�����ش���ľ�γ��
		double dDegreePerPixel = dDegreePerKm * dRange / 230;

		//������½Ǿ�γ��
		double x0 = dCenterX - dDegreePerPixel * 512 / 2;
		double y0 = dCenterY - dDegreePerPixel * 512 / 2;

		//��γ�ȼ��
		double dXInterval = dDegreePerPixel;
		double dYInterval = dDegreePerPixel;

		// 
		pDataInfo->xNum = 512;
		pDataInfo->xMin = dCenterX - dDegreePerPixel * 512 / 2;
		pDataInfo->xMax = dCenterX + dDegreePerPixel * 512 / 2;
		pDataInfo->xInterval = dXInterval;

		pDataInfo->yNum = 512;
		pDataInfo->yMin = dCenterY - dDegreePerPixel * 512 / 2;
		pDataInfo->yMax = dCenterY + dDegreePerPixel * 512 / 2;
		pDataInfo->yInterval = dYInterval;

		pDataInfo->wYear = pRadarInfo->wYear;
		pDataInfo->wMonth = pRadarInfo->wMonth;
		pDataInfo->wDay = pRadarInfo->wDay;
		pDataInfo->wHour = pRadarInfo->wHour;
		pDataInfo->wMinute = pRadarInfo->wMinute;
		pDataInfo->wSecond = pRadarInfo->wSecond;

		//_tcscpy(dataInfo.szComment,pRadarInfo->szComment);
		pDataInfo->SetComment(pRadarInfo->szComment);

		//END
	}


	//����״��Ʒ��Ϣ5��������Ϣ�ֶ�
	void KWsr98Data::MakeProductSpecialInfo(RadarInfo *pRadarInfo)
	{
		int i;

		KWsr98Data* pRadarData = this;
		KWsr98DataOperator* pRadarDataOperator = (KWsr98DataOperator*)this->GetRadarDataOperator();

		MHB* mhbptr = this->MHBPtr();
		PDB* pdbptr = this->PDBPtr();

		int iYearBeg,iMonthBeg,iDayBeg;
		int iYearEnd,iMonthEnd,iDayEnd;
		int iTimeBeg,iTimeEnd;

		// ������Ϣ(���ڱ�ע��Ϣ��) => TCHAR szSpecialInfo[10][64]
		for(i=0;i<10;i++)
		{
			_tcscpy(pRadarInfo->szSpecialInfo[i],_T(""));
		}

		int iProductCode = pRadarData->GetProductCode();
		switch(iProductCode)
		{
		case 16:
		case 17:
		case 18:
		case 19:	// R
		case 20:	// R
		case 21:
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("����: % 6.1f Deg"),(double)pdbptr->iPD03*0.1);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("���: % 6.1f dBZ"),(double)pdbptr->iPD04);
			break;

		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:	// V
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("����: % 6.1f Deg"),(double)pdbptr->iPD03*0.1);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("��С: % 6.1f mps"),(double)pdbptr->iPD04*0.514);
			_stprintf(pRadarInfo->szSpecialInfo[2],_T("���: % 6.1f mps"),(double)pdbptr->iPD05*0.514);
			break;

		case 28:
		case 29:
		case 30:	// SW
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("����: % 6.1f Deg"),(double)pdbptr->iPD03*0.1);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("���: % 6.1f mps"),(double)pdbptr->iPD04*0.514);
			break;

		case 31:	//USP
			pRadarDataOperator->JulianDate2GMTDate(pdbptr->iPD05,&iYearBeg,&iMonthBeg,&iDayBeg);
			iTimeBeg = pdbptr->iPD06;

			pRadarDataOperator->JulianDate2GMTDate(pdbptr->iPD07,&iYearEnd,&iMonthEnd,&iDayEnd);
			iTimeEnd = pdbptr->iPD08;

			_stprintf(pRadarInfo->szSpecialInfo[0],_T("��ʱ: % 6d Сʱ"),pdbptr->iPD02);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("���: % 6.1f mm"),(double)pdbptr->iPD04 * 0.1 * 25.4);
			_stprintf(pRadarInfo->szSpecialInfo[2],_T("��ʼ: %4d-%02d-%02d %02d:%02d")
				,iYearBeg
				,iMonthBeg
				,iDayBeg
				,(iTimeBeg/60)<24 ? (iTimeBeg/60) : (iTimeBeg/60-24)
				,iTimeBeg%60
				);
			_stprintf(pRadarInfo->szSpecialInfo[3],_T("����: %4d-%02d-%02d %02d:%02d")
				,iYearEnd
				,iMonthEnd
				,iDayEnd
				,(iTimeEnd/60)<24 ? (iTimeEnd/60) : (iTimeEnd/60-24)
				,iTimeEnd%60
				);
			_stprintf(pRadarInfo->szSpecialInfo[4],_T("ƫ��: % 6.1f %%"),(double)pdbptr->iPD09*0.01);
			_stprintf(pRadarInfo->szSpecialInfo[5],_T("����: % 6.1f %%"),(double)pdbptr->iPD10*0.01);
			break;

		case 32:
			break;

		case 33:	// HSR
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("����: % 6.1f Deg"),(double)pdbptr->iPD03*0.1);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("���: % 6.1f dBZ"),(double)pdbptr->iPD04);
			break;

		case 34:
			break;

		case 35:
		case 36:
		case 37:	// CR
		case 38:	// CR
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("���: % 6.1f dBZ"),(double)pdbptr->iPD04);
			break;

		case 39:	// CRC
		case 40:	// CRC
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("���: % 6.1f dBZ"),(double)pdbptr->iPD04);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("���: % 6.1f dBZ"),(double)pdbptr->iPD10);
			break;

		case 41:	// ET
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("���: % 6.1f km"),(double)pdbptr->iPD04 * 1000 * 0.305 / 1000);
			break;

		case 42:	// ETC
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("���: % 6.1f km"),(double)pdbptr->iPD04 * 1000 * 0.305 / 1000);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("���: % 6.1f km"),(double)pdbptr->iPD10 * 0.305 / 1000);
			break;

		case 43:	// SWR
			break;

		case 47:	// SWP
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("���: % 6.1f %%"),(double)pdbptr->iPD04);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("�ߴ�: % 6.1f km"),(double)pdbptr->iPD05 * 0.1 * 1.852);
			break;

		case 48:	// VWP
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("���: % 6.1f mps"),(double)pdbptr->iPD04 * 0.514);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("����: % 6.1f Deg"),(double)pdbptr->iPD05);
			_stprintf(pRadarInfo->szSpecialInfo[2],_T("�߶�: % 6.1f km"),(double)pdbptr->iPD06 * 0.01 * 0.305);
			break;

		case 50:	// RCS
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("�� 1: % 6.1fdeg %5.1fkm"),(double)pdbptr->iPD04 * 0.1,(double)pdbptr->iPD05 * 0.1 * 1.852);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("�� 2: % 6.1fdeg %5.1fkm"),(double)pdbptr->iPD06 * 0.1,(double)pdbptr->iPD07 * 0.1 * 1.852);

			union
			{
				char	c[2];
				short	i;
			}un1;

			un1.i = (sfx_int16)pdbptr->iPD08;

			union
			{
				sfx_uint16	ii[2];
				sfx_uint32	ll;
			}un2;

			un2.ii[0] = pdbptr->iPD08;
			un2.ii[1] = pdbptr->iPD09;
			un2.ll &= 0xfffff;

			_stprintf(pRadarInfo->szSpecialInfo[2],_T("����: % 6.1fE%s%d dB"),(double)un2.ll,(un1.i<0 ? "-" : "+"),(int)(un1.c[0]&0x7));
			break;

		case 53:	// WER
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("��λ: % 6.1f Deg"),(double)pdbptr->iPD01 * 0.1);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("��Χ: % 6.1f km"),(double)pdbptr->iPD02 * 0.1 * 1.852);
			_stprintf(pRadarInfo->szSpecialInfo[2],_T("���: % 6.1f dBZ"),(double)pdbptr->iPD04);
			break;

		case 56:	// SRM
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("����: % 6.1f Deg"),(double)pdbptr->iPD03 * 0.1);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("��С: % 6.1f mps"),(double)pdbptr->iPD04 * 0.514);
			_stprintf(pRadarInfo->szSpecialInfo[2],_T("���: % 6.1f mps"),(double)pdbptr->iPD05 * 0.514);
			_stprintf(pRadarInfo->szSpecialInfo[3],_T("����: % 6.1f mps"),(double)pdbptr->iPD08 * 0.1 * 0.514);
			_stprintf(pRadarInfo->szSpecialInfo[4],_T("����: % 6.1f Deg"),(double)pdbptr->iPD09 * 0.1);
			//_stprintf(pRadarInfo->szSpecialInfo[5],_T(" MSF: % 6d"),(signed long)pdbptr->iPD06);
			break;

		case 57:	// VIL
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("���: % 6.1f kg/m**2"),(double)pdbptr->iPD04);
			break;

		case 67:	// ???
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("���: % 6.1f CM E2/3/s"),(double)pdbptr->iPD04 * 0.1);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("�ײ�: % 6.1f km"),(double)pdbptr->iPD05 * 0.305);
			_stprintf(pRadarInfo->szSpecialInfo[2],_T("����: % 6.1f km"),(double)pdbptr->iPD06 * 0.305);
			break;

		case 78:	// OHP
		case 79:	// THP
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("���: % 6.1f mm"),(double)pdbptr->iPD04 * 0.1 * 25.4);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("ƫ��: % 6.1f %%"),(double)pdbptr->iPD05 * 0.01);
			_stprintf(pRadarInfo->szSpecialInfo[2],_T("����: % 6.1f %%"),(double)pdbptr->iPD06 * 0.01);

			pRadarDataOperator->JulianDate2GMTDate(pdbptr->iPD07,&iYearEnd,&iMonthEnd,&iDayEnd);
			iTimeEnd = pdbptr->iPD08;
			_stprintf(pRadarInfo->szSpecialInfo[3],_T("����: %4d-%02d-%02d %02d:%02d")
				,iYearEnd
				,iMonthEnd
				,iDayEnd
				,(iTimeEnd/60)<24 ? (iTimeEnd/60) : (iTimeEnd/60-24)
				,iTimeEnd%60
				);
			break;

		case 80:	// STP
			pRadarDataOperator->JulianDate2GMTDate(pdbptr->iPD05,&iYearBeg,&iMonthBeg,&iDayBeg);
			iTimeBeg = pdbptr->iPD06;

			pRadarDataOperator->JulianDate2GMTDate(pdbptr->iPD07,&iYearEnd,&iMonthEnd,&iDayEnd);
			iTimeEnd = pdbptr->iPD08;

			_stprintf(pRadarInfo->szSpecialInfo[0],_T("���: % 6.1f mm"),(double)pdbptr->iPD04 * 0.1 * 25.4);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("��ʼ: %4d-%02d-%02d %02d:%02d")
				,iYearBeg
				,iMonthBeg
				,iDayBeg
				,(iTimeBeg/60)<24 ? (iTimeBeg/60) : (iTimeBeg/60-24)
				,iTimeBeg%60
				);
			_stprintf(pRadarInfo->szSpecialInfo[2],_T("����: %4d-%02d-%02d %02d:%02d")
				,iYearEnd
				,iMonthEnd
				,iDayEnd
				,(iTimeEnd/60)<24 ? (iTimeEnd/60) : (iTimeEnd/60-24)
				,iTimeEnd%60
				);
			_stprintf(pRadarInfo->szSpecialInfo[3],_T("ƫ��: % 6.1f %%"),(double)pdbptr->iPD09 * 0.01);
			_stprintf(pRadarInfo->szSpecialInfo[4],_T("����: % 6.1f %%"),(double)pdbptr->iPD10 * 0.01);
			break;

		case 84:	// VAD
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("����: % 6.1f mps"),(double)pdbptr->iPD04 * 0.514);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("����: % 6.1f Deg"),(double)pdbptr->iPD05);
			_stprintf(pRadarInfo->szSpecialInfo[2],_T("�߶�: % 6.1f km"),(double)pdbptr->iPD03 * 1000 * 0.305 / 1000);
			_stprintf(pRadarInfo->szSpecialInfo[3],_T("����: % 6.1f Deg"),(double)(signed)pdbptr->iPD06 * 0.1);
			_stprintf(pRadarInfo->szSpecialInfo[4],_T("б��: % 6.1f km"),(double)pdbptr->iPD07 * 0.1 * 1.852);
			_stprintf(pRadarInfo->szSpecialInfo[5],_T("RMSE: % 6.1f mps"),(double)pdbptr->iPD08 * 0.514);
			break;

		case 87:	// CS
		case 88:	// CSC
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("����: % 6.1f Deg"),(double)pdbptr->iPD03 * 0.1);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("���: % 6.1f E-4/s"),(double)pdbptr->iPD04);
			_stprintf(pRadarInfo->szSpecialInfo[2],_T("��λ: % 6.1f Deg"),(double)pdbptr->iPD05 * 0.1);
			_stprintf(pRadarInfo->szSpecialInfo[3],_T("����: % 6.1f km"),(double)pdbptr->iPD06 * 0.1 * 1.852);
			_stprintf(pRadarInfo->szSpecialInfo[4],_T("����: % 6.1f km"),(double)pdbptr->iPD07 * 0.01 * 1.852);
			break;

		case 110:	// CAR
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("����: % 6.1f Deg"),(double)pdbptr->iPD03*0.1);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("���: % 6.1f dBZ"),(double)pdbptr->iPD04);
			break;

		case 113:	// CAV
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("����: % 6.1f Deg"),(double)pdbptr->iPD03*0.1);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("��С: % 6.1f mps"),(double)pdbptr->iPD05*0.514);
			_stprintf(pRadarInfo->szSpecialInfo[2],_T("���: % 6.1f mps"),(double)pdbptr->iPD04*0.514);
			break;

		case 115:	// CAS
			_stprintf(pRadarInfo->szSpecialInfo[0],_T("����: % 6.1f Deg"),(double)pdbptr->iPD03*0.1);
			_stprintf(pRadarInfo->szSpecialInfo[1],_T("���: % 6.1f mps"),(double)pdbptr->iPD04*0.514);
			break;
		}
	}

	// ��ɫת��ΪҪ��ֵ
	double KWsr98Data::Color2Value(COLORREF clr)
	{
		if(IsEmpty())
			return 0;

		KWsr98DataOperator* pWsr98DataOperator = (KWsr98DataOperator*)this->GetRadarDataOperator();
		return pWsr98DataOperator->Color2Value(clr);
	}

	// Ҫ��ֵת��Ϊ��ɫ
	COLORREF KWsr98Data::Value2Color(double value)
	{
		if(IsEmpty())
			return 0;

		KWsr98DataOperator* pWsr98DataOperator = (KWsr98DataOperator*)this->GetRadarDataOperator();
		return pWsr98DataOperator->Value2Color(value);
	}

	// Ҫ��ֵת��Ϊ��ɫ����
	int KWsr98Data::Value2ColorIndex(double value)
	{
		if(IsEmpty())
			return -1;

		KWsr98DataOperator* pWsr98DataOperator = (KWsr98DataOperator*)this->GetRadarDataOperator();
		return pWsr98DataOperator->Value2ColorIndex(value);
	}

	// ȡ�õ�λ
	LPCTSTR KWsr98Data::GetUnit()
	{
		return this->GetProductUnit(this->GetProductCode());
	}

	//=====================================================
	// ��̬���� - beg
	//---------------
	//static 
	bool KWsr98Data::IsWsr98Data(LPCTSTR lpszFile)
	{
		return KRdaData::IsRdaData(lpszFile) || KRpgData::IsRpgData(lpszFile);
	}
	//---------------
	// ��̬���� - end
	//=====================================================

//---------------------------------------------------------
}// namespace Wsr98d - end
//---------------------------------------------------------

/*
	case 16:
	case 17:
	case 18:
	case 19:

	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:

	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:

	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
		  
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:

	case 60:
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:

	case 70:
	case 71:
	case 72:
	case 73:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:

	case 80:
	case 81:
	case 82:
	case 83:
	case 84:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:

	case 90:
	case 91:
	case 92:
	case 93:
	case 94:
	case 95:
	case 96:
	case 97:
	case 98:
	case 99:

	case 100:
	case 101:
	case 102:
	case 103:
	case 104:
	case 105:
	case 106:
	case 107:
	case 108:
	case 109:

	case 110:
	case 111:
	case 112:
	case 113:
	case 114:
	case 115:
	case 116:
	case 117:
	case 118:
	case 119:
	*/	   

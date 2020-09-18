#include "StdAfx.h"
#include "KDataInfo.h"

#include "Axin.inl"

KDataInfo::KDataInfo(void)
{
	int i;

	wFileID = 0;			// �ļ���ʶ
	_tcscpy(this->szFileID,_T("�ı������ļ���ʶ"));	// �ı������ļ���ʶ
	wDataSource = 0;		// ������Դ
	wFormatCode = 0;		// ��ʽ����
	wProductCode = 0;		// ��Ʒ����
	wBytesOrder = 0;		// �ֽ���
	wDataType = 0;			// ��������
	wElementCode = 0;		// Ҫ�ش���
	wFileStorageType = 0;	// �ļ��洢����

	strComment = _T("");	// ע����Ϣ

	wYear = 1970;			// ��
	wMonth = 1;				// ��
	wDay = 1;				// ��
	wHour = 0;				// ʱ
	wMinute = 0;			// ��
	wSecond = 0;			// ��
	dwMillisecond  = 0;		// ����

	wYear2 = 1970;			// ��2
	wMonth2 = 1;			// ��2
	wDay2 = 1;				// ��2
	wHour2 = 0;				// ʱ2
	wMinute2 = 0;			// ��2
	wSecond2 = 0;			// ��2
	dwMillisecond2  = 0;	// ����2

	wTimePeriod = 0;		// ʱЧ

	// ������
	wLayerNums = 1;
	iCurrentLayerIndex = 0;
	for(i=0; i<1000; i++)
	{
		wLayerCode[i] = 0;
	}

	for(i=0; i<100; i++)
	{
		iAncillaryChannelCode[i] = 0;	// ����ͨ������
	}

	wProjectType = 0;		// ͶӰ����
	wPaletteType = 0;		// ��ɫ������

	wTotalNum = 0;			// �ܵ���ɢ������(��վ���� ������),Ҫ�����0
	wElementNum = 0;		// ��վ��ͼҪ�ظ���

	dwFlag = 0;				// վ�����Ʊ��

	wContourNums = 0;		// ��ֵ������
	wClipArea = 0;			// ��������� 0-
	for(i=0; i<100; i++)
	{
		dContourValue[i] = 0;
		xClip[i] = 0;
		yClip[i] = 0;
	}

	dBoldContourValue = 0;
	wContourPortion = 10;

	wSpecificDataNumbers = 0;						// �ض���������
	dwSpecificDataMin = AXIN_SPECIAL_DATA_BEG;		// �ض�������Сֵ
	dwSpecificDataMax = AXIN_SPECIAL_DATA_END;		// �ض��������ֵ


	zNum = 0;				// 

	dwInvalidData = -9999;	// ��Ч����
	dBaseValue = 0;			// ����ֵ
	dScale = 1.0;			// ����ϵ��

	// ͸����ɫ
	bHasTransparentColor = FALSE;
	dwTransparentColor = 0xFFFFFFFF;

	bHasComment = FALSE;			// �Ƿ���ע��
	bHasColorTable = FALSE;			// �Ƿ�����ɫ��

	m_dwDataOffset = 0;
	m_iDataStoreType = 0;

	return;
}

KDataInfo::~KDataInfo(void)
{
}

// �Ƿ���Ч����
BOOL KDataInfo::IsInvalidData(double value)
{
	return FALSE
		|| fabs(value - (LONG) InvalidData())			< 1.0e-12
		|| fabs(value - (LONG) SFX_INVALID_DATA)		< 1.0e-12
		|| fabs(value - (LONG) SFX_INVALID_DATA_FLOAT)	< 1.0e-12
		|| fabs(value - (LONG) SFX_INVALID_DATA_DOUBLE)	< 1.0e-12
		|| fabs(value - (DWORD)InvalidData())			< 1.0e-12
		|| fabs(value - (DWORD)SFX_INVALID_DATA)		< 1.0e-12
		|| fabs(value - (DWORD)SFX_INVALID_DATA_FLOAT)	< 1.0e-12
		|| fabs(value - (DWORD)SFX_INVALID_DATA_DOUBLE)	< 1.0e-12
		; 			  
}

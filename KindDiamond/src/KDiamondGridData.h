#pragma once

#include "KDiamondData.h"

class KIND_EXT_CLASS KDiamondGridData : public KDiamondData
{
public:
	KDiamondGridData(void);
	virtual ~KDiamondGridData(void);

public:
	// �����ֵ��
	virtual BOOL BuildContours();

public:
	// ȡ�õ�ֵ�߹�����
	KContourBuilder* GetContourBuilder() { return m_pContourBuilder; }

public:
	// ��γ������ת��Ϊ�������
	BOOL LonLat2xy(double lon,double lat,int& x,int& y);
	// �������ת��Ϊ��γ������
	BOOL xy2LonLat(int x,int y,double& lon,double& lat);

	double GetGridValue(int row,int col);
	BOOL SetGridValue(int row,int col,double value);

	double GetGridValue(double lon,double lat);
	BOOL SetGridValue(double lon,double lat,double value);

protected:
	// װ������ - ASC�ļ�
	virtual BOOL LoadData_txt(LPCTSTR lpszFile);

protected:
	// �������ݼ�Сֵ�ͼ���ֵ
	void LookupExtremum();

protected:
	// ��ֵ�߹�����
	KContourBuilder* m_pContourBuilder;
};


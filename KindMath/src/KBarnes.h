/******************************************************************************
 ** Header File: KBarnes.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KBarnes - ��ͨ�˲���
 ** Version:
 ** Function: ��ͨ�˲���
              ���GrADS��ͼ����Ķ��������������ļ�,Ҳ�������������������

 ** Author: ShenYongchen(shenyczz@163.com) [������]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

		1. KBarnes barnes;			//��������
		2. SetParam_Filter();		//�����˲�����
		3. SetParam_Source();		//����Դ�������Բ���
		5. SetData();				//����Դ����ָ��
		4. Prepare();				//׼��
		6. Transact();				//�˲�����

		7. GetBarnes(0/1);			//�����˲���ʼ��/�˲���

******************************************************************************/

#pragma once

class KIND_EXT_CLASS KBarnes 
{
public:
	KBarnes (void);
	virtual ~KBarnes (void);

protected:
	void Init();
	void Cleanup();

public:
	//���ò���
	void SetParam_Filter(float g, float c);
	void SetParam_Source(int nx, int ny, float x0, float y0, float xInterval, float yInterval);

	//��������
	void SetData(float* pData,int nx,int ny)	{SetData(&pData,nx,ny);}
	void SetData(float** ppData,int nx,int ny)	{m_ppData = ppData;m_nCols=nx;m_nRows=ny;}

	//Ϊ�˲�������׼��
	void Prepare();

	//�˲�����
	void Transact();

	//�����˲���ʼ��/�˲���(0/1)
	void GetBarnes(float** ppRetData,int* nRow,int* nCol,int flag);
	void GetBarnes(float* pRetData,int* nRow,int* nCol,int flag);

protected:
	//Ϊ�˲�������׼���ڴ�
	void PrepareMemory();	//��ʼ��

	//����ĳ����˲�ֵ
	float Calculate(float x0, float y0,float g,float c,float** ppData);

private:
	//������������
	float _distance(float x0,float y0,float x1,float y1);

	//�����˹Ȩ��
	float _getGauss(float rk,float g,float c);

protected:
	//---��ʼ������
	int			m_nRows;		// ԴY�������� (��)
	int			m_nCols;		// ԴX�������� (��)

	float		m_x0;			// Դ�����������½Ǿ���
	float		m_y0;			// Դ�����������½�γ��

	float		m_xInterval;	// Դ����������
	float		m_yInterval;	// Դγ��������

	float		m_g,m_c;		// �˲�����

	//---��������ڴ沢�����ͷ�֮
	float*		m_pX;
	float*		m_pY;

	float**		m_ppD;			// ���
	float**		m_ppF0;			// ��һ���˲����
	float**		m_ppF1;			// �ڶ����˲����

	//---��Ҫ�ⲿ����
	float**		m_ppData;		// ԭʼ����������ָ��(m_ny,m_nx)
};

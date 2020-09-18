/******************************************************************************
 ** Header File: KGridDensify.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KGridDensify
 ** Version:
 ** Function: ���ܹ����ı�����������

 ** Author: ShenYongchen(shenyczz@163.com) [������]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

		//1. ���������
		KGridDensify	gd;

		//2. ����Դ�������Բ���
		gd.SetInput(int nx0,int ny0,double x0,double y0,double xInterval0,double yInterval0,double* pData0);
			��
		gd.SetGridParam(double xmin,double ymin,double xmax,double ymax,double xInterval,double yInterval);

		//3. ���ü��ܲ���
		gd.SetDensifyParam(double xInterval,double yInterval);

		//5. ʵʩ
		gd.Transact();

		//6. ȡ�ü��ܺ�������������
		gd.GetOutput(int* nx,int* ny,double*& pData);

******************************************************************************/

#pragma once

class KIND_EXT_CLASS KGridDensify
{
public:
	KGridDensify(void);
	virtual ~KGridDensify(void);

protected:
	void Init();
	void Cleanup();

public:
	BOOL IsEmpty() {return m_pv1==NULL;}

public:
	void Empty();

public:
	//����Դ�������Բ���
	void SetInput(int nx,int ny,double x0,double y0,double xInterval,double yInterval,double* pData);
	void SetInput(double xmin,double ymin,double xmax,double ymax,double xInterval,double yInterval,double* pData);

	//���ü��ܲ���
	void SetDensifyParam(double xInterval,double yInterval);

	//׼��
	void Prepare();

	//ʵʩ
	void Transact();


	//ȡ�ü��ܺ�������������
	void GetOutput(int* nx,int* ny,double* x0,double* y0,double* xInterval,double* yInterval,double*& pData);

protected:
	//׼���ڴ�
	void PrepareMemory();

	//��ֵ
	double InterpolateOnePoint(double x,double y);

private:
	//����������
	double _distance(double x0, double y0, double x1, double y1);

protected:
	//---Դԭ�������Բ���
	int			m_nx0;				// �����X������(����)
	int			m_ny0;				// �����Y������(����)

	double		m_x0;				// �����X����ʼֵ
	double		m_y0;				// �����Y����ʼֵ

	double		m_xInterval0;		// �����X����
	double		m_yInterval0;		// �����Y����

	double*		m_pv0;				// �����Ҫ��ֵָ��(�ɵ����߷�����ͷ��ڴ�)

	//---���ܺ��������Բ���
	int			m_nx1;				// �����X������(����)
	int			m_ny1;				// �����Y������(����)

	double		m_xInterval1;		// �����X����
	double		m_yInterval1;		// �����Y����

	double*		m_pv1;				// �����Ҫ��ֵָ��(�ɱ��������ͷ��ڴ�)
};

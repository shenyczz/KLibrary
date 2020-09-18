/******************************************************************************
 ** Header File: KSmooth.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KSmooth
 ** Version:
 ** Function: �����������ݽ���ƽ����ƽ��������3��
              1. Akima��㷨ƽ��
              2. �ֶ����ζ���ʽƽ��
              3. ��������������ƽ��(û�п����ɹ�)

 ** Author: ShenYongchen(shenyczz@163.com) [������]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

		1. KSmooth smth;
		2. smth.Akima(double* x,double* y,int n);					//Akima��㷨ƽ��
 OR        smth.SubsectionPolynomial(double* x,double* y,int n);	//�ֶ����ζ���ʽƽ��
 OR        smth.Tension(double* x,double* y,int n,double sigma);	//��������������ƽ��(û�п����ɹ�)

		3. smth.GetOutput(double*& x,double*& y,int* n);			//ȡ��ƽ���������

******************************************************************************/

#pragma once

class KIND_EXT_CLASS KSmooth
{
public:
	KSmooth(void);
	virtual ~KSmooth(void);

protected:
	void Init();
	void Cleanup();

public:
	//��㷨(Akima��)
	int Akima(double* x,double* y,int n);

	//�ֶ����ζ���ʽ
	int SubsectionPolynomial(double* x,double* y,int n);

	//�������������������(waiting ...)
	int Tension(double *x, double *y,int n,double sigma);

public:
	//ȡ�����
	void GetOutput(double*& x,double*& y,int* n);

protected:
	//׼���ڴ�
	void PrepareMemory(int iSize=0);

private:
	virtual double angle(double x1, double y1, double x2, double y2);
	//virtual double angle(double x1, double y1, double x2, double y2);

	//virtual double distance(double x1,double y1,double x2,double y2);
	virtual double distance(double x1,double y1,double x2,double y2);

	//�ֶβ�ֵ
	void _DoSubsection(double x[],double y[],double alf[],double* xout,double *yout,int iPoint);

protected:
	int m_iNum;			//���߹⻬��ĵ���

	//���߹⻬�������,�ɱ��������ͷ��ڴ�
	double* m_px;		//
	double* m_py;		//
};

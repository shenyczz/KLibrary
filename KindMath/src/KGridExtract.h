/******************************************************************************
 ** Header File: KGridExtract.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KGridExtract
 ** Version:
 ** Function: ����������ȡָ�����������������

 ** Author: ShenYongchen(shenyczz@163.com) [������]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

		//1. ���������
		KGridExtract	ge;

		//2. ����Դ�������Բ���
		ge.SetInput(int& nx,int& ny,double& x0,double& y0,const double xInterval,const double yInterval,double*& pData);

		//3. ���ó�ȡ����
		ge.SetExtractParam(int nxStart,int nxCount,int nyStart,int nyCount);
		//����
		ge.SetExtractParam(double xmin,double ymin,double xmax,double ymax);

		//5. ʵʩ
		if(!ge.Transact())
		{
			//ע��:�ڷ���֮ǰ������Ҫ�ͷŷǱ��������ڴ�
			//     ���������ڴ�й¶
			return;
		}

		//6. ȡ����������������
		ge.GetOutput(int* nx,int* ny,double* x,double* y,double* xInterval,double* yInterval,double*& pData);

******************************************************************************/

#pragma once

class KIND_EXT_CLASS  KGridExtract
{
public:
	KGridExtract(void);
	virtual ~KGridExtract(void);

protected:
	void Init();
	void Cleanup();

public:
	BOOL IsEmpty() {return m_pv1==NULL;}

public:
	void Empty();

public:
	//����Դ�������Բ���
	void SetInput(int nx,int ny,double x0,double y0,const double xInterval,const double yInterval,double* pData);

	//���ó�ȡ����
	virtual void SetExtractParam(int nxStart,int nxCount,int nyStart,int nyCount);
	virtual void SetExtractParam(double xmin,double ymin,double xmax,double ymax);

	//׼��
	BOOL Prepare();

	//ʵʩ
	BOOL Transact();

	//ȡ����������������
	void GetOutput(int* nx,int* ny,double* x,double* y,double* xInterval,double* yInterval,double*& pData);

protected:
	//У����������
	BOOL VerifyInputData();

	//׼���ڴ�
	void PrepareMemory();

protected:
	//---Դ�������Բ���
	int			m_nx0;				// �����X������(����)
	int			m_ny0;				// �����Y������(����)

	double		m_x0;				// �����X����ʼֵ
	double		m_y0;				// �����Y����ʼֵ

	double		m_xInterval;		// �����X����
	double		m_yInterval;		// �����Y����

	double*		m_pv0;				// �����Ҫ��ֵָ��(�ɵ����߷�����ͷ��ڴ�)

	//---���������Բ���
	int			m_nxStart;			//x�����ȡ�Ŀ�ʼ������
	int			m_nyStart;			//y�����ȡ�Ŀ�ʼ������

	int			m_nxCount;			//x�����ȡ�ĸ����(����)
	int			m_nyCount;			//y�����ȡ�ĸ����(����)

	double		m_xStart;			// �����X����ʼֵ
	double		m_yStart;			// �����Y����ʼֵ

	double*		m_pv1;				// �����Ҫ��ֵָ��(�ɱ��������ͷ��ڴ�)
};

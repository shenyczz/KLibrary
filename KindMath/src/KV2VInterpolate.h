/******************************************************************************
 ** Header File: KV2VInterpolate.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KV2VInterpolate
 ** Version:
 ** Function: ��������������ٲ岹���������
			  1. �㷨��ʱ��ķ����Զ�����ɨ��뾶��
			  2. ��V2V�в������ڴ�,Ҳ���ͷ��ڴ�,�ڴ�����ɵ����߸���

 ** Author: ShenYongchen(shenyczz@163.com) [������]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

		//1. ������ֵ����
		KV2VInterpolate v2v;

		//2. ����Դ����
		v2v.SetInput(double* pxi,double* pyi,double* pvi,int ni);

		//3. ����Ŀ������
		v2v.SetOutput(double* pxo,double* pyo,double* pvo,int no);

		//4. ����ɨ��뾶(km)
		//   ���ɨ��뾶 =0,���Զ�����ɨ��뾶,,ʹɨ��뾶��������3�������9����
		//   ���ɨ��뾶!=0,��ɨ��뾶����,nVertexMin ������Ч
		v2v.SetRadius(double dRadius=0.0,int nVertexMin=VERTEX_NUM_MIN);

		//5. �Ƿ�ʵʩ�ϸ��ڲ�
		//v2v.SetStrict(BOOL bStrict=TRUE);

		//6. ʵʩ��ֵ
		v2v.Transact(iMethod);

******************************************************************************/

#pragma once


#define VERTEX_NUM_MIN	3	//��ֵʱ��Сʹ�ö�����

class KIND_EXT_CLASS KV2VInterpolate : public KProgressObject
{
public:
	KV2VInterpolate(void);
	virtual ~KV2VInterpolate(void);

//protected:
//	void Init();
//	void Cleanup();

public:
	//����Դ����
	virtual void SetInput(double* pxi,double* pyi,double* pvi,int ni);		//����Դ��ɢ������ָ��

	//�����������
	virtual void SetOutput(double* pxo,double* pyo,double* pvo,int no);	//���������ɢ������ָ��

	//����ɨ��뾶(km)
	virtual void SetRadius(double dRadius=0.0,int nVertexMin=VERTEX_NUM_MIN);

	//Ϊ��ֵ������׼��
	//virtual void Prepare();

	//�Ƿ�ʵʩ�ϸ��ڲ�
	virtual void SetStrict(BOOL bStrict = TRUE) { m_bStrict = bStrict; }

	//ʵʩ��ֵ
	virtual void Transact(int iMethod=0);

protected:
	//׼���ڴ�
	virtual void PrepareMemory();

	//�Զ�����������岹���ɨ��뾶
	void AutoRadius();

	//����Ա��岹����в�ֵ����
	void Interpolate(int iMethod);

	//�������(x,y)�岹Ҫ��ֵ
	// iMethod: ��ֵ������־
	//       0: Gressman ��ֵ
	//       1: Gauss ��ֵ
	double InterpolateOnePoint(double x, double y, int iMethod);

	//Gressman ��ֵ
	double GressmanInterp(double x, double y);

	//Gauss ��ֵ
	double GaussInterp(double x, double y);

private:
	//����ɨ��뾶r,ʹ����(x0,y0)ΪԲ�ĵ�Բ�ܸ��� n ����ɢ��
	double _CalculateRadius(double x0, double y0,int n);

	//������������
	double _distance(double x0, double y0, double x1, double y1);

	//Gressman Ȩ������
	double GressmanCoefficient(double r, double d);

	//Gauss Ȩ������
	double GaussCoefficient(double r, double d);

protected:
	double	m_dRadius;			//ɨ��뾶
	bool	m_bAutoRadius;		//���ɨ��뾶=0,���Զ�����ɨ��뾶,,ʹɨ��뾶��������m_nStation����
	int		m_nVertexMin;		//�Զ�����ɨ��뾶ʱ��ɨ��뾶�����ٰ�������ɢ����(>=3) VERTEX_NUM_MIN

	double	m_xmin,m_xmax;		//��ֵx��Χ
	double	m_ymin,m_ymax;		//��ֵy��Χ

	double*	m_pRadiusArray;		//ɨ��뾶����,���ڱ����Զ�ɨ��õ��ĺ��ʰ뾶

	BOOL	m_bCorrect;			//�Ƿ���в�ֵ�������

	BOOL	m_bStrict;			//�Ƿ��ϸ��ڲ�(Ҫ��Դ���ݵķ�Χ)

	//�е����߸��������ͷ��ڴ�
	double*	m_pxi;				//Դ����x����ָ��
	double*	m_pyi;				//Դ����y����ָ��
	double*	m_pvi;				//Դ������ɢ��Ҫ��ֵָ��
	int				m_ni;				//Դ��������

	//�е����߸��������ͷ��ڴ�
	double*	m_pxo;				//Ŀ������x����ָ��
	double*	m_pyo;				//Ŀ��Դ����y����ָ��
	double*	m_pvo;				//Ŀ��Դ������ɢ��Ҫ��ֵָ��
	int		m_no;				//Ŀ����������
};

#pragma once

class KIND_EXT_CLASS KRandom
{
private:
	KRandom(void);

public:
	//����һ��[0,1]�����ھ��ȷֲ�α�����
	static double Rand_01_One(double& seed);
	//�������[0,1]�����ھ��ȷֲ�α�����
	static void Rand_01_Series(double *dp, const size_t stCount,long seed=0);

	//��������[a,b]������һ�����ȷֲ�α�������
	static size_t Rand_ab_One(size_t a, size_t b,long& seed);
	//�����������[a,b]�����ھ��ȷֲ�α�����������
	static void Rand_ab_Series(size_t a, size_t b, size_t stCount, long *sp, long seed=0);

	//���������ֵ�뷽���һ����̬�ֲ������
	//�����Ĳ�����mu��ro(�̺ͦ�ֵ)Ҫ���ȸ�����Ҳ�ɴ����������л�á�
	//����㷨�е�nֵҪȡ�ĸ���(����12)������Ҫ�Լ��޸ġ�
	static double Rand_ND_One(double mu, double ro, double& seed);
	//���������ֵ�뷽�����̬�ֲ����������
	//�����Ĳ�����mu��ro(�̺ͦ�ֵ)Ҫ���ȸ�����Ҳ�ɴ����������л�á�
	//����㷨�е�nֵҪȡ�ĸ���(����12)������Ҫ�Լ��޸ġ�
	static void Rand_ND_Series(double mu, double ro, double* dp, size_t stCount, long seed=0);
};

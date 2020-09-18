#include "StdAfx.h"
#include "KRandom.h"

#include "time.h"

#define RandCoef2053	2053	
#define RandCoef13849	13849
#define MODUL65536		65536

KRandom::KRandom(void)
{
}

//����һ��[0,1]�����ھ��ȷֲ�α�����
double KRandom::Rand_01_One(double& seed)
{
	if(!seed) seed = (double)time(NULL);	//��������

	seed = ((unsigned long)seed) % MODUL65536;		//��65536Ϊģ������ȡ��
	seed = RandCoef2053 * (seed) + RandCoef13849;	//�ˡ���ϵ��
	seed = ((unsigned long)seed) % MODUL65536;		//��65536Ϊģ������ȡ��
	double rand = (seed) / (double)MODUL65536;		//ʹ�������[0,1]����

	return( rand );									//�������ֵ
}

//�������[0,1]�����ھ��ȷֲ�α�����
void KRandom::Rand_01_Series(double *dp, const size_t stCount,long seed)
{
	if(!seed) seed = (long)time(NULL);	//��������

	for(size_t st=0; st<stCount; st++)
	{
		seed = RandCoef2053 * (seed) + RandCoef13849;
		seed = (unsigned long)seed % MODUL65536;	//��65536Ϊģ������ȡ��
		dp[st] = seed / (double)MODUL65536;
	}
}

//��������[a,b]������һ�����ȷֲ�α�������
size_t KRandom::Rand_ab_One(size_t a, size_t b,long& seed)
{
	if(!seed) seed = (long)time(NULL);	//��������

	size_t rand;
	size_t stk = b - a + 1;
	size_t stl = 2;
	while(stl < stk) stl = stl + stl;

	size_t modul = 4 * stl;
	stk = seed;
	size_t sti = 1;
	while(sti <= 1)
	{
		stk = 5 * stk;
		stk = stk % modul;
		stl = stk /4 +a;
		if (stl<=b)
		{
			rand=stl; 
			sti=sti+1;
		}
      }
    seed=stk;
    return(rand);
}

//�����������[a,b]�����ھ��ȷֲ�α�����������
void KRandom::Rand_ab_Series(size_t a, size_t b, size_t stCount, long *sp, long seed)
{
	if(!seed) seed = (long)time(NULL);	//��������

	size_t stk = b - a + 1;
	size_t stl = 2;
	while(stl < stk) stl = stl + stl;
	size_t modul = 4 * stl;
	stk = seed;
	size_t sti = 0;
	while(sti < stCount)
	{
		stk = 5 * stk;
		stk = stk % modul;
		stl = stk / 4 +a;
		if (stl <= b)
		{
			sp[sti]=stl; 
			sti = sti + 1;
		}
    }
    seed=stk;
}

//���������ֵ�뷽���һ����̬�ֲ������
//�����Ĳ�����mu��ro(�̺ͦ�ֵ)Ҫ���ȸ�����Ҳ�ɴ����������л�á�
//����㷨�е�nֵҪȡ�ĸ���(����12)������Ҫ�Լ��޸ġ�
double KRandom::Rand_ND_One(double mu, double ro, double& seed)
{
	if(!seed) seed = (double)time(NULL);	//��������

	double rand = 0.0;
	for (size_t n=0; n<=12; n++)
	{
		seed = RandCoef2053 * (seed) + RandCoef13849;	//�ˡ���ϵ��
		seed =(unsigned long)seed % MODUL65536;			//��65536Ϊģ������ȡ��
		rand = rand + (seed) / (double)MODUL65536;
	}
	rand = mu + ro * (rand - 6.0);
	return(rand);
}

//���������ֵ�뷽�����̬�ֲ����������
//�����Ĳ�����mu��ro(�̺ͦ�ֵ)Ҫ���ȸ�����Ҳ�ɴ����������л�á�
//����㷨�е�nֵҪȡ�ĸ���(����12)������Ҫ�Լ��޸ġ�
void KRandom::Rand_ND_Series(double mu, double ro, double* dp, size_t stCount, long seed)
{
	if(!seed) seed = (long)time(NULL);	//��������

	for (size_t st = 0; st < stCount; st++)
	{
		double rand = 0.0;
		for (size_t n=1; n<=12; n++)
		{
			seed = (seed) * RandCoef2053 + RandCoef13849;
			seed = (unsigned long)seed % MODUL65536;			//��65536Ϊģ������ȡ��
			rand = rand + (seed) / (double)MODUL65536;
		}
		dp[st] = mu + ro * (rand - 6.0);
	}
}

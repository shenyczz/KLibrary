/******************************************************************************
 ** KMath.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KMath - ��ѧ��
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: ������.֣�� (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 

 ** Modifier: 
 ** DateTime: 
 **  Explain: ������һ����̬�࣬��������ѧ�ϵ�һЩ
              ����
              ���õ���ѧ����

 **    Usage: 

              //1. 
              use

******************************************************************************/

#pragma once

class KIND_EXT_CLASS KMath
{
private:
	KMath(void) { }

public:
	// ���������
	static bool FloatEqual(double v1, double v2);

	// ��������
	static double Distance(double x1, double y1, double x2, double y2);

public:
	static const int i = 3;

public:
	static double e;
	static double Pi;
	static double Epslon;
};

/******************************************************************************
 ** KBytesOrder.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KBytesOrder - ���С�Сβ�ֽ�����
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: ������.֣�� (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 

 ** Modifier: 
 ** DateTime: 
 **  Explain: 

 **    Usage: 

              //1. 
              use

******************************************************************************/

#pragma once

class KIND_EXT_CLASS KBytesOrder
{
protected:
	KBytesOrder(void);
	virtual ~KBytesOrder(void);

public:
	static void ReverseBytes(short* iValue);
	static void ReverseBytes(unsigned short* iValue);

	static void ReverseBytes(long* lValue,BOOL bMidEndian = FALSE);
	static void ReverseBytes(unsigned long* lValue,BOOL bMidEndian = FALSE);
};

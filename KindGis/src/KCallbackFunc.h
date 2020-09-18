/******************************************************************************
 ** KCallbackFunc.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KCallbackFunc - 回调函数类
 **  Version: 
 ** Function: 
 **  Explain: 定义一些静态的回调函数

 **   Author: 申永辰.郑州 (shenyczz@163.com)
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

class KIND_EXT_CLASS KCallbackFunc
{
public:
	static void CALLBACK ShowProgressCallback(LPVOID lpCallbackParam);

	static void ShowProgressUseMFCStatusBar(LPVOID lpCallbackParam);
	static void ShowProgressUserProgressCtrl(LPVOID lpCallbackParam); 
};

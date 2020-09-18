#pragma once

#include "ApiCommon.h"
#include "PInvokeStructs.h"

//---------------------------------------------------------
//序号:T-
//功能:
//参数:
//返回:
//说明:
//用法:
//---------------------------------------------------------


#pragma region 简单参数封送处理

//---------------------------------------------------------
//序号:T01
//功能:数值封送
//参数:数值
//返回:放大10倍后的数值
//用法:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetValue(int iValue);
//---------------------------------------------------------
MASC_API int _STDCALL GetValue();

//---------------------------------------------------------
//序号:T02
//功能:字符串封送
//参数:字符串
//返回:加工过的字符串
//用法:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern IntPtr GetString(String str);
//
//	IntPtr intPtr = App.GetString(str);
//	String strRet = Marshal.PtrToStringAuto(intPtr);
//---------------------------------------------------------
MASC_API LPCTSTR _STDCALL GetString(LPCTSTR strValue);

//---------------------------------------------------------
//序号:T03.1
//功能:数值数组封送
//参数:
//返回:数组每个元素乘以2
//用法:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern void GetArray(int[] ary, int length);
//	public static extern void GetArrayPtr(int[] ary, int length);
//	public static extern void GetArrayRef(int[] ary, int length);
//---------------------------------------------------------
MASC_API void _STDCALL GetArray(int pData[], int nSize);
MASC_API void _STDCALL GetArrayPtr(int* pData, int nSize);
MASC_API void _STDCALL GetArrayRef(int* pData, int nSize);
//---------------------------------------------------------
//序号:T03.2.1
//功能:数值数组封送 -- c++ => c#
//	   c#传递引用(out)参数,
//	   c++本地动态生成数组并赋值,包括内容和长度。
//参数:
//返回:
//用法:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern void GetArrayIntPtr(out IntPtr p, out int length);
//
//	IntPtr p;
//	int len;
//	PInvokeApi.GetArrayIntPtr(out p, out len);
//	double[] ary4 = new double[len];
//	Marshal.Copy(p, ary4, 0, len);
//---------------------------------------------------------
MASC_API void _STDCALL GetArrayDoubleIntPtr(double*& pData, int& nSize);


//---------------------------------------------------------
//序号:T03.2.2
//功能:数组传递 - C++ => C#
//参数:
//返回:
//用法:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern IntPtr GetArrayDoubleIntPtr2(out int len);
//
//	IntPtr intPtr = PInvokeApi.GetArrayDoubleIntPtr2(out len);
//	double[] da2 = new double[len];
//	Marshal.Copy(intPtr, da2, 0, len);
//---------------------------------------------------------
MASC_API double* _STDCALL GetArrayDoubleIntPtr2(int& nSize);


#pragma endregion



#pragma region 简单结构封送 - Simple



//---------------------------------------------------------
//序号:T04.1
//功能:简单结构封送
//参数:
//返回:结构大小
//说明:参数值传递，对结构变量的改变是否能传递到调用者(No)
//用法:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetSimple(Simple simple);
//---------------------------------------------------------
MASC_API int _STDCALL GetSimple(Simple simple);
//---------------------------------------------------------
//序号:T04.2
//功能:简单结构封送
//参数:
//返回:结构大小
//说明:参数指针传递，对结构变量的改变是否能传递到调用者(Yes)
//用法:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetSimplePtr(ref Simple simple);
//---------------------------------------------------------
MASC_API int _STDCALL GetSimplePtr(Simple* pSimple);
//---------------------------------------------------------
//序号:T04.3
//功能:简单结构封送
//参数:
//返回:结构大小
//说明:参数引用传递，对结构变量的改变是否能传递到调用者(Yes)
//用法:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetSimpleRef(ref Simple simple);
//---------------------------------------------------------
MASC_API int _STDCALL GetSimpleRef(Simple& simple);
//---------------------------------------------------------
//序号:T04.4
//功能:简单结构封送
//参数:
//返回:结构大小
//说明:参数IntPtr传递,对结构变量的改变是否能传递到调用者(Yes)
//用法:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetSimpleIntPtr(IntPtr p);
//
//	IntPtr p = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(Simple)));
//	PInvokeApi.GetSimpleIntPtr(p);
//	Simple simple = (Simple)Marshal.PtrToStructure(p, typeof(Simple));
//	Marshal.FreeHGlobal(p);
//---------------------------------------------------------
MASC_API int _STDCALL GetSimpleIntPtr(Simple* pSimple);
//---------------------------------------------------------
//序号:T04.5
//功能:简单结构封送
//参数:
//返回:结构大小
//说明:参数指针引用传递,可以在本地动态生成Simple对象返回,对结构变量的改变是否能传递到调用者(Yes)
//用法:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetSimpleIntPtrOut(out IntPtr p);
//
//	IntPtr p;
//	PInvokeApi.GetSimpleIntPtr(out p);
//	Simple simple = (Simple)Marshal.GetSimpleIntPtrOut(p, typeof(Simple));
//---------------------------------------------------------
MASC_API int _STDCALL GetSimpleIntPtrOut(Simple*& pSimple);


#pragma endregion



#pragma region 复杂结构封送 - Student


//---------------------------------------------------------
//序号:SN5.1
//功能:复杂结构封送
//参数:
//返回:结构大小
//说明:参数值传递，对结构变量的改变是否能传递到调用者(No)
//用法:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetStudent(Student student);
//---------------------------------------------------------
MASC_API int _STDCALL GetStudent(Student student);
//---------------------------------------------------------
//序号:SN5.2
//功能:复杂结构封送
//参数:
//返回:结构大小
//说明:参数指针传递，对结构变量的改变是否能传递到调用者(Yes)
//用法:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetStudentPtr(ref Student student);
//---------------------------------------------------------
MASC_API int _STDCALL GetStudentPtr(Student* pStudent);
//---------------------------------------------------------
//序号:SN5.3
//功能:复杂结构封送
//参数:
//返回:结构大小
//说明:参数引用传递，对结构变量的改变是否能传递到调用者(Yes)
//用法:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetStudentRef(ref Student student);
//---------------------------------------------------------
MASC_API int _STDCALL GetStudentRef(Student& student);
//---------------------------------------------------------
//序号:SN5.4
//功能:复杂结构封送
//参数:
//返回:结构大小
//说明:参数引用传递，对结构变量的改变是否能传递到调用者(Yes)
//用法:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetStudentIntPtr(IntPtr p);
//
//	IntPtr p = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(Student)));
//	PInvokeApi.GetStudentIntPtr(p);
//	Student student = (Student)Marshal.PtrToStructure(p, typeof(Student));
//	Marshal.FreeHGlobal(p);
//---------------------------------------------------------
MASC_API int _STDCALL GetStudentIntPtr(Student* pStudent);
//---------------------------------------------------------
//序号:SN5.5
//功能:复杂结构封送
//参数:
//返回:结构大小
//说明:参数指针引用传递,可以在本地动态生成Student对象返回,对结构变量的改变是否能传递到调用者(Yes)
//用法:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetStudentIntPtrOut(out IntPtr p);
//---------------------------------------------------------
MASC_API int _STDCALL GetStudentIntPtrOut(Student*& pStudent);



#pragma endregion


#pragma region 复杂结构封送 - Group



#pragma endregion

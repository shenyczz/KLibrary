#pragma once

#include "ApiCommon.h"
#include "PInvokeStructs.h"

//---------------------------------------------------------
//���:T-
//����:
//����:
//����:
//˵��:
//�÷�:
//---------------------------------------------------------


#pragma region �򵥲������ʹ���

//---------------------------------------------------------
//���:T01
//����:��ֵ����
//����:��ֵ
//����:�Ŵ�10�������ֵ
//�÷�:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetValue(int iValue);
//---------------------------------------------------------
MASC_API int _STDCALL GetValue();

//---------------------------------------------------------
//���:T02
//����:�ַ�������
//����:�ַ���
//����:�ӹ������ַ���
//�÷�:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern IntPtr GetString(String str);
//
//	IntPtr intPtr = App.GetString(str);
//	String strRet = Marshal.PtrToStringAuto(intPtr);
//---------------------------------------------------------
MASC_API LPCTSTR _STDCALL GetString(LPCTSTR strValue);

//---------------------------------------------------------
//���:T03.1
//����:��ֵ�������
//����:
//����:����ÿ��Ԫ�س���2
//�÷�:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern void GetArray(int[] ary, int length);
//	public static extern void GetArrayPtr(int[] ary, int length);
//	public static extern void GetArrayRef(int[] ary, int length);
//---------------------------------------------------------
MASC_API void _STDCALL GetArray(int pData[], int nSize);
MASC_API void _STDCALL GetArrayPtr(int* pData, int nSize);
MASC_API void _STDCALL GetArrayRef(int* pData, int nSize);
//---------------------------------------------------------
//���:T03.2.1
//����:��ֵ������� -- c++ => c#
//	   c#��������(out)����,
//	   c++���ض�̬�������鲢��ֵ,�������ݺͳ��ȡ�
//����:
//����:
//�÷�:
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
//���:T03.2.2
//����:���鴫�� - C++ => C#
//����:
//����:
//�÷�:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern IntPtr GetArrayDoubleIntPtr2(out int len);
//
//	IntPtr intPtr = PInvokeApi.GetArrayDoubleIntPtr2(out len);
//	double[] da2 = new double[len];
//	Marshal.Copy(intPtr, da2, 0, len);
//---------------------------------------------------------
MASC_API double* _STDCALL GetArrayDoubleIntPtr2(int& nSize);


#pragma endregion



#pragma region �򵥽ṹ���� - Simple



//---------------------------------------------------------
//���:T04.1
//����:�򵥽ṹ����
//����:
//����:�ṹ��С
//˵��:����ֵ���ݣ��Խṹ�����ĸı��Ƿ��ܴ��ݵ�������(No)
//�÷�:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetSimple(Simple simple);
//---------------------------------------------------------
MASC_API int _STDCALL GetSimple(Simple simple);
//---------------------------------------------------------
//���:T04.2
//����:�򵥽ṹ����
//����:
//����:�ṹ��С
//˵��:����ָ�봫�ݣ��Խṹ�����ĸı��Ƿ��ܴ��ݵ�������(Yes)
//�÷�:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetSimplePtr(ref Simple simple);
//---------------------------------------------------------
MASC_API int _STDCALL GetSimplePtr(Simple* pSimple);
//---------------------------------------------------------
//���:T04.3
//����:�򵥽ṹ����
//����:
//����:�ṹ��С
//˵��:�������ô��ݣ��Խṹ�����ĸı��Ƿ��ܴ��ݵ�������(Yes)
//�÷�:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetSimpleRef(ref Simple simple);
//---------------------------------------------------------
MASC_API int _STDCALL GetSimpleRef(Simple& simple);
//---------------------------------------------------------
//���:T04.4
//����:�򵥽ṹ����
//����:
//����:�ṹ��С
//˵��:����IntPtr����,�Խṹ�����ĸı��Ƿ��ܴ��ݵ�������(Yes)
//�÷�:
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
//���:T04.5
//����:�򵥽ṹ����
//����:
//����:�ṹ��С
//˵��:����ָ�����ô���,�����ڱ��ض�̬����Simple���󷵻�,�Խṹ�����ĸı��Ƿ��ܴ��ݵ�������(Yes)
//�÷�:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetSimpleIntPtrOut(out IntPtr p);
//
//	IntPtr p;
//	PInvokeApi.GetSimpleIntPtr(out p);
//	Simple simple = (Simple)Marshal.GetSimpleIntPtrOut(p, typeof(Simple));
//---------------------------------------------------------
MASC_API int _STDCALL GetSimpleIntPtrOut(Simple*& pSimple);


#pragma endregion



#pragma region ���ӽṹ���� - Student


//---------------------------------------------------------
//���:SN5.1
//����:���ӽṹ����
//����:
//����:�ṹ��С
//˵��:����ֵ���ݣ��Խṹ�����ĸı��Ƿ��ܴ��ݵ�������(No)
//�÷�:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetStudent(Student student);
//---------------------------------------------------------
MASC_API int _STDCALL GetStudent(Student student);
//---------------------------------------------------------
//���:SN5.2
//����:���ӽṹ����
//����:
//����:�ṹ��С
//˵��:����ָ�봫�ݣ��Խṹ�����ĸı��Ƿ��ܴ��ݵ�������(Yes)
//�÷�:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetStudentPtr(ref Student student);
//---------------------------------------------------------
MASC_API int _STDCALL GetStudentPtr(Student* pStudent);
//---------------------------------------------------------
//���:SN5.3
//����:���ӽṹ����
//����:
//����:�ṹ��С
//˵��:�������ô��ݣ��Խṹ�����ĸı��Ƿ��ܴ��ݵ�������(Yes)
//�÷�:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetStudentRef(ref Student student);
//---------------------------------------------------------
MASC_API int _STDCALL GetStudentRef(Student& student);
//---------------------------------------------------------
//���:SN5.4
//����:���ӽṹ����
//����:
//����:�ṹ��С
//˵��:�������ô��ݣ��Խṹ�����ĸı��Ƿ��ܴ��ݵ�������(Yes)
//�÷�:
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
//���:SN5.5
//����:���ӽṹ����
//����:
//����:�ṹ��С
//˵��:����ָ�����ô���,�����ڱ��ض�̬����Student���󷵻�,�Խṹ�����ĸı��Ƿ��ܴ��ݵ�������(Yes)
//�÷�:
//	[DllImport(dll_name, CharSet = CharSet.Auto)]
//	public static extern int GetStudentIntPtrOut(out IntPtr p);
//---------------------------------------------------------
MASC_API int _STDCALL GetStudentIntPtrOut(Student*& pStudent);



#pragma endregion


#pragma region ���ӽṹ���� - Group



#pragma endregion

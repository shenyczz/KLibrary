#include "stdafx.h"
#include "PInvoke.h"



MASC_API int _STDCALL GetValue()
{
	return 16;
}


MASC_API LPCTSTR _STDCALL GetString(LPCTSTR strValue)
{
	_tcscpy_s(g_Buffer, _T(""));

	KString str(_T(""));
	str.Format(_T("���Ǵ���Ĳ���(This is in parameter) OK?: ��%s��"), strValue);
	_tcscpy_s(g_Buffer, (LPCTSTR)str);

	return g_Buffer;
}



MASC_API void _STDCALL GetArray(int pData[], int nSize)
{
	int v = 0;

	for (int i = 0; i < nSize; i++)
	{
		pData[i] *= 2;
	}

	return;
}

MASC_API void _STDCALL GetArrayPtr(int* pData, int nSize)
{
	int v = 0;

	for (int i = 0; i < nSize; i++)
	{
		pData[i] *= 3;
	}

	return;
}

MASC_API void _STDCALL GetArrayRef(int* pData, int nSize)
{
	int v = 0;

	for (int i = 0; i < nSize; i++)
	{
		pData[i] *= 4;
	}

	return;
}



MASC_API void _STDCALL GetArrayDoubleIntPtr(double*& pData, int& nSize)
{
	nSize = 3;

	//��ApiCommon�����ڴ�
	pData = new double[nSize];
	g_ApiCommon.Data = pData;

	int k = 1;
	for (int i = 0; i < nSize; i++)
	{
		*(pData + i) = (i + 1) * k;
	}

	return;
}

MASC_API double* _STDCALL GetArrayDoubleIntPtr2(int& nSize)
{
	nSize = 6;
	double* pData = NULL;

	//��ApiCommon�����ڴ�
	pData = new double[nSize];
	g_ApiCommon.Data = pData;

	int k = 1;
	for (int i = 0; i < nSize; i++)
	{
		pData[i] = (i + 1) * k;
	}

	return pData;
}




//���:T04.1--T04.5
MASC_API int _STDCALL GetSimple(Simple simple)
{
	int size = sizeof(Simple);

	simple.Value = 41;

	return size;
}
MASC_API int _STDCALL GetSimplePtr(Simple* pSimple)
{
	int size = sizeof(Simple);

	pSimple->Value = 42;

	return size;
}
MASC_API int _STDCALL GetSimpleRef(Simple& simple)
{
	int size = sizeof(Simple);

	simple.Value = 43;

	return size;
}
MASC_API int _STDCALL GetSimpleIntPtr(Simple* pSimple)
{
	int size = sizeof(Simple);

	pSimple->Value = 44;

	return size;
}
MASC_API int _STDCALL GetSimpleIntPtrOut(Simple*& pSimple)
{
	int size = sizeof(Simple);

	//��ApiCommon�����ڴ�
	pSimple = new Simple();
	g_ApiCommon.Data = pSimple;
	pSimple->Value = 45;

	return size;
}


//���:SN5.1--SN5.5
MASC_API int _STDCALL GetStudent(Student student)
{
	int size = sizeof(Student);

	student.Age = 51;
	_tcscpy_s(student.Name, _T("GetStudent"));
	_tcscpy_s(student.Address, _T("����������51��"));
	for (int i = 0; i < 3; i++)
	{
		student.Scores[i] = 5.1;
	}

	return size;
}
MASC_API int _STDCALL GetStudentPtr(Student* pStudent)
{
	int size = sizeof(Student);

	pStudent->Age = 52;
	_tcscpy_s(pStudent->Name, _T("GetStudentPtr"));
	_tcscpy_s(pStudent->Address, _T("����������52��"));
	for (int i = 0; i < 3; i++)
	{
		pStudent->Scores[i] = 5.2;
	}

	return size;
}
MASC_API int _STDCALL GetStudentRef(Student& student)
{
	int size = sizeof(Student);

	student.Age = 53;
	_tcscpy_s(student.Name, _T("GetStudentRef"));
	_tcscpy_s(student.Address, _T("����������53��"));
	for (int i = 0; i < 3; i++)
	{
		student.Scores[i] = 5.3;
	}

	return size;
}
MASC_API int _STDCALL GetStudentIntPtr(Student* pStudent)
{
	int size = sizeof(Student);

	pStudent->Age = 54;
	_tcscpy_s(pStudent->Name, _T("GetStudentIntPtr"));
	_tcscpy_s(pStudent->Address, _T("����������54��"));
	for (int i = 0; i < 3; i++)
	{
		pStudent->Scores[i] = 5.4;
	}

	return size;
}
MASC_API int _STDCALL GetStudentIntPtrOut(Student*& pStudent)
{
	int size = sizeof(Simple);

	//��ApiCommon�����ڴ�
	pStudent = new Student();
	g_ApiCommon.Data = pStudent;

	pStudent->Age = 55;
	_tcscpy_s(pStudent->Name, _T("GetStudentIntPtrOut"));
	_tcscpy_s(pStudent->Address, _T("����������55��"));
	for (int i = 0; i < 3; i++)
	{
		pStudent->Scores[i] = 5.5;
	}

	return size;
}



/*
MASC_API int _STDCALL GetPerson(Person person)
{
	int size = sizeof(Person);

	_tcscpy_s(person.Name, _T("GetPerson"));
	person.Age = 10;
	_tcscpy_s(person.Address, _T("����������10��"));

	return size;
}
MASC_API int _STDCALL GetPersonPtr(Person* pPerson)
{
	int size = sizeof(Person);
	
	_tcscpy_s(pPerson->Name, _T("GetPersonPtr"));
	pPerson->Age = 20;
	_tcscpy_s(pPerson->Address, _T("����������20��"));

	return size;
}
MASC_API int _STDCALL GetPersonRef(Person& person)
{
	int size = sizeof(Person);

	_tcscpy_s(person.Name, _T("GetPersonRef"));
	person.Age = 30;
	_tcscpy_s(person.Address, _T("����������30��"));

	return size;
}
MASC_API int _STDCALL GetPersonIntPtr(Person* pPerson)
{
	int size = sizeof(Person);

	_tcscpy_s(pPerson->Name, _T("GetPersonIntPtr"));
	pPerson->Age = 40;
	_tcscpy_s(pPerson->Address, _T("����������40��"));

	return size;
}
*/
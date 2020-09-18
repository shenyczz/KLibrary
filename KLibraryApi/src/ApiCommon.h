#pragma once

//#include "KLibraryApi.h"

namespace Api{}
using namespace Api;

class ApiCommon
{
public:
	ApiCommon();
	~ApiCommon();

public:
	//用来管理动态内存分配
	__declspec(property(get = getData, put = putData)) LPVOID Data;
	void putData(LPVOID p);
	LPVOID getData() { return m_lpData; }

	__declspec(property(get = getBuffer, put = putBuffer)) LPCTSTR Buffer;
	void putBuffer(LPCTSTR lpsz);
	LPCTSTR getBuffer() { return m_Buffer; }

protected:
	LPVOID m_lpData;
	TCHAR m_Buffer[MAX_PATH];


//public:
//	KRpgData m_RpgData;
//	KRpgData* GetRpgData() { return &m_RpgData; }

};

extern ApiCommon g_ApiCommon;

extern TCHAR g_Buffer[MAX_PATH];

extern KDib g_Dib;
extern KRpgData g_RpgData;

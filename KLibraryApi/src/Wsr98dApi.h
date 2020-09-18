#pragma once

#include "ApiCommon.h"
#include "Wsr98dStructs.h"

namespace Wsr98dApi{}
using namespace Wsr98dApi;

namespace Wsr98dApi
{

	MASC_API bool _STDCALL RPG_LoadData(LPCTSTR lpszFile);

	MASC_API HBITMAP _STDCALL RPG_BuildImage(float fZoom, bool bDrawCoordinate, bool bDrawInfoArea);
	MASC_API HBITMAP _STDCALL RPG_ToImageColor(void);
	MASC_API HBITMAP _STDCALL RPG_ToImageGray(void);

	//---------------------------------------------------------
	//���:
	//����:	RPG����ת��Ϊ�������
	//����:	pData - ����ָ��
	//		radarGridInfo - �״�����Ϣ
	//����:
	//˵��:	RPG_LoadData()֮��ʹ��
	//�÷�:
	//
	//[DllImport(dll_name, CharSet = CharSet.Auto)]
	//public static extern bool RPG_ToGrid(out IntPtr p, out RadarGridInfo radarGridInfo);
	//---------------------------------------------------------
	MASC_API int _STDCALL RPG_ToGrid(double*& pData, RadarGridInfo& radarGridInfo);


}

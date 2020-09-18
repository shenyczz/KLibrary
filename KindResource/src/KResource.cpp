#include "stdafx.h"
#include "KResource.h"

KResource::KResource(void)
{
}

KResource::~KResource(void)
{
}


// ȡ�ò˵���Դ
//static 
HBITMAP KResource::LoadBitmap(UINT uID)
{
	HMODULE hModule	= ::LoadLibrary(KindResource_Library);
	HBITMAP hBITMAP	= ::LoadBitmap(hModule,MAKEINTRESOURCE(uID));
	FreeLibrary(hModule);

	return hBITMAP;
}

// ȡ�ù����Դ
//static 
HCURSOR KResource::LoadCursor(UINT uID)
{
	HMODULE hModule	= ::LoadLibrary(KindResource_Library);
	HCURSOR hCursor	= ::LoadCursor(hModule,MAKEINTRESOURCE(uID));
	FreeLibrary(hModule);
	return hCursor;
}

// ȡ��ͼ����Դ
//static
HICON KResource::LoadIcon(UINT uID)
{
	HMODULE hModule	= ::LoadLibrary(KindResource_Library);
	//HICON hIcon	= ::LoadIcon(hModule,MAKEINTRESOURCE(uID));	// ���������
	HICON hIcon = (HICON) ::LoadImage(hModule, MAKEINTRESOURCE(uID), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	FreeLibrary(hModule);
	return hIcon;
}

// ȡ�ö��������Դ
// static 
HCURSOR KResource::LoadAnimateCursor(UINT uID,LPCTSTR lpType)
{
	HMODULE hModule		= ::LoadLibrary(KindResource_Library);
	HRSRC hResInfo		= ::FindResource(hModule,MAKEINTRESOURCE(uID), lpType);
	DWORD dwSize		= ::SizeofResource(hModule,hResInfo);
	HGLOBAL hResource	= ::LoadResource(hModule,hResInfo);
	LPVOID pv			= ::LockResource(hResource);

	TCHAR szTempFile[MAX_PATH] = _T("C:\\sfxres.temp.anifile");
	FILE* fp = _tfopen(szTempFile,_T("wb"));
	if(fp==NULL)
	{
		return NULL;
	}
	fwrite(pv,sizeof(BYTE),dwSize,fp);
	fclose(fp);

	UnlockResource(hResource);
	FreeResource(hResource);
	FreeLibrary(hModule);

	HCURSOR hAniCursor = ::LoadCursorFromFile(szTempFile);
	::DeleteFile(szTempFile);

	return hAniCursor;
}

// ȡ�ò˵���Դ
//static 
HMENU KResource::LoadMenu(UINT uID)
{
	HMODULE hModule		= ::LoadLibrary(KindResource_Library);
	HMENU hMenu = ::LoadMenu(hModule,MAKEINTRESOURCE(uID));
	return hMenu;
}

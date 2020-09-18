#pragma once

class KTest
{
public:
	KTest(void);
	~KTest(void);

public:
	static BOOL Asc2Axd(IN LPCTSTR lpszFileAsc, OUT LPCTSTR lpszFileAxd, LPCTSTR lpszComment);
	static void Fire();
	// 生成 ENVI 文件头
	static void EnviHdr(KGridData* pGridData);
	static BOOL Layer2Bmp(LPCTSTR lpszFile);

	static int ProductCode;
};

/*
int GetOSVer()  
{  
OSVERSIONINFO   osver;     
osver.dwOSVersionInfoSize   =   sizeof(OSVERSIONINFO);     
GetVersionEx(&osver);     
if(osver.dwPlatformId == 2)  
{  
if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)  
{  
printf("xp\n");  
return(2);  
}  
if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)  
{  
printf("windows 2003\n");  
return(3);  
}  
if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 0)  
{  
printf("vista and 2008\n");  
return(4);  
}  
if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 1)  
{  
printf("2008 R2 and Windows 7\n");  
return(5);  
}  
}  
return 0;  
} 
*/

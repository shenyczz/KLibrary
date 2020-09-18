#pragma once

#include "KindResource.inl"

class KIND_EXT_CLASS KResource
{
public:
	KResource(void);
	virtual ~KResource(void);

public:
	// 取得菜单资源
	static HBITMAP LoadBitmap(UINT uID);
	// 取得光标资源
	static HCURSOR LoadCursor(UINT uID);
	// 取得图标资源
	static HICON LoadIcon(UINT uID);
	// 取得菜单资源
	static HMENU LoadMenu(UINT uID);
	// 取得动画光标资源
	static HCURSOR LoadAnimateCursor(UINT uID, LPCTSTR lpType = _T(""));
};

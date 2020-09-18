#pragma once

#include "KindResource.inl"

class KIND_EXT_CLASS KResource
{
public:
	KResource(void);
	virtual ~KResource(void);

public:
	// ȡ�ò˵���Դ
	static HBITMAP LoadBitmap(UINT uID);
	// ȡ�ù����Դ
	static HCURSOR LoadCursor(UINT uID);
	// ȡ��ͼ����Դ
	static HICON LoadIcon(UINT uID);
	// ȡ�ò˵���Դ
	static HMENU LoadMenu(UINT uID);
	// ȡ�ö��������Դ
	static HCURSOR LoadAnimateCursor(UINT uID, LPCTSTR lpType = _T(""));
};

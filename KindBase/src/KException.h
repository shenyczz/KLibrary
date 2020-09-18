#pragma once

class KIND_EXT_CLASS KException
{
public:
	KException(void);
	KException(const KException& e);
	KException(LPCTSTR lpszMessage);
	virtual ~KException(void);

public:
	LPCTSTR Message();

protected:
	_tstring m_strMessage;
};

/******************************************************************************
 ** KdFile.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KdFile
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: ������.֣�� (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

#include "KCustomDataObject.h"

class KIND_EXT_CLASS KdFile : public KCustomDataObject
{
public:
	KdFile(void);
	virtual ~KdFile(void);

public:
	// ������ݲ������ڴ�
	virtual void Empty();
	//���������Ƿ����(������Ҫ���������и�����Ҫȡ������)
	virtual BOOL IsEmpty();

public:
	// �ļ�ͷ
	void* FileHeader() { return m_pFileHeader; }

	// ������Ϣ��
	DMB* DMBPtr() { return (DMB*)((char*)m_pFileHeader + KD_BLOCK_OFFSET_DMB);}
	// ��չ��Ϣ��
	EMB* EMBPtr() { return (EMB*)((char*)m_pFileHeader + KD_BLOCK_OFFSET_EMB); }
	// ע�����ݿ�
	CDB* CDBPtr() { return (CDB*)((char*)m_pFileHeader + KD_BLOCK_OFFSET_CDB); }
	// ����������
	DDB* DDBPtr() { return (DDB*)((char*)m_pFileHeader + KD_BLOCK_OFFSET_DDB); }

protected:
	// ����ת��Ϊ��ɫ
	virtual COLORREF* Data2Colors(COLORREF clrBack) { return NULL; }

protected:
	// �ļ�ͷ
	void* m_pFileHeader;

	// ��ɫ����ָ��
	COLORREF* m_pColors;

public: // static function
	// �Ƿ� KD �ļ�
	static BOOL IsKdFile(LPCTSTR lpszFile);

	static BOOL CreateKdFile(LPCTSTR lpszFile, BYTE* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile(LPCTSTR lpszFile, char* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile(LPCTSTR lpszFile, WORD* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile(LPCTSTR lpszFile, SHORT* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile(LPCTSTR lpszFile, DWORD* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile(LPCTSTR lpszFile, LONG* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile(LPCTSTR lpszFile, float* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile(LPCTSTR lpszFile, double* pData, KDataInfo* pDataInfo, KPalette* pPalette);

private:
	static BOOL IsKdFile_bin(LPCTSTR lpszFile);
	static BOOL IsKdFile_txt(LPCTSTR lpszFile);

	static BOOL CreateKdFile(LPCTSTR lpszFile, void* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile_easy(LPCTSTR lpszFile, void* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile_hard(LPCTSTR lpszFile, void* pData, KDataInfo* pDataInfo, KPalette* pPalette);
};


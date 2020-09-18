/******************************************************************************
 ** KDataObjectAlgorithm.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KDataObjectAlgorithm
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: 申永辰.郑州 (shenyczz@163.com)
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

class KIND_EXT_CLASS KDataAlgorithm : public KObject
{
public:
	KDataAlgorithm(void);
	virtual ~KDataAlgorithm(void);


	#pragma region DateTime -- 日期时间
		public:	__declspec(property(get = get_DateTime, put = put_DateTime)) DATETIME DateTime;
		public:	DATETIME get_DateTime() { return m_DateTime; }
		public:	VOID put_DateTime(DATETIME value) { m_DateTime = value; }
		private:DATETIME m_DateTime;
	#pragma endregion

	#pragma region DataOffset -- 数据偏移地址
		//public:	__declspec(property(get = get_DataOffset, put = put_DataOffset)) DWORD DataOffset;
		//public:	DWORD get_DataOffset() { return m_dwDataOffset; }
		//public:	VOID put_DataOffset(DWORD offset) { m_dwDataOffset = offset; }
		//private:DWORD m_dwDataOffset;
	#pragma endregion

};


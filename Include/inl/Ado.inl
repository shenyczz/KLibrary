#ifndef __INCLUDED_ADO_INL__
#define __INCLUDED_ADO_INL__

#include <icrsint.h>

namespace Ado
{
//---------------------------------------------------------
}// namespace Ado - end
//---------------------------------------------------------
using namespace Ado;

#pragma pack(1)

#define ADO_CHUNK_SIZE	100

//字段信息
typedef struct tagAdoFieldInfo
{
	TCHAR m_szName[30];				// 字段名称
	short m_nType;					// 字段类型 adEmpty
	long m_lSize;					// 字段长度
	long m_lDefinedSize;			// 
	long m_lAttributes;				// 
	short m_nOrdinalPosition;		// 
	BOOL m_bRequired;				// 
	BOOL m_bAllowZeroLength;		// 
	long m_lCollatingOrder;			// 
}AdoFieldInfo;

#pragma pack()

//#pragma message ("Make sure [msado15.dll] will usually be in C:\\Program Files\\Common Files\\System\\ado")
//#import "C:\Program Files\Common Files\System\ado\msado15.dll" \
//	rename_namespace("ADODB") \
//	rename("EOF", "EndOfFile")

#pragma message ("Make sure [msado60.tlb] will usually be in C:\\Program Files\\Common Files\\System\\ado")
#import "C:\Program Files\Common Files\System\ado\msado60.tlb" \
	rename_namespace("ADODB") \
	rename("EOF","EndOfFile")
using namespace ADODB;

//#pragma message ("Make sure [msado60_Backcompat_i386.tlb] will usually be in C:\\Program Files\\Common Files\\System\\ado")
//#import "C:\Program Files\Common Files\System\ado\msado60_Backcompat_i386.tlb" \
//	rename_namespace("ADODB") \
//	rename("EOF","EndOfFile")
//using namespace ADODB;

#pragma message ("Make sure [oledb32.dll] will usually be in C:\\Program Files\\Common Files\\System\\oledb32")
#import "C:\Program Files\Common Files\System\ole db\oledb32.dll" \
	no_auto_exclude \
	rename_namespace("OLEDB")
using namespace OLEDB;
//
//#pragma message ("Make sure [msjro.dll] will usually be in C:\\Program Files\\Common Files\\System\\ado")
//#import "C:\Program Files\Common Files\System\ado\msjro.dll" \
//	no_namespace \
//	rename("ReplicaTypeEnum", "_ReplicaTypeEnum") 


#pragma warning (disable: 4146)
#pragma warning (disable: 4251)

#endif	//#ifndef __INCLUDED_ADO_INL__

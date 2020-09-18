#ifndef __INCLUDED_KIND_DEFS_H__
#define __INCLUDED_KIND_DEFS_H__

// 自定义消息 - Beg
//----------------

// 自定义常量 - Beg
//----------------
// 无效数据(DWORD)
#ifndef _KIND_INVALID_VALUE_
	#define KIND_INVALID_VALUE	0XCCCCCCCC
#endif

#ifndef _KIND_INVALID_FLOAT_VALUE_
	#define KIND_INVALID_FLOAT_VALUE	-9999
#endif

#ifndef _KIND_E_
	#define E	2.17828
#endif

#ifndef _KIND_PI_
	#define PI	3.1415926535897932
#endif

#ifndef _KIND_EPS_
	#define EPS	1.0e-12
#endif

#ifndef _SFX_INVALID_DATA_
	#define SFX_INVALID_DATA		0XCCCCCCCC
#endif

#ifndef _SFX_INVALID_DATA_FLOAT_
	#define SFX_INVALID_DATA_FLOAT	0XCCCCCCCC
#endif

#ifndef _SFX_INVALID_DATA_DOUBLE_
	#define SFX_INVALID_DATA_DOUBLE	0XCCCCCCCC
#endif

// 自定义操作 - Beg
//----------------

#ifndef _KIND_ASSERT_
	#define Assert(x) { if( !(x) ) throw;}
#endif

#ifndef _KIND_DELETE_
	#define _delete(p) { delete (p); (p) = 0; }
	#define _deletea(p) { delete [] (p); (p) = 0; }
#endif

#ifndef _SFX_MSG_
	#define sfx_msg
#endif

#ifndef _SFX_FUN_
	#define sfx_fun
#endif


//---------------------------------------------------------
// 自定义数据类型 - Beg
//--------------------
// sfx_typedef

typedef signed char			sfx_char;
typedef unsigned char		sfx_uchar;

typedef wchar_t				sfx_wchar;

typedef signed char			sfx_int8;

typedef	short				sfx_int16;
typedef signed short		sfx_int16;
typedef signed short int	sfx_int16;

typedef long				sfx_int32;
typedef signed long			sfx_int32;
typedef signed long int		sfx_int32;

typedef signed long long	sfx_int64;

typedef unsigned char		sfx_uint8;

typedef unsigned short		sfx_uint16;
typedef unsigned short int	sfx_uint16;

typedef unsigned long		sfx_uint32;
typedef unsigned long int	sfx_uint32;

typedef unsigned long long	sfx_uint64;

typedef float				sfx_float16;
typedef double				sfx_float32;
typedef long double			sfx_float64;
//--------------------
// 自定义数据类型 - End
//---------------------------------------------------------

#pragma pack(1)

// 数据类型
enum DataType
{
	DataType_Unknown	= 0,

	DataType_Byte		= 1,
	DataType_Short		= 2,
	DataType_Long		= 4,

	DataType_Char		= DataType_Byte,
	DataType_Ascii		= DataType_Byte,
	DataType_Word		= DataType_Short,
	DataType_Dword		= DataType_Long,

	DataType_Float		= 40,
	DataType_Double		= 80,
};

// 日期时间(16字节)
//
typedef struct tagDateTime
{
	WORD	wYear;				// 年
	WORD	wMonth;				// 月
	WORD	wDay;				// 日
	WORD	wHour;				// 时
	WORD	wMinute;			// 分
	WORD	wSecond;			// 秒
	DWORD	dwMillisecond;		// 毫秒

}DATETIME,DATE_TIME;

// VARGB 结构
// 8 字节
struct VARGB
{
	FLOAT	V;			// 要素值
	union
	{
		DWORD dwArgb;	// ARGB
		struct
		{
			BYTE	B;	// B 分量 
			BYTE	G;	// G 分量 
			BYTE	R;	// R 分量 
			BYTE	A;	// A 分量
		};
	};

	VARGB()
	{ V = 0; A = 0; R = 0; G = 0; B = 0; }

	VARGB(const VARGB& vargb)
	{ V = vargb.V; A = vargb.A; R = vargb.R; G = vargb.G; B = vargb.B; }

	VARGB(float v, COLORREF clr)
	{ V = v; A = 255; R = GetRValue(clr); G = GetGValue(clr); B = GetBValue(clr); }

	VARGB(float v, int r, int g, int b)
	{ V = v; A = 255; R = r; G = g; B = b; }

	VARGB(float v, int a, int r, int g, int b)
	{ V = v; A = a; R = r; G = g; B = b; }

	FLOAT& Value()	{ return V; }
	DWORD& Argb()	{ return dwArgb; }
	DWORD Rgb()		{ return RGB(R,G,B); }

};

#pragma pack()

#endif // #ifndef __INCLUDED_KIND_DEFS_H__

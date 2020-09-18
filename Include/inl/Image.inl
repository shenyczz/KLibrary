#ifndef __INCLUDED_IMAGE_INL__
#define __INCLUDED_IMAGE_INL__

#define PALVERSION	0x300							//调色板版本
#define DIB_HEADER_MARKER	((WORD)('M'<<8)|'B')	//DIB文件头标记 0x4D42
#define BMP_HEADER_MARKER	((WORD)('M'<<8)|'B')	//BMP文件头标记 0x4D42
#define DIBWIDTHBYTES(bits)	((((bits)+31)/32)*4)	//DIB宽度字节数(必须是4的整数倍)

#endif	//#ifndef __INCLUDED_IMAGE_INL__

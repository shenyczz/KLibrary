#ifndef __INCLUDED_IMAGE_INL__
#define __INCLUDED_IMAGE_INL__

#define PALVERSION	0x300							//��ɫ��汾
#define DIB_HEADER_MARKER	((WORD)('M'<<8)|'B')	//DIB�ļ�ͷ��� 0x4D42
#define BMP_HEADER_MARKER	((WORD)('M'<<8)|'B')	//BMP�ļ�ͷ��� 0x4D42
#define DIBWIDTHBYTES(bits)	((((bits)+31)/32)*4)	//DIB����ֽ���(������4��������)

#endif	//#ifndef __INCLUDED_IMAGE_INL__

#ifndef __INCLUDED_KindBase_H__
#define __INCLUDED_KindBase_H__

//-----------------------------------------------------------------------------
#include "KindBase.inl"

//---------------------------------------------------------
//                ȫ���ඨ��
//---------------------------------------------------------
#include "KGlobal.h"				// ȫ�־�̬��
#include "KBytesOrder.h"			// �ֽ���̬��

#include "KExtents.h"				// ����

#include "KObject.h"				// ����
#include "KString.h"				// �ַ���
#include "KException.h"				// �쳣

#include "KObjectCollection.h"		// ջ�����Ѽ�
#include "KObjectPtrCollection.h"	// �Ѷ����Ѽ�

#include "KGridInfo.h"				// ������Ϣ
#include "KDataInfo.h"				// ������Ϣ
#include "KStation.h"				// վ��
#include "KStationInfo.h"			// վ����Ϣ

//---------------------------------------------------------
//                �ӿڶ���
//---------------------------------------------------------

//---------------------------------------------------------
//                ģ�嶨��
//---------------------------------------------------------
#include "TMatrix.h"			// ��ѧ����
#include "TCollection.h"		// �����Ѽ�
#include "TPtrCollection.h"		// ����ָ���Ѽ�
#include "TEventHandler.h"		// �¼�������

//-----------------------------------------------------------------------------
#ifdef DLL_NAME
	#undef DLL_NAME
	#define DLL_NAME		KIND_BASE_DLL_NAME
#endif
//-----------------------------------------------------------------------------
#define LIB_COMMENT			SFX_LIB
#define DISPLAY_MESSAGE		TOSTRING(link_message)SFX_DLL

#pragma comment(lib, LIB_COMMENT) 
#pragma message(DISPLAY_MESSAGE)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KindBase_H__

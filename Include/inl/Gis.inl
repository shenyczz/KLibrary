#ifndef __INCLUDED_GIS_INL__
#define __INCLUDED_GIS_INL__

// ������Ϣ���� GisTool
//
enum GisTool : unsigned short
{
	GisTool_None					= 0,		// ȱʡ״̬�µĹ���״̬

	GisTool_MapController			= 0x1000,	// ��ͼ����,���ӹ��߰���:
		GisTool_Map_Beg				= 0x1000,	// ��ͼ���� - ��ʼ
		GisTool_Cancel				= 0x1001,	// ȡ��
		GisTool_Reset				= 0x1002,	// ��λ
		GisTool_Refresh				= 0x1004,	// ˢ��
		GisTool_ZoomIn				= 0x1008,	// �Ŵ�
		GisTool_ZoomOut				= 0x1010,	// ��С
		GisTool_ZoomPan				= 0x1020,	// ����
		GisTool_Map_End				= 0x1888,	// ��ͼ���� - ����

	GisTool_BlackboardController				= 0x4000,	// �ڰ����,���ӹ��߰���:
		GisTool_Blackboard_Beg					= 0x4000,	// �ڰ���� - ����
		// Ŀ�����
		GisTool_Blackboard_Select_Object		= 0x4001,	// ѡ��Ŀ��
		GisTool_Blackboard_Delete_Object		= 0x4002,	// ɾ��Ŀ��
		GisTool_Blackboard_Move_Object			= 0x4004,	// �ƶ�Ŀ��
		// ����ͼ�λ���
		GisTool_Blackboard_Draw_Point			= 0x4100,	// ���Ƶ�
		GisTool_Blackboard_Draw_Line			= 0x4101,	// ������
		GisTool_Blackboard_Draw_Rect			= 0x4102,	// ���ƾ���
		GisTool_Blackboard_Draw_Ellipse			= 0x4103,	// ������Բ
		GisTool_Blackboard_Draw_Polyline		= 0x4104,	// ��������
		GisTool_Blackboard_Draw_Polygon			= 0x4105,	// ���ƶ����
		GisTool_Blackboard_Draw_Curve			= 0x4106,	// ��������
		GisTool_Blackboard_Draw_Close_Curve		= 0x4107,	// ���Ʊպ�����
		// ����ͼ�λ���
		GisTool_Blackboard_Draw_Label			= 0x4200,	// ���Ʊ�ע
		GisTool_Blackboard_Draw_Layer_Legend	= 0x4201,	// ����ͼ��
		GisTool_Blackboard_Draw_Custom_Legend	= 0x4202,	// �����Զ���ͼ��
		GisTool_Blackboard_End					= 0x4888,	// �ڰ���� - ����

	GisTool_MisceController						= 0x8000,	// �������,���ӹ��߰���:
		GisTool_Misce_Beg						= 0x8000,	// ������� - ��ʼ
		GisTool_Misce_Landmark_Calibrate		= 0x8001,	// ����
		GisTool_Misce_End						= 0x8888,	// ������� - ����

};

#pragma pack(1)
#pragma pack()

#endif	//#ifndef __INCLUDED_GIS_INL__

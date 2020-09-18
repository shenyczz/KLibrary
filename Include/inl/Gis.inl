#ifndef __INCLUDED_GIS_INL__
#define __INCLUDED_GIS_INL__

// 地理信息工具 GisTool
//
enum GisTool : unsigned short
{
	GisTool_None					= 0,		// 缺省状态下的工具状态

	GisTool_MapController			= 0x1000,	// 地图控制,其子工具包括:
		GisTool_Map_Beg				= 0x1000,	// 地图控制 - 开始
		GisTool_Cancel				= 0x1001,	// 取消
		GisTool_Reset				= 0x1002,	// 复位
		GisTool_Refresh				= 0x1004,	// 刷新
		GisTool_ZoomIn				= 0x1008,	// 放大
		GisTool_ZoomOut				= 0x1010,	// 缩小
		GisTool_ZoomPan				= 0x1020,	// 漫游
		GisTool_Map_End				= 0x1888,	// 地图控制 - 结束

	GisTool_BlackboardController				= 0x4000,	// 黑板控制,其子工具包括:
		GisTool_Blackboard_Beg					= 0x4000,	// 黑板控制 - 结束
		// 目标操作
		GisTool_Blackboard_Select_Object		= 0x4001,	// 选择目标
		GisTool_Blackboard_Delete_Object		= 0x4002,	// 删除目标
		GisTool_Blackboard_Move_Object			= 0x4004,	// 移动目标
		// 常用图形绘制
		GisTool_Blackboard_Draw_Point			= 0x4100,	// 绘制点
		GisTool_Blackboard_Draw_Line			= 0x4101,	// 绘制线
		GisTool_Blackboard_Draw_Rect			= 0x4102,	// 绘制矩形
		GisTool_Blackboard_Draw_Ellipse			= 0x4103,	// 绘制椭圆
		GisTool_Blackboard_Draw_Polyline		= 0x4104,	// 绘制折线
		GisTool_Blackboard_Draw_Polygon			= 0x4105,	// 绘制多边形
		GisTool_Blackboard_Draw_Curve			= 0x4106,	// 绘制曲线
		GisTool_Blackboard_Draw_Close_Curve		= 0x4107,	// 绘制闭合曲线
		// 特殊图形绘制
		GisTool_Blackboard_Draw_Label			= 0x4200,	// 绘制标注
		GisTool_Blackboard_Draw_Layer_Legend	= 0x4201,	// 绘制图例
		GisTool_Blackboard_Draw_Custom_Legend	= 0x4202,	// 绘制自定义图例
		GisTool_Blackboard_End					= 0x4888,	// 黑板控制 - 结束

	GisTool_MisceController						= 0x8000,	// 杂项控制,其子工具包括:
		GisTool_Misce_Beg						= 0x8000,	// 杂项控制 - 开始
		GisTool_Misce_Landmark_Calibrate		= 0x8001,	// 定标
		GisTool_Misce_End						= 0x8888,	// 杂项控制 - 结束

};

#pragma pack(1)
#pragma pack()

#endif	//#ifndef __INCLUDED_GIS_INL__

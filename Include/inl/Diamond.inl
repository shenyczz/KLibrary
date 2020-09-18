#ifndef __INCLUDED_DIAMOND_INL__
#define __INCLUDED_DIAMOND_INL__

namespace Diamond {};
using namespace Diamond;

#define	DIAM_FID_TXT_MARK			(_T("DIAMOND"))

#define DIAM_COMMENT_LENGTH				64		// 注释信息长度

// DIAM_FMT_CODE 系统文件格式代码
//
#define	DIAM_FMT_CODE								// Diamond 系统文件格式代码
#define	DIAM_FMT_CODE_BEG				1			// Diamond 系统文件格式产品代码定义开始
#define	DIAM_FMT_CODE_MAX				299			// 文件格式最大值 0-99 100-199 200-299

#define	DIAM_FMT_CODE_NONE				0			// Diamond 系统文件格式代码
#define	DIAM_FMT_CODE_SURFACE			1			// 地面全要素填图数据
#define	DIAM_FMT_CODE_HIGH				2			// 高空全要素填图数据
#define	DIAM_FMT_CODE_TIN				3			// 三角形网格数据文件格式(通用填图和离散点等值线)
#define	DIAM_FMT_CODE_GRID				4			// 矩形网格数据文件格式
#define	DIAM_FMT_CODE_TLOGP				5			// TLOGP和站点剖面图数据
#define	DIAM_FMT_CODE_FAX				6			// 传真图
#define	DIAM_FMT_CODE_TYPHOON_PATH		7			// 台风路径数据
#define	DIAM_FMT_CODE_CITY_FORECAST		8			// 城市站点预报
#define	DIAM_FMT_CODE_MAP				9			// 地图线条数据

#define	DIAM_FMT_CODE_COMBIN			10			// 综合图数据
#define	DIAM_FMT_CODE_VECTOR			11			// 风场矢量数据
#define	DIAM_FMT_CODE_RADAR				12			// 单点雷达图像(PPI)
#define	DIAM_FMT_CODE_IMAGE				13			// 图像数据(卫星云图、雷达拼图及地形图)
#define	DIAM_FMT_CODE_EDIT				14			// 保存被编辑图形的图元数据
#define	DIAM_FMT_CODE_PALETTE			15			// 调色板文件格式
#define	DIAM_FMT_CODE_STATION			16			// 预报站点数据
#define	DIAM_FMT_CODE_FSTATION_INFO		17			// 预报站点文字信息
#define	DIAM_FMT_CODE_GRID_SECTION		18			// 格点数据剖面图
#define	DIAM_FMT_CODE_COMMAND			19			// Micaps 命令行参数

#define	DIAM_FMT_CODE_TAB				20			// 表格数据
#define	DIAM_FMT_CODE_21					21			//
#define	DIAM_FMT_CODE_22					22			//
#define	DIAM_FMT_CODE_23					23			//
#define	DIAM_FMT_CODE_24					24			//
#define	DIAM_FMT_CODE_25					25			//
#define	DIAM_FMT_CODE_26					26			//
#define	DIAM_FMT_CODE_27					27			//
#define	DIAM_FMT_CODE_28					28			//
#define	DIAM_FMT_CODE_29					29			//

#define	DIAM_FMT_CODE_110				110			//
#define	DIAM_FMT_CODE_STAMP				111			// 邮票图
#define	DIAM_FMT_CODE_112				112			//
#define	DIAM_FMT_CODE_113				113			//
#define	DIAM_FMT_CODE_114				114			//
#define	DIAM_FMT_CODE_115				115			//
#define	DIAM_FMT_CODE_116				116			//
#define	DIAM_FMT_CODE_117				117			//
#define	DIAM_FMT_CODE_118				118			//
#define	DIAM_FMT_CODE_119				119			//

#define	DIAM_FMT_CODE_770				770			//
#define	DIAM_FMT_CODE_771				771			//
#define	DIAM_FMT_CODE_772				772			//
#define	DIAM_FMT_CODE_773				773			//
#define	DIAM_FMT_CODE_774				774			//
#define	DIAM_FMT_CODE_775				775			//
#define	DIAM_FMT_CODE_776				776			//
#define	DIAM_FMT_CODE_777				777			//
#define	DIAM_FMT_CODE_778				778			//
#define	DIAM_FMT_CODE_779				779			//

#define DIAM_FMT_CVID_MIN				1		// 等值线数量最小值
#define DIAM_FMT_CVID_MAX				99		// 等值线数量最大值
#define DIAM_FMT_CVID_CONTOUR			9999	// 等值线数量 = 9999 的含义见说明书

#pragma pack(1)
#pragma pack()

#endif	//#ifndef __INCLUDED_DIAMOND_INL__

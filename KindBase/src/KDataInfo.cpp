#include "StdAfx.h"
#include "KDataInfo.h"

#include "Axin.inl"

KDataInfo::KDataInfo(void)
{
	int i;

	wFileID = 0;			// 文件标识
	_tcscpy(this->szFileID,_T("文本数据文件标识"));	// 文本数据文件标识
	wDataSource = 0;		// 数据来源
	wFormatCode = 0;		// 格式代码
	wProductCode = 0;		// 产品代码
	wBytesOrder = 0;		// 字节序
	wDataType = 0;			// 数据类型
	wElementCode = 0;		// 要素代码
	wFileStorageType = 0;	// 文件存储类型

	strComment = _T("");	// 注释信息

	wYear = 1970;			// 年
	wMonth = 1;				// 月
	wDay = 1;				// 日
	wHour = 0;				// 时
	wMinute = 0;			// 分
	wSecond = 0;			// 秒
	dwMillisecond  = 0;		// 毫秒

	wYear2 = 1970;			// 年2
	wMonth2 = 1;			// 月2
	wDay2 = 1;				// 日2
	wHour2 = 0;				// 时2
	wMinute2 = 0;			// 分2
	wSecond2 = 0;			// 秒2
	dwMillisecond2  = 0;	// 毫秒2

	wTimePeriod = 0;		// 时效

	// 层属性
	wLayerNums = 1;
	iCurrentLayerIndex = 0;
	for(i=0; i<1000; i++)
	{
		wLayerCode[i] = 0;
	}

	for(i=0; i<100; i++)
	{
		iAncillaryChannelCode[i] = 0;	// 辅助通道代码
	}

	wProjectType = 0;		// 投影类型
	wPaletteType = 0;		// 调色板类型

	wTotalNum = 0;			// 总的离散点数量(总站点数 总项数),要求大于0
	wElementNum = 0;		// 单站填图要素个数

	dwFlag = 0;				// 站点名称标记

	wContourNums = 0;		// 等值线数量
	wClipArea = 0;			// 剪切区标记 0-
	for(i=0; i<100; i++)
	{
		dContourValue[i] = 0;
		xClip[i] = 0;
		yClip[i] = 0;
	}

	dBoldContourValue = 0;
	wContourPortion = 10;

	wSpecificDataNumbers = 0;						// 特定数据数量
	dwSpecificDataMin = AXIN_SPECIAL_DATA_BEG;		// 特定数据最小值
	dwSpecificDataMax = AXIN_SPECIAL_DATA_END;		// 特定数据最大值


	zNum = 0;				// 

	dwInvalidData = -9999;	// 无效数据
	dBaseValue = 0;			// 基本值
	dScale = 1.0;			// 缩放系数

	// 透明颜色
	bHasTransparentColor = FALSE;
	dwTransparentColor = 0xFFFFFFFF;

	bHasComment = FALSE;			// 是否有注释
	bHasColorTable = FALSE;			// 是否有颜色表

	m_dwDataOffset = 0;
	m_iDataStoreType = 0;

	return;
}

KDataInfo::~KDataInfo(void)
{
}

// 是否无效数据
BOOL KDataInfo::IsInvalidData(double value)
{
	return FALSE
		|| fabs(value - (LONG) InvalidData())			< 1.0e-12
		|| fabs(value - (LONG) SFX_INVALID_DATA)		< 1.0e-12
		|| fabs(value - (LONG) SFX_INVALID_DATA_FLOAT)	< 1.0e-12
		|| fabs(value - (LONG) SFX_INVALID_DATA_DOUBLE)	< 1.0e-12
		|| fabs(value - (DWORD)InvalidData())			< 1.0e-12
		|| fabs(value - (DWORD)SFX_INVALID_DATA)		< 1.0e-12
		|| fabs(value - (DWORD)SFX_INVALID_DATA_FLOAT)	< 1.0e-12
		|| fabs(value - (DWORD)SFX_INVALID_DATA_DOUBLE)	< 1.0e-12
		; 			  
}

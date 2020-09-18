#pragma once

#include "KLDFileProcessor.h"

namespace Agmrs { namespace Mersi
{

	class KIND_EXT_CLASS KMersiDataProcessor : public KLDFileProcessor
	{
	public:
		KMersiDataProcessor(void);
		virtual ~KMersiDataProcessor(void);

	public:
		// 取得像元类型
		virtual int GetPixelType(int i,int j);
		// 产品生成 - 地表温度
		virtual BOOL Lst(int iMethod);
};

//---------------------------------------------------------
}}// namespace Agmrs::Mersi - end
//---------------------------------------------------------

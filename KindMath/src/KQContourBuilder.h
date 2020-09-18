#pragma once

#include "KContourBuilder.h"
#include "KQContourTracer.h"

namespace ContourBuilder
{

	class KIND_EXT_CLASS KQContourBuilder : public KContourBuilder
	{
	public:
		KQContourBuilder(void);
		virtual ~KQContourBuilder(void);

	public:
		//构造等值线
		virtual BOOL BuildContours();	//派生类根据需要重载

	private:
		KQContourTracer m_QContourTracer;	//四边形网格等值线追踪器
	};

//---------------------------------------------------------
}// namespace ContourBuilder - end
//---------------------------------------------------------

#pragma once

#include "KRender.h" 

namespace Renders
{

	class KIND_EXT_CLASS KAgmrsDataRender : public KRender
	{
	public:
		KAgmrsDataRender(void);
		virtual ~KAgmrsDataRender(void);

	public:
		virtual void Rendering(HDC hDC, LPVOID lpvData);
	};

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------

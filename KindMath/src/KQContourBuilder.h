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
		//�����ֵ��
		virtual BOOL BuildContours();	//�����������Ҫ����

	private:
		KQContourTracer m_QContourTracer;	//�ı��������ֵ��׷����
	};

//---------------------------------------------------------
}// namespace ContourBuilder - end
//---------------------------------------------------------

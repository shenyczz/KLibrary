#pragma once

#include "KLDAlgorithm.h"

namespace Agmrs { namespace Avhrr
{

	class KIND_EXT_CLASS KAvhrrAlgorithm : public KLDAlgorithm
	{
	public:
		KAvhrrAlgorithm(void);
		virtual ~KAvhrrAlgorithm(void);
/*
	public:
		//-------------------------------------------------
		//                  植被指数
		//-------------------------------------------------
		// 归一化植被指数
		virtual double Ndvi();
		// 比值植被指数
		virtual double Rvi();
		// 差值植被指数
		virtual double Dvi();
		*/
	};

//---------------------------------------------------------
}}// namespace Agmrs::Avhrr - end
//---------------------------------------------------------

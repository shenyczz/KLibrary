#pragma once

#include "KAgmrsAlgorithm.h"

namespace Agmrs { namespace LDFile
{

	class KIND_EXT_CLASS KLDAlgorithm : public KAgmrsAlgorithm
	{
	public:
		KLDAlgorithm(void);
		~KLDAlgorithm(void);

	public:
		//-------------------------------------------------
		///植被指数
		//-------------------------------------------------
		// 归一化植被指数
		virtual double Ndvi();
		// 比值植被指数
		virtual double Rvi();
		// 差值植被指数
		virtual double Dvi();
		// 土壤调整植被指数
		virtual double Savi();
		// 增强型植被指数
		virtual double Evi();

		// 植被指数导出产品
		virtual double Lai();

		//-------------------------------------------------
		///干旱指数
		//-------------------------------------------------
		// 植被供水指数
		virtual double Vswi() { return AGMRS_INVALID_DATA; }
		// 地表含水量指数
		virtual double Swci() { return AGMRS_INVALID_DATA; }
		//-------------------------------------------------
		///其他指数
		//-------------------------------------------------
		// 归一化积雪指数
		virtual double Ndsi() { return AGMRS_INVALID_DATA; }
		//-------------------------------------------------
		/// 地表温度
		//-------------------------------------------------
		virtual double Lst(int iMethod = 0);

	protected:
		// 水汽
		double Vapour();
		// 植被覆盖度
		double Pv(double ndvi_min, double ndvi_max);
		// 地表比辐射率
		double Lse(double es);
		// 热辐射计算
		double Lsensor(double Tb);

	protected:
		double Lst_regress();

		KFormulaParser m_FormulaParser;

	};

//---------------------------------------------------------
}}// namespace Agmrs::LDFile - end
//---------------------------------------------------------

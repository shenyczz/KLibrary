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
		///ֲ��ָ��
		//-------------------------------------------------
		// ��һ��ֲ��ָ��
		virtual double Ndvi();
		// ��ֲֵ��ָ��
		virtual double Rvi();
		// ��ֲֵ��ָ��
		virtual double Dvi();
		// ��������ֲ��ָ��
		virtual double Savi();
		// ��ǿ��ֲ��ָ��
		virtual double Evi();

		// ֲ��ָ��������Ʒ
		virtual double Lai();

		//-------------------------------------------------
		///�ɺ�ָ��
		//-------------------------------------------------
		// ֲ����ˮָ��
		virtual double Vswi() { return AGMRS_INVALID_DATA; }
		// �ر�ˮ��ָ��
		virtual double Swci() { return AGMRS_INVALID_DATA; }
		//-------------------------------------------------
		///����ָ��
		//-------------------------------------------------
		// ��һ����ѩָ��
		virtual double Ndsi() { return AGMRS_INVALID_DATA; }
		//-------------------------------------------------
		/// �ر��¶�
		//-------------------------------------------------
		virtual double Lst(int iMethod = 0);

	protected:
		// ˮ��
		double Vapour();
		// ֲ�����Ƕ�
		double Pv(double ndvi_min, double ndvi_max);
		// �ر�ȷ�����
		double Lse(double es);
		// �ȷ������
		double Lsensor(double Tb);

	protected:
		double Lst_regress();

		KFormulaParser m_FormulaParser;

	};

//---------------------------------------------------------
}}// namespace Agmrs::LDFile - end
//---------------------------------------------------------

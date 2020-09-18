#pragma once

#include "KLDAlgorithm.h"

namespace Agmrs { namespace Mersi
{

	class KIND_EXT_CLASS KMersiAlgorithm : public KLDAlgorithm
	{
	public:
		KMersiAlgorithm(void);
		virtual ~KMersiAlgorithm(void);

	public:
		virtual double Lst(int iMethod = 0);
	};

//---------------------------------------------------------
}}// namespace Agmrs::Mersi - end
//---------------------------------------------------------

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
		// ȡ����Ԫ����
		virtual int GetPixelType(int i,int j);
		// ��Ʒ���� - �ر��¶�
		virtual BOOL Lst(int iMethod);
};

//---------------------------------------------------------
}}// namespace Agmrs::Mersi - end
//---------------------------------------------------------

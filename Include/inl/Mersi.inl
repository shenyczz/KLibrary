#ifndef __INCLUDED_AGMRS_MERSI_INL__
#define __INCLUDED_AGMRS_MERSI_INL__

namespace Agmrs { namespace Mersi
{
//---------------------------------------------------------
}}// namespace Agmrs::Mersi - end
//---------------------------------------------------------
using namespace Agmrs::Mersi;

namespace Agmrs { namespace Mersi
{

//----------------------------------------------------------
// MERSI �������� - Beg
//--------------------
//#define AGMRS_MERSI_DATA_MARKER					AGMRS_LOCAL_DATA_MARKER
#define AGMRS_MERSI_DATA_CHANNEL_NUM_MAX		20						// MERSI����ͨ�������ֵ
#define AGMRS_MERSI_DATA_CHANNEL_NUMBERS		20						// MERSI����ͨ����
#define AGMRS_MERSI_DATA_VALUE_MAX				0x0FFF					// MERSIͨ���������ֵ

//--------------------
// MERSI �������� - End
//----------------------------------------------------------

#pragma pack(1)

	struct MersiChannel
	{
		enum
		{
			Red = 3,		// 0.645um - ����ͨ��
			Nir = 4,		// 0.859um - ������ͨ��
			Blu = 1,		// 0.469um - ����ͨ��
			Grn = 2,		// 0.555um - �̹�ͨ��
			C940nm = 16,	// 940nm - 940umͨ��
			T11um = 5,		// 11.00um - �Ⱥ���ͨ��
		};
	};

#pragma pack()

//---------------------------------------------------------
}}// namespace Agmrs::Mersi - end
//---------------------------------------------------------

#endif	//#ifndef __INCLUDED_AGMRS_MERSI_INL__

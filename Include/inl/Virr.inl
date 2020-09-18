#ifndef __INCLUDED_AGMRS_VIRR_INL__
#define __INCLUDED_AGMRS_VIRR_INL__

namespace Agmrs { namespace Virr
{
//---------------------------------------------------------
}}// namespace Agmrs::Virr - end
//---------------------------------------------------------
using namespace Agmrs::Virr;

namespace Agmrs { namespace Virr
{

//----------------------------------------------------------
// VIRR �������� - Beg
//--------------------
//#define AGMRS_VIRR_DATA_MARKER				AGMRS_LOCAL_DATA_MARKER
#define AGMRS_VIRR_DATA_CHANNEL_NUM_MAX		20						// VIRR ����ͨ�������ֵ
#define AGMRS_VIRR_DATA_CHANNEL_NUMBERS		10						// VIRR ����ͨ����
#define AGMRS_VIRR_DATA_VALUE_MAX			0x0FFF					// VIRR ͨ���������ֵ

//--------------------
// AVHRR �������� - End
//----------------------------------------------------------

#pragma pack(1)

	struct VirrChannel
	{
		enum
		{
			Red = 1,	// 0.645um - ����ͨ��
			Nir = 2,	// 0.859um - ������ͨ��
			Blu = 7,	// 0.469um - ����ͨ��
			Grn = 9,	// 0.555um - �̹�ͨ��
			T3930nm = 3,	// 3930nm	- 3930nmͨ��
			T11um = 4,	// 11.00um - �Ⱥ���ͨ��
			T12um = 5,	// 12.00um - �Ⱥ���ͨ��
		};
	};

#pragma pack()

//---------------------------------------------------------
}}// namespace Agmrs::Virr - end
//---------------------------------------------------------

#endif	//#ifndef __INCLUDED_AGMRS_VIRR_INL__

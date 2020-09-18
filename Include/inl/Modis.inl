#ifndef __INCLUDED_AGMRS_MODIS_INL__
#define __INCLUDED_AGMRS_MODIS_INL__

namespace Agmrs { namespace Modis
{
//---------------------------------------------------------
}}// namespace Agmrs::Modis - end
//---------------------------------------------------------
using namespace Agmrs::Modis;

namespace Agmrs { namespace Modis
{

//----------------------------------------------------------
// MODIS�������� - Beg
//--------------------
#define AGMRS_MODIS_DATA_VALUE_MAX				0x0FFF						// MODISͨ���������ֵ
#define AGMRS_MODIS_DATA_INVALID				0x0FFF						// MODISͨ��������Чֵ
#define AGMRS_MODIS_DATA_INVALID2				0x0FFE						// MODISͨ��������Чֵ2

#define AGMRS_MODIS_DATA_CHANNEL_NUM_MAX		40							// MODIS����ͨ�������ֵ
#define AGMRS_MODIS_DATA_CHANNEL_NUM			36							// MODIS����ͨ����
//--------------------
// MODIS�������� - End
//----------------------------------------------------------

#pragma pack(1)

	struct ModisChannel
	{
		enum
		{
			Red = 1,	// 0.645um - ����ͨ��
			Nir = 2,	// 0.859um - ������ͨ��
			Blu = 3,	// 0.469um - ����ͨ��
			Grn = 4,	// 0.555um - �̹�ͨ��
			C940nm = 19,	// 940nm	- 940nmͨ��
			T3930nm = 21,	// 3930nm	- 3930nmͨ��
			T11um = 31,	// 11.00um - �Ⱥ���ͨ��
			T12um = 32,	// 12.00um - �Ⱥ���ͨ��
		};
	};

#pragma pack()

//---------------------------------------------------------
}}// namespace Agmrs::Modis - end
//---------------------------------------------------------

#endif	//#ifndef __INCLUDED_AGMRS_MODIS_INL__

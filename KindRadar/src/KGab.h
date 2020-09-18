#pragma once

namespace Wsr98d { namespace Rpg { namespace Helper
{

	class KIND_EXT_CLASS KGab
	{
	public:
		KGab(void);
		virtual ~KGab(void);

	protected:
		double ComputeLongitude(double lon0,double lat0,double azimuth,double range);
		double ComputeLatitude(double lon0,double lat0,double azimuth,double range);
	};

//---------------------------------------------------------
}}}// namespace Wsr98d::Rpg::Helper - end
//---------------------------------------------------------

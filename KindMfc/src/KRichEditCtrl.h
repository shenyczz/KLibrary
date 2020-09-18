#pragma once

// KRichEditCtrl

namespace Gui
{

	class KIND_EXT_CLASS KRichEditCtrl : public CRichEditCtrl
	{
		DECLARE_DYNAMIC(KRichEditCtrl)

	public:
		KRichEditCtrl();
		virtual ~KRichEditCtrl();

	protected:
		DECLARE_MESSAGE_MAP()
	};

}// namespace Gui - end


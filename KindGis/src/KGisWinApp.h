#pragma once



namespace Framework
{

	// KGisWinApp

	class KIND_EXT_CLASS KGisWinApp : public KWinAppEx
	{
		DECLARE_DYNCREATE(KGisWinApp)

	protected:
		KGisWinApp();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
		virtual ~KGisWinApp();

	public:
		virtual BOOL InitInstance();
		virtual int ExitInstance();

	protected:
		DECLARE_MESSAGE_MAP()
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------


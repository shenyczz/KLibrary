#pragma once

#include "KMouseListener.h"

namespace Mouse
{

	class KIND_EXT_CLASS KMouseListenerCollection
	{
	public:
		KMouseListenerCollection(void);
		virtual ~KMouseListenerCollection(void);

	public:
		// ���õ�ǰ����������ֵ
		int SetCurrentMouseListenerKey(int theKey, int theSubKey = GisTool_None);
		// ȡ�õ�ǰ����������ֵ
		int GetCurrentMouseListenerKey() { return m_nCurrentMouseListenerKey; }
		// ȡ�õ�ǰ���������Ӽ�ֵ
		int GetCurrentMouseListenerSubKey() { return m_nCurrentMouseListenerSubKey; }

	public:
		// ���һ����������
		void AddMouseListener(int key, KMouseListener* pMouseListener);

		// ������������
		KMouseListener* Find(int key);

		// ȡ�õ�ǰ��������
		KMouseListener* GetCurrentMouseListener();

		// �Ƴ�һ����������
		void RemoveMouseListener(int key);
		void RemoveMouseListener(KMouseListener* pMouseListener);

		// �Ƴ�ȫ����������
		void RemoveAllMouseListener();


	private:
		// ����������
		int m_nCurrentMouseListenerKey;
		// �������Ӽ�
		int m_nCurrentMouseListenerSubKey;

	private:
		// ��������ӳ��
		KMouseListenerMap m_MouselistenerContainer;
	};

//---------------------------------------------------------
}// namespace Mouse - end
//---------------------------------------------------------

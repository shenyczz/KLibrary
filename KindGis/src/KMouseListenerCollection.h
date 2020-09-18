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
		// 设置当前鼠标监听器键值
		int SetCurrentMouseListenerKey(int theKey, int theSubKey = GisTool_None);
		// 取得当前鼠标监听器键值
		int GetCurrentMouseListenerKey() { return m_nCurrentMouseListenerKey; }
		// 取得当前鼠标监听器子键值
		int GetCurrentMouseListenerSubKey() { return m_nCurrentMouseListenerSubKey; }

	public:
		// 添加一个鼠标监听器
		void AddMouseListener(int key, KMouseListener* pMouseListener);

		// 查找鼠标监听器
		KMouseListener* Find(int key);

		// 取得当前鼠标监听器
		KMouseListener* GetCurrentMouseListener();

		// 移除一个鼠标监听器
		void RemoveMouseListener(int key);
		void RemoveMouseListener(KMouseListener* pMouseListener);

		// 移除全部鼠标监听器
		void RemoveAllMouseListener();


	private:
		// 鼠标监听主键
		int m_nCurrentMouseListenerKey;
		// 鼠标监听子键
		int m_nCurrentMouseListenerSubKey;

	private:
		// 鼠标监听器映射
		KMouseListenerMap m_MouselistenerContainer;
	};

//---------------------------------------------------------
}// namespace Mouse - end
//---------------------------------------------------------

#include "StdAfx.h"
#include "KMouseListenerCollection.h"

namespace Mouse
{

	KMouseListenerCollection::KMouseListenerCollection(void)
	{
		m_nCurrentMouseListenerKey = GisTool_None;
		m_nCurrentMouseListenerSubKey = GisTool_None;
	}

	KMouseListenerCollection::~KMouseListenerCollection(void)
	{
		RemoveAllMouseListener(); 
		m_nCurrentMouseListenerKey = GisTool_None;
		m_nCurrentMouseListenerSubKey = GisTool_None;
	}

	// 设置当前鼠标监听器键值
	int KMouseListenerCollection::SetCurrentMouseListenerKey(int theKey, int theSubKey)
	{
		int oldKey = m_nCurrentMouseListenerKey;

		m_nCurrentMouseListenerKey = theKey;
		m_nCurrentMouseListenerSubKey = theSubKey;

		return oldKey ;
	}

	// 添加一个鼠标监听器
	void KMouseListenerCollection::AddMouseListener(int key, KMouseListener* pMouseListener)
	{
		if ( pMouseListener == NULL )
			return;

		m_MouselistenerContainer.insert(KMouseListenerMap::value_type(key,pMouseListener) ) ;

		this->SetCurrentMouseListenerKey(key,GisTool_None);
	}

	// 查找鼠标监听器
	KMouseListener* KMouseListenerCollection::Find(int key)
	{
		return m_MouselistenerContainer[key];
	}

	// 取得当前鼠标监听器
	KMouseListener* KMouseListenerCollection::GetCurrentMouseListener()
	{
		return m_MouselistenerContainer[m_nCurrentMouseListenerKey];
	}

	// 移除一个鼠标监听器
	void KMouseListenerCollection::RemoveMouseListener(int key)
	{
		KMouseListenerMap::iterator itr = m_MouselistenerContainer.find(key);

		if( itr != m_MouselistenerContainer.end() )
		{
			KMouseListener* pMouseListener = (*itr).second;
			_delete(pMouseListener);
			(*itr).second = NULL;
		}

		m_MouselistenerContainer.erase(itr);
	}
	void KMouseListenerCollection::RemoveMouseListener(KMouseListener* pMouseListener)
	{
		KMouseListenerMap::iterator itr;
		KMouseListenerMap::iterator ibeg = m_MouselistenerContainer.begin();
		KMouseListenerMap::iterator iend = m_MouselistenerContainer.end();
		for( itr = ibeg; itr != iend; itr++ )
		{
			if( (*itr).second == pMouseListener)
				break;
		}

		if( itr != iend )
		{
			KMouseListener* p = (*itr).second;
			_delete(p);
			(*itr).second = NULL;
			pMouseListener = NULL;

			m_MouselistenerContainer.erase(itr);
		}
	}

	// 移除全部鼠标监听器
	void KMouseListenerCollection::RemoveAllMouseListener()
	{
		KMouseListenerMap::iterator itr;
		KMouseListenerMap::iterator ibeg = m_MouselistenerContainer.begin();
		KMouseListenerMap::iterator iend = m_MouselistenerContainer.end();
		for( itr = ibeg; itr != iend; itr++ )
		{
			KMouseListener* pMouseListener = (*itr).second;
			if(pMouseListener)
			{
				_delete(pMouseListener);
				(*itr).second = NULL;
			}
		}

		m_MouselistenerContainer.clear();
	}

//---------------------------------------------------------
}// namespace Mouse - end
//---------------------------------------------------------

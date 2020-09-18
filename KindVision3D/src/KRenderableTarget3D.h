/******************************************************************************
 **
 ** Announce: If it works, I know it was written by
 **           ShenYongchen(shenyczz@163.com), ZhengZhou, HeNan.
 **           if not, then I don't know nothing.
 **
 **     Name: KRenderEngine - ‰÷»æ“˝«Ê
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: …Í”¿≥Ω.÷£÷› (shenyczz@163.com)
 ** DateTime: 2000 - 
 ** Web Site: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

#include "KTarget3D.h"
#include "TPtrCollection.h"
using namespace Collections;

namespace Vision3D
{
}// namespace Vision3D

namespace Vision3D { namespace Rendering
{
	class KRenderEngine;
}}// namespace Vision3D::Rendering


namespace Vision3D
{

	class KIND_EXT_CLASS KRenderableTarget3D : public KTarget3D
	{
	public:
		KRenderableTarget3D(void);
		virtual ~KRenderableTarget3D(void);

	public:
		virtual void Initialize(KRenderEngine* pRenderEngine)	{}
		virtual void Update(KRenderEngine* pRenderEngine)		{}
		virtual void Render(KRenderEngine* pRenderEngine)		{}

	public:
		TPtrCollection<KRenderableTarget3D*>* Children() { return m_pChildren; }

	public:
		BOOL IsInitialized;

	protected:
		TPtrCollection<KRenderableTarget3D*>* m_pChildren;
	};

}// namespace Vision3D

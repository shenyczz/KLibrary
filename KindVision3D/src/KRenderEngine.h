/******************************************************************************
 **
 ** Announce: If it works, I know it was written by
 **           ShenYongchen(shenyczz@163.com), ZhengZhou, HeNan.
 **           if not, then I don't know nothing.
 **
 **     Name: KRenderEngine - äÖÈ¾ÒýÇæ
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: ÉêÓÀ³½.Ö£ÖÝ (shenyczz@163.com)
 ** DateTime: 2000 - 
 ** Web Site: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

#include "KWorld.h"

typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstring;

namespace Vision3D
{
}// namespace Vision3D

namespace Vision3D { namespace Rendering
{
}}// namespace Vision3D::Rendering

namespace Vision3D { namespace Rendering
{

	class KIND_EXT_CLASS KRenderEngine
	{
	public:
		KRenderEngine();
		virtual ~KRenderEngine(void);

	public:
		// ´°¿Ú¾ä±ú
		virtual void SetHwnd(HWND hWnd) { m_hWnd = hWnd; }
		virtual HWND GetHwnd() { return m_hWnd; }

		IDirect3DDevice9* GetDevice3d() { return m_pDevice3d; }
		void SetDevice3d(IDirect3DDevice9* pDevice) { m_pDevice3d = pDevice; }

		BOOL IsRendering() { return m_IsRendering; }

		void BeginRender() { m_IsRendering = TRUE; }
		void EndRender() { m_IsRendering = FALSE; }

		KWorld* World() { return m_pWorld; }

	public:
		BOOL InitD3D();
		void Render();
		void Present();

	public:
		//__declspec(property(get=GetDevice3d,put=SetDevice3d)) IDirect3DDevice9* Device3d;

	protected:
		void Test();

	protected:
		void Cleanup();

	protected:
		HWND m_hWnd;

		IDirect3D9* m_pD3D;
		IDirect3DDevice9* m_pDevice3d;

		D3DXMATRIX	m_WorldMatrix;


	protected:
		KWorld* m_pWorld;
		BOOL m_IsRendering;

	private:
	};

}}// namespace Vision3D::Rendering

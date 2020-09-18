#include "StdAfx.h"
#include "KRenderEngine.h"

#include "CustomVertex.h"

namespace Vision3D { namespace Rendering
{

	KRenderEngine::KRenderEngine()
	{
		m_hWnd = NULL;
		m_pD3D = NULL;
		m_pDevice3d = NULL;
		m_IsRendering = FALSE;

		m_pWorld = new KWorld();
	}


	KRenderEngine::~KRenderEngine(void)
	{
		Cleanup();
	}

	BOOL KRenderEngine::InitD3D()
	{
		HWND hWnd = m_hWnd;
		if(NULL == hWnd)
			return FALSE;

		// 1.创建 D3D 对象, 用来创建 D3DDevice
		if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
			return FALSE;

		// 2.检查硬件,确定顶点处理方式
		D3DCAPS9 caps;
		D3DDEVTYPE deviceType = D3DDEVTYPE_HAL;
		m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

		int vp = 0;
		if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
			vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else
			vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		// 3.设置呈现参数
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp,sizeof(d3dpp));
		//d3dpp.BackBufferWidth            = 0;
		//d3dpp.BackBufferHeight           = 0;
		d3dpp.BackBufferFormat           = D3DFMT_UNKNOWN;	//D3DFMT_A8R8G8B8;
		//d3dpp.BackBufferCount            = 1;
		//d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
		//d3dpp.MultiSampleQuality         = 0;
		d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
		//d3dpp.hDeviceWindow              = hWnd;
		d3dpp.Windowed                   = TRUE;
		//d3dpp.EnableAutoDepthStencil     = true; 
		//d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;	// D3DFMT_D16
		//d3dpp.Flags                      = 0;
		//d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		//d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

		// 4.创建 D3DDevice
		if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,vp,&d3dpp, &m_pDevice3d)))
			return FALSE;

		// 设备状态设置
		// ...

		// 初始化数据
		//Test();

		return TRUE;
	}

	void KRenderEngine::Cleanup()
	{
		if( m_pDevice3d != NULL )
		{
			m_pDevice3d->Release();
			m_pDevice3d = NULL;
		}

		if( m_pD3D != NULL )
		{
			m_pD3D->Release();
			m_pD3D = NULL;
		}

		_delete(m_pWorld);
	}

	void KRenderEngine::Render()
	{
		IDirect3DDevice9* pDevice3d = GetDevice3d();
		if(!pDevice3d)
			return;

		this->BeginRender();
		//-------------------------------------------------
		try
		{
			// 初始化Direct3D目标(Target,ZBuffer)
			pDevice3d->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 128 ), 1.0f, 0 );

			if(SUCCEEDED(pDevice3d->BeginScene()))
			{
				//-------------------------------------
				// 渲染 3D
				//---------
				// 设置填充模式
				//device3d.RenderState.FillMode = FillMode.WireFrame;
				//device3d.RenderState.FillMode = FillMode.Solid;

				// Enables or disables depth buffering.
				//device3d.RenderState.ZBufferEnable = true;

				//Test();

				// 绘制天体
				if(m_pWorld)
				{
					m_pWorld->Render(this);
				}
				//-------------------------------------
				// 渲染 2D
				//---------
				//device3d.RenderState.ZBufferEnable = false;
				//-------------------------------------
				pDevice3d->EndScene();
			}

			// 
			//pDevice3d->Present(NULL,NULL,NULL,NULL);
		}
		catch (...)
		{
			throw "KRenderEngine::Render";
		}
		//-------------------------------------------------
		this->EndRender();
		return;

		/*
		long startTicks = 0;
		PerformanceTimer.QueryPerformanceCounter(ref startTicks);

		try
		{
			// 引擎绘制开始
			this.BeginRender();

			// 初始化Direct3D目标(Target,ZBuffer)
			device3d.Clear(ClearFlags.Target | ClearFlags.ZBuffer, this.Background, 1.0f, 0);

			// 没有初始化天体
			if (m_World == null)
			{
				device3d.BeginScene();
				device3d.EndScene();
				device3d.Present();
				Thread.Sleep(25);
				return;
			}

			//if (m_WorkerThread == null)
			if (false)
			{
				m_WorkerThreadRunning = true;
				m_WorkerThread = new Thread(new ThreadStart(WorkerThreadFunc));
				m_WorkerThread.Name = "RenderEngine.WorkerThreadFunc";
				m_WorkerThread.IsBackground = true;
				if (World.Settings.UseBelowNormalPriorityUpdateThread)
				{
					m_WorkerThread.Priority = ThreadPriority.BelowNormal;
				}
				else
				{
					m_WorkerThread.Priority = ThreadPriority.Normal;
				}
				// BelowNormal makes rendering smooth, but on slower machines updates become slow or stops
				// TODO: Implement dynamic FPS limiter (or different solution)
				m_WorkerThread.Start();
			}

			// 更新3D设备
			this.WorldCamera.Update(device3d);

			//-----------------------------------------
			device3d.BeginScene();
			{
				//-------------------------------------
				// 渲染 3D
				//---------
				// 设置填充模式
				//device3d.RenderState.FillMode = FillMode.WireFrame;
				device3d.RenderState.FillMode = FillMode.Solid;

				// Enables or disables depth buffering.
				device3d.RenderState.ZBufferEnable = true;

				// 绘制天体
				m_World.Render(this);
				//-------------------------------------
				// 渲染 2D
				//---------
				device3d.RenderState.ZBufferEnable = false;
				//-------------------------------------
			}
			device3d.EndScene();
			//-----------------------------------------
		}
		catch (Exception ex)
		{
			throw ex;
		}
		finally
		{
			// 引擎绘制结束
			this.EndRender();
		}

		// 更新鼠标光标
		//drawArgs.UpdateMouseCursor(this);

		return;
		*/
	}

	void KRenderEngine::Present()
	{
		IDirect3DDevice9* pDevice3d = GetDevice3d();
		if(!pDevice3d) return;

		pDevice3d->Present(NULL,NULL,NULL,NULL);
	}

	void KRenderEngine::Test()
	{
		IDirect3DDevice9* pDevice3d = GetDevice3d();
		if(!pDevice3d)
			return;

		return;
	}

}}// namespace Vision3D::Rendering

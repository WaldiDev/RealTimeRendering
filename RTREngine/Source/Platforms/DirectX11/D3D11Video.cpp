#include "D3D11Video.h"
#include "DirectX11.h"
#include "Core/Window.h"
#include "Core/Logging/Log.h"
#include "D3D11Viewport.h"

namespace
{
	bool CreateDeviceAndContext(ID3D11Device** device, ID3D11DeviceContext1** deviceContext1)
	{
		RTR_ENGINE_TRACE("Start CreateDeviceAndContext");

		HRESULT result = E_FAIL;
		D3D_FEATURE_LEVEL usedFeatureLevel;
		const D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
		};

		UINT deviceFlags = 0;
#ifdef RTR_DEBUG
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		ID3D11DeviceContext* deviceContext = nullptr;
		result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, featureLevels, 1, D3D11_SDK_VERSION, device, &usedFeatureLevel, &deviceContext);
		if (FAILED(result))
		{
			RTR_PRINT_RESULT_WITH_MESSAGE("Unable to initialize D3D11", result);
			return false;
		}

		result = deviceContext->QueryInterface(IID_PPV_ARGS(deviceContext1));
		if (FAILED(result))
		{
			RTR_PRINT_RESULT_WITH_MESSAGE("QueryInterface for DeviceContext failed", result);
			return false;
		}

		deviceContext->Release();

		RTR_ENGINE_TRACE("Finished CreateDeviceAndContext");
		return true;
	}

	bool CreateSpawnChain(ID3D11Device* device, const rtr::Window& window, UINT swapChainFlags, IDXGISwapChain1** swapChain1)
	{
		RTR_ENGINE_TRACE("Start CreateSpawnChain");

		HRESULT result = E_FAIL;

		IDXGIDevice2* dxgiDevice = nullptr;
		result = device->QueryInterface(IID_PPV_ARGS(&dxgiDevice));

		IDXGIAdapter* dxgiAdapter = nullptr;
		result = dxgiDevice->GetAdapter(&dxgiAdapter);

		IDXGIFactory2* dxgiFactory = nullptr;
		result = dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width					= window.GetClientWidth();
		swapChainDesc.Height				= window.GetClientHeight();
		swapChainDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SampleDesc.Count		= 1;
		swapChainDesc.SampleDesc.Quality	= 0;
		swapChainDesc.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount			= 2;
		swapChainDesc.SwapEffect			= DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		swapChainDesc.Flags					= swapChainFlags;

		result = dxgiFactory->CreateSwapChainForHwnd(device, static_cast<HWND>(window.GetNativeWindow()), &swapChainDesc, nullptr, nullptr, swapChain1);

		dxgiFactory->Release();
		dxgiAdapter->Release();
		dxgiDevice->Release();
		if (FAILED(result))
		{
			RTR_PRINT_RESULT_WITH_MESSAGE("CreateSwapChainForHwnd failed", result);
			return false;
		}

		RTR_ENGINE_TRACE("Finished CreateSpawnChain");
		return true;
	}

	bool CreateBackBuffer(ID3D11Device* device, IDXGISwapChain1* swapChain1, ID3D11RenderTargetView** backBuffer)
	{
		RTR_ENGINE_TRACE("Start CreateBackBuffer");

		HRESULT result = E_FAIL;
		ID3D11Texture2D* backBufferTexture = nullptr;
		swapChain1->GetBuffer(0, IID_PPV_ARGS(&backBufferTexture));

		result = device->CreateRenderTargetView(backBufferTexture, nullptr, backBuffer);

		backBufferTexture->Release();
		if (FAILED(result))
		{
			RTR_PRINT_RESULT_WITH_MESSAGE("CreateRenderTargetView failed", result);
			return false;
		}

		RTR_ENGINE_TRACE("Finished CreateBackBuffer");
		return true;
	}

#ifdef RTR_DEBUG
	bool CreateDebugLayer(ID3D11Device* device, ID3D11Debug** debug, ID3D11InfoQueue** infoQueue)
	{
		RTR_ENGINE_TRACE("Start CreateDebugLayer");

		HRESULT result = E_FAIL;
		result = device->QueryInterface(IID_PPV_ARGS(debug));
		if (FAILED(result))
		{
			RTR_PRINT_RESULT_WITH_MESSAGE("QueryInterface for ID3D11Debug failed", result);
			return false;
		}

		result = (*debug)->QueryInterface(IID_PPV_ARGS(infoQueue));
		if (FAILED(result))
		{
			RTR_PRINT_RESULT_WITH_MESSAGE("QueryInterface for ID3D11InfoQueue failed", result);
			return false;
		}

		(*infoQueue)->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
		(*infoQueue)->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);

		//D3D11_MESSAGE_ID hide[] =
		//{
		//	D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
		//	// Add more message IDs here as needed 
		//};
		//D3D11_INFO_QUEUE_FILTER filter = {};
		//filter.DenyList.NumIDs = _countof(hide);
		//filter.DenyList.pIDList = hide;
		//mInfoQueue->AddStorageFilterEntries(&filter);

		RTR_ENGINE_TRACE("Finished CreateDebugLayer");
		return true;
	}
#endif // RTR_DEBUG

	bool CheckForTearingSupport()
	{
		// Rather than create the 1.5 factory interface directly, we create the 1.4 interface and query for the 1.5 interface. This will enable the graphics debugging tools which might not support the 1.5 factory interface.
		IDXGIFactory4* dxgiFactory4 = nullptr;
		if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory4))))
		{
			return false;
		}

		IDXGIFactory5* dxgiFactory5 = nullptr;
		if (FAILED(dxgiFactory4->QueryInterface(IID_PPV_ARGS(&dxgiFactory5))))
		{
			dxgiFactory4->Release();
			return false;
		}

		BOOL allowTearing = FALSE;
		HRESULT result = dxgiFactory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
		
		dxgiFactory5->Release();
		dxgiFactory4->Release();

		return SUCCEEDED(result) && allowTearing;
	}
}

namespace rtr
{
	Video* Video::CreateVideo(const Window& window)
	{
		return new D3D11Video(window);
	}

	D3D11Video::D3D11Video(const Window& window)
		: mWindow(window)
		, mVSyncEnabled(true)
		, mTearingSupport(::CheckForTearingSupport())
		, mDevice(nullptr)
		, mDeviceContext(nullptr)
		, mViewport(nullptr)
		, mBlendState(nullptr)
		, mClearColor{ 0.1f, 0.1f, 0.1f, 1.0f }
#ifdef RTR_DEBUG
		, mDebug(nullptr)
		, mInfoQueue(nullptr)
#endif // RTR_DEBUG
	{
		if (!::CreateDeviceAndContext(&mDevice, &mDeviceContext))
		{
			assert(true);
			return;
		}

#ifdef RTR_DEBUG
		if (!::CreateDebugLayer(mDevice, &mDebug, &mInfoQueue))
		{
		}
#endif // RTR_DEBUG

		mViewport = new D3D11Viewport(mDevice, window);
	}

	D3D11Video::~D3D11Video()
	{
		delete mViewport;

		mDeviceContext->ClearState();
		mDeviceContext->Flush();
		SAFE_RELEASE(mDeviceContext);

#ifdef RTR_DEBUG
		SAFE_RELEASE(mInfoQueue);
		mDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
		SAFE_RELEASE(mDebug);
#endif

		SAFE_RELEASE(mDevice);
	}

	void D3D11Video::OnResize(uint32_t width, uint32_t height)
	{
		mDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		mViewport->Resize(mDevice, width, height);
	}

	void D3D11Video::BeginRender()
	{
		/*RECT clientRect = {}
		::GetClientRect(static_cast<HWND>(window.GetNativeWindow()), &clientRect);

		if ((clientRect.right - clientRect.left) != Window.GetClientWidth() || (clientRect.bottom - clientRect.top) != Window.GetClientHeight())
		{
			mSwapChain1->ResizeBuffers(2, (clientRect.right - clientRect.left), )
		}*/

		// Clear the back buffer.
		mDeviceContext->OMSetRenderTargets(1, &mViewport->mRenderTargetView, NULL);
		mDeviceContext->ClearRenderTargetView(mViewport->mRenderTargetView, mClearColor);
	}

	void D3D11Video::EndRender()
	{
		mViewport->Present(mVSyncEnabled);
	}
}
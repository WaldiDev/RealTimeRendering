#include "D3D11Video.h"
#include "DirectX11.h"
#include "Core/Window.h"
#include "Core/Logging/Log.h"

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
			RTR_ENGINE_ERROR("Unable to initialize D3D11. D3D11CreateDevice failed");
			return false;
		}

		result = deviceContext->QueryInterface(IID_PPV_ARGS(deviceContext1));
		if (FAILED(result))
		{
			RTR_ENGINE_ERROR("Unable to initialize D3D11. QueryInterface for DeviceContext failed");
			return false;
		}

		deviceContext->Release();

		RTR_ENGINE_TRACE("Finished CreateDeviceAndContext");
		return true;
	}

	bool CreateSpawnChain(ID3D11Device* device, const rtr::Window& window, IDXGISwapChain1** swapChain1)
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
		swapChainDesc.Width					= window.GetWidth();
		swapChainDesc.Height				= window.GetHeight();
		swapChainDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		swapChainDesc.SampleDesc.Count		= 1;
		swapChainDesc.SampleDesc.Quality	= 0;
		swapChainDesc.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount			= 2;

		result = dxgiFactory->CreateSwapChainForHwnd(device, static_cast<HWND>(window.GetNativeWindow()), &swapChainDesc, nullptr, nullptr, swapChain1);

		dxgiFactory->Release();
		dxgiAdapter->Release();
		dxgiDevice->Release();
		if (FAILED(result))
		{
			RTR_ENGINE_ERROR("Unable to initialize D3D11. CreateSwapChainForHwnd failed");
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
			RTR_ENGINE_ERROR("Unable to initialize D3D11. CreateBackBuffer failed");
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
			RTR_ENGINE_ERROR("Unable to initialize D3D11. QueryInterface for ID3D11Debug failed");
			return false;
		}

		result = (*debug)->QueryInterface(IID_PPV_ARGS(infoQueue));
		if (FAILED(result))
		{
			RTR_ENGINE_ERROR("Unable to initialize D3D11. QueryInterface for ID3D11InfoQueue failed");
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
}

namespace rtr
{
	Video* Video::CreateVideo(const Window& window)
	{
		return new D3D11Video(window);
	}

	D3D11Video::D3D11Video(const Window& window)
		: mWindow(window)
		, mDevice(nullptr)
		, mDeviceContext1(nullptr)
		, mSwapChain1(nullptr)
		, mBackBuffer(nullptr)
		, mBlendState(nullptr)
		, mClearColor{ 0.1f, 0.1f, 0.1f, 1.0f }
#ifdef RTR_DEBUG
		, mDebug(nullptr)
		, mInfoQueue(nullptr)
#endif // RTR_DEBUG
	{
		if (!::CreateDeviceAndContext(&mDevice, &mDeviceContext1))
		{
			return;
		}

#ifdef RTR_DEBUG
		if (!::CreateDebugLayer(mDevice, &mDebug, &mInfoQueue))
		{
			return;
		}
#endif // RTR_DEBUG

		if (!::CreateSpawnChain(mDevice, mWindow, &mSwapChain1))
		{
			return;
		}

		if (!::CreateBackBuffer(mDevice, mSwapChain1, &mBackBuffer))
		{
			return;
		}
	}

	D3D11Video::~D3D11Video()
	{
		SAFE_RELEASE(mBackBuffer);
		SAFE_RELEASE(mSwapChain1);

		mDeviceContext1->ClearState();
		mDeviceContext1->Flush();
		SAFE_RELEASE(mDeviceContext1);

#ifdef RTR_DEBUG
		SAFE_RELEASE(mInfoQueue);
		mDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
		SAFE_RELEASE(mDebug);
#endif

		SAFE_RELEASE(mDevice);
	}

	void D3D11Video::BeginRender()
	{
		// Clear the back buffer.
		mDeviceContext1->OMSetRenderTargets(1, &mBackBuffer, NULL);
		mDeviceContext1->ClearRenderTargetView(mBackBuffer, mClearColor);
	}

	void D3D11Video::EndRender()
	{
		mSwapChain1->Present(1, 0);
	}
}
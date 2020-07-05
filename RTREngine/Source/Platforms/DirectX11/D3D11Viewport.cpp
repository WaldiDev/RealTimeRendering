#include "D3D11Viewport.h"
#include "DirectX11.h"
#include "Core/Window.h"
#include "Core/Logging/Log.h"

namespace
{
	SwapChain* CreateSwapChain(ID3D11Device* device, const rtr::Window& window)
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
		swapChainDesc.SwapEffect			= DXGI_SWAP_EFFECT_FLIP_DISCARD;

		SwapChain* swapChain = nullptr;
		result = dxgiFactory->CreateSwapChainForHwnd(device, static_cast<HWND>(window.GetNativeWindow()), &swapChainDesc, nullptr, nullptr, &swapChain);

		dxgiFactory->Release();
		dxgiAdapter->Release();
		dxgiDevice->Release();

		if (FAILED(result))
		{
			RTR_PRINT_RESULT_WITH_MESSAGE("CreateSwapChainForHwnd failed", result);
			return nullptr;
		}

		RTR_ENGINE_TRACE("Finished CreateSpawnChain");
		return swapChain;
	}

	bool CreateBackBufferResources(ID3D11Device* device, SwapChain* swapChain, ID3D11Texture2D** outBackBuffer, ID3D11RenderTargetView** outRenderTargetView)
	{
		RTR_ENGINE_TRACE("Start CreateBackBuffer");

		swapChain->GetBuffer(0, IID_PPV_ARGS(outBackBuffer));

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc	= {};
		rtvDesc.Format							= DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvDesc.ViewDimension					= D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice				= 0;

		HRESULT result = device->CreateRenderTargetView(*outBackBuffer, &rtvDesc, outRenderTargetView);
		if (FAILED(result))
		{
			RTR_PRINT_RESULT_WITH_MESSAGE("CreateRenderTargetView failed", result);
			return false;
		}

		RTR_ENGINE_TRACE("Finished CreateBackBuffer");
		return true;
	}
}

namespace rtr
{
	D3D11Viewport::D3D11Viewport(ID3D11Device* device, const Window& window)
		: mSwapChain(nullptr)
		, mBackBufferTexture(nullptr)
		, mRenderTargetView(nullptr)
	{
		mSwapChain = CreateSwapChain(device, window);
		
		if (mSwapChain)
		{
			CreateBackBufferResources(device, mSwapChain, &mBackBufferTexture, &mRenderTargetView);
		}
	}

	D3D11Viewport::~D3D11Viewport()
	{
		SAFE_RELEASE(mBackBufferTexture);
		SAFE_RELEASE(mRenderTargetView);
		SAFE_RELEASE(mSwapChain);
	}

	void D3D11Viewport::Resize(ID3D11Device* device, uint32_t width, uint32_t height)
	{
		SAFE_RELEASE(mBackBufferTexture);
		SAFE_RELEASE(mRenderTargetView);

		DXGI_SWAP_CHAIN_DESC desc;
		mSwapChain->GetDesc(&desc);

		HRESULT result = mSwapChain->ResizeBuffers(desc.BufferCount, width, height, desc.BufferDesc.Format, desc.Flags);
		if (FAILED(result))
		{
			RTR_PRINT_RESULT_WITH_MESSAGE("ResizeBuffers failed", result);
			// Todo assert here
		}

		CreateBackBufferResources(device, mSwapChain, &mBackBufferTexture, &mRenderTargetView);
	}

	void D3D11Viewport::Present(bool useVSync)
	{
		const uint32_t syncInterval = (useVSync) ? 1 : 0;
		mSwapChain->Present(syncInterval, 0);
	}

}
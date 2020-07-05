#pragma once
#include <cstdint>

struct ID3D11Device;
struct IDXGISwapChain1;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;

typedef IDXGISwapChain1 SwapChain;

namespace rtr
{
	class Window;

	class D3D11Viewport
	{
	public:
		D3D11Viewport(ID3D11Device* device, const Window& window);
		~D3D11Viewport();

		void Resize(ID3D11Device* device, uint32_t width, uint32_t height);
		void Present(bool useVSync);

		SwapChain* mSwapChain;
		ID3D11Texture2D* mBackBufferTexture;
		ID3D11RenderTargetView* mRenderTargetView;
	};
}
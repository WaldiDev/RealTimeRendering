#pragma once
#include "Core/Video.h"

struct ID3D11Device;
struct ID3D11DeviceContext1;
struct ID3D11BlendState;

#ifdef RTR_DEBUG
struct ID3D11Debug;
struct ID3D11InfoQueue;
#endif

namespace rtr
{
	class Window;
	class D3D11Viewport;

	class D3D11Video : public Video
	{
	public:
		explicit D3D11Video(const Window& window);
		virtual ~D3D11Video();

		virtual void OnResize(uint32_t width, uint32_t height) override;

		void BeginRender() override;
		void EndRender() override;

		ID3D11Device* GetDevice() const { return mDevice; }
		ID3D11DeviceContext1* GetDeviceContext() const { return mDeviceContext; }

	private:
		const Window& mWindow;

		bool mVSyncEnabled;

		bool mTearingSupport;

		ID3D11Device* mDevice;
		ID3D11DeviceContext1* mDeviceContext;
		
		D3D11Viewport* mViewport;
		
		ID3D11BlendState* mBlendState;

		float mClearColor[4];

#ifdef RTR_DEBUG
		ID3D11Debug* mDebug;
		ID3D11InfoQueue* mInfoQueue;
#endif
	};
}
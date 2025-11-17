#pragma once
#include "Render/API/SlipstreamRender.h"
#include "Render/API/SwapChain.h"
#include <dxgi1_4.h>
#include <d3d12.h>

namespace Slipstream::Render
{
	class D3D12TextureImpl;

    class D3D12SwapChainImpl : public ISwapChainImpl
    {
    public:
        ~D3D12SwapChainImpl() override;

    private:
        D3D12SwapChainImpl(const SwapChainDesc& desc, IDXGIFactory* factory);

        SwapChainContext BeginRendering() override;
        void EndFrame(uint backBufferIndex, Waitable& signalWaitable)
        {
			m_FrameEndWaitables[backBufferIndex] = signalWaitable;
        }

		Texture GetBackBufferTexture(uint index) override;

        IDXGISwapChain3* m_SwapChain = nullptr;
		std::shared_ptr<D3D12TextureImpl> m_BackBufferTextures[SLIPSTREAM_RENDER_MAX_SWAPCHAIN_BUFFERS] = { nullptr };

        Waitable m_FrameEndWaitables[SLIPSTREAM_RENDER_MAX_SWAPCHAIN_BUFFERS];

		friend class D3D12GraphicsDeviceImpl;
		friend class D3D12CommandQueueImpl;
    };
}


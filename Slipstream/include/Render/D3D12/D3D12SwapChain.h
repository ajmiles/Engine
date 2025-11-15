#pragma once
#include "Render/API/SwapChain.h"
#include <dxgi1_4.h>
#include <d3d12.h>

namespace Slipstream::Render
{
    class D3D12SwapChainImpl : public ISwapChainImpl
    {
    public:
        ~D3D12SwapChainImpl() override;

    private:
        D3D12SwapChainImpl(const SwapChainDesc& desc, IDXGIFactory* factory);

        SwapChainContext BeginRendering() override
        {
            SwapChainContext context;
            context.AcquireWaitable = Waitable(Fence(), 0); // No fence for D3D12 swap chain acquire.
            context.BackBufferIndex = m_SwapChain->GetCurrentBackBufferIndex();
            return context;
        }

        IDXGISwapChain3* m_SwapChain = nullptr;

		friend class D3D12GraphicsDeviceImpl;
		friend class D3D12CommandQueueImpl;
    };
}


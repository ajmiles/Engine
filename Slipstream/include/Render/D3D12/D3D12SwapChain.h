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
        D3D12SwapChainImpl(IDXGISwapChain* swapChain)
            : m_SwapChain(swapChain)
        {
		}

        IDXGISwapChain* m_SwapChain = nullptr;

		friend class D3D12GraphicsDeviceImpl;
		friend class D3D12CommandQueueImpl;
    };
}


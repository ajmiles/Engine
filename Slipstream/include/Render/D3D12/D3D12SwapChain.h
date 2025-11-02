#pragma once
#include "Render/API/SwapChain.h"
#include <dxgi1_4.h>
#include <d3d12.h>

namespace Slipstream::Render
{
    class D3D12SwapChainImpl : public ISwapChainImpl
    {
    public:
        D3D12SwapChainImpl(const SwapChainDesc& desc);
        ~D3D12SwapChainImpl() override;

    private:
        IDXGISwapChain* m_SwapChain = nullptr;
    };
}


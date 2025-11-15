#include "pch.h"
#include "Render/D3D12/D3D12Fence.h"

namespace Slipstream::Render
{
    D3D12FenceImpl::D3D12FenceImpl(const FenceDesc&, ID3D12Device* device)
    {
        HRESULT hr = device->CreateFence(0, D3D12_FENCE_FLAG_SHARED, IID_PPV_ARGS(&m_Fence));
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to create D3D12 fence");
        }
    }

    D3D12FenceImpl::~D3D12FenceImpl()
    {
        if (m_Fence)
        {
            m_Fence->Release();
            m_Fence = nullptr;
        }
    }
}
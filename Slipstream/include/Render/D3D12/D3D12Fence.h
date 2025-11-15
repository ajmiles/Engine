#pragma once
#include "Render/API/Fence.h"

struct ID3D12Fence;

namespace Slipstream::Render
{
    class D3D12FenceImpl : public FenceImpl
    {
    public:
        D3D12FenceImpl(const FenceDesc& desc, ID3D12Device* device);
        ~D3D12FenceImpl() override;

        void WaitUntilCompleted(uint64_t value) const override
        {
            m_Fence->SetEventOnCompletion(value, nullptr);
        }

    private:
        ID3D12Fence* m_Fence = nullptr;

        friend class D3D12GraphicsDeviceImpl;
		friend class D3D12CommandQueueImpl;
    };
}
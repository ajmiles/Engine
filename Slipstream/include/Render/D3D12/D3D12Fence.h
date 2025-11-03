#pragma once
#include "Render/API/Fence.h"

struct ID3D12Fence;

namespace Slipstream::Render
{
    class D3D12FenceImpl : public FenceImpl
    {
    public:
        ~D3D12FenceImpl() override;

    private:
        D3D12FenceImpl(ID3D12Fence* fence)
            : m_Fence(fence)
        {
        }

        uint64_t GetCompletedValue() const override
        {
			return m_Fence->GetCompletedValue();
        }

    private:
        ID3D12Fence* m_Fence = nullptr;

        friend class D3D12GraphicsDeviceImpl;
		friend class D3D12CommandQueueImpl;
    };
}
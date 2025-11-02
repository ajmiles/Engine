#pragma once
#include "Render/API/CommandAllocator.h"

namespace Slipstream::Render
{
    class D3D12CommandAllocatorImpl : public CommandAllocatorImpl
    {
    private:
        D3D12CommandAllocatorImpl(ID3D12CommandAllocator* allocator);
        ~D3D12CommandAllocatorImpl() override;

        void Reset() override;
        ID3D12CommandAllocator* GetNative() const noexcept { return m_Allocator; }

        ID3D12CommandAllocator* m_Allocator = nullptr;

        friend class D3D12GraphicsDeviceImpl;
        friend class D3D12CommandListImpl;
    };
}
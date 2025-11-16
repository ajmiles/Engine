#pragma once
#include "Render/API/CommandList.h"

namespace Slipstream::Render
{
    class D3D12CommandListImpl : public ICommandListImpl
    {
    public:
        D3D12CommandListImpl(ID3D12GraphicsCommandList* list);
        ~D3D12CommandListImpl();

        void Close() override;

        void Barrier(uint numBarriers, Slipstream::Render::Barrier* barriers) override;

        ID3D12GraphicsCommandList* m_List = nullptr;

        friend class D3D12GraphicsDeviceImpl;
        friend class D3D12CommandQueueImpl;
        friend class D3D12CommandAllocatorImpl;
    };
}
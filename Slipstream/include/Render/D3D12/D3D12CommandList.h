#pragma once
#include "Render/API/CommandList.h"


struct ID3D12CommandList;

namespace Slipstream::Render
{
    class CommandAllocator;

    class D3D12CommandListImpl : public ICommandListImpl
    {
    private:
        D3D12CommandListImpl(ID3D12GraphicsCommandList* list);
        ~D3D12CommandListImpl();

        void Close() override;

        ID3D12GraphicsCommandList* GetNative() const noexcept { return m_List; }

        ID3D12GraphicsCommandList* m_List = nullptr;

        friend class D3D12GraphicsDeviceImpl;
        friend class D3D12CommandQueueImpl;
        friend class D3D12CommandAllocatorImpl;
    };
}
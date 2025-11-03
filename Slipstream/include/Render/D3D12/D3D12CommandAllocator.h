#pragma once
#include "Render/API/CommandAllocator.h"

namespace Slipstream::Render
{
    static D3D12_COMMAND_LIST_TYPE ToD3DType(CommandAllocatorType type)
    {
        switch (type)
        {
        case CommandAllocatorType::Compute: return D3D12_COMMAND_LIST_TYPE_COMPUTE;
        case CommandAllocatorType::Copy:    return D3D12_COMMAND_LIST_TYPE_COPY;
        default:                            return D3D12_COMMAND_LIST_TYPE_DIRECT;
        }
    }

    class D3D12CommandAllocatorImpl : public CommandAllocatorImpl
    {
    private:
        D3D12CommandAllocatorImpl(ID3D12CommandAllocator* allocator, ID3D12GraphicsCommandList* commandList);
        ~D3D12CommandAllocatorImpl() override;

        ICommandListImpl* AllocateCommandList() override;
        void Reset() override;

        ID3D12CommandAllocator* GetNative() const noexcept { return m_Allocator; }

		D3D12CommandListImpl* m_CommandList = nullptr;
        ID3D12CommandAllocator* m_Allocator = nullptr;

        friend class D3D12GraphicsDeviceImpl;
        friend class D3D12CommandListImpl;
    };
}
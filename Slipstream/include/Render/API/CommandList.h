#pragma once
#include "Core/Types.h"
#include "Render/API/CommandQueue.h"

namespace Slipstream::Render
{
    class CommandAllocator;

    class ICommandListImpl
    {
    public:
        virtual ~ICommandListImpl() = default;
        virtual void Close() = 0;

        virtual void Barrier() = 0;
    };

    // Non-owning lightweight handle
    class CommandList
    {
    public:
        CommandList(const CommandList&) noexcept = default;
        CommandList& operator=(const CommandList&) noexcept = default;
        CommandList(CommandList&&) noexcept = default;
        CommandList& operator=(CommandList&&) noexcept = default;
        ~CommandList() = default;

        void Close()        { m_Impl->Close(); }

        void Barrier();

    private:
        CommandList() = delete;
        explicit CommandList(ICommandListImpl* impl) noexcept : m_Impl(impl) {}

        ICommandListImpl* m_Impl;

        friend class D3D12GraphicsDeviceImpl;
        friend class VulkanGraphicsDeviceImpl;
        friend class CommandAllocator;

        friend class D3D12CommandQueueImpl;
        friend class D3D12CommandAllocatorImpl;

        friend class VulkanCommandQueueImpl;
    };
}
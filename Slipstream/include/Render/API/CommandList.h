#pragma once
#include "Core/Types.h"
#include "Render/API/CommandQueue.h"

namespace Slipstream::Render
{
    class CommandAllocator;

    enum class CommandListType
    {
        Graphics,
        Compute,
        Copy
    };

    struct CommandListDesc
    {
        CommandListType Type = CommandListType::Graphics;
    };

    class ICommandListImpl
    {
    public:
        virtual ~ICommandListImpl() = default;
        virtual void Reset(CommandAllocator& allocator) = 0;
        virtual void Close() = 0;
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

        explicit operator bool() const noexcept { return m_Impl != nullptr; }

        void Reset(CommandAllocator& allocator) { if (m_Impl) m_Impl->Reset(allocator); }
        void Close()        { if (m_Impl) m_Impl->Close(); }

    private:
        CommandList() = delete;
        explicit CommandList(ICommandListImpl* impl) noexcept : m_Impl(impl) {}

        ICommandListImpl* m_Impl = nullptr;

        friend class D3D12GraphicsDeviceImpl;
        friend class VulkanGraphicsDeviceImpl;
        friend class GraphicsDevice;

        friend class D3D12CommandQueueImpl;
    };
}
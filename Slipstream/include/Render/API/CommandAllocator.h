#pragma once
#include "Core/Types.h"
#include "Render/API/CommandList.h" // For CommandListType reuse

namespace Slipstream::Render
{
    enum class CommandAllocatorType : uint32_t
    {
        Graphics,
        Compute,
        Copy
    };

    struct CommandAllocatorDesc
    {
        CommandAllocatorType Type = CommandAllocatorType::Graphics;
    };

    // Base interface (no 'I' prefix as requested)
    class CommandAllocatorImpl
    {
    public:
        virtual ~CommandAllocatorImpl() = default;
        virtual ICommandListImpl* AllocateCommandList() = 0;
        virtual void Reset() = 0;
    };

    // Non-owning lightweight handle; user cannot construct directly.
    class CommandAllocator
    {
    public:
        CommandAllocator(const CommandAllocator&) noexcept = default;
        CommandAllocator& operator=(const CommandAllocator&) noexcept = default;
        CommandAllocator(CommandAllocator&&) noexcept = default;
        CommandAllocator& operator=(CommandAllocator&&) noexcept = default;
        ~CommandAllocator() = default;

        explicit operator bool() const noexcept { return m_Impl != nullptr; }

		CommandList AllocateCommandList() { return CommandList(m_Impl->AllocateCommandList()); }
        void Reset() { m_Impl->Reset(); }

    private:
        CommandAllocator() = delete;
        explicit CommandAllocator(CommandAllocatorImpl* impl) noexcept : m_Impl(impl) {}

        CommandAllocatorImpl* m_Impl = nullptr;

        friend class D3D12GraphicsDeviceImpl;
        friend class VulkanGraphicsDeviceImpl;
        friend class GraphicsDevice;

        friend class D3D12CommandListImpl;
    };
}
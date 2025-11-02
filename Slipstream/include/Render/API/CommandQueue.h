#pragma once
#include "Core/Types.h"

namespace Slipstream
{
    namespace Render
    {
        class ICommandQueueImpl
        {
        public:
            virtual ~ICommandQueueImpl() = default;

            virtual void ExecuteCommandList(class CommandList& commandList) = 0;
        };

        enum class CommandQueueType
        {
            Graphics,
            Compute,
            Copy,
            Count
        };

        // Non-owning handle; cannot be user-constructed.
        class CommandQueue
        {
        public:
            ~CommandQueue() = default;

            CommandQueue(const CommandQueue&) noexcept = default;
            CommandQueue& operator=(const CommandQueue&) noexcept = default;
            CommandQueue(CommandQueue&&) noexcept = default;
            CommandQueue& operator=(CommandQueue&&) noexcept = default;

            explicit operator bool() const noexcept { return m_Impl != nullptr; }
            bool IsValid() const noexcept { return m_Impl != nullptr; }

            void ExecuteCommandList(CommandList& commandList);

        private:
            // Only backend device implementations may construct.
            explicit CommandQueue(ICommandQueueImpl* impl) noexcept : m_Impl(impl) {}

            // Prevent user default construction.
            CommandQueue() = delete;

            ICommandQueueImpl* m_Impl = nullptr;

            friend class D3D12GraphicsDeviceImpl;
            friend class VulkanGraphicsDeviceImpl;
        };
    }
}
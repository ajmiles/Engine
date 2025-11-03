#pragma once
#include "Core/Types.h"

namespace Slipstream
{
    namespace Render
    {
        enum class PresentFlags
        {
			None = 0,
        };

        struct PresentDesc
        {
			PresentFlags Flags = PresentFlags::None;
        };

        class ICommandQueueImpl
        {
        public:
            virtual ~ICommandQueueImpl() = default;

            virtual void ExecuteCommandList(class CommandList& commandList) = 0;
			virtual void SignalFence(class Fence& fence, uint64 value) = 0;
			virtual void Present(class SwapChain& swapChain, PresentDesc& desc) = 0;
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

            void ExecuteCommandList(class CommandList& commandList);
			void SignalFence(class Fence& fence, uint64 value);

			void Present(class SwapChain& swapChain, PresentDesc& desc); // For future use with swap chains.

			CommandQueueType GetQueueType() const noexcept { return m_Type; }

        private:
            // Only backend device implementations may construct.
            explicit CommandQueue(ICommandQueueImpl* impl, CommandQueueType type) noexcept : m_Impl(impl), m_Type(type) {}

            // Prevent user default construction.
            CommandQueue() = delete;

			CommandQueueType m_Type;
            ICommandQueueImpl* m_Impl = nullptr;

            friend class D3D12GraphicsDeviceImpl;
            friend class VulkanGraphicsDeviceImpl;
        };
    }
}
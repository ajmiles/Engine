#pragma once
#include "Core/Types.h"
#include "Render/API/Fence.h"

namespace Slipstream
{
    namespace Render
    {
        enum class PresentFlags
        {
            None = 0,
        };

        struct Waitable
        {
            Fence fence;
            uint64_t value = 0;

            void WaitOnCPU() const
            {
                if (fence.IsValid())
                {
                    fence.WaitUntilCompleted(value);
                }
            }
        };

        struct PresentDesc
        {
            PresentFlags Flags = PresentFlags::None;
            uint BackBufferIndex = 0xFFFFFFFF;
            Waitable SwapChainWaitable;
        };

        class ICommandQueueImpl
        {
        public:
            virtual ~ICommandQueueImpl() = default;

            virtual Waitable ExecuteCommandList(class CommandList& commandList, uint numWaits, Waitable* waitables) = 0;
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

            Waitable ExecuteCommandList(class CommandList& commandList, uint numWaits = 0, Waitable* waitables = nullptr);
            void Present(class SwapChain& swapChain, PresentDesc& desc);

        private:
            explicit CommandQueue(ICommandQueueImpl* impl) noexcept : m_Impl(impl) {}

            std::shared_ptr<ICommandQueueImpl> m_Impl;

            friend class D3D12GraphicsDeviceImpl;
            friend class VulkanGraphicsDeviceImpl;

            friend class D3D12SwapChainImpl;
            friend class VulkanSwapChainImpl;
        };
    }
}
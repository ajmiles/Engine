#pragma once
#include "Render/API/CommandQueue.h"
#include <vulkan/vulkan.hpp>

namespace Slipstream::Render
{
    class VulkanCommandQueueImpl final : public ICommandQueueImpl
    {
    public:
        VulkanCommandQueueImpl() = default;
        explicit VulkanCommandQueueImpl(vk::Queue q) : m_Queue(q) {}

        ~VulkanCommandQueueImpl() override = default;

        void ExecuteCommandList(class CommandList& commandList) override;
		void SignalFence(class Fence& fence, uint64 value) override;
		void Present(class SwapChain& swapChain, PresentDesc& desc) override;

        vk::Queue m_Queue;
    };
}


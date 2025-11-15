#pragma once
#include "Render/API/CommandQueue.h"
#include "Render/Vulkan/VulkanFence.h"
#include <vulkan/vulkan.hpp>

namespace Slipstream::Render
{
    class VulkanCommandQueueImpl final : public ICommandQueueImpl
    {
    public:
        VulkanCommandQueueImpl(vk::Device device, uint queueFamilyIndex, uint queueIndex);
        ~VulkanCommandQueueImpl() override;

        Waitable ExecuteCommandList(class CommandList& commandList, uint numWaits, Waitable* waitables) override;
        void Present(class SwapChain& swapChain, PresentDesc& desc) override;

        vk::Queue m_Queue;
        
        std::shared_ptr<VulkanFenceImpl> m_ProgressFence;
        uint64_t m_LastSignalledValue = 0;
    };
}


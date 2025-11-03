#pragma once
#include "../API/GraphicsDevice.h"
#include "Render/API/SwapChain.h"
#include "Render/API/CommandAllocator.h"
#include "Render/Vulkan/VulkanSwapChain.h"

#include <vulkan/vulkan.hpp>
#include <cassert>

namespace Slipstream
{
    namespace Render
    {
        class VulkanCommandQueueImpl;

        class VulkanGraphicsDeviceImpl : public IGraphicsDeviceImpl
        {
        private:
            VulkanGraphicsDeviceImpl(const GraphicsDeviceDesc& desc);
            ~VulkanGraphicsDeviceImpl();

            SwapChain        CreateSwapChain(const SwapChainDesc& desc) override;
            CommandQueue     GetCommandQueue(CommandQueueType type, uint index) override;
            CommandAllocator CreateCommandAllocator(const CommandAllocatorDesc& desc) override;
            Fence            CreateFence(const FenceDesc& desc) override;

            uint FindBestQueueFamilyIndex(vk::QueueFlags desiredFlags);

            vk::Instance       m_instance;
            vk::PhysicalDevice m_physicalDevice;
            vk::Device         m_device;

            VulkanCommandQueueImpl* m_GraphicsQueues = nullptr;
            VulkanCommandQueueImpl* m_ComputeQueues  = nullptr;
            VulkanCommandQueueImpl* m_CopyQueues     = nullptr;

            static const uint INVALID_QUEUE_FAMILY = uint(-1);
            uint m_GraphicsFamily = INVALID_QUEUE_FAMILY;
            uint m_ComputeFamily  = INVALID_QUEUE_FAMILY;
            uint m_CopyFamily     = INVALID_QUEUE_FAMILY;

            std::vector<VulkanSwapChainImpl>        m_SwapChains;

            friend class GraphicsDevice;
        };
    }
}
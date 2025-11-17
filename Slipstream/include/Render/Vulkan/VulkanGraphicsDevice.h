#pragma once
#include "Render/API/GraphicsDevice.h"
#include "Render/Vulkan/VulkanSwapChain.h"

#include <vulkan/vulkan.hpp>
#include <mutex>

namespace Slipstream
{
    namespace Render
    {
        class VulkanGraphicsDeviceImpl : public IGraphicsDeviceImpl
        {
        private:
            VulkanGraphicsDeviceImpl(const GraphicsDeviceDesc& desc);
            ~VulkanGraphicsDeviceImpl();

            SwapChain        CreateSwapChain(const SwapChainDesc& desc) override;
            CommandQueue     GetCommandQueue(CommandQueueType type, uint index) override;
            CommandAllocator CreateCommandAllocator(const CommandAllocatorDesc& desc) override;
            Fence            CreateFence(const FenceDesc& desc) override;

			RenderTargetView CreateRenderTargetView(const RenderTargetViewDesc& desc) override;
			void             DestroyRenderTargetView(const RenderTargetView rtv) override;

            uint FindBestQueueFamilyIndex(vk::QueueFlags desiredFlags);

            vk::Instance       m_instance;
            vk::PhysicalDevice m_physicalDevice;
            vk::Device         m_device;

            std::vector<VulkanCommandQueueImpl> m_GraphicsQueues;
            std::vector<VulkanCommandQueueImpl> m_ComputeQueues;
            std::vector<VulkanCommandQueueImpl> m_CopyQueues;

            static const uint INVALID_QUEUE_FAMILY = uint(-1);
            uint m_GraphicsFamily = INVALID_QUEUE_FAMILY;
            uint m_ComputeFamily  = INVALID_QUEUE_FAMILY;
            uint m_CopyFamily     = INVALID_QUEUE_FAMILY;

            std::vector<VulkanSwapChainImpl>        m_SwapChains;

            VkImageView* m_RTVDescriptorHeap;
            std::vector<RenderTargetView> m_RTVDescriptorHeapFreeList;

			std::mutex m_CriticalSection;

            friend class GraphicsDevice;
        };
    }
}
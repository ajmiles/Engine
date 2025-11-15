#pragma once
#include "Render/API/CommandList.h"
#include <vulkan/vulkan.hpp>

namespace Slipstream::Render
{
    class VulkanCommandListImpl : public ICommandListImpl
    {
    private:
        VulkanCommandListImpl();
        ~VulkanCommandListImpl();

        void Close() override;

        void Barrier() override
        {
        }

		vk::CommandBuffer m_CommandBuffer;

        friend class VulkanGraphicsDeviceImpl;
        friend class VulkanCommandAllocatorImpl;
		friend class VulkanCommandQueueImpl;
    };
}
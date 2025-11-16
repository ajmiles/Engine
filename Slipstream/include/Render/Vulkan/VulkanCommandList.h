#pragma once
#include "Render/API/CommandList.h"
#include <vulkan/vulkan.hpp>

namespace Slipstream::Render
{
    class VulkanCommandListImpl : public ICommandListImpl
    {
    public:
        VulkanCommandListImpl();
        ~VulkanCommandListImpl();

        void Close() override;

        void Barrier(uint numBarriers, Slipstream::Render::Barrier* barriers) override;

		vk::CommandBuffer m_CommandBuffer;

        friend class VulkanGraphicsDeviceImpl;
        friend class VulkanCommandAllocatorImpl;
		friend class VulkanCommandQueueImpl;
    };
}
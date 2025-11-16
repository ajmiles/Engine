#pragma once
#include "Render/API/CommandAllocator.h"
#include <vulkan/vulkan.hpp>

namespace Slipstream::Render
{
	class VulkanCommandListImpl;

    class VulkanCommandAllocatorImpl : public CommandAllocatorImpl
    {
    private:
        VulkanCommandAllocatorImpl(vk::Device device, vk::CommandPool commandPool);
        ~VulkanCommandAllocatorImpl() override;

        CommandList AllocateCommandList() override;
        void Reset() override;

		std::shared_ptr<VulkanCommandListImpl> m_CommandList;

        vk::Device      m_Device;
        vk::CommandPool m_Pool;

        friend class VulkanGraphicsDeviceImpl;
    };
}
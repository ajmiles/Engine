#pragma once
#include "Render/API/CommandAllocator.h"
#include <vulkan/vulkan.hpp>

namespace Slipstream::Render
{
    class VulkanCommandAllocatorImpl : public CommandAllocatorImpl
    {
    private:
        VulkanCommandAllocatorImpl(const CommandAllocatorDesc& desc);
        ~VulkanCommandAllocatorImpl() override;

        void Reset() override;

        vk::Device      m_Device;
        vk::CommandPool m_Pool;

        friend class VulkanGraphicsDeviceImpl;
    };
}
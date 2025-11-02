#pragma once
#include "Render/API/Fence.h"
#include <vulkan/vulkan.h>

namespace Slipstream::Render
{
    class VulkanFenceImpl : public FenceImpl
    {
    public:
        ~VulkanFenceImpl() override;

    private:
        VulkanFenceImpl() = default;

    private:
        VkFence m_Fence = VK_NULL_HANDLE;
        VkDevice m_Device = VK_NULL_HANDLE;

        friend class VulkanGraphicsDeviceImpl;
    };
}
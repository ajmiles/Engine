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
        VulkanFenceImpl(vk::Device device, vk::Semaphore semaphore)
            : m_Device(device), m_Semaphore(semaphore) {}

        uint64_t GetCompletedValue() const override
        {
            uint64_t value;
			vkGetSemaphoreCounterValue(m_Device, m_Semaphore, &value);
			return value;
        }

    private:
        VkSemaphore m_Semaphore = VK_NULL_HANDLE;
        VkDevice m_Device = VK_NULL_HANDLE;

        friend class VulkanGraphicsDeviceImpl;
		friend class VulkanCommandQueueImpl;
    };
}
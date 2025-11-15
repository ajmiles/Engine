#include "pch.h"
#include "Render/Vulkan/VulkanFence.h"

namespace Slipstream::Render
{
    VulkanFenceImpl::VulkanFenceImpl(const FenceDesc& desc, vk::Device device, vk::SemaphoreType semaphoreType) : m_Device(device)
    {
        vk::SemaphoreTypeCreateInfo semaphoreTypeInfo = { semaphoreType, 0 };
        vk::SemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.pNext = &semaphoreTypeInfo;
        m_Semaphore = device.createSemaphore(semaphoreInfo);
    }

    VulkanFenceImpl::~VulkanFenceImpl()
    {
        m_Device.destroySemaphore(m_Semaphore);
    }
}
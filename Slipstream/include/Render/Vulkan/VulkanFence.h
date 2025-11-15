#pragma once
#include "Render/API/Fence.h"
#include <vulkan/vulkan.hpp>

namespace Slipstream::Render
{
    class VulkanFenceImpl : public FenceImpl
    {
    public:
        VulkanFenceImpl(const FenceDesc& desc, vk::Device device, vk::SemaphoreType semaphoreType);
        ~VulkanFenceImpl() override;

        void WaitUntilCompleted(uint64_t value) const override
        {
            vk::SemaphoreWaitInfo waitInfo;
            waitInfo.setSemaphores(m_Semaphore);
            waitInfo.setValues(value);
            m_Device.waitSemaphores(waitInfo, UINT64_MAX);
        }
        
        vk::Semaphore m_Semaphore = VK_NULL_HANDLE;
        vk::Device    m_Device;
    };
}
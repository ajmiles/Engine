#pragma once
#include "Render/API/Fence.h"
#include <vulkan/vulkan.hpp>

namespace Slipstream::Render
{
    class VulkanFenceImpl : public IFenceImpl
    {
    public:
        VulkanFenceImpl(const FenceDesc& desc, vk::Device device, vk::SemaphoreType semaphoreType);
        ~VulkanFenceImpl() override;

        bool HasCompleted(uint64_t value) const override
        {
			uint64_t currentValue = m_Device.getSemaphoreCounterValue(m_Semaphore);
			return (currentValue >= value);
        }

        void WaitUntilCompleted(uint64_t value) const override
        {
            //char str[256];
			//std::snprintf(str, sizeof(str), "Semaphore Addr = %p, VulkanFenceImpl::WaitUntilCompleted(%llu)\n", (void*)&m_Semaphore, value);
            //OutputDebugStringA(str);

            vk::SemaphoreWaitInfo waitInfo;
            waitInfo.setSemaphores(m_Semaphore);
            waitInfo.setValues(value);
            m_Device.waitSemaphores(waitInfo, UINT64_MAX);

			//std::snprintf(str, sizeof(str), "Semaphore Addr = %p, VulkanFenceImpl::WaitUntilCompleted(%llu) - Completed. Result: %d\n", (void*)&m_Semaphore, value, static_cast<int>(res));
			//OutputDebugStringA(str);
        }
        
        vk::Semaphore m_Semaphore = VK_NULL_HANDLE;
        vk::Device    m_Device;
    };
}
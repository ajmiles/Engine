#pragma once
#include "Render/API/SwapChain.h"
#include "Render/Vulkan/VulkanFence.h"
#include <vulkan/vulkan.hpp>

namespace Slipstream::Render
{
    class VulkanSwapChainImpl : public ISwapChainImpl
    {
    public:
        VulkanSwapChainImpl(const SwapChainDesc& desc, vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device);
        ~VulkanSwapChainImpl() override;

        SwapChainContext BeginRendering() override;

    private:
        vk::Device       m_Device;
        vk::SurfaceKHR   m_Surface;
        vk::SwapchainKHR m_SwapChain;

        static const uint MAX_SWAPCHAIN_IMAGES = 4;

        std::shared_ptr<VulkanFenceImpl> m_SwapChainAcquireSemaphores[MAX_SWAPCHAIN_IMAGES];
        std::shared_ptr<VulkanFenceImpl> m_SwapChainPresentSemaphores[MAX_SWAPCHAIN_IMAGES];
        Waitable                         m_SwapChainFences[MAX_SWAPCHAIN_IMAGES];
        uint                             m_NextSemaphoreIndex = 0;
        uint                             m_BufferCount = 0;

        friend class VulkanCommandQueueImpl;
    };
}
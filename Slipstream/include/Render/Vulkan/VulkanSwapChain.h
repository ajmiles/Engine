#pragma once
#include "Render/API/SwapChain.h"
#include <vulkan/vulkan.hpp>

namespace Slipstream::Render
{
    class VulkanSwapChainImpl : public ISwapChainImpl
    {
    public:
        VulkanSwapChainImpl(vk::Device device, vk::SurfaceKHR surface, vk::SwapchainKHR swapchain);
        ~VulkanSwapChainImpl() override;

    private:
        vk::Device       m_Device;
        vk::SurfaceKHR   m_Surface;
        vk::SwapchainKHR m_SwapChain;

        friend class VulkanCommandQueueImpl;
    };
}
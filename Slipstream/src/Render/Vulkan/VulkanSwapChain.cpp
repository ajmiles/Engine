#include "pch.h"
#include "Render/Vulkan/VulkanSwapChain.h"

namespace Slipstream::Render
{
    VulkanSwapChainImpl::VulkanSwapChainImpl(vk::Device device, vk::SurfaceKHR surface, vk::SwapchainKHR swapchain)
        : m_Device(device), m_Surface(surface), m_SwapChain(swapchain)
    {
    }

    VulkanSwapChainImpl::~VulkanSwapChainImpl()
    {
        // TODO Surface?
        m_Device.destroySwapchainKHR(m_SwapChain);
        m_SwapChain = vk::SwapchainKHR{};
    }
}
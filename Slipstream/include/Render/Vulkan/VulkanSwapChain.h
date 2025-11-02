#pragma once
#include "Render/API/SwapChain.h"
#include <vulkan/vulkan.hpp>

namespace Slipstream::Render
{
    class VulkanSwapChainImpl : public ISwapChainImpl
    {
    public:
        VulkanSwapChainImpl() = default;
        ~VulkanSwapChainImpl() override = default;

    private:
        vk::SwapchainKHR m_SwapChain;
    };
}
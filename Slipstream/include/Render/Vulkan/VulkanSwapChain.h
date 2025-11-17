#pragma once
#include "Render/API/SlipstreamRender.h"
#include "Render/API/SwapChain.h"
#include "Render/Vulkan/VulkanFence.h"
#include <vulkan/vulkan.hpp>

namespace Slipstream::Render
{
	class VulkanTextureImpl;

    class VulkanSwapChainImpl : public ISwapChainImpl
    {
    public:
        VulkanSwapChainImpl(const SwapChainDesc& desc, vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device);
        ~VulkanSwapChainImpl() override;

        SwapChainContext BeginRendering() override;

		Texture GetBackBufferTexture(uint index) override;

    private:
        vk::Device       m_Device;
        vk::SurfaceKHR   m_Surface;
        vk::SwapchainKHR m_SwapChain;

		//vk::Image 	                    m_SwapChainImages[SLIPSTREAM_RENDER_MAX_SWAPCHAIN_BUFFERS];
		std::shared_ptr<VulkanTextureImpl>  m_SwapChainImages[SLIPSTREAM_RENDER_MAX_SWAPCHAIN_BUFFERS] = { nullptr };

        std::shared_ptr<VulkanFenceImpl> m_SwapChainAcquireSemaphores[SLIPSTREAM_RENDER_MAX_SWAPCHAIN_BUFFERS];
        std::shared_ptr<VulkanFenceImpl> m_SwapChainPresentSemaphores[SLIPSTREAM_RENDER_MAX_SWAPCHAIN_BUFFERS];

		// Indicates that a buffer is available for rendering.
        Waitable                         m_SwapChainFences[SLIPSTREAM_RENDER_MAX_SWAPCHAIN_BUFFERS];

        //uint                             m_NextSemaphoreIndex = 0;
        uint                             m_BufferCount = 0;

        friend class VulkanCommandQueueImpl;
    };
}
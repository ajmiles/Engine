#pragma once
#include "Render/API/SlipstreamRender.h"
#include "Render/API/SwapChain.h"
#include "Render/Vulkan/VulkanFence.h"
#include <vulkan/vulkan.hpp>

namespace Slipstream::Render
{
	class VulkanTextureImpl;

    struct PerFrameContext
    {
        vk::Semaphore AcquireSemaphore;
        vk::Semaphore PresentSemaphore;
        Waitable FrameCompleteWaitable;

        bool IsFrameComplete() const
        {
            return FrameCompleteWaitable.HasCompleted();
        }
    };

    class VulkanSwapChainImpl : public ISwapChainImpl
    {
    public:
        VulkanSwapChainImpl(const SwapChainDesc& desc, vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device);
        ~VulkanSwapChainImpl() override;

        SwapChainContext BeginRendering() override;

        Texture GetBackBufferTexture(uint index) override;

        //void Present(uint index) override;

    private:
        vk::Device       m_Device;
        vk::SurfaceKHR   m_Surface;
        vk::SwapchainKHR m_SwapChain;

		std::shared_ptr<VulkanTextureImpl>  m_SwapChainImages[SLIPSTREAM_RENDER_MAX_SWAPCHAIN_BUFFERS] = { nullptr };

        PerFrameContext                     m_PerFrameContexts[SLIPSTREAM_RENDER_MAX_SWAPCHAIN_BUFFERS];

        uint                                m_BufferCount = 0;

        friend class VulkanCommandQueueImpl;
    };
}
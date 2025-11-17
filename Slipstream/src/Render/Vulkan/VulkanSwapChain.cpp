#include "pch.h"
#include "Render/Vulkan/VulkanSwapChain.h"
#include "Render/Vulkan/VulkanTexture.h"

namespace Slipstream::Render
{
    VulkanSwapChainImpl::VulkanSwapChainImpl(const SwapChainDesc& desc, vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device)
        : m_Device(device), m_BufferCount(desc.BufferCount)
    {
        // Create a Vulkan surface for the window handle
        vk::Win32SurfaceCreateInfoKHR surfaceInfo;
        surfaceInfo.hinstance = GetModuleHandle(nullptr);
        surfaceInfo.hwnd = (HWND)desc.WindowHandle;

        m_Surface = instance.createWin32SurfaceKHR(surfaceInfo);

        // 1. Get Capabilities (image counts, min/max size, etc.)
        vk::SurfaceCapabilitiesKHR capabilities =
            physicalDevice.getSurfaceCapabilitiesKHR(m_Surface);

        // 2. Get Supported Formats (pixel format and color space)
        std::vector<vk::SurfaceFormatKHR> formats =
            physicalDevice.getSurfaceFormatsKHR(m_Surface);

        // 3. Get Supported Presentation Modes (VSync, Mailbox, etc.)
        std::vector<vk::PresentModeKHR> presentModes =
            physicalDevice.getSurfacePresentModesKHR(m_Surface);

        vk::SurfaceFormatKHR surfaceFormat = formats[3];
        vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo;
        vk::Extent2D extent = capabilities.currentExtent;
        uint32_t imageCount = capabilities.minImageCount;

        vk::SwapchainCreateInfoKHR swapchainInfo = {};
        swapchainInfo.surface = m_Surface;
        swapchainInfo.minImageCount = std::max(imageCount, desc.BufferCount);
        swapchainInfo.imageFormat = surfaceFormat.format;
        swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapchainInfo.imageExtent = extent;
        swapchainInfo.imageArrayLayers = 1;
        swapchainInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        swapchainInfo.preTransform = capabilities.currentTransform;
        swapchainInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        swapchainInfo.presentMode = presentMode;
        swapchainInfo.clipped = VK_TRUE;
		//swapchainInfo.flags = vk::SwapchainCreateFlagBitsKHR::eMutableFormat;

        m_SwapChain = m_Device.createSwapchainKHR(swapchainInfo);

		auto swapChainImages = m_Device.getSwapchainImagesKHR(m_SwapChain);

        for (uint i = 0; i < desc.BufferCount; ++i)
        {
            m_SwapChainAcquireSemaphores[i] = std::make_shared<VulkanFenceImpl>(FenceDesc{}, device, vk::SemaphoreType::eBinary);
            m_SwapChainPresentSemaphores[i] = std::make_shared<VulkanFenceImpl>(FenceDesc{}, device, vk::SemaphoreType::eBinary);
            m_SwapChainImages[i] = std::make_shared<VulkanTextureImpl>(swapChainImages[i]);
        }
    }

    VulkanSwapChainImpl::~VulkanSwapChainImpl()
    {
        // TODO Surface?
        m_Device.destroySwapchainKHR(m_SwapChain);
        m_SwapChain = vk::SwapchainKHR{};
    }

    SwapChainContext VulkanSwapChainImpl::BeginRendering()
    {
        // We need to throttle the CPU to make sure the next semaphore has been signalled
        int firstFreeSemaphoreIndex = -1;

        for (int i = 0; i < m_BufferCount; ++i)
        {
			// Find the first semaphore that is not in use
            if(m_SwapChainFences[i].HasCompleted())
            {
                firstFreeSemaphoreIndex = i;
                break;
			}
        }

        if(firstFreeSemaphoreIndex == -1)
        {
            // All semaphores are in use; wait for the next one to become free
			// TODO: Wait on the oldest semaphore instead of the first one?
            char str[256];
			std::snprintf(str, sizeof(str), "Waiting on SwapChain Fence %p. Value %llu\n", (void*)&static_cast<VulkanFenceImpl*>(m_SwapChainFences[0].fence.m_Impl.get())->m_Semaphore, m_SwapChainFences[0].value);
			OutputDebugStringA(str);
            m_SwapChainFences[0].WaitOnCPU();
            firstFreeSemaphoreIndex = 0;
		}

        vk::Result res;
        uint32_t imageIndex;
        std::tie(res, imageIndex) = m_Device.acquireNextImageKHR(m_SwapChain, UINT64_MAX, m_SwapChainAcquireSemaphores[firstFreeSemaphoreIndex]->m_Semaphore);

        char str[256];
		std::snprintf(str, sizeof(str), "Acquire Using Semaphore %p\n", (void*)&m_SwapChainAcquireSemaphores[firstFreeSemaphoreIndex]->m_Semaphore);
        OutputDebugStringA(str);

        SwapChainContext context;
        context.AcquireWaitable = Waitable(Fence(m_SwapChainAcquireSemaphores[firstFreeSemaphoreIndex]), 0xFFFFFFFF);
        context.BackBufferIndex = imageIndex;

        //m_NextSemaphoreIndex = (m_NextSemaphoreIndex + 1) % m_BufferCount;

        return context;
    }

	Texture VulkanSwapChainImpl::GetBackBufferTexture(uint index)
    {
        return Texture(m_SwapChainImages[index]);
	}
}
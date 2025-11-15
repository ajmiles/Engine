#include "pch.h"
#include "Render/Vulkan/VulkanSwapChain.h"

namespace Slipstream::Render
{
    VulkanSwapChainImpl::VulkanSwapChainImpl(const SwapChainDesc& desc, vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device)
        : m_Device(device), m_BufferCount(desc.BufferCount)
    {
        for (int i = 0; i < desc.BufferCount; ++i)
        {
            m_SwapChainAcquireSemaphores[i] = std::make_shared<VulkanFenceImpl>(FenceDesc{}, device, vk::SemaphoreType::eBinary);
            m_SwapChainPresentSemaphores[i] = std::make_shared<VulkanFenceImpl>(FenceDesc{}, device, vk::SemaphoreType::eBinary);
        }

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

        vk::SurfaceFormatKHR surfaceFormat = formats[1];
        vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo;
        vk::Extent2D extent = capabilities.currentExtent;
        uint32_t imageCount = capabilities.minImageCount;

        vk::SwapchainCreateInfoKHR swapchainInfo = {};
        swapchainInfo.surface = m_Surface;
        swapchainInfo.minImageCount = imageCount;
        swapchainInfo.imageFormat = surfaceFormat.format;
        swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapchainInfo.imageExtent = extent;
        swapchainInfo.imageArrayLayers = 1;
        swapchainInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        swapchainInfo.preTransform = capabilities.currentTransform;
        swapchainInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        swapchainInfo.presentMode = presentMode;
        swapchainInfo.clipped = VK_TRUE;

        m_SwapChain = m_Device.createSwapchainKHR(swapchainInfo);
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
        m_SwapChainFences[m_NextSemaphoreIndex].WaitOnCPU();


        vk::Result res;
        uint32_t imageIndex;
        std::tie(res, imageIndex) = m_Device.acquireNextImageKHR(m_SwapChain, UINT64_MAX, m_SwapChainAcquireSemaphores[m_NextSemaphoreIndex]->m_Semaphore);

        SwapChainContext context;
        context.AcquireWaitable = Waitable(Fence(m_SwapChainAcquireSemaphores[m_NextSemaphoreIndex]), 0xFFFFFFFF);
        context.BackBufferIndex = imageIndex;

        m_NextSemaphoreIndex = (m_NextSemaphoreIndex + 1) % m_BufferCount;

        return context;
    }
}
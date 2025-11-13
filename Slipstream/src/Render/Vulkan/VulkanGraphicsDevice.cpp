#include "pch.h"
#include "Render/Vulkan/VulkanGraphicsDevice.h"
#include "Render/Vulkan/VulkanCommandQueue.h"
#include "Render/Vulkan/VulkanCommandList.h"
#include "Render/Vulkan/VulkanCommandAllocator.h"
#include "Render/Vulkan/VulkanFence.h"
#include <stdexcept>

using namespace Slipstream::Render;

VulkanGraphicsDeviceImpl::VulkanGraphicsDeviceImpl(const GraphicsDeviceDesc& desc)
{
    vk::ApplicationInfo appInfo;
    appInfo.pApplicationName = "Slipstream";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Slipstream";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    // Validation layer & debug utils extension enabling (instance level)
    std::vector<const char*> enabledLayers;
    std::vector<const char*> enabledExtensions;

    if (desc.Flags & GraphicsDeviceFlags::Debug)
    {
        // Query available layers
        auto availableLayers = vk::enumerateInstanceLayerProperties();
        bool validationFound = false;
        for (auto const& layer : availableLayers)
        {
			OutputDebugStringA(layer.layerName);
			OutputDebugStringA("\n");
            if (std::strcmp(layer.layerName, "VK_LAYER_KHRONOS_validation") == 0)
            {
                validationFound = true;
            }
        }
        if (validationFound)
        {
            enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
            // Debug utils extension is optional but commonly used alongside validation
            auto availableExtensions = vk::enumerateInstanceExtensionProperties();
            bool debugUtilsFound = false;
            for (auto const& ext : availableExtensions)
            {
                if (std::strcmp(ext.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0)
                {
                    debugUtilsFound = true;
                    break;
                }
            }
            if (debugUtilsFound)
                enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
    }

	enabledExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

    vk::InstanceCreateInfo instInfo{};
    instInfo.pApplicationInfo = &appInfo;
    instInfo.enabledLayerCount = static_cast<uint32_t>(enabledLayers.size());
    instInfo.ppEnabledLayerNames = enabledLayers.empty() ? nullptr : enabledLayers.data();
    instInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
    instInfo.ppEnabledExtensionNames = enabledExtensions.empty() ? nullptr : enabledExtensions.data();

    m_instance = vk::createInstance(instInfo);

    auto physicalDevices = m_instance.enumeratePhysicalDevices();
    if (physicalDevices.empty())
        throw std::runtime_error("No Vulkan physical devices found");

    m_physicalDevice = physicalDevices[0];

    auto deviceExtensionsList = m_physicalDevice.enumerateDeviceExtensionProperties();
    for (const auto& ext : deviceExtensionsList)
    {
		OutputDebugStringA(ext.extensionName);
		OutputDebugStringA("\n");
    }

    m_GraphicsFamily = FindBestQueueFamilyIndex(vk::QueueFlagBits::eGraphics);
    m_ComputeFamily  = FindBestQueueFamilyIndex(vk::QueueFlagBits::eCompute);
    m_CopyFamily     = FindBestQueueFamilyIndex(vk::QueueFlagBits::eTransfer);

    uint totalQueues = desc.NumGraphicsQueues + desc.NumComputeQueues + desc.NumCopyQueues;
    auto* queueCreateInfos = static_cast<vk::DeviceQueueCreateInfo*>(
        _alloca(sizeof(vk::DeviceQueueCreateInfo) * totalQueues));
    uint* queueFamilies = static_cast<uint*>(
        _alloca(sizeof(uint) * totalQueues));

    uint cursor = 0;
    for (uint i = 0; i < desc.NumGraphicsQueues; ++i) queueFamilies[cursor++] = m_GraphicsFamily;
    for (uint i = 0; i < desc.NumComputeQueues;  ++i) queueFamilies[cursor++] = m_ComputeFamily;
    for (uint i = 0; i < desc.NumCopyQueues;     ++i) queueFamilies[cursor++] = m_CopyFamily;

    for (uint i = 0; i < totalQueues; ++i)
    {
        float priority = 1.0f;
        queueCreateInfos[i] = vk::DeviceQueueCreateInfo({}, queueFamilies[i], 1, &priority);
    }

    vk::PhysicalDeviceVulkan12Features features12;
	features12.timelineSemaphore = VK_TRUE;
    
	vk::PhysicalDeviceVulkan13Features features13;
	features13.synchronization2 = VK_TRUE;
	features12.pNext = &features13;

    std::vector<const char*> deviceExtensions;
	deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    vk::DeviceCreateInfo deviceCreateInfo({}, totalQueues, queueCreateInfos);
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
	deviceCreateInfo.pNext = &features12;

    m_device = m_physicalDevice.createDevice(deviceCreateInfo);

	vk::PhysicalDeviceProperties deviceProps = m_physicalDevice.getProperties();

    if (desc.NumGraphicsQueues)
        m_GraphicsQueues = new VulkanCommandQueueImpl[desc.NumGraphicsQueues];
    if (desc.NumComputeQueues)
        m_ComputeQueues = new VulkanCommandQueueImpl[desc.NumComputeQueues];
    if (desc.NumCopyQueues)
        m_CopyQueues = new VulkanCommandQueueImpl[desc.NumCopyQueues];

    for (uint i = 0; i < desc.NumGraphicsQueues; ++i)
        m_GraphicsQueues[i] = VulkanCommandQueueImpl(m_device.getQueue(m_GraphicsFamily, i), m_device);
    for (uint i = 0; i < desc.NumComputeQueues; ++i)
        m_ComputeQueues[i] = VulkanCommandQueueImpl(m_device.getQueue(m_ComputeFamily, i), m_device);
    for (uint i = 0; i < desc.NumCopyQueues; ++i)
        m_CopyQueues[i] = VulkanCommandQueueImpl(m_device.getQueue(m_CopyFamily, i), m_device);
}

VulkanGraphicsDeviceImpl::~VulkanGraphicsDeviceImpl()
{
    if (m_GraphicsQueues) delete[] m_GraphicsQueues;
    if (m_ComputeQueues) delete[] m_ComputeQueues;
    if (m_CopyQueues) delete[] m_CopyQueues;
    if (m_device) m_device.destroy();
    if (m_instance) m_instance.destroy();
}

CommandQueue VulkanGraphicsDeviceImpl::GetCommandQueue(CommandQueueType type, uint index)
{
    switch (type)
    {
    default:
    case CommandQueueType::Graphics:
        return CommandQueue(static_cast<ICommandQueueImpl*>(&m_GraphicsQueues[index]), type);
    case CommandQueueType::Compute:
        return CommandQueue(static_cast<ICommandQueueImpl*>(&m_ComputeQueues[index]), type);
    case CommandQueueType::Copy:
        return CommandQueue(static_cast<ICommandQueueImpl*>(&m_CopyQueues[index]), type);
    }
}

uint VulkanGraphicsDeviceImpl::FindBestQueueFamilyIndex(vk::QueueFlags desiredFlags)
{
    uint bestQueueIndex = UINT32_MAX;
    uint leastExtraBits = UINT32_MAX;
    auto props = m_physicalDevice.getQueueFamilyProperties();
    for (uint32_t i = 0; i < props.size(); ++i)
    {
        if ((props[i].queueFlags & desiredFlags) == desiredFlags)
        {
            auto extra = props[i].queueFlags & ~desiredFlags;
            uint bits = std::popcount(static_cast<uint>(extra));
            if (bits < leastExtraBits)
            {
                leastExtraBits = bits;
                bestQueueIndex = i;
            }
        }
    }
    if (bestQueueIndex == UINT32_MAX)
        throw std::runtime_error("No suitable queue family found");
    return bestQueueIndex;
}

// Add implementation
SwapChain VulkanGraphicsDeviceImpl::CreateSwapChain(const SwapChainDesc& desc)
{
	// Create a Vulkan surface for the window handle
    vk::Win32SurfaceCreateInfoKHR surfaceInfo;
    surfaceInfo.hinstance = GetModuleHandle(nullptr);
    surfaceInfo.hwnd = (HWND)desc.WindowHandle;

    vk::SurfaceKHR surface = m_instance.createWin32SurfaceKHR(surfaceInfo);

    // 1. Get Capabilities (image counts, min/max size, etc.)
    vk::SurfaceCapabilitiesKHR capabilities =
        m_physicalDevice.getSurfaceCapabilitiesKHR(surface);

    // 2. Get Supported Formats (pixel format and color space)
    std::vector<vk::SurfaceFormatKHR> formats =
        m_physicalDevice.getSurfaceFormatsKHR(surface);

    // 3. Get Supported Presentation Modes (VSync, Mailbox, etc.)
    std::vector<vk::PresentModeKHR> presentModes =
        m_physicalDevice.getSurfacePresentModesKHR(surface);

    vk::SurfaceFormatKHR surfaceFormat = formats[1];
    vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo;
    vk::Extent2D extent = capabilities.currentExtent;
    uint32_t imageCount = capabilities.minImageCount;

    // Create a Vulkan command pool allocator
    uint family;

    switch (desc.PresentQueue.GetQueueType())
    {
    case CommandQueueType::Graphics: family = m_GraphicsFamily; break;
    case CommandQueueType::Compute:  family = m_ComputeFamily;  break;
    case CommandQueueType::Copy:     family = m_CopyFamily;     break;
    default:
        throw std::runtime_error("Invalid CommandAllocatorType");
    }

	vk::SwapchainCreateInfoKHR swapchainInfo = {};
	swapchainInfo.surface = surface;
	swapchainInfo.minImageCount = imageCount + 1;
	swapchainInfo.imageFormat = surfaceFormat.format;
	swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainInfo.imageExtent = extent;
	swapchainInfo.imageArrayLayers = 1;
	swapchainInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
	swapchainInfo.preTransform = capabilities.currentTransform;
	swapchainInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	swapchainInfo.presentMode = presentMode;
	swapchainInfo.clipped = VK_TRUE;
    //swapchainInfo.queueFamilyIndexCount = 1;
	//swapchainInfo.pQueueFamilyIndices = &family;

    vk::SwapchainKHR swapchain;
    try
    {
        swapchain = m_device.createSwapchainKHR(swapchainInfo);
    }
    catch (vk::SystemError& err)
    {
		OutputDebugStringA(err.what());
    }

    VulkanSwapChainImpl* impl = new VulkanSwapChainImpl(m_device, surface, swapchain);

    return SwapChain(static_cast<ISwapChainImpl*>(impl));
}

CommandAllocator VulkanGraphicsDeviceImpl::CreateCommandAllocator(const CommandAllocatorDesc& desc)
{
	// Create a Vulkan command pool allocator
    uint family;

    switch (desc.Type)
    {
    case CommandAllocatorType::Graphics: family = m_GraphicsFamily; break;
	case CommandAllocatorType::Compute:  family = m_ComputeFamily;  break;
	case CommandAllocatorType::Copy:     family = m_CopyFamily;     break;
        default:
			throw std::runtime_error("Invalid CommandAllocatorType");
    }

    vk::CommandPoolCreateInfo commandPoolInfo = {};
    commandPoolInfo.queueFamilyIndex = family;
    commandPoolInfo.flags = vk::CommandPoolCreateFlagBits::eTransient | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

	vk::CommandPool commandPool = m_device.createCommandPool(commandPoolInfo);

    VulkanCommandAllocatorImpl* impl = new VulkanCommandAllocatorImpl(m_device, commandPool);
    return CommandAllocator(static_cast<CommandAllocatorImpl*>(impl));
}

Fence VulkanGraphicsDeviceImpl::CreateFence(const FenceDesc& desc)
{
	vk::SemaphoreTypeCreateInfo semaphoreTypeInfo = { vk::SemaphoreType::eTimeline, 0 };
	vk::SemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.pNext = &semaphoreTypeInfo;
    vk::Semaphore semaphore = m_device.createSemaphore(semaphoreInfo);

	VulkanFenceImpl* impl = new VulkanFenceImpl(m_device, semaphore);
    return Fence(static_cast<FenceImpl*>(impl));
}
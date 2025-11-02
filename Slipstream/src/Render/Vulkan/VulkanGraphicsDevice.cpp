#include "pch.h"
#include "Render/Vulkan/VulkanGraphicsDevice.h"
#include "Render/Vulkan/VulkanCommandQueue.h"
#include "Render/Vulkan/VulkanCommandList.h"
#include "Render/Vulkan/VulkanCommandAllocator.h"
#include <stdexcept>

using namespace Slipstream::Render;

VulkanGraphicsDeviceImpl::VulkanGraphicsDeviceImpl(const GraphicsDeviceDesc& desc)
{
    vk::ApplicationInfo appInfo;
    vk::InstanceCreateInfo instInfo({}, &appInfo);
    m_instance = vk::createInstance(instInfo);

    auto physicalDevices = m_instance.enumeratePhysicalDevices();
    if (physicalDevices.empty())
        throw std::runtime_error("No Vulkan physical devices found");

    m_physicalDevice = physicalDevices[0];

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

    vk::DeviceCreateInfo deviceCreateInfo({}, totalQueues, queueCreateInfos);
    m_device = m_physicalDevice.createDevice(deviceCreateInfo);

    if (desc.NumGraphicsQueues)
        m_GraphicsQueues = new VulkanCommandQueueImpl[desc.NumGraphicsQueues];
    if (desc.NumComputeQueues)
        m_ComputeQueues = new VulkanCommandQueueImpl[desc.NumComputeQueues];
    if (desc.NumCopyQueues)
        m_CopyQueues = new VulkanCommandQueueImpl[desc.NumCopyQueues];

    for (uint i = 0; i < desc.NumGraphicsQueues; ++i)
        m_GraphicsQueues[i] = VulkanCommandQueueImpl(m_device.getQueue(m_GraphicsFamily, i));
    for (uint i = 0; i < desc.NumComputeQueues; ++i)
        m_ComputeQueues[i] = VulkanCommandQueueImpl(m_device.getQueue(m_ComputeFamily, i));
    for (uint i = 0; i < desc.NumCopyQueues; ++i)
        m_CopyQueues[i] = VulkanCommandQueueImpl(m_device.getQueue(m_CopyFamily, i));
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
        return CommandQueue(static_cast<ICommandQueueImpl*>(&m_GraphicsQueues[index]));
    case CommandQueueType::Compute:
        return CommandQueue(static_cast<ICommandQueueImpl*>(&m_ComputeQueues[index]));
    case CommandQueueType::Copy:
        return CommandQueue(static_cast<ICommandQueueImpl*>(&m_CopyQueues[index]));
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
            uint bits = 0;// std::popcount(static_cast<uint>(extra));
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
    // TODO
    return SwapChain(static_cast<ISwapChainImpl*>(nullptr));
}

CommandList VulkanGraphicsDeviceImpl::CreateCommandList(const CommandListDesc& desc)
{
    VulkanCommandListImpl* impl = new VulkanCommandListImpl(desc);
    return CommandList(impl);
}

CommandAllocator VulkanGraphicsDeviceImpl::CreateCommandAllocator(const CommandAllocatorDesc& desc)
{
    VulkanCommandAllocatorImpl* impl = new VulkanCommandAllocatorImpl(desc);
    return CommandAllocator(static_cast<CommandAllocatorImpl*>(impl));
}

Fence VulkanGraphicsDeviceImpl::CreateFence(const FenceDesc& desc)
{
    // TODO
    return Fence(static_cast<FenceImpl*>(nullptr));
}
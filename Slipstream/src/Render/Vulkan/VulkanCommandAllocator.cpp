#include "pch.h"
#include "Render/Vulkan/VulkanCommandAllocator.h"
#include <stdexcept>

using namespace Slipstream::Render;

VulkanCommandAllocatorImpl::VulkanCommandAllocatorImpl(const CommandAllocatorDesc& desc)
{

}

VulkanCommandAllocatorImpl::~VulkanCommandAllocatorImpl()
{
    if (m_Device && m_Pool)
    {
        m_Device.destroyCommandPool(m_Pool);
        m_Pool = vk::CommandPool();
    }
}

void VulkanCommandAllocatorImpl::Reset()
{
    m_Device.resetCommandPool(m_Pool);
}
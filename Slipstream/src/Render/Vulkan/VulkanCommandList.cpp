#include "pch.h"
#include "Render/Vulkan/VulkanCommandList.h"

using namespace Slipstream::Render;

VulkanCommandListImpl::VulkanCommandListImpl()
{
    // Framework only: actual creation of command pool and buffer deferred.
    // desc.Type can be used later to pick queue family / usage.
    // TODO: Acquire vk::Device + create vk::CommandPool + allocate vk::CommandBuffer.
}

VulkanCommandListImpl::~VulkanCommandListImpl()
{
    // Framework only: destroy Vulkan resources when they are added.
    // TODO: Destroy command buffer / command pool.
}

void VulkanCommandListImpl::Close()
{    
	m_CommandBuffer.end();
}
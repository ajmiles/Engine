#include "pch.h"
#include "Render/Vulkan/VulkanCommandList.h"

using namespace Slipstream::Render;

VulkanCommandListImpl::VulkanCommandListImpl(const CommandListDesc&)
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


void VulkanCommandListImpl::Reset(CommandAllocator&)
{
    // Framework only: actual reset logic deferred until command pool/buffer creation.
}

void VulkanCommandListImpl::Close()
{
    // Framework only: actual close logic deferred until command buffer creation.
}
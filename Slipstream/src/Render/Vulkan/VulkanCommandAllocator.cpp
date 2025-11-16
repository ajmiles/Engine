#include "pch.h"
#include "Render/Vulkan/VulkanCommandAllocator.h"
#include "Render/Vulkan/VulkanCommandList.h"
#include <stdexcept>

using namespace Slipstream::Render;

VulkanCommandAllocatorImpl::VulkanCommandAllocatorImpl(vk::Device device, vk::CommandPool commandPool)
    : m_Device(device)
	, m_Pool(commandPool)
{
	m_CommandList = std::make_shared<VulkanCommandListImpl>();
}

VulkanCommandAllocatorImpl::~VulkanCommandAllocatorImpl()
{
    if (m_Device && m_Pool)
    {
        m_Device.destroyCommandPool(m_Pool);
        m_Pool = vk::CommandPool();
    }
}

CommandList VulkanCommandAllocatorImpl::AllocateCommandList()
{
    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.setCommandPool(m_Pool);
    allocInfo.setLevel(vk::CommandBufferLevel::ePrimary);
    allocInfo.setCommandBufferCount(1);

    vk::CommandBuffer commandBuffer;
    if (m_Device.allocateCommandBuffers(&allocInfo, &commandBuffer) != vk::Result::eSuccess)
        throw std::runtime_error("Failed to allocate Vulkan command buffer");

	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

	commandBuffer.begin(beginInfo);

	m_CommandList.get()->m_CommandBuffer = commandBuffer;

    //char str[256];
	//sprintf_s(str, "Allocated Vulkan Command Buffer: %p\n", (void*)commandBuffer);
	//OutputDebugStringA(str);

    return CommandList(m_CommandList);
}

void VulkanCommandAllocatorImpl::Reset()
{
    m_Device.resetCommandPool(m_Pool);
}
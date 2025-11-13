#include "pch.h"
#include "Render/Vulkan/VulkanCommandQueue.h"
#include "Render/Vulkan/VulkanCommandList.h"
#include "Render/Vulkan/VulkanFence.h"
#include "Render/Vulkan/VulkanSwapChain.h"

using namespace Slipstream::Render;


void VulkanCommandQueueImpl::ExecuteCommandList(CommandList& commandList)
{
	VulkanCommandListImpl* impl = static_cast<VulkanCommandListImpl*>(commandList.m_Impl);

	vk::SubmitInfo submitInfo;
	submitInfo.setCommandBufferCount(1);
	submitInfo.setPCommandBuffers(&impl->m_CommandBuffer);
	
	m_Queue.submit(submitInfo, nullptr);
}

void VulkanCommandQueueImpl::SignalFence(Fence& fence, uint64_t value)
{
    VulkanFenceImpl* impl = static_cast<VulkanFenceImpl*>(fence.m_Impl);
    VkSemaphore semaphore = impl->m_Semaphore;

    vk::SemaphoreSubmitInfo signalInfo(semaphore, value, vk::PipelineStageFlagBits2::eBottomOfPipe);
    vk::SubmitInfo2 submitInfo({}, {}, {}, signalInfo);

	m_Queue.submit2(submitInfo, nullptr);
}

void VulkanCommandQueueImpl::Present(SwapChain& swapChain, PresentDesc& desc)
{
	VulkanSwapChainImpl* impl = static_cast<VulkanSwapChainImpl*>(swapChain.m_Impl);

	vk::Result res;
	uint32_t   image;
	std::tie(res, image) = m_Device.acquireNextImageKHR(impl->m_SwapChain, UINT64_MAX, nullptr, nullptr);

	vk::PresentInfoKHR presentInfo;
	presentInfo.setSwapchains(impl->m_SwapChain);
	presentInfo.setImageIndices(image);

	m_Queue.presentKHR(presentInfo);
}
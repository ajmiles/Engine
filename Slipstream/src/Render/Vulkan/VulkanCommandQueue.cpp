#include "pch.h"
#include "Render/Vulkan/VulkanCommandQueue.h"
#include "Render/Vulkan/VulkanCommandList.h"
#include "Render/Vulkan/VulkanFence.h"
#include "Render/Vulkan/VulkanSwapChain.h"

using namespace Slipstream::Render;

VulkanCommandQueueImpl::VulkanCommandQueueImpl(vk::Device device, uint queueFamilyIndex, uint queueIndex)
{
    m_ProgressFence = std::make_shared<VulkanFenceImpl>(FenceDesc{}, device, vk::SemaphoreType::eTimeline);

    m_Queue = device.getQueue(queueFamilyIndex, queueIndex);
}

VulkanCommandQueueImpl::~VulkanCommandQueueImpl()
{

}

Waitable VulkanCommandQueueImpl::ExecuteCommandList(CommandList& commandList, uint numWaits, Waitable* waitables)
{
    vk::CommandBufferSubmitInfo commandBufferSubmitInfo = {};
    commandBufferSubmitInfo.setCommandBuffer(static_cast<VulkanCommandListImpl*>(commandList.m_Impl.get())->m_CommandBuffer);

	vk::SubmitInfo2 submitInfo;
    submitInfo.setCommandBufferInfoCount(1);
    submitInfo.setPCommandBufferInfos(&commandBufferSubmitInfo);

    // TODO. Don't use vector here...
    std::vector<vk::SemaphoreSubmitInfo> waitSemaphores;
    waitSemaphores.reserve(numWaits);

    for (uint i = 0; i < numWaits; ++i)
    {
        vk::SemaphoreSubmitInfo waitSemaphoreInfo = {};
        waitSemaphoreInfo.setSemaphore(static_cast<VulkanFenceImpl*>(waitables[i].fence.m_Impl.get())->m_Semaphore);
        waitSemaphoreInfo.setValue(waitables[i].value);

        waitSemaphores.emplace_back(waitSemaphoreInfo);
    }

    submitInfo.setWaitSemaphoreInfos(waitSemaphores);

    vk::SemaphoreSubmitInfo signalSemaphoreInfo = {};
    signalSemaphoreInfo.setSemaphore(m_ProgressFence->m_Semaphore);
    signalSemaphoreInfo.setValue(++m_LastSignalledValue);

    submitInfo.setSignalSemaphoreInfoCount(1);
    submitInfo.setPSignalSemaphoreInfos(&signalSemaphoreInfo);
    
    //char str[256];
	//std::snprintf(str, 256, "ExecuteCommandList Fence = %p Signal Value: %llu\n", (void*)&m_ProgressFence->m_Semaphore, m_LastSignalledValue);
    //OutputDebugStringA(str);

    m_Queue.submit2(submitInfo);

    Waitable result;
    result.fence = Fence(m_ProgressFence);
    result.value = m_LastSignalledValue;
    return result;
}

void VulkanCommandQueueImpl::Present(SwapChain& swapChain, PresentDesc& desc)
{
	VulkanSwapChainImpl* impl = static_cast<VulkanSwapChainImpl*>(swapChain.m_Impl);
    VulkanFenceImpl* waitFenceImpl = static_cast<VulkanFenceImpl*>(desc.SwapChainWaitable.fence.m_Impl.get());

    vk::Semaphore presentSemaphore = impl->m_SwapChainPresentSemaphores[desc.BackBufferIndex]->m_Semaphore;
    impl->m_SwapChainFences[desc.BackBufferIndex] = desc.SwapChainWaitable;

    {
        // Since PresentKHR doesn't support timeline semaphores, we have to use the binary semaphore here.
        // We'll make a dummy submit2 to signal the binary semaphore when the timeline semaphore reaches the desired value.
        // and then use that binary semaphore in Present

        vk::SubmitInfo2 submitInfo;
        submitInfo.setCommandBufferInfoCount(0);
        submitInfo.setWaitSemaphoreInfoCount(1);
        submitInfo.setSignalSemaphoreInfoCount(1);

        vk::SemaphoreSubmitInfo waitSemaphoreInfo = {};
        waitSemaphoreInfo.setSemaphore(waitFenceImpl->m_Semaphore);
        waitSemaphoreInfo.setValue(desc.SwapChainWaitable.value);

        submitInfo.setPWaitSemaphoreInfos(&waitSemaphoreInfo);

        vk::SemaphoreSubmitInfo signalSemaphoreInfo = {};
        signalSemaphoreInfo.setSemaphore(presentSemaphore);

        submitInfo.setPSignalSemaphoreInfos(&signalSemaphoreInfo);

        //char str[256];
		//std::snprintf(str, 256, "Waitable Fence = %p Present Wait Fence Value: %llu\n", (void*)&waitFenceImpl->m_Semaphore, desc.SwapChainWaitable.value);
        //OutputDebugStringA(str);

        m_Queue.submit2(submitInfo);
    }

    // TODO sort this mess out

	vk::PresentInfoKHR presentInfo;
	presentInfo.setSwapchains(impl->m_SwapChain);
    presentInfo.setSwapchainCount(1);
    presentInfo.setPImageIndices(&desc.BackBufferIndex);
    presentInfo.setPWaitSemaphores((const vk::Semaphore*)&presentSemaphore);
    presentInfo.setWaitSemaphoreCount(1);

	m_Queue.presentKHR(presentInfo);
}
#include "pch.h"
#include "Render/Vulkan/VulkanCommon.h"
#include "Render/Vulkan/VulkanCommandList.h"
#include "Render/Vulkan/VulkanTexture.h"

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

void VulkanCommandListImpl::BeginPass(RenderingInfo& info)
{
	vk::RenderingAttachmentInfo colorAttachments[SLIPSTREAM_RENDER_MAX_RENDER_TARGETS] = {};
	vk::RenderingInfo renderingInfo;

	renderingInfo.setRenderArea(vk::Rect2D{ { info.RenderArea.X, info.RenderArea.Y }, { (uint)info.RenderArea.Width, (uint)info.RenderArea.Height } });
	renderingInfo.setLayerCount(1);
	renderingInfo.setViewMask(0);
	renderingInfo.setColorAttachmentCount(info.NumRenderTargets);
	renderingInfo.setPColorAttachments(colorAttachments);
	

	for (uint i = 0; i < info.NumRenderTargets; ++i)
	{
		vk::ImageView* imageView = reinterpret_cast<vk::ImageView*>(info.RenderTargetInfos[i].RTV);
		Color<float>& clearColor = info.RenderTargetInfos[i].ClearColor;

		vk::RenderingAttachmentInfo& colorAttachment = colorAttachments[i];
		colorAttachment.imageView = *imageView;
		colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
		colorAttachment.loadOp = ToVulkanType(info.RenderTargetInfos[i].ClearOp);
		colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
		colorAttachment.clearValue.color = vk::ClearColorValue{ clearColor.R, clearColor.G, clearColor.B, clearColor.A };
	}

	m_CommandBuffer.beginRendering(renderingInfo);
}

void VulkanCommandListImpl::EndPass()
{
	m_CommandBuffer.endRendering();
}

void VulkanCommandListImpl::Barrier(uint numBarriers, Slipstream::Render::Barrier* barriers)
{
	uint numImageBarriers = 0;
	uint numBufferBarriers = 0;
	uint numMemoryBarriers = 0;

    for(uint i = 0; i < numBarriers; ++i)
    {
        switch (barriers[i].Type)
        {
        case BarrierType::Texture: ++numImageBarriers; break;
        case BarrierType::Buffer: ++numBufferBarriers; break;
        case BarrierType::Global: ++numMemoryBarriers; break;
        }
	}

	auto imageBarriers = (vk::ImageMemoryBarrier2 *)_alloca(sizeof(vk::ImageMemoryBarrier2) * numImageBarriers);
	auto bufferBarriers = (vk::BufferMemoryBarrier2 *)_alloca(sizeof(vk::BufferMemoryBarrier2) * numBufferBarriers);
	auto memoryBarriers = (vk::MemoryBarrier2 *)_alloca(sizeof(vk::MemoryBarrier2) * numMemoryBarriers);

    numImageBarriers = 0;
	numBufferBarriers = 0;
	numMemoryBarriers = 0;

    for (uint i = 0; i < numBarriers; ++i)
    {
        switch (barriers[i].Type)
        {
            case BarrierType::Texture:
            {
				vk::ImageMemoryBarrier2 imageBarrier = {};
				imageBarrier.srcStageMask = ToVulkanType(barriers[i].Texture.SyncBefore);
				imageBarrier.srcAccessMask = ToVulkanType(barriers[i].Texture.AccessBefore);
				imageBarrier.oldLayout = ToVulkanType(barriers[i].Texture.LayoutBefore);

				imageBarrier.dstStageMask = ToVulkanType(barriers[i].Texture.SyncAfter);
				imageBarrier.dstAccessMask = ToVulkanType(barriers[i].Texture.AccessAfter);
				imageBarrier.newLayout = ToVulkanType(barriers[i].Texture.LayoutAfter);

				imageBarrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor; // TODO: Handle depth/stencil aspects.
				imageBarrier.subresourceRange.baseMipLevel = barriers[i].Texture.Subresources.FirstMipLevel;
				imageBarrier.subresourceRange.levelCount = barriers[i].Texture.Subresources.NumMipLevels;
				imageBarrier.subresourceRange.baseArrayLayer = barriers[i].Texture.Subresources.FirstArraySlice;
				imageBarrier.subresourceRange.layerCount = barriers[i].Texture.Subresources.NumArraySlices;
				
				imageBarrier.image = static_cast<VulkanTextureImpl*>(barriers[i].Texture.Texture->m_Impl.get())->m_Image;

				//char str[256];
				//std::snprintf(str, 256, "VulkanCommandListImpl::Barrier - Texture Barrier Image: %p, OldLayout: %d, NewLayout: %d\n", (void*)imageBarrier.image, (int)imageBarrier.oldLayout, (int)imageBarrier.newLayout);
				//OutputDebugStringA(str);

				imageBarriers[numImageBarriers++] = imageBarrier;
            }break;
            case BarrierType::Buffer:
            {
                vk::BufferMemoryBarrier2& bufferBarrier = bufferBarriers[numBufferBarriers++];
            }break;
            case BarrierType::Global:
            {
                vk::MemoryBarrier2& memoryBarrier = memoryBarriers[numMemoryBarriers++];
            }break;
        }
    }

	vk::DependencyInfo dependencyInfo = {};
	dependencyInfo.setImageMemoryBarrierCount(numImageBarriers);
	dependencyInfo.setPImageMemoryBarriers(imageBarriers);
	dependencyInfo.setBufferMemoryBarrierCount(numBufferBarriers);
	dependencyInfo.setPBufferMemoryBarriers(bufferBarriers);
	dependencyInfo.setMemoryBarrierCount(numMemoryBarriers);
	dependencyInfo.setPMemoryBarriers(memoryBarriers);

	//char str[256];
	//std::snprintf(str, 256, "VulkanCommandListImpl::Barrier - Issuing Barrier with %u Image Barriers, %u Buffer Barriers, %u Memory Barriers\n", numImageBarriers, numBufferBarriers, numMemoryBarriers);
	//OutputDebugStringA(str);

	m_CommandBuffer.pipelineBarrier2(dependencyInfo);
}
#pragma once
#include <vulkan/vulkan.hpp>
#include "Render/API/Barrier.h"

namespace Slipstream::Render
{
	vk::PipelineStageFlags2 ToVulkanType(BarrierSync sync)
	{
		switch (sync)
		{
			case BarrierSync::None:
				return vk::PipelineStageFlagBits2::eNone;
			case BarrierSync::RenderTarget:
				return vk::PipelineStageFlagBits2::eColorAttachmentOutput;
			default:
				assert(false && "Unknown BarrierSync");
				break;
		}
	}

	vk::AccessFlags2 ToVulkanType(BarrierAccess access)
	{
		switch(access)
		{
			case BarrierAccess::None:
				return vk::AccessFlagBits2::eNone;
			case BarrierAccess::RenderTarget:
				return vk::AccessFlagBits2::eColorAttachmentRead | vk::AccessFlagBits2::eColorAttachmentWrite;
			default:
				assert(false && "Unknown BarrierAccess");
				break;
		}

	}

	vk::ImageLayout ToVulkanType(BarrierLayout layout)
	{
		switch (layout)
		{
			case BarrierLayout::Undefined:
				return vk::ImageLayout::eUndefined;
			case BarrierLayout::RenderTarget:
				return vk::ImageLayout::eColorAttachmentOptimal;
			case BarrierLayout::Present:
				return vk::ImageLayout::ePresentSrcKHR;
			default:
				assert(false && "Unknown BarrierLayout");
				break;
		}
	}
}
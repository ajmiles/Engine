#pragma once
#include <vulkan/vulkan.hpp>
#include "Render/API/Barrier.h"

// TODO: These should probably become LUTs instead of switch statements
namespace Slipstream::Render
{
	static vk::Format ToVulkanType(ResourceFormat format)
	{
		switch (format)
		{
			case ResourceFormat::Unknown:
				return vk::Format::eUndefined;
			case ResourceFormat::R8G8B8A8_UNORM:
				return vk::Format::eR8G8B8A8Unorm;
			case ResourceFormat::R8G8B8A8_UNORM_SRGB:
				return vk::Format::eR8G8B8A8Srgb;
			default:
				assert(false && "Unknown ResourceFormat");
				return vk::Format::eUndefined;
				break;
		}
	}

	static vk::AttachmentLoadOp ToVulkanType(ClearOp clearOp)
	{
		switch (clearOp)
		{
			case ClearOp::Clear:
				return vk::AttachmentLoadOp::eClear;
			case ClearOp::Load:
				return vk::AttachmentLoadOp::eLoad;
			case ClearOp::Discard:
				return vk::AttachmentLoadOp::eDontCare;
			default:
				assert(false && "Unknown ClearOp");
				return vk::AttachmentLoadOp::eDontCare;
				break;
		}
	}

	static vk::PipelineStageFlags2 ToVulkanType(BarrierSync sync)
	{
		switch (sync)
		{
			case BarrierSync::None:
				return vk::PipelineStageFlagBits2::eNone;
			case BarrierSync::RenderTarget:
				return vk::PipelineStageFlagBits2::eColorAttachmentOutput;
			default:
				assert(false && "Unknown BarrierSync");
				return vk::PipelineStageFlagBits2::eNone;
				break;
		}
	}

	static vk::AccessFlags2 ToVulkanType(BarrierAccess access)
	{
		switch(access)
		{
			case BarrierAccess::None:
				return vk::AccessFlagBits2::eNone;
			case BarrierAccess::RenderTarget:
				return vk::AccessFlagBits2::eColorAttachmentRead | vk::AccessFlagBits2::eColorAttachmentWrite;
			default:
				assert(false && "Unknown BarrierAccess");
				return vk::AccessFlagBits2::eNone;
				break;
		}
	}

	static vk::ImageLayout ToVulkanType(BarrierLayout layout)
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
				return vk::ImageLayout::eUndefined;
				break;
		}
	}
}
#pragma once
#include "Core/Types.h"

#define SLIPSTREAM_RENDER_API_VERSION 1

#define SLIPSTREAM_RENDER_MAX_SWAPCHAIN_BUFFERS 3
#define SLIPSTREAM_RENDER_MAX_RENDER_TARGETS 8

#define SLIPSTREAM_RENDER_ALL_REMAINING_MIP_LEVELS 0xFFFFFFFF
#define SLIPSTREAM_RENDER_ALL_REMAINING_ARRAY_SLICES 0xFFFFFFFF

namespace Slipstream::Render
{
	typedef uint64_t RenderTargetView;

	enum class ResourceFormat
	{
		Unknown,
		R8G8B8A8_UNORM,
		R8G8B8A8_UNORM_SRGB,
	};

	struct RenderTargetViewDesc
	{
		class Texture* Texture = nullptr;
		ResourceFormat Format = ResourceFormat::Unknown;
	};

	enum class ClearOp
	{
		Clear,
		Load,
		Discard,
	};

	struct TargetInfo
	{
		RenderTargetView RTV;
		ClearOp ClearOp;
		Color<float> ClearColor;
	};

	struct RenderingInfo
	{
		uint NumRenderTargets;
		TargetInfo* RenderTargetInfos;
		Rect<int> RenderArea;
	};
}



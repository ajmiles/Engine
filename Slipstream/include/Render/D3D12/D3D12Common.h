#pragma once
#include <d3d12.h>
#include "Render/API/Barrier.h"

namespace Slipstream::Render
{
	static DXGI_FORMAT ToD3D12Type(ResourceFormat format)
	{
		switch (format)
		{
			case ResourceFormat::Unknown:
				return DXGI_FORMAT_UNKNOWN;
			case ResourceFormat::R8G8B8A8_UNORM:
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			case ResourceFormat::R8G8B8A8_UNORM_SRGB:
				return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			default:
				assert(false && "Unknown ResourceFormat");
				return DXGI_FORMAT_UNKNOWN;
				break;
		}
	}

	static D3D12_BARRIER_SYNC ToD3D12Type(BarrierSync sync)
	{
		switch (sync)
		{
			case BarrierSync::None:
				return D3D12_BARRIER_SYNC_NONE;
			case BarrierSync::RenderTarget:
				return D3D12_BARRIER_SYNC_RENDER_TARGET;
			default:
				assert(false && "Unknown BarrierSync");
				return D3D12_BARRIER_SYNC_NONE;
				break;
		}
	}

	static D3D12_BARRIER_ACCESS ToD3D12Type(BarrierAccess access)
	{
		switch (access)
		{
			case BarrierAccess::None:
				return D3D12_BARRIER_ACCESS_NO_ACCESS;
			case BarrierAccess::RenderTarget:
				return D3D12_BARRIER_ACCESS_RENDER_TARGET;
			default:
				assert(false && "Unknown BarrierAccess");
				return D3D12_BARRIER_ACCESS_NO_ACCESS;
				break;
		}
	}

	static D3D12_BARRIER_LAYOUT ToD3D12Type(BarrierLayout layout)
	{
		switch (layout)
		{
			case BarrierLayout::Undefined:
				return D3D12_BARRIER_LAYOUT_UNDEFINED;
			case BarrierLayout::RenderTarget:
				return D3D12_BARRIER_LAYOUT_RENDER_TARGET;
			case BarrierLayout::Present:
				return D3D12_BARRIER_LAYOUT_PRESENT;
			default:
				assert(false && "Unknown BarrierLayout");
				return D3D12_BARRIER_LAYOUT_UNDEFINED;
				break;
		}
	}
}
#include "pch.h"
#include "Render/D3D12/D3D12Common.h"
#include "Render/D3D12/D3D12CommandList.h"
#include "Render/D3D12/D3D12CommandAllocator.h"
#include "Render/D3D12/D3D12Texture.h"
#include <stdexcept>

using namespace Slipstream::Render;

D3D12CommandListImpl::D3D12CommandListImpl(ID3D12GraphicsCommandList7* list)
    : m_List(list)
{

}

D3D12CommandListImpl::~D3D12CommandListImpl()
{
    if (m_List)
    {
        m_List->Release();
        m_List = nullptr;
    }
}

void D3D12CommandListImpl::Close()
{
    HRESULT hr = m_List->Close();
    if (FAILED(hr))
        throw std::runtime_error("Failed to close D3D12 command list");
}

void D3D12CommandListImpl::BeginPass(RenderingInfo& renderingInfo)
{
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargets[SLIPSTREAM_RENDER_MAX_RENDER_TARGETS] = {};

    for (uint i = 0; i < renderingInfo.NumRenderTargets; ++i)
    {
        renderTargets[i] = D3D12_CPU_DESCRIPTOR_HANDLE{ renderingInfo.RenderTargetInfos[i].RTV };

        if(renderingInfo.RenderTargetInfos[i].ClearOp == ClearOp::Clear)
        {
            Color<float>& clearColor = renderingInfo.RenderTargetInfos[i].ClearColor;
            FLOAT clearColorValues[4] = { clearColor.R, clearColor.G, clearColor.B, clearColor.A };
            m_List->ClearRenderTargetView(renderTargets[i], clearColorValues, 0, nullptr);
		}
        else
        {
			assert(false && "Load and Discard clear operations not implemented yet");
        }
	}

	// Set render targets
    m_List->OMSetRenderTargets(
        renderingInfo.NumRenderTargets,
        reinterpret_cast<D3D12_CPU_DESCRIPTOR_HANDLE*>(renderTargets),
        FALSE,
        nullptr);
}

void D3D12CommandListImpl::EndPass()
{
    // End render pass
}

void D3D12CommandListImpl::Barrier(uint numBarriers, Slipstream::Render::Barrier* barriers)
{
    uint numImageBarriers = 0;
    uint numBufferBarriers = 0;
    uint numMemoryBarriers = 0;

    for (uint i = 0; i < numBarriers; ++i)
    {
        switch (barriers[i].Type)
        {
        case BarrierType::Texture: ++numImageBarriers; break;
        case BarrierType::Buffer: ++numBufferBarriers; break;
        case BarrierType::Global: ++numMemoryBarriers; break;
        }
    }

    D3D12_BARRIER_GROUP barrierGroups[3] = {};

    D3D12_BARRIER_GROUP& imageBarrierGroup = barrierGroups[0];
    D3D12_TEXTURE_BARRIER* pTextureBarriers = (D3D12_TEXTURE_BARRIER*)_alloca(sizeof(D3D12_TEXTURE_BARRIER) * numImageBarriers);
	imageBarrierGroup.Type = D3D12_BARRIER_TYPE_TEXTURE;
	imageBarrierGroup.NumBarriers = numImageBarriers;    
	imageBarrierGroup.pTextureBarriers = pTextureBarriers;

	D3D12_BARRIER_GROUP& bufferBarrierGroup = barrierGroups[1];
	D3D12_BUFFER_BARRIER* pBufferBarriers = (D3D12_BUFFER_BARRIER*)_alloca(sizeof(D3D12_BUFFER_BARRIER) * numBufferBarriers);
	bufferBarrierGroup.Type = D3D12_BARRIER_TYPE_BUFFER;
	bufferBarrierGroup.NumBarriers = numBufferBarriers;
	bufferBarrierGroup.pBufferBarriers = pBufferBarriers;

	D3D12_BARRIER_GROUP& globalBarrierGroup = barrierGroups[2];
	D3D12_GLOBAL_BARRIER* pGlobalBarriers = (D3D12_GLOBAL_BARRIER*)_alloca(sizeof(D3D12_GLOBAL_BARRIER) * numMemoryBarriers);
	globalBarrierGroup.Type = D3D12_BARRIER_TYPE_GLOBAL;
	globalBarrierGroup.NumBarriers = numMemoryBarriers;
	globalBarrierGroup.pGlobalBarriers = pGlobalBarriers;

	numImageBarriers = 0;
	numBufferBarriers = 0;
	numMemoryBarriers = 0;

    for (uint i = 0; i < numBarriers; ++i)
    {
        switch (barriers[i].Type)
        {
        case BarrierType::Texture:
        {
            TextureBarrier& src = barriers[i].Texture;
            D3D12_TEXTURE_BARRIER& dst = pTextureBarriers[numImageBarriers++];
			dst.SyncBefore = ToD3D12Type(src.SyncBefore);
            dst.SyncAfter = ToD3D12Type(src.SyncAfter);
            dst.AccessBefore = ToD3D12Type(src.AccessBefore);
            dst.AccessAfter = ToD3D12Type(src.AccessAfter);
            dst.LayoutBefore = ToD3D12Type(src.LayoutBefore);
            dst.LayoutAfter = ToD3D12Type(src.LayoutAfter);
            dst.pResource = static_cast<D3D12TextureImpl*>(barriers[i].Texture.Texture->m_Impl.get())->m_Resource;
			dst.Subresources.FirstPlane = 0;
            dst.Subresources.NumPlanes = 1;
            dst.Subresources.IndexOrFirstMipLevel = src.Subresources.FirstMipLevel;
            dst.Subresources.NumMipLevels = src.Subresources.NumMipLevels;
            dst.Subresources.FirstArraySlice = src.Subresources.FirstArraySlice;
            dst.Subresources.NumArraySlices = src.Subresources.NumArraySlices;
			dst.Flags = D3D12_TEXTURE_BARRIER_FLAG_NONE;

            if(src.Subresources.NumMipLevels == SLIPSTREAM_RENDER_ALL_REMAINING_MIP_LEVELS ||
               src.Subresources.NumArraySlices == SLIPSTREAM_RENDER_ALL_REMAINING_ARRAY_SLICES)
            {
                D3D12_RESOURCE_DESC desc = dst.pResource->GetDesc();
                if (src.Subresources.NumMipLevels == SLIPSTREAM_RENDER_ALL_REMAINING_MIP_LEVELS)
                {
                    dst.Subresources.NumMipLevels = desc.MipLevels - src.Subresources.FirstMipLevel;
                }
                if (src.Subresources.NumArraySlices == SLIPSTREAM_RENDER_ALL_REMAINING_ARRAY_SLICES)
                {
					assert(desc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER && "Buffer resources do not have array slices");
                    if (desc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE3D)
                    {
                        dst.Subresources.NumArraySlices = desc.DepthOrArraySize - src.Subresources.FirstArraySlice;
                    }
                    else
                    {
						dst.Subresources.NumArraySlices = 1;
                    }
                }
			}

        }break;
        case BarrierType::Buffer:
        {
            BufferBarrier& src = barriers[i].Buffer;
            D3D12_BUFFER_BARRIER& dst = pBufferBarriers[numBufferBarriers++];
			assert(false && "Buffer barriers not implemented yet");
            //dst.SyncBefore = ToD3D12Type(src.SyncBefore);
            //dst.SyncAfter = ToD3D12Type(src.SyncAfter);
            //dst.AccessBefore = ToD3D12Type(src.AccessBefore);
            //dst.AccessAfter = ToD3D12Type(src.AccessAfter);
        }break;
        case BarrierType::Global:
        {
            GlobalBarrier& src = barriers[i].Global;
            D3D12_GLOBAL_BARRIER& dst = pGlobalBarriers[numMemoryBarriers++];
			assert(false && "Global barriers not implemented yet");
            //dst.SyncBefore = ToD3D12Type(src.SyncBefore);
            //dst.SyncAfter = ToD3D12Type(src.SyncAfter);
            //dst.AccessBefore = ToD3D12Type(src.AccessBefore);
            //dst.AccessAfter = ToD3D12Type(src.AccessAfter);
        }break;
        }
    }

	m_List->Barrier(ARRAYSIZE(barrierGroups), barrierGroups);
}
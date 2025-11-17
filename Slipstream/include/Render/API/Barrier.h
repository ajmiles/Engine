#pragma once
#include "Render/API/Texture.h"
#include "Render/API/SlipstreamRender.h"

namespace Slipstream::Render 
{
    enum class BarrierSync
    {
        None = 0,
        RenderTarget = 1
    };

    enum class BarrierAccess
    {
        None = 0,
        RenderTarget = 1
    };

    enum class BarrierLayout
    {
        Undefined,
        RenderTarget,
        Present
    };

    enum class BarrierType
    {
        Global,
        Texture,
        Buffer
    };

    struct SubresourceRange
    {
        uint FirstMipLevel;
		uint NumMipLevels;
        uint FirstArraySlice;
		uint NumArraySlices;

        void SetToAllSubresources()
        {
            FirstMipLevel = 0;
            NumMipLevels = SLIPSTREAM_RENDER_ALL_REMAINING_MIP_LEVELS;
            FirstArraySlice = 0;
            NumArraySlices = SLIPSTREAM_RENDER_ALL_REMAINING_ARRAY_SLICES;
		}
    };

    struct GlobalBarrier
    {
        BarrierSync     SyncBefore;
        BarrierAccess   AccessBefore;
        BarrierSync     SyncAfter;
        BarrierAccess   AccessAfter;
    };

    struct TextureBarrier
    {
        BarrierSync     SyncBefore;
        BarrierAccess   AccessBefore;
        BarrierLayout   LayoutBefore;
        BarrierSync     SyncAfter;
        BarrierAccess   AccessAfter;
        BarrierLayout   LayoutAfter;
        class Texture*  Texture;
        SubresourceRange Subresources;
    };

    struct BufferBarrier
    {
		class Buffer* Buffer;
    };

    struct Barrier
    {
        BarrierType Type;
        union
        {
            GlobalBarrier Global;
            TextureBarrier Texture;
            BufferBarrier Buffer;
        };

        static Barrier CreateTextureBarrier(
            class Texture* texture,
            BarrierSync syncBefore,
            BarrierAccess accessBefore,
            BarrierLayout layoutBefore,
            BarrierSync syncAfter,
            BarrierAccess accessAfter,
            BarrierLayout layoutAfter,
            const SubresourceRange* subresources = nullptr)
        {
            Barrier barrier = {};
            barrier.Type = BarrierType::Texture;
            barrier.Texture.Texture = texture;
            barrier.Texture.SyncBefore = syncBefore;
            barrier.Texture.AccessBefore = accessBefore;
            barrier.Texture.LayoutBefore = layoutBefore;
            barrier.Texture.SyncAfter = syncAfter;
            barrier.Texture.AccessAfter = accessAfter;
            barrier.Texture.LayoutAfter = layoutAfter;

            if (subresources == nullptr)
				barrier.Texture.Subresources.SetToAllSubresources();
            else
                barrier.Texture.Subresources = *subresources;

            return barrier;
		}
    };

}
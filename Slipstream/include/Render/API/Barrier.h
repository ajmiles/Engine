#pragma once

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
    RenderTarget
};

enum class BarrierType
{
    Global,
    Texture,
    Buffer
};

struct SubresourceRange
{
    // TODO.
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
    // TODO Texture reference
    SubresourceRange Subresources;
};

struct BufferBarrier
{
    // Buffer barrier specifics
};

struct Barrier
{
    BarrierType type;
    union
    {
        GlobalBarrier Global;
        TextureBarrier Texture;
        BufferBarrier Buffer;
    };
};
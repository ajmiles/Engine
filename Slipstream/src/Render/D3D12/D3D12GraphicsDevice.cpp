#include "pch.h"
#include "Render/D3D12/D3D12GraphicsDevice.h"
#include "Render/D3D12/D3D12CommandQueue.h"
#include "Render/D3D12/D3D12SwapChain.h"
#include "Render/D3D12/D3D12CommandList.h"
#include "Render/D3D12/D3D12CommandAllocator.h"
#include "Render/D3D12/D3D12Fence.h"
#include <stdexcept>

using namespace Slipstream::Render;

// TODO. Put this somewhere common?
static D3D12_COMMAND_LIST_TYPE ToD3DType(CommandAllocatorType type)
{
    switch (type)
    {
    case CommandAllocatorType::Compute: return D3D12_COMMAND_LIST_TYPE_COMPUTE;
    case CommandAllocatorType::Copy:    return D3D12_COMMAND_LIST_TYPE_COPY;
    default:                            return D3D12_COMMAND_LIST_TYPE_DIRECT;
    }
}

D3D12GraphicsDeviceImpl::D3D12GraphicsDeviceImpl(const GraphicsDeviceDesc& desc)
{
    // Enable the D3D12 debug layer if requested.
    if(desc.Flags & GraphicsDeviceFlags::Debug)
    {
        ID3D12Debug* debugController = nullptr;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
            debugController->Release();
        }
    }

    HRESULT hr = D3D12CreateDevice(
        nullptr,
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&m_device)
    );
    if (FAILED(hr))
        throw std::runtime_error("Failed to create D3D12 device");

    if (desc.NumGraphicsQueues)
        m_GraphicsQueues = new D3D12CommandQueueImpl[desc.NumGraphicsQueues];
    if (desc.NumComputeQueues)
        m_ComputeQueues = new D3D12CommandQueueImpl[desc.NumComputeQueues];
    if (desc.NumCopyQueues)
        m_CopyQueues = new D3D12CommandQueueImpl[desc.NumCopyQueues];

    auto CreateQueue = [&](D3D12_COMMAND_LIST_TYPE type, ID3D12CommandQueue** out)
    {
        D3D12_COMMAND_QUEUE_DESC qdesc = {};
        qdesc.Type = type;
        qdesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        HRESULT hq = m_device->CreateCommandQueue(&qdesc, IID_PPV_ARGS(out));
        if (FAILED(hq))
            throw std::runtime_error("Failed to create D3D12 command queue");
    };

    for (uint i = 0; i < desc.NumGraphicsQueues; ++i)
        CreateQueue(D3D12_COMMAND_LIST_TYPE_DIRECT, &m_GraphicsQueues[i].m_Queue);
    for (uint i = 0; i < desc.NumComputeQueues; ++i)
        CreateQueue(D3D12_COMMAND_LIST_TYPE_COMPUTE, &m_ComputeQueues[i].m_Queue);
    for (uint i = 0; i < desc.NumCopyQueues; ++i)
        CreateQueue(D3D12_COMMAND_LIST_TYPE_COPY, &m_CopyQueues[i].m_Queue);
}

D3D12GraphicsDeviceImpl::~D3D12GraphicsDeviceImpl()
{
    delete[] m_GraphicsQueues;
    delete[] m_ComputeQueues;
    delete[] m_CopyQueues;

    m_device->Release();
}

CommandQueue D3D12GraphicsDeviceImpl::GetCommandQueue(CommandQueueType type, uint index)
{
    switch (type)
    {
    default:
    case CommandQueueType::Graphics:
        return CommandQueue(static_cast<ICommandQueueImpl*>(&m_GraphicsQueues[index]));
    case CommandQueueType::Compute:
        return CommandQueue(static_cast<ICommandQueueImpl*>(&m_ComputeQueues[index]));
    case CommandQueueType::Copy:
        return CommandQueue(static_cast<ICommandQueueImpl*>(&m_CopyQueues[index]));
    }
}

SwapChain D3D12GraphicsDeviceImpl::CreateSwapChain(const SwapChainDesc& desc)
{
    // Create the swap chain
    D3D12SwapChainImpl* swapChainImpl = new D3D12SwapChainImpl(desc);

    return SwapChain(swapChainImpl);
}

CommandList D3D12GraphicsDeviceImpl::CreateCommandList(const CommandListDesc& desc)
{
    ID3D12GraphicsCommandList* d3dCommandList = nullptr;

    HRESULT hr = m_device->CreateCommandList1(
        0,
        ToD3DType(static_cast<CommandAllocatorType>(desc.Type)),
        D3D12_COMMAND_LIST_FLAG_NONE,
        IID_PPV_ARGS(&d3dCommandList)
    );

    if (FAILED(hr))
        throw std::runtime_error("Failed to create D3D12 command list");

    D3D12CommandListImpl* impl = new D3D12CommandListImpl(d3dCommandList);

    return CommandList(static_cast<ICommandListImpl*>(impl));
}

CommandAllocator D3D12GraphicsDeviceImpl::CreateCommandAllocator(const CommandAllocatorDesc& desc)
{
    ID3D12CommandAllocator* allocator = nullptr;

    HRESULT hr = m_device->CreateCommandAllocator(
        ToD3DType(desc.Type),
        IID_PPV_ARGS(&allocator)
    );

    if (FAILED(hr))
        throw std::runtime_error("Failed to create D3D12 command allocator");

    D3D12CommandAllocatorImpl* impl = new D3D12CommandAllocatorImpl(allocator);

    return CommandAllocator(static_cast<CommandAllocatorImpl*>(impl));
}

Fence D3D12GraphicsDeviceImpl::CreateFence(const FenceDesc& desc)
{
    ID3D12Fence* fence = nullptr;
    HRESULT hr = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
    if (FAILED(hr))
        throw std::runtime_error("Failed to create D3D12 fence");

    D3D12FenceImpl* impl = new D3D12FenceImpl(fence);
    return Fence(static_cast<FenceImpl*>(impl));
}
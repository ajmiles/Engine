#include "pch.h"
#include "Render/D3D12/D3D12GraphicsDevice.h"
#include "Render/D3D12/D3D12CommandQueue.h"
#include "Render/D3D12/D3D12SwapChain.h"
#include "Render/D3D12/D3D12CommandList.h"
#include "Render/D3D12/D3D12CommandAllocator.h"
#include "Render/D3D12/D3D12Fence.h"
#include <stdexcept>

using namespace Slipstream::Render;

D3D12GraphicsDeviceImpl::D3D12GraphicsDeviceImpl(const GraphicsDeviceDesc& desc)
{
    // Create an IDXGIFactory for adapter enumeration, swap chain creation etc
	HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));

    if(FAILED(hr))
		throw std::runtime_error("Failed to create DXGI Factory");

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

    hr = D3D12CreateDevice(
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
        return CommandQueue(static_cast<ICommandQueueImpl*>(&m_GraphicsQueues[index]), type);
    case CommandQueueType::Compute:
        return CommandQueue(static_cast<ICommandQueueImpl*>(&m_ComputeQueues[index]), type);
    case CommandQueueType::Copy:
        return CommandQueue(static_cast<ICommandQueueImpl*>(&m_CopyQueues[index]), type);
    }
}

SwapChain D3D12GraphicsDeviceImpl::CreateSwapChain(const SwapChainDesc& desc)
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = 0; // Use automatic sizing.
    swapChainDesc.Height = 0; // Use automatic sizing.
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = FALSE;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = desc.BufferCount;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

	IDXGISwapChain1* dxgiSwapChain = nullptr;
	IDXGIFactory2* dxgiFactory2 = nullptr;
	// Query for IDXGIFactory2 to use CreateSwapChainForHwnd
	m_dxgiFactory->QueryInterface(IID_PPV_ARGS(&dxgiFactory2));

	D3D12CommandQueueImpl* presentQueueImpl = static_cast<D3D12CommandQueueImpl*>(desc.PresentQueue.m_Impl);

    HRESULT hr = dxgiFactory2->CreateSwapChainForHwnd(
        presentQueueImpl->m_Queue,
        (HWND)(desc.WindowHandle),
        &swapChainDesc,
        nullptr,
        nullptr,
        &dxgiSwapChain
    );

    if(FAILED(hr))
		throw std::runtime_error("Failed to create D3D12 swap chain");

    // Create the swap chain
    D3D12SwapChainImpl* swapChainImpl = new D3D12SwapChainImpl(dxgiSwapChain);

    return SwapChain(swapChainImpl);
}
/*
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
*/
CommandAllocator D3D12GraphicsDeviceImpl::CreateCommandAllocator(const CommandAllocatorDesc& desc)
{
    ID3D12CommandAllocator* allocator = nullptr;

    HRESULT hr = m_device->CreateCommandAllocator(
        ToD3DType(desc.Type),
        IID_PPV_ARGS(&allocator)
    );

    if (FAILED(hr))
        throw std::runtime_error("Failed to create D3D12 command allocator");

    ID3D12GraphicsCommandList* d3dCommandList = nullptr;

    hr = m_device->CreateCommandList1(
        0,
        ToD3DType(static_cast<CommandAllocatorType>(desc.Type)),
        D3D12_COMMAND_LIST_FLAG_NONE,
        IID_PPV_ARGS(&d3dCommandList)
    );

    if (FAILED(hr))
        throw std::runtime_error("Failed to create D3D12 command list");

    D3D12CommandAllocatorImpl* impl = new D3D12CommandAllocatorImpl(allocator, d3dCommandList);

    return CommandAllocator(static_cast<CommandAllocatorImpl*>(impl));
}

Fence D3D12GraphicsDeviceImpl::CreateFence(const FenceDesc&)
{
    ID3D12Fence* fence = nullptr;
    HRESULT hr = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
    if (FAILED(hr))
        throw std::runtime_error("Failed to create D3D12 fence");

    D3D12FenceImpl* impl = new D3D12FenceImpl(fence);
    return Fence(static_cast<FenceImpl*>(impl));
}
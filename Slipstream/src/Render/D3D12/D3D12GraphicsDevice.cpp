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

    m_GraphicsQueues.reserve(desc.NumGraphicsQueues);
    m_ComputeQueues.reserve(desc.NumComputeQueues);
    m_CopyQueues.reserve(desc.NumCopyQueues);

    for (uint i = 0; i < desc.NumGraphicsQueues; ++i)
        m_GraphicsQueues.emplace_back(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT);
    for (uint i = 0; i < desc.NumComputeQueues; ++i)
        m_ComputeQueues.emplace_back(m_device, D3D12_COMMAND_LIST_TYPE_COMPUTE);
    for (uint i = 0; i < desc.NumCopyQueues; ++i)
        m_CopyQueues.emplace_back(m_device, D3D12_COMMAND_LIST_TYPE_COPY);
}

D3D12GraphicsDeviceImpl::~D3D12GraphicsDeviceImpl()
{
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
    D3D12SwapChainImpl* swapChainImpl = new D3D12SwapChainImpl(desc, m_dxgiFactory);
    return SwapChain(swapChainImpl);
}

CommandAllocator D3D12GraphicsDeviceImpl::CreateCommandAllocator(const CommandAllocatorDesc& desc)
{
    ID3D12CommandAllocator* allocator = nullptr;

    HRESULT hr = m_device->CreateCommandAllocator(
        D3D12CommandAllocatorImpl::ToD3DType(desc.Type),
        IID_PPV_ARGS(&allocator)
    );

    if (FAILED(hr))
        throw std::runtime_error("Failed to create D3D12 command allocator");

    ID3D12GraphicsCommandList* d3dCommandList = nullptr;

    hr = m_device->CreateCommandList1(
        0,
        D3D12CommandAllocatorImpl::ToD3DType(static_cast<CommandAllocatorType>(desc.Type)),
        D3D12_COMMAND_LIST_FLAG_NONE,
        IID_PPV_ARGS(&d3dCommandList)
    );

    if (FAILED(hr))
        throw std::runtime_error("Failed to create D3D12 command list");

    D3D12CommandAllocatorImpl* impl = new D3D12CommandAllocatorImpl(allocator, d3dCommandList);

    return CommandAllocator(static_cast<CommandAllocatorImpl*>(impl));
}

Fence D3D12GraphicsDeviceImpl::CreateFence(const FenceDesc& desc)
{
    std::shared_ptr<D3D12FenceImpl> impl = std::make_shared<D3D12FenceImpl>(desc, m_device);
    return Fence(impl);
}
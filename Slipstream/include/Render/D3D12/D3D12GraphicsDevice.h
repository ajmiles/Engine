#pragma once
#include "Render/API/GraphicsDevice.h"
#include "Render/API/CommandQueue.h"
#include "Render/API/CommandList.h"
#include "Render/API/CommandAllocator.h"
#include <d3d12.h>
#include <cassert>

namespace Slipstream::Render
{
    class D3D12CommandQueueImpl;

    class D3D12GraphicsDeviceImpl : public IGraphicsDeviceImpl
    {
    private:
        D3D12GraphicsDeviceImpl(const GraphicsDeviceDesc& desc);
        ~D3D12GraphicsDeviceImpl();

        SwapChain        CreateSwapChain(const SwapChainDesc& desc) override;
        CommandQueue     GetCommandQueue(CommandQueueType type, uint index) override;
        CommandAllocator CreateCommandAllocator(const CommandAllocatorDesc& desc) override;
        Fence            CreateFence(const FenceDesc& desc) override;

		IDXGIFactory* m_dxgiFactory = nullptr;
        ID3D12Device4* m_device = nullptr;

        D3D12CommandQueueImpl* m_GraphicsQueues = nullptr;
        D3D12CommandQueueImpl* m_ComputeQueues  = nullptr;
        D3D12CommandQueueImpl* m_CopyQueues     = nullptr;

        friend class GraphicsDevice;
    };
}
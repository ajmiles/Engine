#pragma once
#include "Render/API/GraphicsDevice.h"
#include "Render/API/CommandQueue.h"
#include "Render/API/CommandList.h"
#include "Render/API/CommandAllocator.h"
#include <d3d12.h>

#include <mutex>

namespace Slipstream::Render
{
    class D3D12GraphicsDeviceImpl : public IGraphicsDeviceImpl
    {
    private:
        D3D12GraphicsDeviceImpl(const GraphicsDeviceDesc& desc);
        ~D3D12GraphicsDeviceImpl();

        SwapChain        CreateSwapChain(const SwapChainDesc& desc) override;
        CommandQueue     GetCommandQueue(CommandQueueType type, uint index) override;
        CommandAllocator CreateCommandAllocator(const CommandAllocatorDesc& desc) override;
        Fence            CreateFence(const FenceDesc& desc) override;

		RenderTargetView CreateRenderTargetView(const RenderTargetViewDesc& desc) override;
		void DestroyRenderTargetView(const RenderTargetView rtv) override;

		IDXGIFactory* m_dxgiFactory = nullptr;
        ID3D12Device4* m_device = nullptr;

        std::vector<D3D12CommandQueueImpl> m_GraphicsQueues;
        std::vector<D3D12CommandQueueImpl> m_ComputeQueues;
        std::vector<D3D12CommandQueueImpl> m_CopyQueues;

		ID3D12DescriptorHeap* m_RTVDescriptorHeap = nullptr;
		std::vector<RenderTargetView> m_RTVDescriptorHeapFreeList;

        ID3D12DescriptorHeap* m_DSVDescriptorHeap = nullptr;
		std::vector<uint16> m_DSVDescriptorHeapFreeList;

        std::mutex m_CriticalSection;

        friend class GraphicsDevice;
    };
}
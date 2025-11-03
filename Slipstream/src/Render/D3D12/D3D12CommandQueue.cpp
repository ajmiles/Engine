#include "pch.h"
#include "Render/D3D12/D3D12CommandQueue.h"
#include "Render/D3D12/D3D12CommandList.h"
#include "Render/D3D12/D3D12Fence.h"
#include "Render/D3D12/D3D12SwapChain.h"
#include "Render/API/CommandList.h"

namespace Slipstream::Render
{
    void D3D12CommandQueueImpl::ExecuteCommandList(CommandList& commandList)
    {
        auto d3d12CommandList = static_cast<D3D12CommandListImpl*>(commandList.m_Impl);
        ID3D12CommandList* ppCommandLists[] = { d3d12CommandList->GetNative() };
        m_Queue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
    }

	void D3D12CommandQueueImpl::SignalFence(Fence& fence, uint64 value)
    {
        auto d3d12FenceImpl = static_cast<D3D12FenceImpl*>(fence.m_Impl);
        m_Queue->Signal(d3d12FenceImpl->m_Fence, value);
	}

    void D3D12CommandQueueImpl::Present(SwapChain& swapChain, PresentDesc& desc)
    {
		D3D12SwapChainImpl* swapChainImpl = static_cast<D3D12SwapChainImpl*>(swapChain.m_Impl);
		swapChainImpl->m_SwapChain->Present(1, 0);
	}
}
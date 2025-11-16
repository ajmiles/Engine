#include "pch.h"
#include "Render/D3D12/D3D12CommandQueue.h"
#include "Render/D3D12/D3D12CommandList.h"
#include "Render/D3D12/D3D12Fence.h"
#include "Render/D3D12/D3D12SwapChain.h"
#include "Render/API/CommandList.h"

namespace Slipstream::Render
{
    D3D12CommandQueueImpl::D3D12CommandQueueImpl(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type)
    {
        m_ProgressFence = std::make_shared<D3D12FenceImpl>(FenceDesc{}, device);

        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Type = type;
        queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.NodeMask = 0;
        HRESULT hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_Queue));
        if (FAILED(hr))
            throw std::runtime_error("Failed to create D3D12 command queue");
    }

    D3D12CommandQueueImpl::~D3D12CommandQueueImpl()
    {
        if (m_Queue)
        {
            m_Queue->Release();
            m_Queue = nullptr;
        }
    }

    Waitable D3D12CommandQueueImpl::ExecuteCommandList(CommandList& commandList, uint numWaits, Waitable* waitResults)
    {
        D3D12CommandListImpl* d3d12CommandList = static_cast<D3D12CommandListImpl*>(commandList.m_Impl.get());
        ID3D12CommandList* ppCommandLists[] = { d3d12CommandList->m_List };
        
        m_Queue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
        m_Queue->Signal(m_ProgressFence->m_Fence, ++m_LastSignalledValue);

        return Waitable(Fence(m_ProgressFence), m_LastSignalledValue);
    }

    void D3D12CommandQueueImpl::Present(SwapChain& swapChain, PresentDesc& desc)
    {
		D3D12SwapChainImpl* swapChainImpl = static_cast<D3D12SwapChainImpl*>(swapChain.m_Impl);
		swapChainImpl->m_SwapChain->Present(1, 0);

		m_Queue->Signal(m_ProgressFence->m_Fence, ++m_LastSignalledValue);

		Waitable frameEndWaitable = Waitable(Fence(m_ProgressFence), m_LastSignalledValue);
        swapChainImpl->EndFrame(desc.BackBufferIndex, frameEndWaitable);
	}
}
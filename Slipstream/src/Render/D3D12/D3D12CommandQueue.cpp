#include "pch.h"
#include "Render/D3D12/D3D12CommandQueue.h"
#include "Render/D3D12/D3D12CommandList.h"
#include "Render/API/CommandList.h"

namespace Slipstream::Render
{
    void D3D12CommandQueueImpl::ExecuteCommandList(CommandList& commandList)
    {
        auto d3d12CommandList = static_cast<D3D12CommandListImpl*>(commandList.m_Impl);
        ID3D12CommandList* ppCommandLists[] = { d3d12CommandList->GetNative() };
        m_Queue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
    }
}
#pragma once
#include "Render/API/CommandQueue.h"
#include "Render/D3D12/D3D12Fence.h"
#include <d3d12.h>

namespace Slipstream::Render
{
    class D3D12CommandQueueImpl : public ICommandQueueImpl
    {
    public:
        D3D12CommandQueueImpl(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type);
        ~D3D12CommandQueueImpl() override;

        Waitable ExecuteCommandList(class CommandList& commandList, uint numWaits, Waitable* waitables) override;
        void Present(class SwapChain& swapChain, PresentDesc& desc) override;

        ID3D12CommandQueue* m_Queue = nullptr;
        
        std::shared_ptr<D3D12FenceImpl> m_ProgressFence;
        uint64_t m_LastSignalledValue = 0;
    };
} // namespace Slipstream::Render
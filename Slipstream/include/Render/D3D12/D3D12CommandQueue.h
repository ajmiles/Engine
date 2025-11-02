#pragma once
#include "Render/API/CommandQueue.h"
#include <d3d12.h>

namespace Slipstream::Render
{
    class D3D12CommandQueueImpl : public ICommandQueueImpl
    {
    private:
        D3D12CommandQueueImpl() = default;
        ~D3D12CommandQueueImpl() override
        {
            m_Queue->Release();
        }

        void ExecuteCommandList(class CommandList& commandList) override;

        ID3D12CommandQueue* m_Queue = nullptr;

        friend class D3D12GraphicsDeviceImpl;
    };
} // namespace Slipstream::Render
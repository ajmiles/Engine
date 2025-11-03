#include "pch.h"
#include "Render/API/CommandQueue.h"
#include "Render/API/CommandList.h"

using namespace Slipstream::Render;

void CommandQueue::ExecuteCommandList(CommandList& commandList)
{
    m_Impl->ExecuteCommandList(commandList);
}

void CommandQueue::SignalFence(Fence& fence, uint64 value)
{
    m_Impl->SignalFence(fence, value);
}

void CommandQueue::Present(SwapChain& swapChain, PresentDesc& desc)
{
    m_Impl->Present(swapChain, desc);
}
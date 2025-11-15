#include "pch.h"
#include "Render/API/CommandQueue.h"
#include "Render/API/CommandList.h"

using namespace Slipstream::Render;

Waitable CommandQueue::ExecuteCommandList(CommandList& commandList, uint numWaits, Waitable* waitables)
{
    return m_Impl->ExecuteCommandList(commandList, numWaits, waitables);
}

void CommandQueue::Present(SwapChain& swapChain, PresentDesc& desc)
{
    m_Impl->Present(swapChain, desc);
}
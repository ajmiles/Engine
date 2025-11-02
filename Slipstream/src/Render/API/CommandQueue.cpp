#include "pch.h"
#include "Render/API/CommandQueue.h"
#include "Render/API/CommandList.h"

using namespace Slipstream::Render;

void CommandQueue::ExecuteCommandList(CommandList& commandList)
{
    m_Impl->ExecuteCommandList(commandList);
}
#include "Slipstream.h"
#include <thread> // Add this include for std::this_thread
#include <chrono> // Add this include for std::chrono

using namespace Slipstream::System;
using namespace Slipstream::Render;

int WinMain()
{
	Window window;

    GraphicsDeviceDesc gdDesc = {};
    gdDesc.API = GraphicsAPI::Direct3D12;
    gdDesc.Flags = GraphicsDeviceFlags::Debug;

    GraphicsDevice graphicsDevice(gdDesc);
    CommandQueue graphicsQueue = graphicsDevice.GetCommandQueue(CommandQueueType::Graphics, 0);

    uint64_t nextFenceValue = 0;

    SwapChainDesc scDesc = { graphicsQueue, window.GetHandle(), 2 };
    SwapChain swapChain = graphicsDevice.CreateSwapChain(scDesc);
    CommandAllocator commandAllocator = graphicsDevice.CreateCommandAllocator({ CommandAllocatorType::Graphics });
    CommandList commandList = graphicsDevice.CreateCommandList({ CommandListType::Graphics });
    Fence fence = graphicsDevice.CreateFence({});

    while (window.Update())
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(16));
        commandAllocator.Reset();
        commandList.Reset(commandAllocator);
        commandList.Close();

        graphicsQueue.ExecuteCommandList(commandList);
        graphicsQueue.SignalFence(fence);
    }

	return 0;
}
#include "Slipstream.h"
#include <thread> // Add this include for std::this_thread
#include <chrono> // Add this include for std::chrono

using namespace Slipstream::System;
using namespace Slipstream::Render;

int WinMain()
{
	Window window;

    GraphicsDeviceDesc gdDesc = {};
    gdDesc.API = GraphicsAPI::Vulkan;
    gdDesc.Flags = GraphicsDeviceFlags::Debug;

    GraphicsDevice graphicsDevice(gdDesc);
    CommandQueue graphicsQueue = graphicsDevice.GetCommandQueue(CommandQueueType::Graphics, 0);

    uint64_t nextFenceValue = 0;

    SwapChainDesc scDesc = { graphicsQueue, window.GetHandle(), 2 };
    SwapChain swapChain = graphicsDevice.CreateSwapChain(scDesc);
    CommandAllocator commandAllocator = graphicsDevice.CreateCommandAllocator({ CommandAllocatorType::Graphics });
    //CommandList commandList = graphicsDevice.CreateCommandList({ CommandListType::Graphics });
    Fence fence = graphicsDevice.CreateFence({});

    while (window.Update())
    {
        commandAllocator.Reset();
		CommandList commandList = commandAllocator.AllocateCommandList();
        commandList.Close();

        graphicsQueue.ExecuteCommandList(commandList);
        
		PresentDesc presentDesc = {};
		graphicsQueue.Present(swapChain, presentDesc);

        graphicsQueue.SignalFence(fence, ++nextFenceValue);

        while(fence.GetCompletedValue() < nextFenceValue)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	return 0;
}
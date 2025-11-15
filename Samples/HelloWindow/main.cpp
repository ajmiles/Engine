#include "Slipstream.h"
#include <thread> // Add this include for std::this_thread
#include <chrono> // Add this include for std::chrono

using namespace Slipstream::System;
using namespace Slipstream::Render;

static const uint NUM_FRAMES = 2;

int WinMain()
{
	Window window;

    GraphicsDeviceDesc gdDesc = {};
    gdDesc.API = GraphicsAPI::Direct3D12;
    gdDesc.Flags = GraphicsDeviceFlags::Debug;

    GraphicsDevice graphicsDevice(gdDesc);
    CommandQueue graphicsQueue = graphicsDevice.GetCommandQueue(CommandQueueType::Graphics, 0);


    SwapChainDesc scDesc = { graphicsQueue, window.GetHandle(), NUM_FRAMES };
    SwapChain swapChain = graphicsDevice.CreateSwapChain(scDesc);

    CommandAllocator commandAllocators[NUM_FRAMES] =
    {   graphicsDevice.CreateCommandAllocator({ CommandAllocatorType::Graphics }),
        graphicsDevice.CreateCommandAllocator({ CommandAllocatorType::Graphics }) 
    };

    Waitable waitables[NUM_FRAMES] = {};

    while (window.Update())
    {
        SwapChainContext swapChainContext = swapChain.BeginRendering();
        uint currentBackBufferIndex = swapChainContext.BackBufferIndex;

        CommandAllocator& allocator = commandAllocators[currentBackBufferIndex];
        allocator.Reset();

        CommandList commandList = allocator.AllocateCommandList();
        // Do something with the commandList here...

        commandList.Close();

        // Since this command list depends on the swap chain acquiring the back buffer, we must wait for the acquire waitable.
        waitables[currentBackBufferIndex] = graphicsQueue.ExecuteCommandList(commandList, 1, &swapChainContext.AcquireWaitable);

        PresentDesc presentDesc = { PresentFlags::None, swapChainContext.BackBufferIndex, waitables[currentBackBufferIndex] };
        graphicsQueue.Present(swapChain, presentDesc);
	}

	return 0;
}
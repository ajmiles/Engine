#define NOMINMAX
#include <Windows.h>
#include "Slipstream.h"
#include <thread>

using namespace Slipstream::System;
using namespace Slipstream::Render;

static const uint NUM_FRAMES = 2;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Window window;

    GraphicsDeviceDesc gdDesc = {};
    gdDesc.API = GraphicsAPI::D3D12;
    gdDesc.Flags = GraphicsDeviceFlags::Debug;

    GraphicsDevice graphicsDevice(gdDesc);
    CommandQueue graphicsQueue = graphicsDevice.GetCommandQueue(CommandQueueType::Graphics, 0);

    SwapChainDesc scDesc = { graphicsQueue, window.GetHandle(), NUM_FRAMES };
    SwapChain swapChain = graphicsDevice.CreateSwapChain(scDesc);

    CommandAllocator commandAllocators[NUM_FRAMES] =
    {   graphicsDevice.CreateCommandAllocator({ CommandAllocatorType::Graphics }),
        graphicsDevice.CreateCommandAllocator({ CommandAllocatorType::Graphics })
    };

    int frameNumber = 0;

    while (window.Update())
    {
		char str[256];
		std::snprintf(str, 256, "Frame %d\n", frameNumber++);
		OutputDebugStringA(str);

        SwapChainContext swapChainContext = swapChain.BeginRendering();
        Texture backBufferTexture = swapChain.GetBackBufferTexture(swapChainContext.BackBufferIndex);

        CommandAllocator& allocator = commandAllocators[swapChainContext.BackBufferIndex];
        allocator.Reset();

        CommandList commandList = allocator.AllocateCommandList();
        
		// Newly acquired back buffers are in the Undefined layout; transition to Present layout
        Barrier swapChain_UndefinedToPresentBarrier = Barrier::CreateTextureBarrier(
            &backBufferTexture,
            BarrierSync::None, BarrierAccess::None, BarrierLayout::Undefined,
            BarrierSync::None, BarrierAccess::None, BarrierLayout::Present
        );

		commandList.Barrier(1, &swapChain_UndefinedToPresentBarrier);
        commandList.Close();

        Waitable waitable = graphicsQueue.ExecuteCommandList(commandList, 1, &swapChainContext.AcquireWaitable);

        PresentDesc presentDesc = { PresentFlags::None, swapChainContext.BackBufferIndex, waitable };
        graphicsQueue.Present(swapChain, presentDesc);
	}

    OutputDebugStringA("Exit!\n");

	return 0;
}
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
    gdDesc.API = GraphicsAPI::Vulkan;
    gdDesc.Flags = GraphicsDeviceFlags::Debug;

    GraphicsDevice graphicsDevice(gdDesc);
    CommandQueue graphicsQueue = graphicsDevice.GetCommandQueue(CommandQueueType::Graphics, 0);

    SwapChainDesc scDesc = { graphicsQueue, window.GetHandle(), NUM_FRAMES };
    SwapChain swapChain = graphicsDevice.CreateSwapChain(scDesc);

    CommandAllocator commandAllocators[NUM_FRAMES] =
    {   graphicsDevice.CreateCommandAllocator({ CommandAllocatorType::Graphics }),
        graphicsDevice.CreateCommandAllocator({ CommandAllocatorType::Graphics })
    };

	RenderTargetView rtvs[NUM_FRAMES];

    for(int i = 0; i < NUM_FRAMES; ++i)
    {
        Texture backBufferTexture = swapChain.GetBackBufferTexture(i);
        RenderTargetViewDesc rtvDesc = { &backBufferTexture, ResourceFormat::R8G8B8A8_UNORM_SRGB };

        rtvs[i] = graphicsDevice.CreateRenderTargetView(rtvDesc);
	}

    // Newly acquired back buffers are in the Undefined layout; transition to Present layout
    Barrier swapChain_UndefinedToRTV = Barrier::CreateTextureBarrier(
        nullptr,
        BarrierSync::None, BarrierAccess::None, BarrierLayout::Undefined,
        BarrierSync::RenderTarget, BarrierAccess::RenderTarget, BarrierLayout::RenderTarget
    );

    // Once we're done rendering to the back buffer, transition it to Present layout
    Barrier swapChain_RTVToPresent = Barrier::CreateTextureBarrier(
        nullptr,
        BarrierSync::RenderTarget, BarrierAccess::RenderTarget, BarrierLayout::RenderTarget,
        BarrierSync::None, BarrierAccess::None, BarrierLayout::Present
    );

	TargetInfo targetInfos[1] = {};
	targetInfos[0] = { rtvs[0], ClearOp::Clear, { 0.529f, 0.808f, 0.922f, 1.0f } };

    char str[256];
    int frameNumber = 0;

    while (window.Update())
    {		
		std::snprintf(str, 256, "Frame %d\n", frameNumber++);
		OutputDebugStringA(str);

		Rect<int> windowRect = window.GetClientRect();

        SwapChainContext swapChainContext = swapChain.BeginRendering();
        Texture backBufferTexture = swapChain.GetBackBufferTexture(swapChainContext.BackBufferIndex);

        CommandAllocator& allocator = commandAllocators[swapChainContext.BackBufferIndex];
        allocator.Reset();

        CommandList commandList = allocator.AllocateCommandList();

        {
            swapChain_UndefinedToRTV.Texture.Texture = &backBufferTexture;
            commandList.Barrier(1, &swapChain_UndefinedToRTV);

			RenderingInfo renderingInfo = { 1, targetInfos, { 0, 0, windowRect.Width, windowRect.Height } };
            targetInfos[0].RTV = rtvs[swapChainContext.BackBufferIndex];

			commandList.BeginPass(renderingInfo);
            // Do rendering stuff here
			commandList.EndPass();

            swapChain_RTVToPresent.Texture.Texture = &backBufferTexture;
            commandList.Barrier(1, &swapChain_RTVToPresent);
        }

        commandList.Close();

        Waitable waitable = graphicsQueue.ExecuteCommandList(commandList, 1, &swapChainContext.AcquireWaitable);

        PresentDesc presentDesc = { PresentFlags::None, swapChainContext.BackBufferIndex, waitable };
        graphicsQueue.Present(swapChain, presentDesc);
	}

    OutputDebugStringA("Exit!\n");

	return 0;
}
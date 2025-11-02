#pragma once
#include <memory>
#include "Core/Types.h"
#include "Render/API/CommandQueue.h"
#include "Render/API/CommandList.h"
#include "Render/API/CommandAllocator.h"
#include "Render/API/SwapChain.h"
#include "Render/API/Fence.h"

namespace Slipstream
{
    namespace Render
    {
        enum class GraphicsAPI
        {
            Direct3D12,
            Vulkan,
            Metal,
            Count
        };

        enum GraphicsDeviceFlags
        {
            None = 0,
            Debug = 1 << 0,
        };

        struct GraphicsDeviceDesc
        {
            GraphicsAPI API = GraphicsAPI::Vulkan;
            GraphicsDeviceFlags Flags = GraphicsDeviceFlags::None;

            uint NumGraphicsQueues = 1;
            uint NumComputeQueues  = 1;
            uint NumCopyQueues     = 1;
        };

        class IGraphicsDeviceImpl
        {
        public:
            virtual ~IGraphicsDeviceImpl() = default;
            virtual CommandQueue     GetCommandQueue(CommandQueueType type, uint index) = 0;
            virtual SwapChain        CreateSwapChain(const SwapChainDesc& desc) = 0;
            virtual CommandList      CreateCommandList(const CommandListDesc& desc) = 0;
            virtual CommandAllocator CreateCommandAllocator(const CommandAllocatorDesc& desc) = 0;
            virtual Fence            CreateFence(const FenceDesc& desc) = 0;
        };

        class GraphicsDevice
        {
        public:
            GraphicsDevice();
            GraphicsDevice(const GraphicsDeviceDesc& desc);
            ~GraphicsDevice();

            CommandQueue     GetCommandQueue(CommandQueueType type, uint index);
            SwapChain        CreateSwapChain(const SwapChainDesc& desc);
            CommandList      CreateCommandList(const CommandListDesc& desc);
            CommandAllocator CreateCommandAllocator(const CommandAllocatorDesc& desc);
            Fence            CreateFence(const FenceDesc& desc);

            const GraphicsDeviceDesc& GetDesc() const noexcept { return m_Desc; }

        private:
            GraphicsDeviceDesc m_Desc;
            std::unique_ptr<IGraphicsDeviceImpl> m_Impl;
        };
    }
}




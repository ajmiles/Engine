#pragma once
#include "Core/Types.h"
#include "System/API/Window.h"
#include "Render/API/CommandQueue.h"
#include "Render/API/Texture.h"

namespace Slipstream::Render
{
    struct SwapChainDesc
    {
        CommandQueue    PresentQueue;
        uint64_t        WindowHandle       = 0;
        uint            BufferCount        = 2;
    };

    struct SwapChainContext
    {
        Waitable AcquireWaitable;
        uint BackBufferIndex = 0;
    };

    class ISwapChainImpl
    {
    public:
        virtual ~ISwapChainImpl() = default;

        virtual SwapChainContext BeginRendering() = 0;
		virtual Texture GetBackBufferTexture(uint index) = 0;
    };

    // Non-owning handle; cannot be user-constructed.
    class SwapChain
    {
    public:
        SwapChain(const SwapChain&) noexcept = default;
        SwapChain& operator=(const SwapChain&) noexcept = default;
        SwapChain(SwapChain&&) noexcept = default;
        SwapChain& operator=(SwapChain&&) noexcept = default;

        ~SwapChain() = default;

        SwapChainContext BeginRendering()
        {
            return m_Impl->BeginRendering();
        }

        Texture GetBackBufferTexture(uint index)
        {
			return m_Impl->GetBackBufferTexture(index);
        }

    private:
        SwapChain(const SwapChainDesc& desc);
        SwapChain() = delete;
        explicit SwapChain(ISwapChainImpl* impl) noexcept : m_Impl(impl) {}

        ISwapChainImpl* m_Impl = nullptr;

        friend class D3D12GraphicsDeviceImpl;
        friend class VulkanGraphicsDeviceImpl;
        
        friend class GraphicsDevice;

		friend class D3D12CommandQueueImpl;
        friend class VulkanCommandQueueImpl;
    };
}
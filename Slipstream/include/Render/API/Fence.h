#pragma once
#include "Core/Types.h"

namespace Slipstream::Render
{
    struct FenceDesc
    {
        // Future expansion.
    };

    class IFenceImpl
    {
    public:
        virtual ~IFenceImpl() = default;

        virtual void WaitUntilCompleted(uint64_t value) const = 0;
    };


    class Fence
    {
    public:
        Fence() = default;
        ~Fence() = default;

        bool IsValid() const noexcept { return m_Impl != nullptr; }
        void WaitUntilCompleted(uint64_t value) const { m_Impl->WaitUntilCompleted(value); }

    private:
        explicit Fence(std::shared_ptr<IFenceImpl> impl) noexcept;

        std::shared_ptr<IFenceImpl> m_Impl;

        friend class D3D12GraphicsDeviceImpl;
        friend class VulkanGraphicsDeviceImpl;

		friend class D3D12CommandQueueImpl;
		friend class VulkanCommandQueueImpl;

        friend class D3D12SwapChainImpl;
        friend class VulkanSwapChainImpl;
    };
}
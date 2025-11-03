#pragma once
#include "Core/Types.h"

namespace Slipstream::Render
{
    struct FenceDesc
    {
        // Future expansion.
    };

    class FenceImpl
    {
    public:
        virtual ~FenceImpl() = default;
		virtual uint64_t GetCompletedValue() const = 0;
    };

    // Non-owning lightweight handle
    class Fence
    {
    public:
        Fence() = default;
        ~Fence();

        Fence(const Fence&) = delete;
        Fence& operator=(const Fence&) = delete;
        Fence(Fence&& other) noexcept;
        Fence& operator=(Fence&& other) noexcept;

        uint64_t GetCompletedValue() const
        {
			return m_Impl->GetCompletedValue();
        }

    private:
        explicit Fence(FenceImpl* impl) noexcept;

    private:
        FenceImpl* m_Impl = nullptr;

        friend class D3D12GraphicsDeviceImpl;
        friend class VulkanGraphicsDeviceImpl;

		friend class D3D12CommandQueueImpl;
		friend class VulkanCommandQueueImpl;
    };
}
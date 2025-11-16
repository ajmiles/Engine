#pragma once
#include "Core/Types.h"

namespace Slipstream::Render
{
    class ITextureImpl
    {
    public:
        virtual ~ITextureImpl() = default;
    };

    class Texture
    {
    public:
        Texture(const Texture&) noexcept = default;
        Texture& operator=(const Texture&) noexcept = default;
        Texture(Texture&&) noexcept = default;
        Texture& operator=(Texture&&) noexcept = default;
        ~Texture() = default;

    private:
        Texture() = delete;
        explicit Texture(std::shared_ptr<ITextureImpl> impl) noexcept : m_Impl(impl) {}

        std::shared_ptr<ITextureImpl> m_Impl;

		friend class D3D12SwapChainImpl;
		friend class VulkanSwapChainImpl;

        friend class D3D12TextureImpl;
		friend class VulkanTextureImpl;

		friend class D3D12CommandListImpl;
		friend class VulkanCommandListImpl;
    };
}
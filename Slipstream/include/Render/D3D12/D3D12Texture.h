#pragma once
#include "Render/API/Buffer.h"

namespace Slipstream::Render
{
    class D3D12TextureImpl : public ITextureImpl
    {
    public:
        D3D12TextureImpl(ID3D12Resource* resource) : m_Resource(resource) {}
		~D3D12TextureImpl() = default;

    private:
        ID3D12Resource* m_Resource = nullptr;
    };
}
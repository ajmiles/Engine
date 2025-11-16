#pragma once
#include "Render/API/Buffer.h"

namespace Slipstream::Render
{
    class D3D12BufferImpl : public IBufferImpl
    {
        D3D12BufferImpl(ID3D12Resource* resource);
        ~D3D12BufferImpl();

        ID3D12Resource* m_Resource = nullptr;
    };
}
#include "pch.h"
#include "Render/API/Fence.h"
#include "Render/API/GraphicsDevice.h"

namespace Slipstream::Render
{
    Fence::Fence(FenceImpl* impl) noexcept
        : m_Impl(impl)
    {
    }

    Fence::~Fence()
    {
        if (m_Impl)
        {
            delete m_Impl;
        }
    }

    Fence::Fence(Fence&& other) noexcept
        : m_Impl(other.m_Impl)
    {
        other.m_Impl = nullptr;
    }

    Fence& Fence::operator=(Fence&& other) noexcept
    {
        if (this != &other)
        {
            if (m_Impl)
            {
                delete m_Impl;
            }
            m_Impl = other.m_Impl;
            other.m_Impl = nullptr;
        }
        return *this;
    }
}
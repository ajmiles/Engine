#include "pch.h"
#include "Render/API/Fence.h"
#include "Render/API/GraphicsDevice.h"

namespace Slipstream::Render
{
    Fence::Fence(std::shared_ptr<IFenceImpl> impl) noexcept
        : m_Impl(impl)
    {
    }
}
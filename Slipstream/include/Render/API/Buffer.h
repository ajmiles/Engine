#pragma once
#include "Core/Types.h"

namespace Slipstream::Render
{
    class IBufferImpl
    {
    public:
        virtual ~IBufferImpl() = default;
    };

    class Buffer
    {
    public:
        Buffer(const Buffer&) noexcept = default;
        Buffer& operator=(const Buffer&) noexcept = default;
        Buffer(Buffer&&) noexcept = default;
        Buffer& operator=(Buffer&&) noexcept = default;
        ~Buffer() = default;

    private:
        Buffer() = delete;
        explicit Buffer(IBufferImpl* impl) noexcept : m_Impl(impl) {}

        std::shared_ptr<IBufferImpl> m_Impl;
    };
}
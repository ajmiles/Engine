#pragma once
#include "Render/API/Buffer.h"

namespace Slipstream::Render
{
    class VulkanBufferImpl : public IBufferImpl
    {
        VulkanBufferImpl(VkBuffer buffer);
        ~VulkanBufferImpl();

        vk::Buffer m_Buffer;
    };
}
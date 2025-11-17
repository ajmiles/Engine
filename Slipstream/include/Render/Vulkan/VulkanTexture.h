#pragma once
#include "Render/API/Buffer.h"

namespace Slipstream::Render
{
    class VulkanTextureImpl : public ITextureImpl
    {
    public:
		VulkanTextureImpl(VkImage image) : m_Image(image) {}
		~VulkanTextureImpl() = default;

    private:
        vk::Image m_Image;

		friend class VulkanGraphicsDeviceImpl;
		friend class VulkanCommandListImpl;
    };
}
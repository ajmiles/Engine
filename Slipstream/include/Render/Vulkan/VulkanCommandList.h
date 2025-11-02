#pragma once
#include "Render/API/CommandList.h"
#include <vulkan/vulkan.hpp>

namespace Slipstream::Render
{
    class VulkanCommandListImpl : public ICommandListImpl
    {
    private:
        VulkanCommandListImpl(const CommandListDesc& desc);
        ~VulkanCommandListImpl();

        void Reset(CommandAllocator& allocator) override;
        void Close() override;

        friend class VulkanGraphicsDeviceImpl;
    };
}
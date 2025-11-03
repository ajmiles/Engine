#include "pch.h"
#include "Render/API/GraphicsDevice.h"
#include "Render/D3D12/D3D12GraphicsDevice.h"
#include "Render/Vulkan/VulkanGraphicsDevice.h"
#include <stdexcept>

using namespace Slipstream::Render;

GraphicsDevice::GraphicsDevice()
    : GraphicsDevice(GraphicsDeviceDesc())
{
}

GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc& desc)
    : m_Desc(desc)
{
    switch (m_Desc.API)
    {
    case GraphicsAPI::Direct3D12:
        m_Impl.reset(new D3D12GraphicsDeviceImpl(m_Desc));
        break;
    case GraphicsAPI::Vulkan:
        m_Impl.reset(new VulkanGraphicsDeviceImpl(m_Desc));
        break;
    default:
        throw std::runtime_error("Unsupported graphics API");
    }
}

GraphicsDevice::~GraphicsDevice() = default;

CommandQueue GraphicsDevice::GetCommandQueue(CommandQueueType type, uint index)
{
    return m_Impl->GetCommandQueue(type, index);
}

SwapChain GraphicsDevice::CreateSwapChain(const SwapChainDesc& desc)
{
    return m_Impl->CreateSwapChain(desc);
}

CommandAllocator GraphicsDevice::CreateCommandAllocator(const CommandAllocatorDesc& desc)
{
    return m_Impl->CreateCommandAllocator(desc);
}

Fence GraphicsDevice::CreateFence(const FenceDesc& desc)
{
    return m_Impl->CreateFence(desc);
}
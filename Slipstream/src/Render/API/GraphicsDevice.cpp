#include "pch.h"
#include "Render/API/GraphicsDevice.h"
#include "Render/D3D12/D3D12GraphicsDevice.h"
#include "Render/Vulkan/VulkanGraphicsDevice.h"

using namespace Slipstream::Render;

GraphicsDevice::GraphicsDevice()
	: GraphicsDevice(GraphicsDeviceDesc())
{
}

GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc& desc)
	: m_Impl(nullptr)
{
	switch (desc.api)
	{
	case GraphicsAPI::Direct3D12:
		m_Impl = new ID3D12GraphicsDeviceImpl();
		break;
	case GraphicsAPI::Vulkan:
		m_Impl = new IVulkanGraphicsDeviceImpl();
		break;
	default:
		throw std::runtime_error("Unsupported graphics API");
	}
}

GraphicsDevice::~GraphicsDevice()
{
	delete m_Impl;
}
#pragma once
#include "../API/GraphicsDevice.h"

using namespace Slipstream::Render;

class IVulkanGraphicsDeviceImpl : public IGraphicsDeviceImpl
{
public:
	IVulkanGraphicsDeviceImpl();
	~IVulkanGraphicsDeviceImpl();

private:
	vk::Instance m_instance;
	vk::PhysicalDevice m_physicalDevice;
	vk::Device m_device;
};


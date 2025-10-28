#include "pch.h"
#include "Render/Vulkan/VulkanGraphicsDevice.h"

using namespace Slipstream::Render;

IVulkanGraphicsDeviceImpl::IVulkanGraphicsDeviceImpl()
{
	static vk::Instance instance = vk::createInstance({}, nullptr);

	// Select the first available physical device
	auto physicalDevices = instance.enumeratePhysicalDevices();

	if (physicalDevices.empty())
	{
		throw std::runtime_error("No Vulkan physical devices found");
	}

	m_physicalDevice = physicalDevices[0];

	// Create logical device with graphics queue
	float queuePriority = 1.0f;
	vk::DeviceQueueCreateInfo queueCreateInfo(
		{},
		0, // Assuming queue family index 0 supports graphics; in a real application, you should query this
		1,
		&queuePriority
	);
	vk::DeviceCreateInfo deviceCreateInfo(
		{},
		1,
		&queueCreateInfo
	);
	m_device = m_physicalDevice.createDevice(deviceCreateInfo);
}

IVulkanGraphicsDeviceImpl::~IVulkanGraphicsDeviceImpl()
{
	// Cleanup code for Vulkan graphics device would go here
}
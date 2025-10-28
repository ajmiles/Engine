#pragma once
#include "Render/API/GraphicsDevice.h"

using namespace Slipstream::Render;

class ID3D12GraphicsDeviceImpl : public IGraphicsDeviceImpl
{
	public:
		ID3D12GraphicsDeviceImpl();
		~ID3D12GraphicsDeviceImpl();

	private:
		ID3D12Device* m_device;
};


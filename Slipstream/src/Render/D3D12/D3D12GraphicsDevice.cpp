#include "pch.h"
#include "Render/D3D12/D3D12GraphicsDevice.h"

using namespace Slipstream::Render;

ID3D12GraphicsDeviceImpl::ID3D12GraphicsDeviceImpl()
{
	HRESULT hr = D3D12CreateDevice(
		nullptr,                    // default adapter
		D3D_FEATURE_LEVEL_11_0,    // minimum feature level
		IID_PPV_ARGS(&m_device)    // output device
	);
}

ID3D12GraphicsDeviceImpl::~ID3D12GraphicsDeviceImpl()
{
	m_device->Release();
}
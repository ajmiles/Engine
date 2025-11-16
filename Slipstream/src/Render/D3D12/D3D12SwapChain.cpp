#include "pch.h"
#include "Render/D3D12/D3D12SwapChain.h"
#include "Render/D3D12/D3D12CommandQueue.h"
#include "Render/D3D12/D3D12Texture.h"

using namespace Slipstream::Render;

D3D12SwapChainImpl::D3D12SwapChainImpl(const SwapChainDesc& desc, IDXGIFactory* factory)
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = 0; // Use automatic sizing.
    swapChainDesc.Height = 0; // Use automatic sizing.
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = FALSE;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = desc.BufferCount;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

    IDXGISwapChain1* dxgiSwapChain = nullptr;
    IDXGIFactory2* dxgiFactory2 = nullptr;
    factory->QueryInterface(IID_PPV_ARGS(&dxgiFactory2));

    auto presentQueueImpl = static_cast<D3D12CommandQueueImpl*>(desc.PresentQueue.m_Impl.get());

    HRESULT hr = dxgiFactory2->CreateSwapChainForHwnd(
        presentQueueImpl->m_Queue,
        (HWND)(desc.WindowHandle),
        &swapChainDesc,
        nullptr,
        nullptr,
        &dxgiSwapChain
    );

    if (FAILED(hr))
        throw std::runtime_error("Failed to create D3D12 swap chain");

    // Create the swap chain
    hr = dxgiSwapChain->QueryInterface(IID_PPV_ARGS(&m_SwapChain));

    if (FAILED(hr))
        throw std::runtime_error("Failed to query IDXGISwapChain3 interface");

    dxgiSwapChain->Release();

    for(uint i = 0; i < desc.BufferCount; ++i)
    {
        ID3D12Resource* backBufferResource = nullptr;
        hr = m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&backBufferResource));

        if (FAILED(hr))
            throw std::runtime_error("Failed to get back buffer from swap chain");

		m_BackBufferTextures[i] = std::make_shared<D3D12TextureImpl>(backBufferResource);

        backBufferResource->Release();
	}
}

D3D12SwapChainImpl::~D3D12SwapChainImpl()
{
    if (m_SwapChain)
    {
        m_SwapChain->Release();
        m_SwapChain = nullptr;
    }
}

Texture D3D12SwapChainImpl::GetBackBufferTexture(uint index)
{
	return Texture(m_BackBufferTextures[index]);
}

SwapChainContext D3D12SwapChainImpl::BeginRendering()
{
	// Before continuing, throttle here to ensure we don't get too far ahead of the GPU.
	m_FrameEndWaitables[m_SwapChain->GetCurrentBackBufferIndex()].WaitOnCPU();

    SwapChainContext context;
    context.AcquireWaitable = Waitable(Fence(), 0); // No fence for D3D12 swap chain acquire.
    context.BackBufferIndex = m_SwapChain->GetCurrentBackBufferIndex();
    return context;
}
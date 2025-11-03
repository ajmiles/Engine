#include "pch.h"
#include "Render/D3D12/D3D12SwapChain.h"

using namespace Slipstream::Render;

D3D12SwapChainImpl::~D3D12SwapChainImpl()
{
    if (m_SwapChain)
    {
        m_SwapChain->Release();
        m_SwapChain = nullptr;
    }
}
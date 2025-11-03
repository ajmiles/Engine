#include "pch.h"
#include "Render/D3D12/D3D12CommandList.h"
#include "Render/D3D12/D3D12CommandAllocator.h"
#include <stdexcept>

using namespace Slipstream::Render;

D3D12CommandListImpl::D3D12CommandListImpl(ID3D12GraphicsCommandList* list)
    : m_List(list)
{

}

D3D12CommandListImpl::~D3D12CommandListImpl()
{
    if (m_List)
    {
        m_List->Release();
        m_List = nullptr;
    }
}

void D3D12CommandListImpl::Close()
{
    HRESULT hr = m_List->Close();
    if (FAILED(hr))
        throw std::runtime_error("Failed to close D3D12 command list");
}
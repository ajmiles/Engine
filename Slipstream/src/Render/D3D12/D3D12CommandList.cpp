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


void D3D12CommandListImpl::Reset(CommandAllocator& allocator)
{
    // Obtain the native D3D12 command allocator from the wrapper.
    auto d3dAllocatorImpl = static_cast<D3D12CommandAllocatorImpl*>(allocator.m_Impl);
    ID3D12CommandAllocator* d3dAllocator = d3dAllocatorImpl->GetNative();

    HRESULT hr = m_List->Reset(d3dAllocator, nullptr);
    if (FAILED(hr))
        throw std::runtime_error("Failed to reset D3D12 command list");
}

void D3D12CommandListImpl::Close()
{
    HRESULT hr = m_List->Close();
    if (FAILED(hr))
        throw std::runtime_error("Failed to close D3D12 command list");
}
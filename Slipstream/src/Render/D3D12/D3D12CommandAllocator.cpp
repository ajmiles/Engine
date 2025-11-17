#include "pch.h"
#include "Render/D3D12/D3D12CommandAllocator.h"
#include "Render/D3D12/D3D12CommandList.h"
#include <stdexcept>

using namespace Slipstream::Render;

D3D12CommandAllocatorImpl::D3D12CommandAllocatorImpl(ID3D12CommandAllocator* allocator, ID3D12GraphicsCommandList7* commandList)
	: m_Allocator(allocator)
{
    m_CommandList = std::make_shared<D3D12CommandListImpl>(commandList);
}

D3D12CommandAllocatorImpl::~D3D12CommandAllocatorImpl()
{
    if (m_Allocator)
    {
        m_Allocator->Release();
        m_Allocator = nullptr;
    }
}

CommandList D3D12CommandAllocatorImpl::AllocateCommandList()
{
    ID3D12GraphicsCommandList7* d3dCommandList = m_CommandList->m_List;
	HRESULT hr = d3dCommandList->Reset(m_Allocator, nullptr);
    if (FAILED(hr))
		throw std::runtime_error("Failed to reset D3D12 command list");

    return CommandList(m_CommandList);
}

void D3D12CommandAllocatorImpl::Reset()
{
    m_Allocator->Reset();
}
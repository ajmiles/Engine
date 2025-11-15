#include "pch.h"
#include "Render/D3D12/D3D12CommandAllocator.h"
#include "Render/D3D12/D3D12CommandList.h"
#include "Render/D3D12/D3D12GraphicsDevice.h" // If device access is later needed
#include <stdexcept>

using namespace Slipstream::Render;

D3D12CommandAllocatorImpl::D3D12CommandAllocatorImpl(ID3D12CommandAllocator* allocator, ID3D12GraphicsCommandList* commandList)
	: m_Allocator(allocator)
{
    m_CommandList = new D3D12CommandListImpl(commandList);
}

D3D12CommandAllocatorImpl::~D3D12CommandAllocatorImpl()
{
    if (m_Allocator)
    {
        m_Allocator->Release();
        m_Allocator = nullptr;
    }
}

ICommandListImpl* D3D12CommandAllocatorImpl::AllocateCommandList()
{
    ID3D12GraphicsCommandList* d3dCommandList = m_CommandList->m_List;
	HRESULT hr = d3dCommandList->Reset(m_Allocator, nullptr);
    if (FAILED(hr))
		throw std::runtime_error("Failed to reset D3D12 command list");

    return m_CommandList;
}

void D3D12CommandAllocatorImpl::Reset()
{
    m_Allocator->Reset();
}
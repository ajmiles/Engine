#include "pch.h"
#include "Render/D3D12/D3D12CommandAllocator.h"
#include "Render/D3D12/D3D12GraphicsDevice.h" // If device access is later needed
#include <stdexcept>

using namespace Slipstream::Render;

D3D12CommandAllocatorImpl::D3D12CommandAllocatorImpl(ID3D12CommandAllocator* allocator)
    : m_Allocator(allocator)
{
    
}

D3D12CommandAllocatorImpl::~D3D12CommandAllocatorImpl()
{
    if (m_Allocator)
    {
        m_Allocator->Release();
        m_Allocator = nullptr;
    }
}

void D3D12CommandAllocatorImpl::Reset()
{
    m_Allocator->Reset();
}
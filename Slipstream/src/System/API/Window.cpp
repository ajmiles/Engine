#pragma once
#include "pch.h"
#include "System/API/Window.h"
#include "System/Windows/Win32Window.h"

using namespace Slipstream::System;

Window::Window() : Window(WindowDesc())
{

}

Window::Window(const WindowDesc& desc)
{
#if defined(_WIN32)
	m_Impl = new Win32WindowImpl(desc);
#else
#error "Window implementation not available for this platform"
#endif
}

bool Window::Update() const
{
    return m_Impl->Update();
}

uint64_t Window::GetHandle() const
{
    return m_Impl->GetHandle();
}
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
	m_Impl = new IWIn32WindowImpl();
}
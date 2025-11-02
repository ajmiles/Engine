#include "pch.h"
#include "System/API/Window.h"
#include "System/Windows/Win32Window.h"

using namespace Slipstream::System;

Win32WindowImpl::Win32WindowImpl(const WindowDesc& desc)
{
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = &WindowProc;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = L"SlipstreamWindowClass";
    RegisterClassEx(&wc);

    m_hWnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        L"Slipstream Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        desc.width, desc.height,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );

    ShowWindow(m_hWnd, SW_SHOW);
}

Win32WindowImpl::~Win32WindowImpl()
{

}

bool Win32WindowImpl::Update() const
{
    MSG msg;
    while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            return false;
    }
    return true;
}

uint64_t Win32WindowImpl::GetHandle() const
{
    return reinterpret_cast<uint64_t>(m_hWnd);
}

LRESULT CALLBACK Win32WindowImpl::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}
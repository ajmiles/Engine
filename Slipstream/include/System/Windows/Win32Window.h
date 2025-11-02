#pragma once
#include "System/API/Window.h"

namespace Slipstream
{
	namespace System
	{
		class Win32WindowImpl : public IWindowImpl
		{
        private:
			Win32WindowImpl(const WindowDesc& desc);
			~Win32WindowImpl() override;

            bool Update() const override;
			uint64_t GetHandle() const override;

            static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

            HWND m_hWnd = nullptr;

            friend class Window;
		};
	}
}



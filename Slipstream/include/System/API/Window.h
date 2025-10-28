#pragma once
#include "Core/Types.h"

namespace Slipstream
{
	namespace System
	{
		class IWindowImpl
		{
		public:
			virtual ~IWindowImpl() = default;
		};

		struct WindowDesc
		{
			uint width;
			uint height;
			bool fullscreen;

			WindowDesc() : width(1280), height(720), fullscreen(false)
			{
			}
		};

		class Window
		{
		public:
			Window();
			Window(const WindowDesc& desc);
			~Window() = default;
		private:
			IWindowImpl* m_Impl;
		};
	}
}



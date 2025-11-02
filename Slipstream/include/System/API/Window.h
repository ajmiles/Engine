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

            virtual bool Update() const = 0;
            virtual uint64_t GetHandle() const = 0;
		};

		struct WindowDesc
		{
			uint width;
			uint height;

			WindowDesc() : width(1280), height(720)
			{
			}
		};

		class Window
		{
		public:
			Window();
			Window(const WindowDesc& desc);
			~Window() = default;

            bool Update() const;
            uint64_t GetHandle() const;

		private:
			IWindowImpl* m_Impl;
		};
	}
}



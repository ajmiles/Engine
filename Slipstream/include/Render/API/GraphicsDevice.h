#pragma once
namespace Slipstream
{
	namespace Render
	{
		enum class GraphicsAPI
		{
			Direct3D12,
			Vulkan,
			Metal,
			Count
		};

		struct GraphicsDeviceDesc
		{
			GraphicsAPI api;

			GraphicsDeviceDesc() : api(GraphicsAPI::Vulkan)
			{
			}
		};

		class IGraphicsDeviceImpl
		{
			public:
				virtual ~IGraphicsDeviceImpl() = default;
		};

		class GraphicsDevice
		{
		public:
			GraphicsDevice();
			GraphicsDevice(const GraphicsDeviceDesc& desc);
			~GraphicsDevice();

		private:
			IGraphicsDeviceImpl* m_Impl;
		};
	}
}




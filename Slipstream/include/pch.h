#pragma once

#define COMPILE_D3D12_SUPPORT
#define COMPILE_VULKAN_SUPPORT

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <Windows.h>
#endif

#if defined(COMPILE_D3D12_SUPPORT)
#include <d3d12.h>
#endif

#if defined(COMPILE_VULKAN_SUPPORT)
#include <vulkan/vulkan.hpp>
//VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

#include <stdexcept>
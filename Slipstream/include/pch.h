#pragma once

#define COMPILE_D3D12_SUPPORT
#define COMPILE_VULKAN_SUPPORT

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#if defined(COMPILE_D3D12_SUPPORT)
#include <d3d12.h>
#include <dxgi1_4.h>
#endif

#if defined(COMPILE_VULKAN_SUPPORT)
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_win32.h>
#endif

#include <stdexcept>
#include <memory>
#include <vector>
#include <cassert>
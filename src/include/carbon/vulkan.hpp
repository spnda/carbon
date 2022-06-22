#pragma once

// This is not a header that includes the whole carbon library.
// Instead, this includes all Vulkan related headers and handles
// some pragma warnings correctly.

#ifdef _MSC_VER
#pragma warning(disable : 26812) // C26812: The enum type '' is unscoped. Prefer 'enum class' over 'enum'.
#endif

#include <vulkan/vulkan_core.h>

#if defined(__APPLE__) && __has_include("vulkan/vulkan_metal.h")
#include <vulkan/vulkan_metal.h>
#elif defined(WIN32) && __has_include("vulkan/vulkan_win32.h")
#include <vulkan/vulkan_win32.h>
#endif

// VMA automatically defines these when compiling with clang, however this triggers a lot of
// warnings and things I am not interested in.
#define VMA_NULLABLE
#define VMA_NOT_NULL

#include "VkBootstrap.h"
#include <vk_mem_alloc.h>

#ifdef _MSC_VER
#pragma warning(default : 4005)
#endif

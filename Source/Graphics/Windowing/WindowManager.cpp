#include "WindowManager.h"
#include <iostream>
#include <assert.h>
#include "VulkanManager.h"
using namespace VulkanCoreSpace;

WindowManager::WindowManager( uint32_t size_x, uint32_t size_y, std::string name)
{
    _surface_size_x = size_x;
    _surface_size_y = size_y;
    _window_name = name;

    _InitOSWindow();
    _InitSurface();
}

WindowManager::~WindowManager()
{
    _DeInitSurface();
    _DeInitOSWindow();
}

void WindowManager::Close()
{
    _window_should_run = false;
}

bool WindowManager::Update()
{
    _UpdateOSWindow();
    return _window_should_run;
}

void WindowManager::_InitSurface()
{
    _InitOSSurface();

    auto gpu = VulkanManager::GetSingleton()->GetVulkanPhysicalDevice();

    VkBool32 WSI_supported = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(gpu, VulkanManager::GetSingleton()->GetQueueFamilyIndex(), _surface, &WSI_supported);
    if (!WSI_supported) {
        assert(0 && "WSI not supported");
        std::exit(-1);
    }

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, _surface, &_surface_capabilities);

    if (_surface_capabilities.currentExtent.width < UINT32_MAX)
    {
        _surface_size_x = _surface_capabilities.currentExtent.width;
        _surface_size_y = _surface_capabilities.currentExtent.height;
    }

    {
        uint32_t format_count = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, _surface, &format_count, nullptr);
        if (format_count == 0) {
            assert(0 && "Surface formats missing.");
            std::exit(-1);
        }
        std::vector<VkSurfaceFormatKHR> formats(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, _surface, &format_count, formats.data());
        if (formats[0].format == VK_FORMAT_UNDEFINED) {
            _surface_format.format = VK_FORMAT_B8G8R8A8_UNORM;
            _surface_format.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        }
        else {
            _surface_format = formats[0];
        }
    }
}

void WindowManager::_DeInitSurface()
{
    vkDestroySurfaceKHR(VulkanManager::GetSingleton()->GetVulkanInstance(), _surface, nullptr);
}
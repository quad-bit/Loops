/* -----------------------------------------------------
This source code is public domain ( CC0 )
The code is provided as-is without limitations, requirements and responsibilities.
Creators and contributors to this source code are provided as a token of appreciation
and no one associated with this source code can be held responsible for any possible
damages or losses of any kind.

Original file creator:  Teagan Chouinard (GLFW)
Contributors:
Niko Kauppi (Code maintenance)
----------------------------------------------------- */

#include "Built_Options.h"
#include "Platform.h"

#include "Shared.h"
#include "WindowManager.h"

#include <assert.h>
#include <iostream>

#include "VulkanManager.h"
using namespace VulkanCoreSpace;

#if USE_FRAMEWORK_GLFW

void WindowManager::_InitOSWindow()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    _glfw_window = glfwCreateWindow(_surface_size_x, _surface_size_y, _window_name.c_str(), nullptr, nullptr);
    glfwGetFramebufferSize(_glfw_window, (int*)&_surface_size_x, (int*)&_surface_size_y);
}

void WindowManager::_DeInitOSWindow()
{
    glfwDestroyWindow(_glfw_window);
}

void WindowManager::_UpdateOSWindow()
{
    glfwPollEvents();

    if (glfwWindowShouldClose(_glfw_window)) {
        Close();
    }
}

void WindowManager::_InitOSSurface()
{
    if (VK_SUCCESS != glfwCreateWindowSurface(VulkanManager::GetSingleton()->GetVulkanInstance(), _glfw_window, nullptr, &_surface)) {
        glfwTerminate();
        assert(0 && "GLFW could not create the window surface.");
        return;
    }
}

#endif // USE_FRAMEWORK_GLFW

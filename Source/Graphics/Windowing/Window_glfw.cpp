
#include "WindowManager.h"
#include <Assertion.h>

using namespace Loops::Graphics::Windowing;
using namespace Loops::Core;

#if defined(GLFW_ENABLED)

void WindowManager::InitOSWindow()
{
    glfwInit();
    if (glfwVulkanSupported() == GLFW_FALSE) 
    {
        ASSERT_MSG(0, " GLFW Failed to initialize with Vulkan.");
        std::exit(-1);
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindow = glfwCreateWindow(Settings::windowWidth, Settings::windowHeight, Settings::windowName.c_str(), nullptr, nullptr);
    glfwGetFramebufferSize(glfwWindow, (int*)&Settings::windowWidth, (int*)&Settings::windowHeight);
}

void WindowManager::DeInitOSWindow()
{
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

void WindowManager::UpdateOSWindow()
{
    glfwPollEvents();

    if (glfwWindowShouldClose(glfwWindow)) 
    {
        Close();
    }
}

#endif 
#pragma once
#include <vulkan\vulkan.h>
#include <GLFW\glfw3.h>


class ValidationManager;

class VulkanManager
{

private:
    VulkanManager();
    VulkanManager(VulkanManager const &) {}
    VulkanManager const & operator= (VulkanManager const &) {}

    static VulkanManager* instance;

    ValidationManager * validationManagerObj;

    VkInstance vkInstanceObj                = VK_NULL_HANDLE;
    VkDevice vkLogicalDeviceObj             = VK_NULL_HANDLE;
    VkPhysicalDevice vkPhysicalDeviceObj    = VK_NULL_HANDLE;
    VkAllocationCallbacks* pAllocator       = VK_NULL_HANDLE;
    VkQueue graphicsQueueObj                = VK_NULL_HANDLE;
    VkSurfaceKHR surface                    = VK_NULL_HANDLE;
    uint32_t surfaceWidth;
    uint32_t surfaceHeight;

    VkSurfaceFormatKHR surfaceFormat        = {};
    VkSurfaceCapabilitiesKHR surfaceCapabilities = {};

    VkPhysicalDeviceProperties                      physicalDeviceProps{};
    VkPhysicalDeviceMemoryProperties                physicalDeviceMemProps{};
    VkPhysicalDeviceFeatures                        physicalDeviceFeatures{}, enabledPhysicalDeviceFeatures{};
    uint32_t                                        vulkanGraphicsQueueFamilyIndex;

    void CreateInstance();
    void CreateDevice();

    void GetPhysicalDevice();
    void CalculateQueueFamilyIndex();

public:
    static VulkanManager* GetInstance();
    ~VulkanManager();
        
    void Init();
    void DeInit();
    void Update();
    void CreateSurface(GLFWwindow * glfwWindow);
    VkSurfaceKHR * GetSurface() { return &surface; }
    VkSurfaceFormatKHR * GetSurfaceFormat() { return &surfaceFormat; }
};

#include "VulkanManager.h"
#include "ValidationManager.h"
#include "VulkanUtility.h"
#include <Settings.h>
#include <Assertion.h>
#include <algorithm>

using namespace std;

VulkanManager* VulkanManager::instance = nullptr;

VulkanManager::VulkanManager()
{
    validationManagerObj = new ValidationManager();
}

void VulkanManager::CreateInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pApplicationName = "Random Game";
    appInfo.pEngineName = "Loops";
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfoObj{};
    createInfoObj.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfoObj.pApplicationInfo = &appInfo;
    createInfoObj.enabledExtensionCount = (uint32_t)validationManagerObj->instanceExtensionNameList.size();
    createInfoObj.enabledLayerCount = (uint32_t)validationManagerObj->instanceLayerNameList.size();
    createInfoObj.pNext = &(validationManagerObj->VkDebugReportCallbackCreateInfoEXTObj);
    createInfoObj.ppEnabledExtensionNames = validationManagerObj->instanceExtensionNameList.data();
    createInfoObj.ppEnabledLayerNames = validationManagerObj->instanceLayerNameList.data();

    ErrorCheck(vkCreateInstance(&createInfoObj, pAllocator, &vkInstanceObj));

    CoreObjects::instanceObj = &vkInstanceObj;
}

void VulkanManager::CreateDevice()
{
    GetPhysicalDevice();
    CalculateQueueFamilyIndex();

    float priority{ 1.0f };
    VkDeviceQueueCreateInfo VkDeviceQueueCreateInfoObj{};
    VkDeviceQueueCreateInfoObj.queueCount = 1;
    VkDeviceQueueCreateInfoObj.pQueuePriorities = &priority;
    VkDeviceQueueCreateInfoObj.queueFamilyIndex = vulkanGraphicsQueueFamilyIndex;
    VkDeviceQueueCreateInfoObj.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

    VkDeviceCreateInfo vkDeviceCreateInfoObj{};
    vkDeviceCreateInfoObj.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    vkDeviceCreateInfoObj.queueCreateInfoCount = 1;
    vkDeviceCreateInfoObj.pQueueCreateInfos = &VkDeviceQueueCreateInfoObj;
    vkDeviceCreateInfoObj.enabledExtensionCount = (uint32_t)validationManagerObj->deviceExtensionNameList.size();
    vkDeviceCreateInfoObj.enabledLayerCount = 0;
    vkDeviceCreateInfoObj.pEnabledFeatures = &enabledPhysicalDeviceFeatures;
    vkDeviceCreateInfoObj.ppEnabledExtensionNames = validationManagerObj->deviceExtensionNameList.data();
    vkDeviceCreateInfoObj.ppEnabledLayerNames = nullptr;

    ErrorCheck(vkCreateDevice(vkPhysicalDeviceObj, &vkDeviceCreateInfoObj, pAllocator, &vkLogicalDeviceObj));

    CoreObjects::logicalDeviceObj = &vkLogicalDeviceObj;

    vkGetDeviceQueue(vkLogicalDeviceObj, vulkanGraphicsQueueFamilyIndex, 0, &graphicsQueueObj);
}


void VulkanManager::CreateSurface(GLFWwindow * glfwWindow)
{
#if defined(GLFW_ENABLED)
    if (VK_SUCCESS != glfwCreateWindowSurface(vkInstanceObj, glfwWindow, nullptr, &surface)) 
    {
        glfwTerminate();
        ASSERT_MSG(0, "GLFW could not create the window surface.");
        std::exit(-1);
    }
#else
    ASSERT_MSG(0, "GLFW not getting used, need to implement OS specific implementation");
    std::exit(-1);
#endif

    VkBool32 WSI_supported = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDeviceObj, vulkanGraphicsQueueFamilyIndex, surface, &WSI_supported);
    if (!WSI_supported) 
    {
        ASSERT_MSG(0, "WSI not supported");
        std::exit(-1);
    }

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDeviceObj, surface, &surfaceCapabilities);

    if (surfaceCapabilities.currentExtent.width < UINT32_MAX)
    {
        surfaceWidth = surfaceCapabilities.currentExtent.width;
        surfaceHeight = surfaceCapabilities.currentExtent.height;
    }

    {
        uint32_t format_count = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDeviceObj, surface, &format_count, nullptr);
        if (format_count == 0) 
        {
            ASSERT_MSG(0, "Surface formats missing.");
            std::exit(-1);
        }
        std::vector<VkSurfaceFormatKHR> formats(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDeviceObj, surface, &format_count, formats.data());
        if (formats[0].format == VK_FORMAT_UNDEFINED) 
        {
            surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
            surfaceFormat.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        }
        else 
        {
            surfaceFormat = formats[0];
        }
    }
}

void VulkanManager::GetPhysicalDevice()
{
    uint32_t count = 0;
    vkEnumeratePhysicalDevices(vkInstanceObj, &count, nullptr);
    std::vector<VkPhysicalDevice> deviceList(count);
    vkEnumeratePhysicalDevices(vkInstanceObj, &count, deviceList.data());

    for (auto dev : deviceList)
    {
        VkPhysicalDeviceProperties deviceProp = {};
        vkGetPhysicalDeviceProperties(dev, &deviceProp);

        if (deviceProp.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ||
            deviceProp.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            vkPhysicalDeviceObj = dev;
            CoreObjects::physicalDeviceObj = &vkPhysicalDeviceObj;
            break;
        }
    }

    if (vkPhysicalDeviceObj == VK_NULL_HANDLE)
    {
        ASSERT_MSG(0, "gpu required");
        std::exit(-1);
    }

    vkGetPhysicalDeviceMemoryProperties(vkPhysicalDeviceObj, &physicalDeviceMemProps);
    vkGetPhysicalDeviceProperties(vkPhysicalDeviceObj, &physicalDeviceProps);
    vkGetPhysicalDeviceFeatures(vkPhysicalDeviceObj, &physicalDeviceFeatures);

    /*
    VkSampleCountFlags counts = std::min(physicalDeviceProps.limits.framebufferColorSampleCounts, physicalDeviceProps.limits.framebufferDepthSampleCounts);
    if (counts & VK_SAMPLE_COUNT_64_BIT) { maxSampleCount = VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { maxSampleCount = VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { maxSampleCount = VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { maxSampleCount = VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { maxSampleCount = VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { maxSampleCount = VK_SAMPLE_COUNT_2_BIT; }

    // enable sample shading features.
    if (physicalDeviceFeatures.sampleRateShading)
    {
        enabledPhysicalDeviceFeatures.sampleRateShading = VK_TRUE;
    }
    */
}

void VulkanManager::CalculateQueueFamilyIndex()
{
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDeviceObj, &count, nullptr);
    std::vector<VkQueueFamilyProperties> propertyList(count);
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDeviceObj, &count, propertyList.data());

    vulkanGraphicsQueueFamilyIndex = -1;
    for (uint32_t i = 0; i < count; i++)
    {
        if (propertyList[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            vulkanGraphicsQueueFamilyIndex = i;
            break;
        }
    }

    if (vulkanGraphicsQueueFamilyIndex == -1)
    {
        ASSERT_MSG(0, "No queue family found for grahics");
        std::exit(-1);
    }
}

void VulkanManager::Init()
{
    CreateInstance();
    validationManagerObj->InitDebug(&vkInstanceObj, pAllocator);
    CreateDevice();
}

void VulkanManager::DeInit()
{
    vkDestroySurfaceKHR(vkInstanceObj, surface, pAllocator);
    vkDestroyDevice(vkLogicalDeviceObj, pAllocator);
    validationManagerObj->DeinitDebug();
    vkDestroyInstance(vkInstanceObj, pAllocator);
}

void VulkanManager::Update()
{

}

VulkanManager * VulkanManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new VulkanManager();
    }
    return instance;
}

VulkanManager::~VulkanManager()
{
    delete validationManagerObj;
}

#include "VulkanManager.h"
#include "ValidationManager.h"
#include "VulkanUtility.h"
#include "VulkanMemoryManager.h"
#include "VkQueueFactory.h"
#include "VkCommandBufferFactory.h"
#include "VkSynchroniserFactory.h"
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

void VulkanManager::CreateLogicalDevice()
{
    std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfoList = VkQueueFactory::GetInstance()->FindQueue();

    VkDeviceCreateInfo vkDeviceCreateInfoObj{};
    vkDeviceCreateInfoObj.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    vkDeviceCreateInfoObj.queueCreateInfoCount = (uint32_t)deviceQueueCreateInfoList.size();
    vkDeviceCreateInfoObj.pQueueCreateInfos = deviceQueueCreateInfoList.data();
    vkDeviceCreateInfoObj.enabledExtensionCount = (uint32_t)validationManagerObj->deviceExtensionNameList.size();
    vkDeviceCreateInfoObj.enabledLayerCount = 0;
    vkDeviceCreateInfoObj.pEnabledFeatures = &enabledPhysicalDeviceFeatures;
    vkDeviceCreateInfoObj.ppEnabledExtensionNames = validationManagerObj->deviceExtensionNameList.data();
    vkDeviceCreateInfoObj.ppEnabledLayerNames = nullptr;

    ErrorCheck(vkCreateDevice(vkPhysicalDeviceObj, &vkDeviceCreateInfoObj, pAllocator, &vkLogicalDeviceObj));

    CoreObjects::logicalDeviceObj = &vkLogicalDeviceObj;
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
    vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDeviceObj, 
        VkQueueFactory::GetInstance()->GetGraphicsQueueFamilyIndex(), 
        surface, &WSI_supported);
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
}

void VulkanManager::Init()
{
    CreateInstance();
    GetPhysicalDevice();
    validationManagerObj->InitDebug(&vkInstanceObj, pAllocator);
    VkQueueFactory::GetInstance()->Init();
    CreateLogicalDevice();

    //TODO : expose queue creation.

    VkQueueFactory::GetInstance()->CreateGraphicsQueues(&CoreObjects::renderQueueId, 1);
    QueuePurpose * renderPurpose = new QueuePurpose{ QueuePurpose::RENDER };
    VkQueueFactory::GetInstance()->SetQueuePurpose(renderPurpose, QueueType::GRAPHICS, CoreObjects::renderQueueId);
    
    VkQueueFactory::GetInstance()->CreateGraphicsQueues(&CoreObjects::presentationQueuedId, 1);
    QueuePurpose * presentPurpose = new QueuePurpose{ QueuePurpose::PRESENT };
    VkQueueFactory::GetInstance()->SetQueuePurpose(presentPurpose, QueueType::GRAPHICS, CoreObjects::presentationQueuedId);

    VkQueueFactory::GetInstance()->CreateComputeQueues(&CoreObjects::computeQueueId, 1);
    VkQueueFactory::GetInstance()->CreateTransferQueues(&CoreObjects::transferQueueId, 1);

    VulkanMemoryManager::GetSingleton()->Init(physicalDeviceMemProps);
    VkCommandBufferFactory::GetInstance()->Init();
    VkSynchroniserFactory::GetInstance()->Init();
}

void VulkanManager::DeInit()
{
    VkSynchroniserFactory::GetInstance()->Init();
    delete VkSynchroniserFactory::GetInstance();

    VkCommandBufferFactory::GetInstance()->DeInit();
    delete VkCommandBufferFactory::GetInstance();

    VulkanMemoryManager::GetSingleton()->DeInit();
    delete VulkanMemoryManager::GetSingleton();

    vkDestroySurfaceKHR(vkInstanceObj, surface, pAllocator);
    vkDestroyDevice(vkLogicalDeviceObj, pAllocator);
    validationManagerObj->DeinitDebug();
    vkDestroyInstance(vkInstanceObj, pAllocator);

    VkQueueFactory::GetInstance()->DeInit();
    delete VkQueueFactory::GetInstance();
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

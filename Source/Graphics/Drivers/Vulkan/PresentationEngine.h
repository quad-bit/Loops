#pragma once
#include <vulkan\vulkan.h>
#include <vector>

using namespace std;

struct AttachmentInfo;
class PresentationEngine
{

private:
    PresentationEngine(){}
    PresentationEngine(PresentationEngine const &) {}
    PresentationEngine const & operator= (PresentationEngine const &) {}

    static PresentationEngine* instance;
        
    VkSurfaceKHR* surfaceObj;
    VkSurfaceFormatKHR * surfaceFormat;
    uint32_t swapChainImageCount;
    VkSurfaceCapabilitiesKHR surfaceCapabilities={};
    VkSwapchainKHR swapchainObj;
    VkPresentModeKHR presentMode;

    vector<VkImage> swapChainImageList;
    vector<VkImageView> swapChainImageViewList;

    uint32_t activeSwapchainImageID;

public:
    void Init(VkSurfaceKHR* surfaceObj, VkSurfaceFormatKHR * surfaceFormat);
    vector<VkImage> * CreateSwapchainImage(AttachmentInfo* info, uint32_t count);
    vector<VkImageView> * CreateSwapchainImageViews(AttachmentInfo* info, uint32_t count);
    void DestroySwapChain();
    void DestroySwapChainImageView();

    void DeInit();
    void Update();
    static PresentationEngine* GetInstance();
    ~PresentationEngine();

    uint32_t VkGetAvailableSwapChainId(VkFence * fence, VkSemaphore * semaphore);

};

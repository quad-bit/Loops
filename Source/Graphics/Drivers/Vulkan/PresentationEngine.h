#pragma once
#include <vulkan\vulkan.h>
#include <vector>

using namespace std;

namespace Loops::Graphics::Vulkan
{
    class PresentationEngine
    {

    private:
        PresentationEngine(){}
        PresentationEngine(PresentationEngine const &) {}
        PresentationEngine const & operator= (PresentationEngine const &) {}

        static PresentationEngine* instance;
        
        VkSurfaceKHR* surfaceObj;
        uint32_t swapChainImageCount;
        VkSurfaceCapabilitiesKHR surfaceCapabilities={};
        VkSwapchainKHR swapchainObj;

        vector<VkImage> swapChainImageList;
        vector<VkImageView> swapChainImageViewList;

    public:
        void Init(VkSurfaceKHR* surfaceObj, VkSurfaceFormatKHR * surfaceFormat);
        void CreateSwapchainImageViews(VkSurfaceKHR* surfaceObj, VkSurfaceFormatKHR * surfaceFormat);
        void DeInit();
        void Update();
        static PresentationEngine* GetInstance();
        ~PresentationEngine();
    };
}

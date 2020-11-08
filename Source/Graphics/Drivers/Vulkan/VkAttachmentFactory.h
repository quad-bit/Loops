#pragma once
#include <vector>
#include <vulkan\vulkan.h>
#include <stdint.h>

using namespace std;

namespace Loops::Graphics::Vulkan
{
    namespace Loops::Core::Utility::Wrappers
    {
        struct ImageInfo;
    };
    
    class AttachmentWrapper
    {
    public:
        uint32_t id;
        VkImage* image;
        VkImageView* imageView;
    };

    class VkAttachmentFactory
    {
    private:
        VkAttachmentFactory() {}
        VkAttachmentFactory(VkAttachmentFactory const &) {}
        VkAttachmentFactory const & operator= (VkAttachmentFactory const &) {}

        static VkAttachmentFactory* instance;
        static uint32_t imageId;
        
        void CreateSwapChainImages();
        void CreateSwapChainImageViews();
        void DestroySwapChainImages();
        void DestroySwapChainImageViews();

        vector<AttachmentWrapper> attachmentList;

    public:
        void Init();
        void DeInit();
        void Update();
        static VkAttachmentFactory* GetInstance();
        ~VkAttachmentFactory();

        void CreateColorAttachment(Loops::Core::Utility::Wrappers::ImageInfo * info, uint32_t count, bool defaultTarget, vector<uint32_t>* ids);
        void CreateDepthAttachment(Loops::Core::Utility::Wrappers::ImageInfo * info, uint32_t count, bool defaultTarget, vector<uint32_t>* ids);
        void DestroyAttachment(vector<uint32_t> ids, bool defaultTarget);
        uint32_t GetId();
    };
}
#pragma once
#include <vector>
#include <vulkan\vulkan.h>
#include <stdint.h>

using namespace std;

struct ImageInfo;
class AttachmentWrapper
{
public:
    uint32_t id;
    bool attachmentActive;
    VkImage* image;
    VkImageView* imageView;
    VkDeviceMemory* imageMemory;
    VkImageUsageFlags usage;

    AttachmentWrapper()
    {
        attachmentActive = false;
        id = -1;
    }

    void DeActivateAttachment();
    
};

struct AttachmentInfo
{
    uint32_t width, height, mips, layers;
    VkFormat format;
    VkImageViewType viewType;
    VkImageType imageType;
    VkColorSpaceKHR colorSpace;
    VkImageUsageFlags usage;
    VkSampleCountFlagBits sampleCount;
};

class VkAttachmentFactory
{
private:
    VkAttachmentFactory() {}
    VkAttachmentFactory(VkAttachmentFactory const &) {}
    VkAttachmentFactory const & operator= (VkAttachmentFactory const &) {}

    static VkAttachmentFactory* instance;
    static uint32_t imageId;

    vector<AttachmentWrapper * > attachmentList;
    vector<VkImage* > imageList;
    vector<VkImageView* > imageViewList;
    vector<VkDeviceMemory* > memoryList;

    AttachmentInfo* UnwrapImageInfo(ImageInfo * imageInfo);

public:
    void Init();
    void DeInit();
    void Update();
    static VkAttachmentFactory* GetInstance();
    ~VkAttachmentFactory();

    VkFormat FindBestDepthFormat(VkFormat inputFormat);

    void CreateColorAttachment(ImageInfo * info, uint32_t count, bool defaultTarget, vector<uint32_t>* ids);
    void CreateDepthAttachment(ImageInfo * info, uint32_t count, bool stencilRequired, bool defaultTarget, vector<uint32_t>* ids);
    void DestroyAttachment(vector<uint32_t> ids, bool defaultTarget);
    uint32_t GetId();
};

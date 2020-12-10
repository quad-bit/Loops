#pragma once
#include <vector>
#include <vulkan\vulkan.h>
#include <stdint.h>

using namespace std;

enum class Format;
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
    VkImageLayout initialLayout;
};


class VkAttachmentFactory
{
private:
    VkAttachmentFactory() {}
    VkAttachmentFactory(VkAttachmentFactory const &) {}
    VkAttachmentFactory const & operator= (VkAttachmentFactory const &) {}

    static VkAttachmentFactory* instance;
    static uint32_t imageId;
    VkFormat bestDepthFormat;

    vector<AttachmentWrapper * > attachmentList;
    vector<VkImage* > imageList;
    vector<VkImageView* > imageViewList;
    vector<VkDeviceMemory* > memoryList;

    AttachmentInfo* UnwrapImageInfo(ImageInfo * imageInfo);
    uint32_t GetId();

public:
    void Init();
    void DeInit();
    void Update();
    static VkAttachmentFactory* GetInstance();
    ~VkAttachmentFactory();

    uint32_t FindBestDepthFormat(Format * format, uint32_t count);

    void CreateColorAttachment(ImageInfo * info, uint32_t count, bool defaultTarget, vector<uint32_t>* ids);
    //void CreateDepthAttachment(ImageInfo * info, uint32_t count, bool stencilRequired, bool defaultTarget, vector<uint32_t>* ids);
    void CreateDepthAttachment(VkImageCreateInfo * info, uint32_t count, VkImageViewCreateInfo * viewInfo,
        bool stencilRequired, bool defaultTarget, vector<uint32_t>* ids);
    void DestroyAttachment(vector<uint32_t> ids, bool defaultTarget);
    VkFormat GetBestDepthFormat() { return bestDepthFormat;}

    VkImageView * GetImageView(uint32_t id);

};

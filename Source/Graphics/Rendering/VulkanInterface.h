#pragma once
#include <stdint.h>
#include <vector>

using namespace std;

struct ImageInfo;
struct RenderPassAttachmentInfo;
struct SubpassInfo;
struct SubpassDependency;
enum class ImageFormat;

class VulkanInterface
{
private:

public:
    VulkanInterface();
    ~VulkanInterface();

    void Init();
    void DeInit();
  
    uint32_t FindBestDepthFormat(ImageFormat * imageFormat, uint32_t count);

    void CreateRenderTarget(ImageInfo * info, uint32_t count, bool defaultTarget, 
        std::vector<uint32_t>* ids);
    void DestroyRenderTarget(std::vector<uint32_t>* ids, bool defaultTarget);
    
    void CreateDepthTarget(ImageInfo * info, uint32_t count, bool stencilRequired, 
        bool defaultTarget, std::vector<uint32_t>* ids);
    void DestroyDepthTarget(std::vector<uint32_t>* ids, bool defaultTarget);

    void CreateRenderPass(
        const RenderPassAttachmentInfo* renderpassAttachmentList, uint32_t attachmentCount,
        const SubpassInfo* subpassList, uint32_t subpassCount,
        const SubpassDependency* dependencyList, uint32_t dependencyCount,
        uint32_t& renderPassId);
    void DestroyRenderPass(uint32_t id);

    void CreateFrameBuffer(uint32_t numFrameBuffers, uint32_t * imageViewId, uint32_t viewsPerFB,
        uint32_t renderPassId, uint32_t width, uint32_t height, uint32_t * ids);
    void DestroyFrameBuffer(uint32_t * pid, uint32_t count);

};

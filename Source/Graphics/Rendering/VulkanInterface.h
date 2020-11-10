#pragma once
#include <stdint.h>
#include <vector>

using namespace std;

struct ImageInfo;

class VulkanInterface
{
private:

public:
    VulkanInterface();
    ~VulkanInterface();

    void Init();
    void DeInit();
    void CreateRenderTarget(ImageInfo * info, uint32_t count, bool defaultTarget, std::vector<uint32_t>* ids);
    void DestroyRenderTarget(std::vector<uint32_t>* ids, bool defaultTarget);
    void CreateDepthTarget(ImageInfo * info, uint32_t count, bool stencilRequired, bool defaultTarget, std::vector<uint32_t>* ids);
    void DestroyDepthTarget(std::vector<uint32_t>* ids, bool defaultTarget);

};

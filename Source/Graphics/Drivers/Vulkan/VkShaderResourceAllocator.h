#pragma once

#include <vulkan\vulkan.h>
#include <map>
#include <vector>
#include "RenderingWrapper.h"



class VkShaderResourceAllocator
{
private:
    std::map<ShaderResourceClassification, std::vector<uint32_t>> resourceSetClassifyingMap;
    
    void VkShaderResourceAllocator::CreateResourceSetting();

public:
    VkShaderResourceAllocator();
    ~VkShaderResourceAllocator();

    //VkDescriptorSet * AllocateDescriptor(SetWrapper * setWrapper, )
};
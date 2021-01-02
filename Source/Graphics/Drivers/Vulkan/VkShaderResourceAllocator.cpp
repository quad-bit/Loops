#include "VkShaderResourceAllocator.h"

void VkShaderResourceAllocator::CreateResourceSetting()
{
    std::vector<uint32_t> perSceneSets{ 0, 1 };    //"View", "Lights"
    std::vector<uint32_t> perMaterialSets{ 2, 3 };   //"Sampler", "DiffuseTexture"
    std::vector<uint32_t> perMaterialInstanceSets{ 4, 5 };   //"Transform"

    resourceSetClassifyingMap.insert(std::pair < ShaderResourceClassification, std::vector<uint32_t>>{
        ShaderResourceClassification::PER_SCENE, perSceneSets
    });

    resourceSetClassifyingMap.insert(std::pair < ShaderResourceClassification, std::vector<uint32_t>>{
        ShaderResourceClassification::PER_MATERIAL, perMaterialSets
    });

    resourceSetClassifyingMap.insert(std::pair < ShaderResourceClassification, std::vector<uint32_t>>{
        ShaderResourceClassification::PER_MATERIAL_INSTANCE, perMaterialInstanceSets
    });
}

VkShaderResourceAllocator::VkShaderResourceAllocator()
{
}

VkShaderResourceAllocator::~VkShaderResourceAllocator()
{
}

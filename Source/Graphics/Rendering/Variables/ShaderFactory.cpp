#include "ShaderFactory.h"
#include "AttributeHelper.h"
#include <CorePrecompiled.h>
#include <algorithm>
#include <Shader.h>
#include <GraphicsPipelineManager.h>

#if (RENDERING_API == VULKAN)
#include <VulkanInterface.h>
#elif (RENDERING_API == DX)
#include <DxInterface.h>
#endif

ShaderFactory* ShaderFactory::instance = nullptr;

ShaderFactory * ShaderFactory::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new ShaderFactory();
    }
    return instance;
}

ShaderFactory::~ShaderFactory()
{
}



void ShaderFactory::CreateShader(const uint32_t & meshId, Shader * shaders, const uint32_t & shaderCount)
{
    // Get the ids for each shader object.
    char ** shaderNames = new char*[shaderCount];
    ShaderType * types = new ShaderType[shaderCount];
    uint32_t * ids = new uint32_t[shaderCount];

    for (uint32_t i = 0; i < shaderCount; i++)
    {
        shaderNames[i] = const_cast<char*>(shaders[i].shaderName);
        types[i] = shaders[i].shaderType;
    }

    apiInterface->GetShaderIds(shaderNames, types, ids, shaderCount);

    for (uint32_t i = 0; i < shaderCount; i++)
    {
        shaders[i].shaderId = ids[i];
    }

    GraphicsPipelineManager<ApiInterface>::GetInstance()->CreatShaderPipelineState(meshId, shaders, shaderCount);
    GraphicsPipelineManager<ApiInterface>::GetInstance()->CreatResourceLayoutState(meshId, shaders, shaderCount);

    delete[] ids;
    delete[] types;
    delete[] shaderNames;
}

void ShaderFactory::Init(ApiInterface * apiInterface)
{
    this->apiInterface = apiInterface;
}

void ShaderFactory::DeInit()
{
}

void ShaderFactory::Update()
{
}


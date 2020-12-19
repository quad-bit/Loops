#include "HashManager.h"
#include "HashHelper.h"
#include "Assertion.h"
#include "Shader.h"

HashManager* HashManager::instance = nullptr;


uint32_t HashManager::CheckForHash(const std::size_t & hash, const uint32_t & stateId, const PipelineStates & state)
{
    std::vector<HashObjectWrapper>::iterator it;
    it = std::find_if(hashList.begin(), hashList.end(), [&](HashObjectWrapper e) { return e.hash == hash; });

    if (it != hashList.end())
    {
        return it->objectId;
    }
    else
    {
        HashObjectWrapper obj;
        obj.hash = hash;
        obj.objectId = stateId;
        obj.state = state;

        hashList.push_back(obj);
        return -1;
    }
}

void HashManager::Init()
{
    
}

void HashManager::DeInit()
{
    hashList.clear();
    hashList.resize(0);
}

void HashManager::Update()
{

}

HashManager * HashManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new HashManager();
    }
    return instance;
}

HashManager::~HashManager()
{
}

int HashManager::FindVertexInputStateHash(VertexInputState * inputInfo, uint32_t stateId)
{
    std::size_t hash = 0UL;
    size_t hashAttrib, hashBinding, hashPipelineState;

    for (uint32_t i = 0; i < inputInfo->attribCount; i++)
    {
        hashAttrib = std::hash<VertexInputAttributeInfo>{}(inputInfo->attribInfo[i]);
        HashCombine(hash, hashAttrib);
    }

    for (uint32_t i = 0; i < inputInfo->bindingCount; i++)
    {
        hashBinding = std::hash<VertexInputBindingInfo>{}(inputInfo->bindingInfo[i]);
        HashCombine(hash, hashBinding);
    }

    hashPipelineState = std::hash<uint32_t>{}((uint32_t)inputInfo->state);
    HashCombine(hash, hashPipelineState);

    return CheckForHash(hash, stateId, inputInfo->state);
}

int HashManager::FindInputAssemblyStateHash(InputAssemblyState * inputInfo, uint32_t stateId)
{
    std::size_t hash = 0UL;
    size_t hashPrimitive, hashRestart, hashState;

    hashPrimitive = std::hash<uint32_t>{}((uint32_t)*inputInfo->primitiveType);
    hashRestart = std::hash<bool>{}(inputInfo->isPrimtiveRestartEnabled);
    hashState = std::hash<uint32_t>{}((uint32_t)inputInfo->state);

    HashCombine(hash, hashPrimitive, hashRestart, hashState);

    return CheckForHash(hash, stateId, inputInfo->state);
}

int HashManager::FindShaderStateHash(Shader * shaders, const uint32_t & shaderCount, uint32_t stateId, PipelineStates * state)
{
    std::size_t hash = 0UL;
    size_t shaderCountHash;
    size_t vertexShaderHash = 0UL, fragmentShaderHash = 0UL;
    ASSERT_MSG(shaderCount <= 2, "More than two shaders, yet to be handled");

    shaderCountHash = std::hash<uint32_t>{}(shaderCount);

    for (uint32_t i = 0; i < shaderCount; i++)
    {
        size_t shaderTypeHash = std::hash<uint32_t>{}((uint32_t)shaders[i].shaderType);
        size_t shaderNameHash = std::hash<const char*>{}(shaders[i].shaderName);
        size_t shaderId = std::hash<uint32_t>{}(shaders[i].shaderId);

        if (shaders[i].shaderType == ShaderType::VERTEX)
        {
            HashCombine(vertexShaderHash, shaderTypeHash, shaderNameHash, shaderId);
        }
        else if (shaders[i].shaderType == ShaderType::FRAGMENT)
        {
            HashCombine(fragmentShaderHash, shaderTypeHash, shaderNameHash, shaderId);
        }
    }

    HashCombine(hash, shaderCountHash, vertexShaderHash, fragmentShaderHash);

    return CheckForHash(hash, stateId, *state);
}

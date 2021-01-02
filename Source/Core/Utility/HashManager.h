#pragma once

#include "RenderingWrapper.h"
#include <map>
#include <vector>

class Shader;
struct ShaderStateWrapper;
struct SetWrapper;
enum class PipelineStates;

struct PipelineObjectHashWrapper
{
    uint32_t objectId;
    PipelineStates state;
    std::size_t hash;
};

struct DescriptorSetLayoutWrapper
{
    uint32_t objectId;
    std::size_t hash = 0UL;
};

//combination of sets
struct SetLayoutCombinationWrapper
{
    uint32_t layoutId;
    std::vector<SetWrapper*> setList;
    std::size_t hash;
};

class HashManager
{
private:
    HashManager(){}
    HashManager(HashManager const &) {}
    HashManager const & operator= (HashManager const &) {}

    static HashManager* instance;
    
    std::map<PipelineStates, std::vector<std::size_t>> stateToHashMap;
    std::vector<PipelineObjectHashWrapper> pipelineHashList;
    std::vector<DescriptorSetLayoutWrapper> descriptorSetLayoutWrapperHashList;
    std::vector<SetLayoutCombinationWrapper> setCombinationWrapperHashList;

    uint32_t CheckForPipeLineObjectHash(const std::size_t & hash, const uint32_t & stateId, const PipelineStates & state);

public:
    void Init();
    void DeInit();
    void Update();
    static HashManager* GetInstance();
    ~HashManager();

    int FindVertexInputStateHash(VertexInputState * inputInfo, uint32_t stateId);
    int FindInputAssemblyStateHash(InputAssemblyState * inputInfo, uint32_t stateId);
    int FindShaderStateHash(Shader * inputInfo, const uint32_t & shaderCount, 
        uint32_t stateId, PipelineStates * state);

    int FindDescriptorSetHash(SetWrapper * obj, uint32_t id);
    int FindResourceLayoutHash(SetWrapper ** obj, const uint32_t & setCount, uint32_t id);

};



#pragma once

#include "RenderingWrapper.h"
#include <map>
#include <vector>

class Shader;
struct ShaderStateWrapper;
enum class PipelineStates;

struct HashObjectWrapper
{
    uint32_t objectId;
    PipelineStates state;
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
    std::vector<HashObjectWrapper> hashList;

    uint32_t CheckForHash(const std::size_t & hash, const uint32_t & stateId, const PipelineStates & state);

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
};



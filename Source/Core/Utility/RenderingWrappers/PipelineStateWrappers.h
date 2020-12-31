#pragma once
#include "RenderingWrapper.h"
#include "Shader.h"

struct StateWrapperBase
{
protected:
    uint32_t id;
public:
    PipelineStates state;
    std::vector<uint32_t> meshIdList;

    const uint32_t & GetId()
    {
        return id;
    }

    virtual void Execute() {}
};

template<typename T>
void AssignId(uint32_t * id)
{
    *id = T::idCounter++;
}

template<typename T>
void DecrementIdCounter()
{
    T::idCounter--;
}

struct VertexInputWrapper : public StateWrapperBase
{
    VertexInputState * inputState;
    static uint32_t idCounter;

    VertexInputWrapper()
    {
        AssignId<VertexInputWrapper>(&this->id);
        state = PipelineStates::VertexInputState;
    }
};


struct InputAssemblyWrapper : public StateWrapperBase
{
    InputAssemblyState * assemblyState;
    static uint32_t idCounter;

    InputAssemblyWrapper()
    {
        AssignId<InputAssemblyWrapper>(&this->id);
        state = PipelineStates::InputAssemblyState;
    }
};

struct ShaderStateWrapper : public StateWrapperBase
{
    Shader * shader;// one shader equivalent to one shader module
    uint32_t shaderCount;

    static uint32_t idCounter;

    ShaderStateWrapper()
    {
        AssignId<ShaderStateWrapper>(&this->id);
        state = PipelineStates::ShaderStage;
    }
};

struct ShaderResourceStateWrapper : public StateWrapperBase
{
    Shader * shader;
    uint32_t shaderCount;
    std::vector<SetWrapper*> resourcesSetList;
    uint32_t pipelineId;

    static uint32_t idCounter;

    ShaderResourceStateWrapper()
    {
        AssignId<ShaderResourceStateWrapper>(&this->id);
        state = PipelineStates::ShaderResourcesLayout;
    }
};


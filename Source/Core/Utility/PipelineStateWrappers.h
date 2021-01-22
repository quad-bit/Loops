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

struct TessellationStateWrapper : public StateWrapperBase
{
    static uint32_t idCounter;
    TessellationState * tessellationState;
    TessellationStateWrapper()
    {
        AssignId<TessellationStateWrapper>(&this->id);
        state = PipelineStates::TessellationState;
    }
};

struct ColorBlendStateWrapper : public StateWrapperBase
{
    static uint32_t idCounter;
    ColorBlendState * colorBlendState;
    ColorBlendStateWrapper()
    {
        AssignId<ColorBlendStateWrapper >(&this->id);
        state = PipelineStates::ColorBlendState;
    }
};

struct ViewPortStateWrapper : public StateWrapperBase
{
    static uint32_t idCounter;
    ViewportState * viewportState;
    ViewPortStateWrapper()
    {
        AssignId<ViewPortStateWrapper>(&this->id);
        state = PipelineStates::ViewportState;
    }
};

struct RasterizationStateWrapper : public StateWrapperBase
{
    static uint32_t idCounter;
    RasterizationState * rasterState;
    RasterizationStateWrapper()
    {
        AssignId<RasterizationStateWrapper>(&this->id);
        state = PipelineStates::RasterizationState;
    }
};

struct MultiSampleStateWrapper : public StateWrapperBase
{
    static uint32_t idCounter;
    MultiSampleState * multiSampleState;
    MultiSampleStateWrapper()
    {
        AssignId<MultiSampleStateWrapper>(&this->id);
        state = PipelineStates::MultisampleState;
    }
};

struct DepthStencilStateWrapper : public StateWrapperBase
{
    static uint32_t idCounter;
    DepthStencilState * depthState;
    DepthStencilStateWrapper()
    {
        AssignId<DepthStencilStateWrapper>(&this->id);
        state = PipelineStates::DepthStencilState;
    }
};

struct DynamicStateWrapper : public StateWrapperBase
{
    static uint32_t idCounter;
    DynamicStateList * dynamicStates;
    DynamicStateWrapper()
    {
        AssignId<DynamicStateWrapper>(&this->id);
        state = PipelineStates::DynamicState;
    }
};

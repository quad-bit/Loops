#pragma once
#include "RenderingWrapper.h"

struct StateWrapperBase
{
protected:
    uint32_t id;
public:
    std::vector<uint32_t> meshIdList;

    const uint32_t & GetId()
    {
        return id;
    }

};

struct VertexInputWrapper : public StateWrapperBase
{
    VertexInputState * inputState;
    static uint32_t idCounter;

    void AssignId()
    {
        id = idCounter;
        idCounter++;
    }
    
    void DecrementIdCounter()
    {
        idCounter--;
    }

    VertexInputWrapper()
    {
        AssignId();
    }
};


struct InputAssemblyWrapper : public StateWrapperBase
{
    InputAssemblyState * assemblyState;
    static uint32_t idCounter;

    void AssignId()
    {
        id = idCounter;
        idCounter++;
    }

    void DecrementIdCounter()
    {
        idCounter--;
    }

    InputAssemblyWrapper()
    {
        AssignId();
    }
};


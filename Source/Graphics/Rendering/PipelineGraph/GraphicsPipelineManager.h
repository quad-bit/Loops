#pragma once
#include "RenderingWrapper.h"
#include "HashManager.h"
#include "PipelineStateWrappers.h"


template <typename T>
class GraphicsPipelineManager
{
private:
    GraphicsPipelineManager(){}
    GraphicsPipelineManager(GraphicsPipelineManager const &) {}
    GraphicsPipelineManager const & operator= (GraphicsPipelineManager const &) {}

    static GraphicsPipelineManager* instance;
    
    T * apiInterface;

    std::vector<VertexInputWrapper> vertexInputStateWrapperList;
    std::vector<InputAssemblyWrapper> vertexInputAssemblyWrapperList;

    uint32_t defaultAssemblyListIndex;
    void CreateVertexAssemblyDefault();

public:
    void Init(T * apiInterface);
    void DeInit();
    void Update();
    static GraphicsPipelineManager* GetInstance();
    ~GraphicsPipelineManager();

    void CreateVertexInputState(const uint32_t & meshId, VertexInputState * inputStateInfo );
    void CreateVertexAssemblyState(const uint32_t & meshId, InputAssemblyState * assembly);
    void AssignDefaultState(const uint32_t & meshId, const PipelineStates & state);
};

template<typename T>
inline void GraphicsPipelineManager<T>::CreateVertexAssemblyDefault()
{
    defaultAssemblyListIndex = (uint32_t)vertexInputAssemblyWrapperList.size();
    InputAssemblyWrapper wrapper = {};
    wrapper.assemblyState = new InputAssemblyState;
    wrapper.assemblyState->primitiveType = new PrimtiveType{ PrimtiveType::TOPOLOGY_TRIANGLE_LIST };
    wrapper.assemblyState->isPrimtiveRestartEnabled = false;

    HashManager::GetInstance()->FindInputAssemblyStateHash(wrapper.assemblyState, wrapper.GetId());

    vertexInputAssemblyWrapperList.push_back(wrapper);
}

template<typename T>
inline void GraphicsPipelineManager<T>::Init(T * apiInterface)
{
    this->apiInterface = apiInterface;
    CreateVertexAssemblyDefault();
}

template<typename T>
inline void GraphicsPipelineManager<T>::DeInit()
{
    for each(VertexInputWrapper wrapper in vertexInputStateWrapperList)
    {
        delete[] wrapper.inputState->attribInfo;
        delete[] wrapper.inputState->bindingInfo;
        delete wrapper.inputState;
        wrapper.meshIdList.clear();
    }

    vertexInputStateWrapperList.clear();
    vertexInputStateWrapperList.resize(0);

    for each(InputAssemblyWrapper wrapper in vertexInputAssemblyWrapperList)
    {
        delete wrapper.assemblyState->primitiveType;
        delete wrapper.assemblyState;
        wrapper.meshIdList.clear();
    }

    vertexInputAssemblyWrapperList.clear();
    vertexInputAssemblyWrapperList.resize(0);
}

template<typename T>
inline void GraphicsPipelineManager<T>::Update()
{
}

template<typename T>
inline GraphicsPipelineManager<T>::~GraphicsPipelineManager()
{
}

template<typename T>
inline void GraphicsPipelineManager<T>::CreateVertexInputState(const uint32_t & meshId, VertexInputState * inputStateInfo)
{
    VertexInputWrapper wrapper = {};
    wrapper.inputState = inputStateInfo;

    uint32_t id = HashManager::GetInstance()->FindVertexInputStateHash(inputStateInfo, wrapper.GetId());
    
    if (id == -1)
    {
        // create a new pipeline node encapsulate in graph node, add it to graph
        // TODO : Create pipeline node
        // add wrapper to list
        wrapper.meshIdList.push_back(meshId);
        vertexInputStateWrapperList.push_back(wrapper);
    }
    else
    {
        // reduce meshid counter by 1
        wrapper.DecrementIdCounter();
        // delete input info
        delete inputStateInfo;
        // add the mesh id to the existing wrapper obj in the list
        std::vector<VertexInputWrapper>::iterator it;
        it = std::find_if(vertexInputStateWrapperList.begin(), vertexInputStateWrapperList.end(), [&](VertexInputWrapper e) { return e.GetId() == id; });

        ASSERT_MSG(it != vertexInputStateWrapperList.end(), "wrapper not found");
        
        it->meshIdList.push_back(meshId);
    }
}

template<typename T>
inline void GraphicsPipelineManager<T>::CreateVertexAssemblyState(const uint32_t & meshId, InputAssemblyState * assembly)
{
    InputAssemblyWrapper wrapper = {};
    wrapper.assemblyState = assembly;

    uint32_t id = HashManager::GetInstance()->FindInputAssemblyStateHash(assembly, wrapper.GetId());

    if (id == -1)
    {
        // create a new pipeline node encapsulate in graph node, add it to graph
        // TODO : Create pipeline node
        // add wrapper to list
        wrapper.meshIdList.push_back(meshId);
        vertexInputAssemblyWrapperList.push_back(wrapper);
    }
    else
    {
        // reduce meshid counter by 1
        wrapper.DecrementIdCounter();
        // delete input info
        delete assembly->primitiveType;
        delete assembly;
        // add the mesh id to the existing wrapper obj in the list
        std::vector<InputAssemblyWrapper>::iterator it;
        it = std::find_if(vertexInputAssemblyWrapperList.begin(), vertexInputAssemblyWrapperList.end(), [&](InputAssemblyWrapper e) { return e.GetId() == id; });

        ASSERT_MSG(it != vertexInputAssemblyWrapperList.end(), "wrapper not found");

        it->meshIdList.push_back(meshId);
    }
}

template<typename T>
inline void GraphicsPipelineManager<T>::AssignDefaultState(const uint32_t & meshId, const PipelineStates & state)
{
    switch (state)
    {
    case PipelineStates::InputAssemblyState :
        vertexInputAssemblyWrapperList[defaultAssemblyListIndex].meshIdList.push_back(meshId);
        break;

    default:
        ASSERT_MSG(0, "Invalid state");
    }
}

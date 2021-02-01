#pragma once

#include "DrawGraphNode.h"
#include "CorePrecompiled.h"
#include "Settings.h"
#include "Graph.h"

#if (RENDERING_API == VULKAN)
#include "VulkanInterface.h"
typedef VulkanInterface ApiInterface;
#elif (RENDERING_API == DX)
#include "DxInterface.h"
typedef DxInterface ApiInterface
#endif

#include "ForwardDrawGraph.h"
#include "DeferredDrawGraph.h"

enum class RendererType
{
    Forward,
    Deferred
};

template <typename T>
class DrawCommandBuffer;

class DrawGraphManager
{
private:
    DrawGraphManager() {}
    DrawGraphManager(DrawGraphManager const &) {}
    DrawGraphManager const & operator= (DrawGraphManager const &) {}
    static DrawGraphManager* instance;
    
    ApiInterface * apiInterface;
    RendererType rendererType;
    
    ForwardGraph<ApiInterface> * fwdGraph = nullptr;
    DeferredGraph<ApiInterface> * dfrdGraph = nullptr;

    int maxDrawNodes = 30;

    std::vector<DrawGraphNode *> drawGraphNodeList;
    Graph<DrawGraphNode> * drawGraph;// TODO: add the graph construction without predefined length
    //GraphNode<StateWrapperBase> * CreateGraphNode(StateWrapperBase * stateWrapper);

    //void CreateGraphEdges();
    //void CreateGraphEdges(const uint32_t & meshId, PipelineStates src, PipelineStates dest);
    //GraphNode<StateWrapperBase> * GetNode(PipelineStates state, StateWrapperBase * stateObj);

public:
    void Init(RendererType rendererType);
    void DeInit();
    
    template <typename T>
    void Update(DrawCommandBuffer<T> * dcb);
    
    static DrawGraphManager* GetInstance();
    ~DrawGraphManager();

    void AddNode(GraphNode<DrawGraphNode> * node);
    void CreateGraphEdges(GraphNode<DrawGraphNode> * src, GraphNode<DrawGraphNode> * dest);

};

template<typename T>
inline void DrawGraphManager::Update(DrawCommandBuffer<T>* dcb)
{
}

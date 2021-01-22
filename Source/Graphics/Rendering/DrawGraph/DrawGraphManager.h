#pragma once

#include "DrawGraphNode.h"
#include "CorePrecompiled.h"

template <typename T>
class DrawCommandBuffer;

template <typename T>
class DrawGraphManager
{
private:
    DrawGraphManager() {}
    DrawGraphManager(DrawGraphManager const &) {}
    DrawGraphManager const & operator= (DrawGraphManager const &) {}
    static DrawGraphManager* instance;
    T * apiInterface;
    
    int maxGraphNodes = 30;
    //Graph<StateWrapperBase> * pipelineGraph;// TODO: add the graph construction without predefined length
    //GraphNode<StateWrapperBase> * CreateGraphNode(StateWrapperBase * stateWrapper);

    //void CreateGraphEdges();
    //void CreateGraphEdges(const uint32_t & meshId, PipelineStates src, PipelineStates dest);
    //GraphNode<StateWrapperBase> * GetNode(PipelineStates state, StateWrapperBase * stateObj);

public:
    void Init();
    void DeInit();
    void Update(DrawCommandBuffer<T> * dcb);
    static DrawGraphManager* GetInstance();
    ~DrawGraphManager();
};

template<typename T>
inline void DrawGraphManager<T>::Init()
{
    PLOGD << "DrawGraphManager Init";
}

template<typename T>
inline void DrawGraphManager<T>::DeInit()
{
    PLOGD << "DrawGraphManager DeInit";
}

template<typename T>
inline void DrawGraphManager<T>::Update(DrawCommandBuffer<T> * dcb)
{
}

template<typename T>
inline DrawGraphManager<T>::~DrawGraphManager()
{
}

#pragma once
#include "DrawGraphNode.h"
#include "Graph.h"

template <typename T>
class ForwardGraph
{
private:
    Graph<DrawGraphNode> * drawGraph;

public:
    void Init(Graph<DrawGraphNode> * drawGraph);
    void DeInit();
    void Update(DrawCommandBuffer<T> * dcb);
    ~ForwardGraph();

    void AddNode(GraphNode<DrawGraphNode> * node);
};

template<typename T>
inline void ForwardGraph<T>::Init(Graph<DrawGraphNode>* drawGraph)
{
    this->drawGraph = drawGraph;
}

template<typename T>
inline void ForwardGraph<T>::DeInit()
{
}

template<typename T>
inline void ForwardGraph<T>::Update(DrawCommandBuffer<T>* dcb)
{
}

template<typename T>
inline ForwardGraph<T>::~ForwardGraph()
{
}

template<typename T>
inline void ForwardGraph<T>::AddNode(GraphNode<DrawGraphNode> * node)
{
    drawGraph->Push(node);
}

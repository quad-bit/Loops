#pragma once

#include <vector>
#include <assert.h>
#include "Stack.h"
#include <queue>
#include <deque>

template <class T>
class Graph;

template<class T>
class GraphNode
{
private:
    std::uint32_t nodeId;
    static std::uint32_t idCounter;

    std::uint32_t GetId() { return idCounter++; }

public:
    T * node;
    GraphNode(T * obj) :node(obj)
    {
        nodeId = GetId();
    }

    ~GraphNode()
    {

    }

    T * GetNodeData()
    {
        return node;
    }

    const std::uint32_t & GetNodeId() { return nodeId; }

    friend class Graph<T>;
};

template<typename T>
std::uint32_t GraphNode<T>::idCounter = 0;


template <class T>
class Graph
{
private:
    std::vector<GraphNode<T>*> vertices;
    int maxVertices, numOfExistingVerts;
    int matrixExtendingLimit = 20;// when the matrix gets exhausted relocate it with additional matrixExtendingLimit size
    char **adjMatrix;
    char ** localMat;
    char *visitedVertices;

public:
    Graph(int numVerts) : maxVertices(numVerts), adjMatrix(NULL)
    {
        assert(numVerts > 0);
        numOfExistingVerts = 0;
        vertices.reserve(maxVertices);

        visitedVertices = new char[maxVertices];
        assert(visitedVertices != NULL);

        memset(visitedVertices, 0, maxVertices);

        adjMatrix = new char*[maxVertices];
        assert(adjMatrix != NULL);

        localMat = new char*[maxVertices];
        assert(localMat != NULL);

        for (int i = 0; i < maxVertices; i++)
        {
            adjMatrix[i] = new char[maxVertices];
            localMat[i] = new char[maxVertices];

            assert(adjMatrix[i] != NULL);
            memset(adjMatrix[i], 0, maxVertices);
            memset(localMat[i], 0, maxVertices);
        }
    }

    ~Graph()
    {
        if (adjMatrix != NULL)
        {
            for (int i = 0; i < maxVertices; i++)
            {
                if (adjMatrix[i] != NULL)
                {
                    delete[] adjMatrix[i];
                    adjMatrix[i] = NULL;
                }

                if (localMat[i] != NULL)
                {
                    delete[] localMat[i];
                    localMat[i] = NULL;
                }
            }

            delete[] adjMatrix;
            adjMatrix = NULL;

            delete[] localMat;
            localMat = NULL;
        }


        if (visitedVertices != NULL)
        {
            delete[] visitedVertices;
            visitedVertices = NULL;
        }

        vertices.clear();
    }

    bool Push(T * node);
    bool Push(GraphNode<T> * graphNode);
    void AttachEdge(int index1, int index2);
    void AttachDirectedEdge(int index1, int index2);
    void AttachDirectedEdge(GraphNode<T> * srcNode, GraphNode<T> * destNode);
    int GetNextUnvisitedVertex(int index);
    bool DepthFirstSearch(int startIndex, int endIndex);
    bool BreadthFirstSearch(int startIndex, int endIndex);
    bool BreadthFirstTraversal(GraphNode<T> * srcNode, GraphNode<T> * destNode);
    bool DepthFirstTraversal(GraphNode<T> * srcNode, GraphNode<T> * destNode);
    void FindAllPaths(int startIndex, int endIndex);
    void ExtendMatrix();
    void CopyMat(char ** src, char ** dest);
    void PrintAdjMatrix();
};

template<class T>
inline bool Graph<T>::Push(T * node)
{
    if ((int)vertices.size() >= maxVertices)
        ExtendMatrix();

    GraphNode<T> * graphNode = new GraphNode<T>(node);

    vertices.push_back(graphNode);
    numOfExistingVerts++;
    return true;
}

template<class T>
inline bool Graph<T>::Push(GraphNode<T>* graphNode)
{
    if ((int)vertices.size() >= maxVertices)
        ExtendMatrix();

    vertices.push_back(graphNode);
    numOfExistingVerts++;

    return true;
}

template<class T>
inline void Graph<T>::AttachEdge(int index1, int index2)
{
    assert(adjMatrix != NULL);

    adjMatrix[index1][index2] = 1;
    adjMatrix[index2][index1] = 1;
}

template<class T>
inline void Graph<T>::AttachDirectedEdge(int index1, int index2)
{
    assert(adjMatrix != NULL);

    adjMatrix[index1][index2] = 1;
}

template<class T>
inline void Graph<T>::AttachDirectedEdge(GraphNode<T> * srcNode, GraphNode<T> * destNode)
{
    assert(adjMatrix != NULL);

    adjMatrix[srcNode->nodeId][destNode->nodeId] = 1;
}

template<class T>
inline int Graph<T>::GetNextUnvisitedVertex(int index)
{
    assert(visitedVertices != NULL);
    assert(adjMatrix != NULL);

    for (int i = 0; i < (int)vertices.size(); i++)
    {
        if (adjMatrix[index][i] == 1 && visitedVertices[i] == 0)
        {
            return i;
        }
    }

    return -1;
}

template<class T>
inline bool Graph<T>::DepthFirstSearch(int startIndex, int endIndex)
{
    assert(visitedVertices != NULL);
    assert(adjMatrix != NULL);

    visitedVertices[startIndex] = 1;

    std::cout << *vertices[startIndex]->GetNodeData(); // REMOVE>>!!!

    Stack<int> searchStack;
    int vert = 0;
    searchStack.Push(startIndex);

    while (!searchStack.Empty())
    {
        vert = GetNextUnvisitedVertex(searchStack.Top());

        if (vert == -1)
        {
            searchStack.Pop();
        }
        else
        {
            visitedVertices[vert] = 1;

            std::cout << *vertices[vert]->GetNodeData(); // REMOVE>>!!!
            searchStack.Push(vert);
        }

        if (vert == endIndex)
        {
            memset(visitedVertices, 0, maxVertices);
            return true;
        }
    }

    memset(visitedVertices, 0, maxVertices);
    return false;
}

template<class T>
inline bool Graph<T>::BreadthFirstSearch(int startIndex, int endIndex)
{

    assert(visitedVertices != NULL);
    assert(adjMatrix != NULL);

    visitedVertices[startIndex] = 1;

    std::cout << *vertices[startIndex]->GetNodeData(); // REMOVE>>!!!

    std::queue<int> searchQueue;
    int vert1 = 0, vert2 = 0;
    searchQueue.push(startIndex);

    while (searchQueue.empty() != true)
    {
        vert1 = searchQueue.front();
        searchQueue.pop();

        if (vert1 == endIndex)
        {
            memset(visitedVertices, 0, maxVertices);
            return true;
        }

        while ((vert2 = GetNextUnvisitedVertex(vert1)) != -1)
        {
            visitedVertices[vert2] = 1;
            std::cout << *vertices[vert2]->GetNodeData();
            searchQueue.push(vert2);
        }
    }

    memset(visitedVertices, 0, maxVertices);
    return false;
}

template<class T>
inline bool Graph<T>::BreadthFirstTraversal(GraphNode<T>* srcNode, GraphNode<T>* destNode)
{
    assert(visitedVertices != NULL);
    assert(adjMatrix != NULL);

    int startIndex = srcNode->nodeId; int endIndex = destNode->nodeId;

    visitedVertices[startIndex] = 1;

    //std::cout << *vertices[startIndex]->GetNodeData(); // REMOVE>>!!!
    srcNode->node->Execute();

    Stack<int> searchStack;
    int vert = 0;
    searchStack.Push(startIndex);

    while (!searchStack.Empty())
    {
        vert = GetNextUnvisitedVertex(searchStack.Top());

        if (vert == -1)
        {
            searchStack.Pop();
        }
        else
        {
            visitedVertices[vert] = 1;

            //std::cout << *vertices[vert]->GetNodeData(); // REMOVE>>!!!
            vertices[vert]->node->Execute();
            searchStack.Push(vert);
        }

        if (vert == endIndex)
        {
            memset(visitedVertices, 0, maxVertices);
            return true;
        }
    }

    memset(visitedVertices, 0, maxVertices);
    return false;
}

template<class T>
inline bool Graph<T>::DepthFirstTraversal(GraphNode<T>* srcNode, GraphNode<T>* destNode)
{
    assert(visitedVertices != NULL);
    assert(adjMatrix != NULL);

    int startIndex = srcNode->nodeId; int endIndex = destNode->nodeId;

    visitedVertices[startIndex] = 1;

//    std::cout << *vertices[startIndex]->GetNodeData(); // REMOVE>>!!!
    srcNode->node->Execute();

    Stack<int> searchStack;
    int vert = 0;
    searchStack.Push(startIndex);

    while (!searchStack.Empty())
    {
        vert = GetNextUnvisitedVertex(searchStack.Top());

        if (vert == -1)
        {
            searchStack.Pop();
        }
        else
        {
            visitedVertices[vert] = 1;

            //std::cout << *vertices[vert]->GetNodeData(); // REMOVE>>!!!
            vertices[vert]->node->Execute();
            searchStack.Push(vert);
        }

        if (vert == endIndex)
        {
            memset(visitedVertices, 0, maxVertices);
            return true;
        }
    }

    memset(visitedVertices, 0, maxVertices);
    return false;

}

template<class T>
inline void Graph<T>::FindAllPaths(int startIndex, int endIndex)
{
    assert(visitedVertices != NULL);
    assert(adjMatrix != NULL);
    assert(localMat != NULL);

    CopyMat(adjMatrix, localMat);

    visitedVertices[startIndex] = 1;

    std::deque<int> searchStack, visitedStack;
    int vert = 0;
    searchStack.push_back(startIndex);

    while (!searchStack.empty())
    {
        int top = searchStack.back();
        vert = GetNextUnvisitedVertex(top);

        if (vert == -1)
        {
            visitedStack.push_back(top);
            searchStack.pop_back();
        }
        else
        {
            if (visitedStack.size() != 0)
            {
                int indexToBeBroken = visitedStack[visitedStack.size() - 1];
                adjMatrix[top][indexToBeBroken] = 0;
                for (std::uint32_t i = 0; i < visitedStack.size(); i++)
                {
                    visitedVertices[visitedStack[i]] = 0;
                }

                visitedStack.clear();
            }

            visitedVertices[vert] = 1;

            searchStack.push_back(vert);
        }

        if (vert == endIndex)
        {
            //path found
            //std::cout << "\n";
            for (std::size_t i = 0; i < searchStack.size(); i++)
            {
                vertices[searchStack[i]]->GetNodeData()->Execute(); // REMOVE>>!!!
            }

            bool pathFound = true;
        }
    }

    memset(visitedVertices, 0, maxVertices);
    CopyMat(localMat, adjMatrix);
}

template<class T>
inline void Graph<T>::ExtendMatrix()
{
    assert(0);
}

template<class T>
inline void Graph<T>::CopyMat(char ** src, char ** dest)
{
    for (int i = 0; i < maxVertices; i++)
    {
        memcpy(dest[i], src[i], maxVertices);
    }

    /*for (std::uint32_t i = 0; i < maxVertices; i++)
    {
    for (std::uint32_t j = 0; j < maxVertices; j++)
    {
    std::cout << (int)src[i][j] ;
    }

    std::cout << "         " ;

    for (std::uint32_t j = 0; j < maxVertices; j++)
    {
    std::cout << (int)dest[i][j];
    }
    std::cout << std::endl;
    }   */
}

template<class T>
inline void Graph<T>::PrintAdjMatrix()
{
    for (std::uint32_t i = 0; i < (std::uint32_t)maxVertices; i++)
    {
        for (std::uint32_t j = 0; j < (std::uint32_t)maxVertices; j++)
        {
            std::cout << (int)adjMatrix[i][j] ;
        }
        std::cout << std::endl;
    }
}

#pragma once

#include <vector>
#include <assert.h>
#include "Stack.h"
//#include "Queue.h"
#include <queue>

template<class T>
class GraphNode
{
private:
	T node;

public:
	GraphNode(T obj) :node(obj)
	{

	}

	~GraphNode()
	{

	}

	T GetNode()
	{
		return node;
	}
};


template <class T>
class Graph
{
private:
	std::vector<GraphNode<T>> vertices;
	int maxVertices;
	char **adjMatrix;
	char *visitedVertices;

public:
	Graph(int numVerts) : maxVertices(numVerts), adjMatrix(NULL)
	{
		assert(numVerts > 0);

		vertices.reserve(maxVertices);

		adjMatrix = new char*[maxVertices];
		assert(adjMatrix != NULL);

		visitedVertices = new char[maxVertices];
		assert(visitedVertices != NULL);

		memset(visitedVertices, 0, maxVertices);

		for (int i = 0; i < maxVertices; i++)
		{
			adjMatrix[i] = new char[maxVertices];

			assert(adjMatrix[i] != NULL);
			memset(adjMatrix[i], 0, maxVertices);
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
			}

			delete[] adjMatrix;
			adjMatrix = NULL;
		}

		if (visitedVertices != NULL)
		{
			delete[] visitedVertices;
			visitedVertices = NULL;
		}

		vertices.clear();
	}

	bool Push(T node);
	void AttachEdge(int index1, int index2);
	void AttachDirectedEdge(int index1, int index2);
	int GetNextUnvisitedVertex(int index);
	bool DepthFirstSearch(int startIndex, int endIndex);
	bool BreadthFirstSearch(int startIndex, int endIndex);
};

template<class T>
inline bool Graph<T>::Push(T node)
{
	if ((int)vertices.size() >= maxVertices)
		return false;

	vertices.push_back(GraphNode<T>(node));
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

	std::cout << vertices[startIndex].GetNode(); // REMOVE>>!!!

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

			std::cout << vertices[vert].GetNode(); // REMOVE>>!!!
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

	std::cout << vertices[startIndex].GetNode(); // REMOVE>>!!!

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
			std::cout << vertices[vert2].GetNode();
			searchQueue.push(vert2);
		}
	}

	memset(visitedVertices, 0, maxVertices);
	return false;
}

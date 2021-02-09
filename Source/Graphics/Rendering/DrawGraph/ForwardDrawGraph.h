#pragma once
#include "DrawGraphNode.h"
#include "Graph.h"

enum ResourceSets
{
    CAMERA = 0,
    PIPELINE = 1,
    LIGHT = 2,
    SURFACE = 3,
    TRANSFORM = 4
};

struct PerPipelineNodeData
{
    GraphNode<DrawGraphNode> * pipelineNode;
    std::vector<uint32_t> setValues;
    std::map<DrawNodeTypes, std::vector<GraphNode<DrawGraphNode> *>> typeToNodeMap;
    std::map<uint32_t, std::vector<GraphNode<DrawGraphNode> *>> setToNodesMap;
};

template <typename T>
class ForwardGraph
{
private:
    Graph<DrawGraphNode> * drawGraph;
    std::vector<GraphNode<DrawGraphNode> *> pipelineNodeList;
    std::map<DrawNodeTypes, std::vector<GraphNode<DrawGraphNode> *>> typeToNodeMap;
    std::map<uint32_t, std::vector<GraphNode<DrawGraphNode> *>> setToNodesMap;
    //std::map<GraphNode<DrawGraphNode> *, std::vector<uint32_t>> pipelineNodeToExpecedSetMap;
    std::vector<PerPipelineNodeData> pipelineNodeDataList;

    void CreateEdgeForPipelineNode(GraphNode<DrawGraphNode> * pipelineNode, PerPipelineNodeData * data);
    void CreateEdge(GraphNode<DrawGraphNode> * newNode);
    PerPipelineNodeData * CreatePipelineData(GraphNode<DrawGraphNode> * pipelineNode);
    bool CheckIfNodeBelongsToPipeline(GraphNode<DrawGraphNode> * pipelineNode, 
        GraphNode<DrawGraphNode> * node, std::vector<uint32_t> * pipelineNodeSetList);
    void AddNodeToPipelineData(PerPipelineNodeData * data, GraphNode<DrawGraphNode> * node);
    void CreateEdgesWithinPipeline(PerPipelineNodeData * data);

    bool CheckForCommonMeshId(GraphNode<DrawGraphNode> * src, GraphNode<DrawGraphNode> * dest);

public:
    void Init(Graph<DrawGraphNode> * drawGraph);
    void DeInit();
    void Update(DrawCommandBuffer<T> * dcb);
    ~ForwardGraph();

    void AddNode(GraphNode<DrawGraphNode> * node);
};

template<typename T>
inline void ForwardGraph<T>::CreateEdgeForPipelineNode(GraphNode<DrawGraphNode> * pipelineNode, PerPipelineNodeData * data)
{
    // for now camera is the first node in the graph as it is getting shared by each and every object.
    // Camera = 0
    // pipeline = 1
    // ...

    // Incoming node to pipeline node
    {
        // iterate through all the non pipeline nodes with set = 0
        std::vector<GraphNode<DrawGraphNode>*> * nodeList = &setToNodesMap[ResourceSets::CAMERA];

        // find match for mesh id among the nodes
        for each(auto obj in *nodeList)
        {
            bool isNodeInPipeline = CheckIfNodeBelongsToPipeline(pipelineNode, obj, &data->setValues);

            if (isNodeInPipeline)
            {
                // if yes, add it to pipeline node data 
                AddNodeToPipelineData(data, obj);
                
                // if yes create an incoming edge.
                drawGraph->AttachDirectedEdge(obj, pipelineNode);
            }
        }
    }// Incoming node to pipeline node

    // out going edges from pipeline node
    {
        // iterate through all the non pipeline nodes with set present in pipeline node's setlist
        uint32_t size = (uint32_t)data->setValues.size();
        for(uint32_t i = 0; i < size; i++)
        {
            auto setValue = data->setValues[i];
            if (setValue != ResourceSets::CAMERA && setValue != ResourceSets::TRANSFORM)// camera already connected
            {
                std::vector<GraphNode<DrawGraphNode>*> * nodeList = &setToNodesMap[setValue];
                for each(auto obj in *nodeList)
                {
                    bool isNodeInPipeline = CheckIfNodeBelongsToPipeline(pipelineNode, obj, &data->setValues);
                    if (isNodeInPipeline)
                    {
                        // if yes, add it to pipeline node data 
                        AddNodeToPipelineData(data, obj);

                        // if yes create an outgoing edge from pipeline node.
                        if(i == 1)
                            drawGraph->AttachDirectedEdge(pipelineNode, obj);
                    }
                }
            }
            // outgoing edge, special case
            // pipeline = 1
            // set 4 = 2 (in list) , TRANFORM
            else if (setValue == ResourceSets::TRANSFORM && i == 1)
            {
                // connect to the mesh node
                std::vector<GraphNode<DrawGraphNode>*> * nodeList = &typeToNodeMap[DrawNodeTypes::MESH];
                for each(auto obj in *nodeList)
                {
                    bool isNodeInPipeline = CheckIfNodeBelongsToPipeline(pipelineNode, obj, &data->setValues);
                    if (isNodeInPipeline)
                    {
                        // if yes, add it to pipeline node data 
                        AddNodeToPipelineData(data, obj);

                        // if yes create an outgoing edge from pipeline node.
                        drawGraph->AttachDirectedEdge(pipelineNode, obj);
                    }
                }

                std::vector<GraphNode<DrawGraphNode>*> * drawingNodeList = &typeToNodeMap[DrawNodeTypes::DRAWING];
                for each(auto obj in *drawingNodeList)
                {
                    bool isNodeInPipeline = CheckIfNodeBelongsToPipeline(pipelineNode, obj, &data->setValues);
                    if (isNodeInPipeline)
                    {
                        // if yes, add it to pipeline node data 
                        AddNodeToPipelineData(data, obj);
                    }
                }
            }
        }

    }// out going edges from pipeline node

    // connect the remaining nodes.
    {
        CreateEdgesWithinPipeline(data);
    }
}

template<typename T>
inline void ForwardGraph<T>::CreateEdge(GraphNode<DrawGraphNode>* newNode)
{
    /*
    //create edges from all pipeline nodes
    for each(auto obj in pipelineNodeDataList)
    {
        // check if the setwrapper of the newNode is present in pipeline's setwrapper list
        std::vector<uint32_t>::iterator it;
        it = std::find_if(obj.setValues.begin(), obj.setValues.end(), [&](uint32_t e) {
            return e == newNode->node->setLevel; });

        if (it != obj.setValues.end())
        {
            // if the set is found in the expected list
            // check for mesh list
            bool nodeBelongsToPipeline = false;

            std::vector<uint32_t> * newNodeMeshList = &newNode->node->meshList;
            std::vector<uint32_t> * pipelineNodeMeshList = &obj.pipelineNode->node->meshList;

            for each (uint32_t var in *pipelineNodeMeshList)
            {
                std::vector<uint32_t>::iterator itt;
                itt = std::find_if(newNodeMeshList->begin(), newNodeMeshList->end(), [&](uint32_t e) {
                    return e == newNode->node->setLevel; });
                if (itt != newNodeMeshList->end())
                {
                    nodeBelongsToPipeline = true;
                    break;
                }
            }

            // if even a single mesh id is present, an edge will exist
            // outgoing and incoming edge ??
        }

        // if yes find the appropriate level for the connections based on the set number
    }
    */
}

template<typename T>
inline PerPipelineNodeData * ForwardGraph<T>::CreatePipelineData(GraphNode<DrawGraphNode>* pipelineNode)
{
    std::vector<SetWrapper *> * setWrapperList = &pipelineNode->node->setWrapperList;
    uint32_t numSetsWrappers = (uint32_t)setWrapperList->size();
    
    PerPipelineNodeData data = {};
    data.pipelineNode = pipelineNode;

    for (uint32_t i = 0; i < numSetsWrappers; i++)
    {
        data.setValues.push_back(setWrapperList->at(i)->setValue);
    }

    pipelineNodeDataList.push_back(data);
    return &pipelineNodeDataList.back();
}

template<typename T>
inline bool ForwardGraph<T>::CheckIfNodeBelongsToPipeline(GraphNode<DrawGraphNode>* pipelineNode, GraphNode<DrawGraphNode>* node, std::vector<uint32_t> * pipelineNodeSetList)
{
    bool nodeBelongsToPipeline = false;
    
    // mesh nodes dont have setvalues
    if (node->node->drawNodeType != DrawNodeTypes::MESH && 
        node->node->drawNodeType != DrawNodeTypes::DRAWING)
    {
        // check if the setvalue of the node is present in setlist of the pipeline node
        std::vector<uint32_t>::iterator it;
        it = std::find_if(pipelineNodeSetList->begin(), pipelineNodeSetList->end(), [&](uint32_t e) {
            return e == node->node->setLevel; });

        if (it == pipelineNodeSetList->end())
        {
            return false;
        }
    }

    // now check for mesh list
    std::vector<uint32_t> * nodeMeshList = &node->node->meshList;
    std::vector<uint32_t> * pipelineNodeMeshList = &pipelineNode->node->meshList;

    for each (uint32_t var in *pipelineNodeMeshList)
    {
        std::vector<uint32_t>::iterator itt;
        itt = std::find_if(nodeMeshList->begin(), nodeMeshList->end(), [&](uint32_t e) {
            return e == var; });
        if (itt != nodeMeshList->end())
        {
            nodeBelongsToPipeline = true;
            break;
        }
    }

    return nodeBelongsToPipeline;
}

template<typename T>
inline void ForwardGraph<T>::AddNodeToPipelineData(PerPipelineNodeData * data, GraphNode<DrawGraphNode> * node)
{
    {
        auto it = data->typeToNodeMap.find(node->node->drawNodeType);
        if (it == data->typeToNodeMap.end())
        {
            // not found
            data->typeToNodeMap.insert(std::pair<DrawNodeTypes, std::vector<GraphNode<DrawGraphNode> *>>({
                node->node->drawNodeType, std::vector<GraphNode<DrawGraphNode> *>{ node } }));

        }
        else
        {
            // found
            it->second.push_back(node);
        }
    }

    if(node->node->drawNodeType != DrawNodeTypes::MESH && 
        node->node->drawNodeType != DrawNodeTypes::DRAWING)
    {
        auto it = data->setToNodesMap.find(node->node->setLevel);
        if (it == data->setToNodesMap.end())
        {
            // not found
            data->setToNodesMap.insert(std::pair<uint32_t, std::vector<GraphNode<DrawGraphNode> *>>({
                node->node->setLevel, std::vector<GraphNode<DrawGraphNode> *>{ node } }));
        }
        else
        {
            it->second.push_back(node);
        }
    }
}

template<typename T>
inline void ForwardGraph<T>::CreateEdgesWithinPipeline(PerPipelineNodeData * data)
{
    for (uint32_t i = 0; i < (uint32_t)data->setValues.size() - 1; i++)
    {
        uint32_t higherSetLevel = data->setValues[i];
        uint32_t lowerSetLevel = data->setValues[i+1];
        
        if (higherSetLevel == ResourceSets::CAMERA || (lowerSetLevel == ResourceSets::TRANSFORM && i == 0))
        {
            // already connected to pipeline node 
            return;
        }

        ASSERT_MSG(0, "Case yet to be handled");

        if (lowerSetLevel == ResourceSets::TRANSFORM)
        {
            // connect to the mesh node
            std::vector<GraphNode<DrawGraphNode>*> * highNodeList = &data->setToNodesMap[higherSetLevel];
            std::vector<GraphNode<DrawGraphNode>*> * lowNodeList = &data->typeToNodeMap[DrawNodeTypes::MESH];
            
            for each(auto highNode in *highNodeList)
            {
                for each(auto lowNode in *lowNodeList)
                {
                    bool shouldBeConnected = CheckForCommonMeshId(highNode, lowNode);
                    if (shouldBeConnected)
                        drawGraph->AttachDirectedEdge(highNode, lowNode);
                }
            }
        }
        else
        {
            std::vector<GraphNode<DrawGraphNode>*> * highNodeList = &data->setToNodesMap[higherSetLevel];
            std::vector<GraphNode<DrawGraphNode>*> * lowNodeList = &data->setToNodesMap[lowerSetLevel];
            // for i == 1 only outgoing edge is required as incoming edges is from pipeline node, already connected.

            // we dont need to create both incoming and outgoing for all nodes, only outgoing should do, i guess.
            {
                for each(auto highNode in *highNodeList)
                {
                    for each(auto lowNode in *lowNodeList)
                    {
                        bool shouldBeConnected = CheckForCommonMeshId(highNode, lowNode);
                        if (shouldBeConnected)
                            drawGraph->AttachDirectedEdge(highNode, lowNode);
                    }
                }
            }
        }
    }
}

template<typename T>
inline bool ForwardGraph<T>::CheckForCommonMeshId(GraphNode<DrawGraphNode>* src, GraphNode<DrawGraphNode>* dest)
{
    bool commonIdFound = false;
    std::vector<uint32_t>* srcMeshList = &src->node->meshList;
    std::vector<uint32_t>* destMeshList = &dest->node->meshList;

    for each(auto srcId in *srcMeshList)
    {
        std::vector<uint32_t>::iterator it;
        it = std::find_if(destMeshList->begin(), destMeshList->end(), [&](uint32_t e) {
            return e == srcId; });

        if (it != destMeshList->end())
        {
            return true;
        }
    }

    return false;
}

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
    std::vector<GraphNode<DrawGraphNode> *>* cameraNodeList = &typeToNodeMap[DrawNodeTypes::CAMERA];
    std::vector<GraphNode<DrawGraphNode> *>* drawingNodeList = &typeToNodeMap[DrawNodeTypes::DRAWING];
    
    /*
    for each(auto cam in *cameraNodeList)
    {
        for each(auto draw in *drawingNodeList)
        {
            drawGraph->FindAllPaths(cam->GetNodeId(), draw->GetNodeId());
        }
    }
    */
    DrawGraphUtil::setOffset = 0;
    DrawGraphUtil::descriptorIdList.clear();
    for each(auto cam in *cameraNodeList)
    {
        drawGraph->DepthFirstTraversal(cam->GetNodeId(), drawingNodeList->back()->GetNodeId());
    }
}

template<typename T>
inline ForwardGraph<T>::~ForwardGraph()
{
}

template<typename T>
inline void ForwardGraph<T>::AddNode(GraphNode<DrawGraphNode> * node)
{
    drawGraph->Push(node);

    DrawNodeTypes nodeType = node->node->drawNodeType;
    if (node->node->drawNodeType == DrawNodeTypes::PIPELINE)
    {
        //pipelineNodeList.push_back(node);
        PerPipelineNodeData * pipelineData = CreatePipelineData(node);
        CreateEdgeForPipelineNode(node, pipelineData);
    }
    else
    {
        //if entry present then insert a new node
        auto it = typeToNodeMap.find(nodeType);
        if (it != typeToNodeMap.end())
        {
            it->second.push_back(node);
        }
        else
        {
            typeToNodeMap.insert(std::pair<DrawNodeTypes, std::vector<GraphNode<DrawGraphNode> *>>({
                nodeType, std::vector<GraphNode<DrawGraphNode> *>{ node } }));
        }

        if (node->node->drawNodeType != DrawNodeTypes::MESH && 
            node->node->drawNodeType != DrawNodeTypes::DRAWING)
        {
            uint32_t setLevel = node->node->setLevel;
            auto itt = setToNodesMap.find(setLevel);
            if (itt != setToNodesMap.end())
            {
                itt->second.push_back(node);
            }
            else
            {
                setToNodesMap.insert(std::pair<uint32_t, std::vector<GraphNode<DrawGraphNode> *>>({
                    setLevel, std::vector<GraphNode<DrawGraphNode> *>{ node } }));
            }
        }
        
        // Check with existing pipeline nodes if this node belongs to any of them
        for each(auto data in pipelineNodeDataList)
        {
            ASSERT_MSG(0, "Case yet to be handled");

            bool isNodeInPipeline = CheckIfNodeBelongsToPipeline(data.pipelineNode, node, &data.setValues);
            // if yes add to the node list 
            AddNodeToPipelineData(&data, node);
            // create edge with existing
        }
    }
}

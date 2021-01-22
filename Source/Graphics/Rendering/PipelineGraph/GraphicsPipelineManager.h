#pragma once
#include "RenderingWrapper.h"
#include "HashManager.h"
#include "PipelineStateWrappers.h"
#include <Graph.h>
#include <vector>

template <typename T>
class GraphicsPipelineManager
{
private:
    GraphicsPipelineManager(){}
    GraphicsPipelineManager(GraphicsPipelineManager const &) {}
    GraphicsPipelineManager const & operator= (GraphicsPipelineManager const &) {}

    static GraphicsPipelineManager* instance;
    
    T * apiInterface;

    std::vector<VertexInputWrapper *> vertexInputStateWrapperList;
    std::vector<InputAssemblyWrapper *> vertexInputAssemblyWrapperList;
    std::vector<ShaderStateWrapper *> shaderStateWrapperList;
    std::vector<ShaderResourceStateWrapper *> shaderResourceStateWrapperList;
    std::vector<TessellationStateWrapper *> tessellationStateWrapperList;
    std::vector<ViewPortStateWrapper *> viewportStateWrapperList;
    std::vector<RasterizationStateWrapper *> rasterizationWrapperList;
    std::vector<MultiSampleStateWrapper *> multisampleStateWrapperList;
    std::vector<DepthStencilStateWrapper *> depthStencilStateWrapperList;
    std::vector<ColorBlendStateWrapper *> colorBlendStateWrapperList;
    std::vector<DynamicStateWrapper *> dynamicStateWrapperList;

    std::map<uint32_t, std::vector<GraphNode<StateWrapperBase>*>> meshToGraphNodeMap;
    void InsertToMeshList(const uint32_t & meshId, PipelineStates state, GraphNode<StateWrapperBase>* node);
    
    uint32_t defaultAssemblyListIndex;
    void CreateVertexAssemblyDefault();
    void CreateTessellationDefault();
    void CreateViewportDefault();
    void CreateRasterizationDefault();
    void CreateMultiSampleDefault();
    void CreateDepthStencilDefault();
    void CreateColorblendDefault();
    void CreateDynamicStateDefault();

    void DestroyVertexInputState();
    void DestroyShaderState();
    void DestroyShaderResourceState();
    void DestroyVertexAssembly();
    void DestroyTessellation();
    void DestroyViewport();
    void DestroyRasterization();
    void DestroyMultiSample();
    void DestroyDepthStencil();
    void DestroyColorblend();
    void DestroyDynamicState();


    int maxGraphNodes = 30;
    Graph<StateWrapperBase> * pipelineGraph;// TODO: add the graph construction without predefined length
    std::vector<PipelineStates> pipelineStateList;
    std::map < PipelineStates, std::vector<GraphNode<StateWrapperBase> *>> stateToNodeMap;
    GraphNode<StateWrapperBase> * CreateGraphNode(StateWrapperBase * stateWrapper);

    void CreateGraphEdges();
    void CreateGraphEdges(const uint32_t & meshId, PipelineStates src, PipelineStates dest);
    GraphNode<StateWrapperBase> * GetNode(PipelineStates state, StateWrapperBase * stateObj);

public:
    void Init(T * apiInterface);
    void DeInit();
    void Update();
    static GraphicsPipelineManager* GetInstance();
    ~GraphicsPipelineManager();

    void CreateVertexInputState(const uint32_t & meshId, VertexInputState * inputStateInfo );
    void CreateVertexAssemblyState(const uint32_t & meshId, InputAssemblyState * assembly);
    void AssignDefaultState(const uint32_t & meshId, const PipelineStates & state);
    void CreatShaderPipelineState(const uint32_t & meshId, Shader * shaders, const uint32_t & shaderCount);
    std::vector<SetWrapper*> CreatResourceLayoutState(const uint32_t & meshId, Shader * shaders, const uint32_t & shaderCount);
};

template<typename T>
inline void GraphicsPipelineManager<T>::InsertToMeshList(const uint32_t & meshId, PipelineStates state, GraphNode<StateWrapperBase>* node)
{
    auto it = meshToGraphNodeMap.find(meshId);

    if (it == meshToGraphNodeMap.end())
        ASSERT_MSG(0, "mesh Id not found");

    it->second[(uint32_t)state] = node;
}

template<typename T>
inline void GraphicsPipelineManager<T>::CreateVertexAssemblyDefault()
{
    defaultAssemblyListIndex = (uint32_t)vertexInputAssemblyWrapperList.size();
    InputAssemblyWrapper * wrapper = new InputAssemblyWrapper;
    wrapper->assemblyState = new InputAssemblyState;
    wrapper->assemblyState->primitiveType = new PrimtiveType{ PrimtiveType::TOPOLOGY_TRIANGLE_LIST };
    wrapper->assemblyState->isPrimtiveRestartEnabled = false;

    HashManager::GetInstance()->FindInputAssemblyStateHash(wrapper->assemblyState, wrapper->GetId());

    vertexInputAssemblyWrapperList.push_back(wrapper);

    CreateGraphNode(wrapper);
    apiInterface->CreateInputAssemblyState(wrapper);
}

template<typename T>
inline void GraphicsPipelineManager<T>::CreateTessellationDefault()
{
    TessellationStateWrapper * wrapper = new TessellationStateWrapper;
    wrapper->tessellationState = new TessellationState;
    wrapper->tessellationState->patchControlPoints = 0;
    tessellationStateWrapperList.push_back(wrapper);

    // Find hash so that the hash manager stores the id of this state.
    int test = HashManager::GetInstance()->FindTessellationStateHash(wrapper->tessellationState, wrapper->GetId());
    ASSERT_MSG(test == -1, "Issue with hashing, should return -1");

}

template<typename T>
inline void GraphicsPipelineManager<T>::CreateViewportDefault()
{
    ViewPortStateWrapper * wrapper = new ViewPortStateWrapper;
    ViewportState * viewport = new ViewportState;
    viewport->pScissors = nullptr; // as viewport and scissor are dynamic states
    viewport->pViewports = nullptr;
    viewport->scissorCount = 1; // probably because its enabled as a dynamic state
    viewport->viewportCount = 1; // probably because its enabled as a dynamic state
    wrapper->viewportState = viewport;
    viewportStateWrapperList.push_back(wrapper);
    
    // Find hash so that the hash manager stores the id of this state.
    int test = HashManager::GetInstance()->FindViewportStateHash(viewport, wrapper->GetId());
    ASSERT_MSG(test == -1, "Issue with hashing, should return -1");
}

template<typename T>
inline void GraphicsPipelineManager<T>::CreateRasterizationDefault()
{
    RasterizationStateWrapper * wrapper = new RasterizationStateWrapper;
    RasterizationState * raster = new RasterizationState;
    raster->cullMode = CullMode::CULL_MODE_BACK_BIT;
    raster->depthBiasClamp = 0;
    raster->depthBiasConstantFactor = 0;
    raster->depthBiasEnable = false;
    raster->depthBiasSlopeFactor = 0;
    raster->depthClampEnable = false;
    raster->frontFace = FrontFace::FRONT_FACE_COUNTER_CLOCKWISE;
    raster->lineWidth = 1.0f;
    raster->polygonMode = PolygonMode::POLYGON_MODE_FILL;
    raster->rasterizerDiscardEnable = false;
    wrapper->rasterState = raster;
    rasterizationWrapperList.push_back(wrapper);

    // Find hash so that the hash manager stores the id of this state.
    int test = HashManager::GetInstance()->FindRasterizationHash(raster, wrapper->GetId());
    ASSERT_MSG(test == -1, "Issue with hashing, should return -1");
}

template<typename T>
inline void GraphicsPipelineManager<T>::CreateMultiSampleDefault()
{
    MultiSampleStateWrapper * wrapper = new MultiSampleStateWrapper;
    MultiSampleState * multisample = new MultiSampleState;
    multisample->alphaToCoverageEnable = false;
    multisample->alphaToOneEnable = false;
    multisample->sampleShadingEnable = false;
    multisample->sampleCount = Samples::SAMPLE_COUNT_1_BIT;
    multisample->sampleShadingEnable = false;
    
    wrapper->multiSampleState = multisample;
    
    multisampleStateWrapperList.push_back(wrapper);
    
    // Find hash so that the hash manager stores the id of this state.
    int test = HashManager::GetInstance()->FindMultiSampleHash(multisample, wrapper->GetId());
    ASSERT_MSG(test == -1, "Issue with hashing, should return -1");
}

template<typename T>
inline void GraphicsPipelineManager<T>::CreateDepthStencilDefault()
{
    DepthStencilStateWrapper * wrapper = new DepthStencilStateWrapper;
    DepthStencilState * depthStencil = new DepthStencilState;
    depthStencil->back.passOp = StencilOp::STENCIL_OP_KEEP;
    depthStencil->back.failOp = StencilOp::STENCIL_OP_KEEP;
    depthStencil->back.compareMask = 0;
    depthStencil->back.compareOp = CompareOp::COMPARE_OP_ALWAYS;
    depthStencil->back.reference = 0;
    depthStencil->back.writeMask = 0;
    depthStencil->back.depthFailOp = StencilOp::STENCIL_OP_KEEP;

    depthStencil->depthBoundsTestEnable = false;
    depthStencil->depthCompareOp = CompareOp::COMPARE_OP_LESS_OR_EQUAL;
    depthStencil->depthTestEnable = true;
    depthStencil->depthWriteEnable = true;
    depthStencil->front = depthStencil->back;
    depthStencil->maxDepthBounds = 0;
    depthStencil->maxDepthBounds = 0;
    depthStencil->stencilTestEnable = false;

    wrapper->depthState = depthStencil;

    depthStencilStateWrapperList.push_back(wrapper);

    // Find hash so that the hash manager stores the id of this state.
    int test = HashManager::GetInstance()->FindDepthStencilHash(depthStencil, wrapper->GetId());
    ASSERT_MSG(test == -1, "Issue with hashing, should return -1");
}

template<typename T>
inline void GraphicsPipelineManager<T>::CreateColorblendDefault()
{
    ColorBlendStateWrapper * wrapper = new ColorBlendStateWrapper;

    PipelineColorBlendAttachmentState * attachment = new PipelineColorBlendAttachmentState[1];
    attachment[0].alphaBlendOp = BlendOp::BLEND_OP_ADD;
    attachment[0].blendEnable = false;
    attachment[0].colorBlendOp = BlendOp::BLEND_OP_ADD;
    attachment[0].colorWriteMask = ColorComponentFlagBits::COLOR_COMPONENT_TRUE_BIT;
    attachment[0].dstAlphaBlendFactor = BlendFactor::BLEND_FACTOR_ZERO;
    attachment[0].dstColorBlendFactor = BlendFactor::BLEND_FACTOR_ZERO;
    attachment[0].srcAlphaBlendFactor = BlendFactor::BLEND_FACTOR_ZERO;
    attachment[0].srcColorBlendFactor = BlendFactor::BLEND_FACTOR_ZERO;

    ColorBlendState * colorBlend = new ColorBlendState;
    colorBlend->attachmentCount = 1;
    colorBlend->blendConstants[0] = 1.0f;
    colorBlend->blendConstants[1] = 1.0f;
    colorBlend->blendConstants[2] = 1.0f;
    colorBlend->blendConstants[3] = 1.0f;
    colorBlend->logicOp = LogicOp::LOGIC_OP_NO_OP;
    colorBlend->logicOpEnable = false;
    colorBlend->pAttachments = attachment;

    wrapper->colorBlendState = colorBlend;
    colorBlendStateWrapperList.push_back(wrapper);

    // Find hash so that the hash manager stores the id of this state.
    int test = HashManager::GetInstance()->FindColorBlendHash(colorBlend, wrapper->GetId());
    ASSERT_MSG(test == -1, "Issue with hashing, should return -1");
}

template<typename T>
inline void GraphicsPipelineManager<T>::CreateDynamicStateDefault()
{
    DynamicStateWrapper * wrapper = new DynamicStateWrapper;
    DynamicStateList * dynamic = new DynamicStateList;
    DynamicState * states = new DynamicState[2];
    states[0] = DynamicState::DYNAMIC_STATE_VIEWPORT;
    states[1] = DynamicState::DYNAMIC_STATE_SCISSOR;
    dynamic->dynamicStateCount = 2;
    dynamic->pDynamicStates = states;
    wrapper->dynamicStates = dynamic;

    dynamicStateWrapperList.push_back(wrapper);
    // Find hash so that the hash manager stores the id of this state.

    int test = HashManager::GetInstance()->FindDynamicStateHash(dynamic, wrapper->GetId());
    ASSERT_MSG(test == -1, "Issue with hashing, should return -1");
}

template<typename T>
inline void GraphicsPipelineManager<T>::DestroyVertexInputState()
{
    for each(VertexInputWrapper * wrapper in vertexInputStateWrapperList)
    {
        if(wrapper->inputState->attribCount > 0)
            delete[] wrapper->inputState->attribInfo;
        
        if (wrapper->inputState->bindingCount > 0)
            delete[] wrapper->inputState->bindingInfo;

        delete wrapper->inputState;
        wrapper->meshIdList.clear();
        delete wrapper;
    }

    vertexInputStateWrapperList.clear();
    vertexInputStateWrapperList.resize(0);
}

template<typename T>
inline void GraphicsPipelineManager<T>::DestroyShaderState()
{
    for each(ShaderStateWrapper * wrapper in shaderStateWrapperList)
    {
        //if (wrapper->shaderCount > 0)
            //delete[] wrapper->shader;  // getting deleted in shader factory

        wrapper->meshIdList.clear();
        delete wrapper;
    }

    shaderStateWrapperList.clear();
}

template<typename T>
inline void GraphicsPipelineManager<T>::DestroyShaderResourceState()
{
    for each(ShaderResourceStateWrapper * wrapper in shaderResourceStateWrapperList)
    {
        delete wrapper;
    }

    shaderResourceStateWrapperList.clear();
}

template<typename T>
inline void GraphicsPipelineManager<T>::DestroyVertexAssembly()
{
    for each(InputAssemblyWrapper * wrapper in vertexInputAssemblyWrapperList)
    {
        delete wrapper->assemblyState->primitiveType;
        delete wrapper->assemblyState;
        wrapper->meshIdList.clear();
        delete wrapper;
    }

    vertexInputAssemblyWrapperList.clear();
}

template<typename T>
inline void GraphicsPipelineManager<T>::DestroyTessellation()
{
    for each(TessellationStateWrapper * wrapper in tessellationStateWrapperList)
    {
        delete wrapper->tessellationState;
        delete wrapper;
    }
    tessellationStateWrapperList.clear();
}

template<typename T>
inline void GraphicsPipelineManager<T>::DestroyViewport()
{
    for each(ViewPortStateWrapper * wrapper in viewportStateWrapperList)
    {
        if (wrapper->viewportState->scissorCount > 0)
            delete wrapper->viewportState->pScissors;

        if (wrapper->viewportState->viewportCount > 0)
            delete wrapper->viewportState->pViewports;

        delete wrapper->viewportState;
        delete wrapper;
    }

    viewportStateWrapperList.clear();
}

template<typename T>
inline void GraphicsPipelineManager<T>::DestroyRasterization()
{
    for each(RasterizationStateWrapper * wrapper in rasterizationWrapperList)
    {
        delete wrapper->rasterState;
        delete wrapper;
    }

    rasterizationWrapperList.clear();
}

template<typename T>
inline void GraphicsPipelineManager<T>::DestroyMultiSample()
{
    for each(MultiSampleStateWrapper* wrapper in multisampleStateWrapperList)
    {
        delete wrapper->multiSampleState;
        delete wrapper;
    }

    multisampleStateWrapperList.clear();
}

template<typename T>
inline void GraphicsPipelineManager<T>::DestroyDepthStencil()
{
    for each(DepthStencilStateWrapper * wrapper in depthStencilStateWrapperList)
    {
        delete wrapper->depthState;
        delete wrapper;
    }

    depthStencilStateWrapperList.clear();
}

template<typename T>
inline void GraphicsPipelineManager<T>::DestroyColorblend()
{
    for each(ColorBlendStateWrapper * wrapper in colorBlendStateWrapperList)
    {
        if (wrapper->colorBlendState->attachmentCount > 0)
            delete[] wrapper->colorBlendState->pAttachments;

        delete wrapper->colorBlendState;
        delete wrapper;
    }

    colorBlendStateWrapperList.clear();
}

template<typename T>
inline void GraphicsPipelineManager<T>::DestroyDynamicState()
{
    for each(DynamicStateWrapper * wrapper in dynamicStateWrapperList)
    {
        if (wrapper->dynamicStates->dynamicStateCount > 0)
            delete[] wrapper->dynamicStates->pDynamicStates;
        delete wrapper->dynamicStates;
        delete wrapper;
    }
    dynamicStateWrapperList.clear();
}

template<typename T>
inline GraphNode<StateWrapperBase> * GraphicsPipelineManager<T>::CreateGraphNode(StateWrapperBase * stateWrapper)
{
    GraphNode<StateWrapperBase> * node = new GraphNode<StateWrapperBase>(stateWrapper);
    pipelineGraph->Push(node);

    //if entry present then insert a new node
    auto it = stateToNodeMap.find(stateWrapper->state);

    if (it != stateToNodeMap.end())
    {
        it->second.push_back(node);
    }
    else
    {
        stateToNodeMap.insert(std::pair< PipelineStates, std::vector<GraphNode<StateWrapperBase> *>>({
            stateWrapper->state, std::vector<GraphNode<StateWrapperBase> *>{node } }));
    }

    return node;
}

//really heavy function, use the other one
template<typename T>
inline void GraphicsPipelineManager<T>::CreateGraphEdges()
{
    // iterate through state nodes (state0, state1 ...)
    PipelineStates src, dest;
    uint32_t stateCounter = 0;

    do
    {
        src = pipelineStateList[stateCounter];
        dest = pipelineStateList[stateCounter + 1];

        std::vector<GraphNode<StateWrapperBase>*> srcGraphNodeList = stateToNodeMap[src];
        std::vector<GraphNode<StateWrapperBase>*> destGraphNodeList = stateToNodeMap[dest];

        for each(GraphNode<StateWrapperBase> * srcNode in srcGraphNodeList)
        {
            for each(GraphNode<StateWrapperBase> * destNode in destGraphNodeList)
            {
                uint32_t size = (uint32_t)srcNode->node->meshIdList.size();
                std::vector<uint32_t> srchMeshIdList = srcNode->node->meshIdList;
                std::vector<uint32_t>::iterator it;// = destNode->node->meshIdList.begin();

                for (uint32_t i = 0; i < size; i++)
                {
                    it = std::find_if(destNode->node->meshIdList.begin(), destNode->node->meshIdList.end(), [&](uint32_t id) { return id == srchMeshIdList[i]; });
                    //ASSERT_MSG(it != destNode->node->meshIdList.end(), "Queue family index not found");

                    if (it != destNode->node->meshIdList.end())
                    {
                        pipelineGraph->AttachDirectedEdge(srcNode, destNode);
                        break;
                    }
                }
            }
        }

        stateCounter++;

    } while (dest != pipelineStateList.back());

}

template<typename T>
inline void GraphicsPipelineManager<T>::CreateGraphEdges(const uint32_t & meshId, PipelineStates src, PipelineStates dest)
{
    auto it = meshToGraphNodeMap.find(meshId);
    if (it == meshToGraphNodeMap.end())
    {
        ASSERT_MSG(0, "id not found");
    }

    GraphNode<StateWrapperBase>* srcNode = it->second[(uint32_t)src];
    GraphNode<StateWrapperBase>* destNode = it->second[(uint32_t)dest];

    pipelineGraph->AttachDirectedEdge(srcNode, destNode);
}

template<typename T>
inline GraphNode<StateWrapperBase>* GraphicsPipelineManager<T>::GetNode(PipelineStates state, StateWrapperBase * stateObj)
{
    auto itt = stateToNodeMap.find(state);
    if (itt == stateToNodeMap.end())
    {
        ASSERT_MSG(0, "state not found");
    }

    std::vector<GraphNode<StateWrapperBase>*> nodeList = itt->second;

    std::vector<GraphNode<StateWrapperBase>*>::iterator it;
    it = std::find_if(nodeList.begin(), nodeList.end(), [&](GraphNode<StateWrapperBase>* e) {
        return (e->GetNode()->state == state) && (e->GetNode()->GetId() == stateObj->GetId()); });

    ASSERT_MSG(it != nodeList.end(), "Node not found");
    return (*it);
}

template<typename T>
inline void GraphicsPipelineManager<T>::Init(T * apiInterface)
{
    this->apiInterface = apiInterface;

    pipelineGraph = new Graph<StateWrapperBase>(maxGraphNodes);

    pipelineStateList.push_back(PipelineStates::VertexInputState);
    pipelineStateList.push_back(PipelineStates::InputAssemblyState);
    pipelineStateList.push_back(PipelineStates::ShaderStage);
    pipelineStateList.push_back(PipelineStates::TessellationState);
    pipelineStateList.push_back(PipelineStates::ViewportState);
    pipelineStateList.push_back(PipelineStates::RasterizationState);
    pipelineStateList.push_back(PipelineStates::MultisampleState);
    pipelineStateList.push_back(PipelineStates::DepthStencilState);
    pipelineStateList.push_back(PipelineStates::ColorBlendState);
    pipelineStateList.push_back(PipelineStates::DynamicState);
    
    CreateVertexAssemblyDefault();
    CreateTessellationDefault();
    CreateViewportDefault();
    CreateRasterizationDefault();
    CreateMultiSampleDefault();
    CreateDepthStencilDefault();
    CreateColorblendDefault();
    CreateDynamicStateDefault();
}

template<typename T>
inline void GraphicsPipelineManager<T>::DeInit()
{
    DestroyVertexInputState();
    DestroyShaderState();
    DestroyShaderResourceState();
    DestroyVertexAssembly();
    DestroyTessellation();
    DestroyViewport();
    DestroyRasterization();
    DestroyMultiSample();
    DestroyDepthStencil();
    DestroyColorblend();
    DestroyDynamicState();

    std::map < PipelineStates, std::vector<GraphNode<StateWrapperBase> *>>::iterator it;
    for (it = stateToNodeMap.begin(); it != stateToNodeMap.end(); it++)
    {
        for each (GraphNode<StateWrapperBase> * node in it->second)
        {
            delete node;
        }

        it->second.clear();
    }

    stateToNodeMap.clear();
    pipelineStateList.clear();
    meshToGraphNodeMap.clear();
    delete pipelineGraph;
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
    VertexInputWrapper* wrapper =  new VertexInputWrapper;
    wrapper->inputState = inputStateInfo;

    uint32_t id = HashManager::GetInstance()->FindVertexInputStateHash(inputStateInfo, wrapper->GetId());
    
    // vertex input will be the first state hence the vector can be resized
    std::vector<GraphNode<StateWrapperBase>*> nodeList;
    nodeList.resize((size_t)PipelineStates::NumStates);
    GraphNode<StateWrapperBase> * node;
    if (id == -1)
    {
        // add wrapper to list
        wrapper->meshIdList.push_back(meshId);
        vertexInputStateWrapperList.push_back(wrapper);

        // create a new pipeline node encapsulate in graph node, add it to graph
        // TODO : Create graph node, push to graph, make the connections
        node = CreateGraphNode(wrapper);
    
        // TODO : as this is vertex input state the connection won't be made here, Done.

        // TODO : Create vulkan vertexinput object

        apiInterface->CreateVertexInputState(wrapper);
    }
    else
    {
        // reduce meshid counter by 1
        DecrementIdCounter<VertexInputWrapper>();
        // delete input info
        delete inputStateInfo;
        // add the mesh id to the existing wrapper obj in the list
        std::vector<VertexInputWrapper*>::iterator it;
        it = std::find_if(vertexInputStateWrapperList.begin(), vertexInputStateWrapperList.end(), [&](VertexInputWrapper * e) { return e->GetId() == id; });

        ASSERT_MSG(it != vertexInputStateWrapperList.end(), "wrapper not found");
        (*it)->meshIdList.push_back(meshId);
        node = GetNode((*it)->state, (*it));
    //    nodeList[(uint32_t)PipelineStates::VertexInputState] = GetNode((*it)->state, (*it));
    }

    nodeList[(uint32_t)PipelineStates::VertexInputState] = node;
    meshToGraphNodeMap.insert(std::pair < uint32_t, std::vector<GraphNode<StateWrapperBase>*>>({
        meshId, nodeList }));
}

template<typename T>
inline void GraphicsPipelineManager<T>::CreateVertexAssemblyState(const uint32_t & meshId, InputAssemblyState * assembly)
{
    InputAssemblyWrapper * wrapper = new InputAssemblyWrapper;
    wrapper->assemblyState = assembly;

    uint32_t id = HashManager::GetInstance()->FindInputAssemblyStateHash(assembly, wrapper->GetId());
    GraphNode<StateWrapperBase> * node;

    if (id == -1)
    {
        // add wrapper to list
        wrapper->meshIdList.push_back(meshId);
        vertexInputAssemblyWrapperList.push_back(wrapper);

        // create a new pipeline node encapsulate in graph node, add it to graph
        // TODO : Create pipeline node, done.
        node = CreateGraphNode(wrapper);

        // TODO : Create vulkan inputAssembly object, done.
        apiInterface->CreateInputAssemblyState(wrapper);
    }
    else
    {
        // reduce meshid counter by 1
        DecrementIdCounter<InputAssemblyWrapper>();

        // delete input info
        delete assembly->primitiveType;
        delete assembly;
        delete wrapper;

        // add the mesh id to the existing wrapper obj in the list
        std::vector<InputAssemblyWrapper*>::iterator it;
        it = std::find_if(vertexInputAssemblyWrapperList.begin(), vertexInputAssemblyWrapperList.end(), [&](InputAssemblyWrapper * e) { return e->GetId() == id; });

        ASSERT_MSG(it != vertexInputAssemblyWrapperList.end(), "wrapper not found");

        (*it)->meshIdList.push_back(meshId);

        node = GetNode((*it)->state, (*it));
    }

    InsertToMeshList(meshId, PipelineStates::InputAssemblyState, node);
    CreateGraphEdges(meshId, PipelineStates::VertexInputState, PipelineStates::InputAssemblyState);
}

template<typename T>
inline void GraphicsPipelineManager<T>::AssignDefaultState(const uint32_t & meshId, const PipelineStates & state)
{
    switch (state)
    {
    case PipelineStates::InputAssemblyState :
        vertexInputAssemblyWrapperList[defaultAssemblyListIndex]->meshIdList.push_back(meshId);
        break;

    default:
        ASSERT_MSG(0, "Invalid state");
    }
}

template<typename T>
inline void GraphicsPipelineManager<T>::CreatShaderPipelineState(const uint32_t & meshId, Shader * shaders, const uint32_t & shaderCount)
{
    ShaderStateWrapper * wrapper = new ShaderStateWrapper;
    wrapper->shader = shaders;
    wrapper->shaderCount = shaderCount;

    // check if the hash exist for the above object
    uint32_t id = HashManager::GetInstance()->FindShaderStateHash(shaders, shaderCount, wrapper->GetId(), &wrapper->state);
    GraphNode<StateWrapperBase> * node;

    // if not add the wrapper to the list
    if (id == -1)
    {
        // add wrapper to list
        wrapper->meshIdList.push_back(meshId);
        shaderStateWrapperList.push_back(wrapper);
        // create a new pipeline node encapsulate in graph node, add it to graph
        // TODO : Create pipeline node , done.
        node = CreateGraphNode(wrapper);

        // TODO : Create vulkan pipeline shader stage object 
        apiInterface->CreateShaderState(wrapper);
    }
    else
    {
        // if it exists push the mesh id to the list of the object
        // reduce meshid counter by 1
        DecrementIdCounter<ShaderStateWrapper>();
        
        // delete input info
        delete wrapper;

        // add the mesh id to the existing wrapper obj in the list
        std::vector<ShaderStateWrapper*>::iterator it;
        it = std::find_if(shaderStateWrapperList.begin(), shaderStateWrapperList.end(), [&](ShaderStateWrapper* e) { return e->GetId() == id; });
        ASSERT_MSG(it != shaderStateWrapperList.end(), "wrapper not found");
        (*it)->meshIdList.push_back(meshId);
        
        node = GetNode((*it)->state, (*it));

    }

    InsertToMeshList(meshId, PipelineStates::ShaderStage, node);
    CreateGraphEdges(meshId, PipelineStates::InputAssemblyState, PipelineStates::ShaderStage);

    //pipelineGraph->PrintAdjMatrix();
}

template<typename T>
inline std::vector<SetWrapper*> GraphicsPipelineManager<T>::CreatResourceLayoutState(const uint32_t & meshId, Shader * shaders, const uint32_t & shaderCount)
{
    ShaderResourceStateWrapper * wrapper = new ShaderResourceStateWrapper;
    wrapper->shader = shaders;
    wrapper->shaderCount = shaderCount;
    
    std::vector<std::string> shaderNames;
    for (uint32_t i = 0; i < shaderCount; i++)
        shaderNames.push_back(shaders[i].shaderName);

    // get the setwrapper list for the shader combination
    wrapper->resourcesSetList = apiInterface->GetSetsForShaders(shaderNames);

    // check if the hash exist for the above object
    int id = HashManager::GetInstance()->FindResourceLayoutHash(wrapper->resourcesSetList.data(), 
        (uint32_t)wrapper->resourcesSetList.size(), wrapper->GetId());
    GraphNode<StateWrapperBase> * node;

    std::vector<SetWrapper*> setWrapperList;

    // if not add the wrapper to the list
    if (id == -1)
    {
        // add wrapper to list
        wrapper->meshIdList.push_back(meshId);
        shaderResourceStateWrapperList.push_back(wrapper);
        // create a new pipeline node encapsulate in graph node, add it to graph
        // TODO : Create pipeline node, done .
        node = CreateGraphNode(wrapper);

        // TODO : Create vulkan pipeline pipeline state object, done 
        wrapper->pipelineId = apiInterface->CreatePipelineLayout(wrapper->resourcesSetList.data(),
            wrapper->resourcesSetList.size());
        setWrapperList = wrapper->resourcesSetList;
    }
    else
    {
        // if it exists push the mesh id to the list of the object
        // reduce meshid counter by 1
        DecrementIdCounter<ShaderResourceStateWrapper>();

        // delete input info
        delete wrapper;

        // add the mesh id to the existing wrapper obj in the list
        std::vector<ShaderResourceStateWrapper*>::iterator it;
        it = std::find_if(shaderResourceStateWrapperList.begin(), shaderResourceStateWrapperList.end(), [&](ShaderResourceStateWrapper* e) { return e->GetId() == id; });
        ASSERT_MSG(it != shaderResourceStateWrapperList.end(), "wrapper not found");
        (*it)->meshIdList.push_back(meshId);

        node = GetNode((*it)->state, (*it));
        setWrapperList = (*it)->resourcesSetList;

    }

    InsertToMeshList(meshId, PipelineStates::ShaderResourcesLayout, node);
    CreateGraphEdges(meshId, PipelineStates::ShaderStage, PipelineStates::ShaderResourcesLayout);

    return setWrapperList;
}

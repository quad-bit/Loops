#include "DrawGraphNode.h"
//#include "DrawCommandBuffer.h"

uint32_t DrawGraphUtil::pipelineLayoutId = 0 ;
std::vector<uint32_t> DrawGraphUtil::descriptorIdList;
uint32_t DrawGraphUtil::setOffset = 0;

DrawCommandBuffer<ApiInterface> * DrawGraphNode::dcb = nullptr;

void DrawGraphNode::Exit()
{
    DrawGraphUtil::descriptorIdList.pop_back();
    DrawGraphUtil::setOffset = (uint32_t)DrawGraphUtil::descriptorIdList.size();
}
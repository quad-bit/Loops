#include "PipelineStateWrappers.h"

std::map<PipelineStates, uint32_t> PipelineUtil::stateToIdMap;
std::vector<uint32_t> PipelineUtil::pipelineStateMeshList;
std::vector<SetWrapper *>  PipelineUtil::setsPerPipeline;
uint32_t PipelineUtil::pipelineLayoutId;

uint32_t VertexInputWrapper::idCounter = 0;
uint32_t InputAssemblyWrapper::idCounter = 0;
uint32_t ShaderStateWrapper::idCounter = 0;
uint32_t ShaderResourceStateWrapper::idCounter = 0;
uint32_t TessellationStateWrapper::idCounter = 0;
uint32_t DepthStencilStateWrapper::idCounter = 0;
uint32_t RasterizationStateWrapper::idCounter = 0;
uint32_t ColorBlendStateWrapper::idCounter = 0;
uint32_t MultiSampleStateWrapper::idCounter = 0;
uint32_t DynamicStateWrapper::idCounter = 0;
uint32_t ViewPortStateWrapper::idCounter = 0;

void PipelineUtil::FillGlobalMeshList(std::vector<uint32_t>& meshList, const PipelineStates & currentState)
{
    if (meshList.size() != 0)
    {
        if (meshList.size() > 0)
        {
            // compare the mesh and pipelineStateMeshList
            if (pipelineStateMeshList.size() == 0 && currentState == PipelineStates::VertexInputState)
            {
                pipelineStateMeshList = meshList;
            }
            else if (pipelineStateMeshList.size() > 0)
            {
                std::vector<uint32_t>::iterator itt;
                for(itt = pipelineStateMeshList.begin(); itt != pipelineStateMeshList.end(); itt++)
                {
                    std::vector<uint32_t>::iterator it;
                    it = std::find_if(meshList.begin(), meshList.end(), [&](uint32_t meshId) { return meshId == *itt; });
                    
                    // if the id in pipelineMeshList is not found in meshList
                    // erase the id from pipelineMeshList 
                    if (it == meshList.end())
                    {
                        itt = pipelineStateMeshList.erase(itt);
                    }

                    if (pipelineStateMeshList.size() == 0)
                        break;
                }
            }
        }
    }
}

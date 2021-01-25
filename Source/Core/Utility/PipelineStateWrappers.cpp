#include "PipelineStateWrappers.h"

std::map<PipelineStates, uint32_t> stateToIdMap;

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

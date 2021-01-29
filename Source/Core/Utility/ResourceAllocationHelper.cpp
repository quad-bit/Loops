#include "ResourceAllocationHelper.h"

std::vector<size_t> AllocationUtility::CalculateOffsetsForDescInUniform(const size_t & dataSizePerDescriptor, const GlobalResourceAllocationConfig & allocConfig,
    const GlobalResourceSharingConfig & resourceSharingConfig)
{
    std::vector<size_t> offsets;
    offsets.resize(allocConfig.numDescriptors);

    uint32_t index = resourceSharingConfig.allocatedUniformCount % resourceSharingConfig.maxUniformPerResource;

    for (uint32_t i = 0; i < allocConfig.numDescriptors; i++)
    {
        offsets[i] = index * allocConfig.numDescriptors * dataSizePerDescriptor + dataSizePerDescriptor * i;
    }
    return offsets;
}


bool AllocationUtility::IsNewAllocationRequired(const GlobalResourceSharingConfig & resourceSharingConfig)
{
    if (resourceSharingConfig.allocatedUniformCount % resourceSharingConfig.maxUniformPerResource == 0)
        return true;
    else
        return false;
}

size_t AllocationUtility::GetDataSizeMeantForSharing(const size_t & sizeSingleDescriptor, const GlobalResourceAllocationConfig & allocConfig,
    const GlobalResourceSharingConfig & resourceSharingConfig)
{
    return sizeSingleDescriptor * allocConfig.numDescriptors * resourceSharingConfig.maxUniformPerResource;
}
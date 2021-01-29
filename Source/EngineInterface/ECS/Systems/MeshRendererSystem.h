
#pragma once
#include "System.h"
#include "ShaderResourceDescription.h"

class MeshRenderer;
class MeshRendererAdditionEvent;

class MeshRendererSystem : public System
{
private:

    uint32_t idCounter = 0;
    uint32_t GenerateId();

    GlobalResourceAllocationConfig allocConfig;
    GlobalResourceSharingConfig resourceSharingConfig;

    SetWrapper * transformSetWrapper;
    std::vector<ShaderBindingDescription *> resDescriptionList;
    uint32_t numDescriptorsPerBinding;

public:
    virtual void Init() override;
    virtual void DeInit() override;
    virtual void Update(float dt) override;

    MeshRendererSystem();
    virtual ~MeshRendererSystem();

    void HandleMeshRendererAddition(MeshRendererAdditionEvent * inputEvent);
};

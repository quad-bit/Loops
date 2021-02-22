#include "LightSystem.h"
#include "EventBus.h"

uint32_t LightSystem::GeneratedLightId()
{
    return idCounter++;
}

void LightSystem::Init()
{
    EventBus::GetInstance()->Subscribe<LightSystem, LightAdditionEvent>(this, &LightSystem::HandleLightAddition);
    EventBus::GetInstance()->Subscribe<LightSystem, MeshAdditionEvent>(this, &LightSystem::HandleMeshAddition);

    allocConfig.numDescriptors = Settings::maxFramesInFlight;
    allocConfig.numMemories = 1;
    allocConfig.numResources = 1;

    resourceSharingConfig.maxUniformPerResource = 20;
    resourceSharingConfig.allocatedUniformCount = 0;
}

void LightSystem::DeInit()
{
    for (uint32_t i = 0; i < resDescriptionList.size(); i++)
    {
        delete resDescriptionList[i];
    }
    resDescriptionList.clear();
}

void LightSystem::Update(float dt)
{
    // write the light uniforms
    int k = 0;
}

void LightSystem::HandleLightAddition(LightAdditionEvent * lightAdditionEvent)
{
}

void LightSystem::HandleMeshAddition(MeshAdditionEvent * meshAdditionEvent)
{
}

LightSystem::LightSystem()
{
}

LightSystem::~LightSystem()
{
}

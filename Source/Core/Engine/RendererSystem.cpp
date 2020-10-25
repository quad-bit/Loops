#include "RendererSystem.h"

#include "MeshFilter.h"

using namespace ComponentSpace;

RendererSystem::RendererSystem()
{
	signature.AddComponent<MeshFilter>();
	signature.AddComponent<MeshRenderer>();
}

void RendererSystem::Init()
{
}

void RendererSystem::DeInit()
{
}

void RendererSystem::Render()
{ // call the vulkan renderer
}

RendererSystem::~RendererSystem()
{
}

#if 0

#include "MeshFilterSystem.h"
#include "Component.h"
#include "Entity.h"
#include "World.h"
//#include <MeshManager.h>
#include "MeshFilter.h"
#include <assert.h>
//#include <Mesh.h>
//#include <VulkanObjectSpace\VertexBuffer.h>
//#include <VulkanObjectSpace\IndexBuffer.h>


//using namespace COMPONENTS;
//using namespace VulkanObjectSpace;

void MeshFilterSystem::Init()
{
}

void MeshFilterSystem::DeInit()
{
}

void MeshFilterSystem::RegisterEntity(Entity * entity)
{
	// load the mesh once mesh filter component gets added to an entity
	registeredEntities.push_back(entity);
	
	ComponentHandle<ComponentSpace::MeshFilter> filterHandle;
	worldObj->Unpack(entity, filterHandle);
	
	CreateMesh<ComponentSpace::MeshFilter>(filterHandle);

	//filterHandle->vertexBufferObj = new VertexBuffer(filterHandle->meshObj);
	//filterHandle->indexBufferObj = new IndexBuffer(filterHandle->meshObj);
}

void MeshFilterSystem::UnRegisterEntity(Entity * entity)
{
}

MeshFilterSystem::MeshFilterSystem()
{
	signature.AddComponent<ComponentSpace::MeshFilter>();
}

MeshFilterSystem::~MeshFilterSystem()
{
}


#endif
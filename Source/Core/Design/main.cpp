#if 0

#include <iostream>
#include "ComponentManager.h"
#include "Entity.h"
#include "World.h"
#include "Component.h"
#include "MeshFilterSystem.h"
#include "TransformSystem.h"
#include "MeshFilter.h"
#include "Settings.h"
#include <VulkanCoreSpace\RenderingManager.h>

#include <memory>

using namespace VulkanCoreSpace;
using namespace ComponentSpace;

void main()
{

	World worldObj;

	// update the family while creating the managers...
	ComponentManager<ComponentSpace::Transform>* transformManager = worldObj.CreateManager<ComponentSpace::Transform>();
	ComponentManager<ComponentSpace::MeshFilter>* meshFilterManager = worldObj.CreateManager<ComponentSpace::MeshFilter>();

	System* meshFilterSystemObj = new MeshFilterSystem();
	worldObj.AddSystem(meshFilterSystemObj);

	System* transformSystemObj = new TransformSystem();
	worldObj.AddSystem(transformSystemObj);

	worldObj.Init();

	RenderingManager::GetSingleton()->Init(Settings::WINDOW_WIDTH, Settings::WINDOW_HEIGHT);

	EntityHandle* playerHandle = worldObj.CreateEntity();
	ComponentSpace::Transform playerTransform;

	Health playerHealth;
	playerHealth.energy = 100;

	ComponentSpace::MeshFilter * meshFilterObj = new ComponentSpace::MeshFilter();
	meshFilterObj->meshType = "CUBE";
	meshFilterObj->hasNormals = true;
	meshFilterObj->hasColors = true;

	playerHandle->AddComponent<ComponentSpace::Transform>(&playerTransform);
	playerHandle->AddComponent<ComponentSpace::MeshFilter>(meshFilterObj);

	ComponentHandle<ComponentSpace::MeshFilter> test = playerHandle->GetComponent<ComponentSpace::MeshFilter>();

	float clearColor[4]{ .3f, .5f, .4f, 1.0f };

	while (true)
	{
		//update
		transformSystemObj->Update(0.2f);

		ComponentHandle<ComponentSpace::Transform> test2 = playerHandle->GetComponent<ComponentSpace::Transform>();
		if (RenderingManager::GetSingleton()->Update(clearColor) == false)
			break;
	}

	playerHandle->RemoveComponent<ComponentSpace::Transform>(&playerTransform);
	playerHandle->RemoveComponent<ComponentSpace::MeshFilter>(meshFilterObj);

	delete meshFilterObj;

	RenderingManager::GetSingleton()->DeInit();
	worldObj.DeInit();

	delete meshFilterSystemObj;

	delete meshFilterManager;
	delete transformManager;
}


#if 0
int oldMain()
{

    World worldObj;

	// update the family while creating the managers...
	ComponentManager<Transform>* transformManager = worldObj.CreateManager<Transform>();
	ComponentManager<Health>* healthManager = worldObj.CreateManager<Health>();
	ComponentManager<ComponentSpace::MeshFilter>* meshFilterManager = worldObj.CreateManager<ComponentSpace::MeshFilter>();

	System* meshFilterSystemObj = new MeshFilterSystem();
	worldObj.AddSystem(meshFilterSystemObj);

	worldObj.Init();

    EntityHandle* playerHandle = worldObj.CreateEntity();
    Transform playerTransform;
    playerTransform.x = 0;
    playerTransform.y = 0;
    playerTransform.z = 0;
    Health playerHealth;
    playerHealth.energy = 100;

	ComponentSpace::MeshFilter meshFilterObj;
	meshFilterObj.meshType = "CUBE";
	meshFilterObj.hasNormals = true;
	meshFilterObj.hasColors = true;

	EntityHandle* treeHandle = worldObj.CreateEntity();
    Transform treeTransform;
    treeTransform.x = 0;
    treeTransform.y = 0;
    treeTransform.z = 10;

    EntityHandle* enemyHandle = worldObj.CreateEntity();
    Transform enemyTransform;
    enemyTransform.x = 10;
    enemyTransform.y = 10;
    enemyTransform.z = 10;
    Health enemyHealth;
    enemyHealth.energy = 12;


	int family = enemyTransform.Family();
	family = enemyHealth.Family();
	family = meshFilterObj.Family();

	//int family = GetComponentFamily<Transform>();

    /*worldObj.AddComponent<Transform>(&enemyTransform, enemyHandle->GetEntity());
    worldObj.AddComponent<Transform>(&playerTransform, playerHandle->GetEntity());
    worldObj.AddComponent<Transform>(&treeTransform, treeHandle->GetEntity());

    worldObj.AddComponent<Health>(&enemyHealth, enemyHandle->GetEntity());
    worldObj.AddComponent<Health>(&playerHealth, playerHandle->GetEntity());
    */

    enemyHandle->AddComponent<Transform>(&enemyTransform);
    playerHandle->AddComponent<Transform>(&playerTransform);
    treeHandle->AddComponent<Transform>(&treeTransform);
	playerHandle->AddComponent<ComponentSpace::MeshFilter>(&meshFilterObj);

    enemyHandle->AddComponent<Health>(&enemyHealth);
    playerHandle->AddComponent<Health>(&playerHealth);

    ComponentHandle<Transform>* enemyTransHandle = transformManager->GetComponentHandle(enemyHandle->GetEntityHandle());
    //enemyTransHandle->DestroyComponent();

    //worldObj.DestroyEntity(treeHandle->GetEntity());

    ComponentHandle<Health> plHealth = playerHandle->GetComponent<Health>();
    plHealth->energy = 1000;

    int test = playerHandle->GetComponent<Health>()->energy;

    /*ComponentManager<Transform>* transformManager = new ComponentManager<Transform>();

    uint32_t ind = 0;
    ind = transformManager->AddComponent(&playerTransform, playerHandle->GetEntity());
    ind = transformManager->AddComponent(&treeTransform, treeHandle->GetEntity());
    ind = transformManager->AddComponent(&enemyTransform, enemyHandle->GetEntity());

    ComponentManager<Health>* healthManager = new ComponentManager<Health>();
    ind = healthManager->AddComponent(&playerHealth, playerHandle->GetEntity());
    ind = healthManager->AddComponent(&enemyHealth, enemyHandle->GetEntity());

    Transform playerTran = *(transformManager->GetComponent(treeHandle->GetEntity()));

    transformManager->RemoveComponent(treeHandle->GetEntity());
    Transform enemTrans = *(transformManager->GetComponent(enemyHandle->GetEntity()));

    delete healthManager;*/

    std::cin.get();

	delete meshFilterManager;
    delete transformManager;
    delete healthManager;

    return 0;
}

#endif

#endif

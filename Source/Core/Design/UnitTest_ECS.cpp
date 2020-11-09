#include "UnitTest_ECS.h"
#include "World.h"
#include "TransformSystem.h"
#include "Transform.h"


void UnitTest()
{
    World worldObj;

    // update the family while creating the managers...
    ComponentManager<Transform>* transformManager = worldObj.CreateManager<Transform>();
    //ComponentManager<MeshFilter>* meshFilterManager = worldObj.CreateManager<ComponentSpace::MeshFilter>();

    //System* meshFilterSystemObj = new MeshFilterSystem();
    //worldObj.AddSystem(meshFilterSystemObj);

    System* transformSystemObj = new TransformSystem();
    worldObj.AddSystem(transformSystemObj);

    worldObj.Init();

    EntityHandle* playerHandle = worldObj.CreateEntity();

    Transform playerTransform;
    playerTransform.position = glm::vec3(10, 10, 10);

    //ComponentSpace::MeshFilter * meshFilterObj = new ComponentSpace::MeshFilter();
    //meshFilterObj->meshType = "CUBE";
    //meshFilterObj->hasNormals = true;
    //meshFilterObj->hasColors = true;

    playerHandle->AddComponent<Transform>(&playerTransform);
    //playerHandle->AddComponent<ComponentSpace::MeshFilter>(meshFilterObj);

    //ComponentHandle<ComponentSpace::MeshFilter> test = playerHandle->GetComponent<ComponentSpace::MeshFilter>();

    ComponentHandle<Transform> obj = playerHandle->GetComponent<Transform>();

    playerHandle->RemoveComponent<Transform>(&playerTransform);
    //playerHandle->RemoveComponent<MeshFilter>(meshFilterObj);

    //delete meshFilterObj;

    worldObj.DeInit();

    //delete meshFilterSystemObj;

    //delete meshFilterManager;
    delete transformManager;

}
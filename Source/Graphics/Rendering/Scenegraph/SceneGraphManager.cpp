#include "SceneGraphManager.h"

SceneGraphManager* SceneGraphManager::instance = nullptr;

void SceneGraphManager::Init()
{

}

void SceneGraphManager::DeInit()
{

}

void SceneGraphManager::Update()
{

}

SceneGraphManager * SceneGraphManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new SceneGraphManager();
    }
    return instance;
}

SceneGraphManager::~SceneGraphManager()
{
}

void SceneGraphManager::CreateSceneGraph()
{
}

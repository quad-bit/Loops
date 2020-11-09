#include "CoreManager.h"
#include "Settings.h"

CoreManager* CoreManager::instance = nullptr;

void CoreManager::Init()
{
    
}

void CoreManager::DeInit()
{
    
}

void CoreManager::Update()
{

}

CoreManager * CoreManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new CoreManager();
    }
    return instance;
}

CoreManager::~CoreManager()
{
}

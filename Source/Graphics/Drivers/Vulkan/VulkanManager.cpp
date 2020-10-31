#include "VulkanManager.h"

using namespace Loops::Graphics::Vulkan;

VulkanManager* VulkanManager::instance = nullptr;

void VulkanManager::Init()
{

}

void VulkanManager::DeInit()
{

}

void VulkanManager::Update()
{

}

VulkanManager * VulkanManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new VulkanManager();
    }
    return instance;
}

VulkanManager::~VulkanManager()
{
}

#include "UniformFactory.h"

UniformFactory* UniformFactory::instance = nullptr;

void UniformFactory::Init()
{

}

void UniformFactory::DeInit()
{

}

void UniformFactory::Update()
{

}

UniformFactory * UniformFactory::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new UniformFactory();
    }
    return instance;
}

UniformFactory::~UniformFactory()
{
}

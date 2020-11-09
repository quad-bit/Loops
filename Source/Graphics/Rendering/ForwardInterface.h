#pragma once


#include "VulkanInterface.h"
#include "DxInterface.h"

template<typename T>
class ForwardRendering
{
private:
    T * apiInterface;
public:
    void Init();
    void SetupRenderer();
    void DislogeRenderer();
    void DeInit();
};

template<typename T>
inline void ForwardRendering<T>::Init()
{
    apiInterface = new T();
    apiInterface->Init();
}

template<typename T>
inline void ForwardRendering<T>::SetupRenderer()
{
}

template<typename T>
inline void ForwardRendering<T>::DislogeRenderer()
{
}

template<typename T>
inline void ForwardRendering<T>::DeInit()
{
    apiInterface->DeInit();
    delete apiInterface;
}

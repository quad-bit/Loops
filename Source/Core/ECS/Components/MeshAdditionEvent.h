#pragma once

#include <Event.h>
#include "RenderingWrapper.h"

class MeshAdditionEvent : public Event
{
public:
    std::vector<SetWrapper *> setWrapperList;
    uint32_t meshId; 
};

#pragma once

#define MAX_NUM_OF_UNIQUE_COMPONENTS 20

class World;

enum class APP_STATE
{
    STARTED,
    RUNNING,
    PAUSED,
    STOPPED,
    NONE
};

extern APP_STATE appState;
extern World * worldObj;

#pragma once

#include <Event.h>

struct KeyInputEvent : public Event
{
    enum class KEY_STATE
    {
        PRESSED,
        DOWN,
        RELEASED,
        NONE
    };

    KEY_STATE keyState = KEY_STATE::NONE;
    const char* keyname;
    const char* keyStateName;
};
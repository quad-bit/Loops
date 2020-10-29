#pragma once
#include <time.h>

using namespace std;

class Timer
{
private:
    Timer();
    Timer(const Timer&) {}
    const Timer& operator=(const Timer&) { }

    static clock_t currentTime, lastTime;

public:
    ~Timer();

    static float DeltaTime;
    static float FixedTime;
    static void Update();
};


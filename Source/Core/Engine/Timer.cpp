#include "Timer.h"
#include <iostream>


float Timer::DeltaTime = 0.0f;
float Timer::FixedTime = 0.0f;
clock_t Timer::currentTime(0);
clock_t Timer::lastTime(0);

Timer::Timer()
{
}


Timer::~Timer()
{
}

void Timer::Update()
{
    currentTime = clock();
    DeltaTime = (float)((currentTime - lastTime) / CLOCKS_PER_SEC);
    lastTime = currentTime;
}

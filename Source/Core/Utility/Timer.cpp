#include "Timer.h"
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <ctime>   // localtime
#include <plog/Log.h> 

Timer* Timer::instance = nullptr;

void Timer::Init()
{
    currentTimeStamp = std::chrono::high_resolution_clock::now();
    previousTimeStamp = std::chrono::high_resolution_clock::now();
}

void Timer::DeInit()
{

}

void Timer::Update()
{

}

Timer * Timer::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Timer();
    }
    return instance;
}

Timer::~Timer()
{
}

void Timer::Tick()
{
    currentTimeStamp = std::chrono::high_resolution_clock::now();
}

void Timer::Reset()
{
    currentTimeStamp = std::chrono::high_resolution_clock::now();
    previousTimeStamp = std::chrono::high_resolution_clock::now();
}

uint32_t Timer::GetSeconds()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");

    PLOGD << ss.str();
    ASSERT_MSG_DEBUG(0, "Not working");
    return 0;
}


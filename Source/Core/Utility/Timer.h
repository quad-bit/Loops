#pragma once
#include <chrono>
#include "Assertion.h"

class Timer
{

private:
    Timer(){}
    Timer(Timer const &) {}
    Timer const & operator= (Timer const &) {}

    static Timer* instance;
    std::chrono::high_resolution_clock::time_point currentTimeStamp, previousTimeStamp;

public:
    void Init();
    void DeInit();
    void Update();
    static Timer* GetInstance();
    ~Timer();

    void Tick();
    
    template <typename T>
    T GetDeltaTime();

    uint32_t GetSeconds();
};

template<typename T>
inline T Timer::GetDeltaTime()
{
    ASSERT_MSG(std::chrono::high_resolution_clock::is_steady, "Not a steady clock");
    Tick();
    std::chrono::duration<T> fs = currentTimeStamp - previousTimeStamp;
    previousTimeStamp = currentTimeStamp;
        
    return fs.count();
}


/*
class Timer {
public:
    Timer() {
        reset();
    }
    void reset() {
        m_timestamp = std::chrono::high_resolution_clock::now();
    }
    float diff() {
        std::chrono::duration<float> fs = std::chrono::high_resolution_clock::now() - m_timestamp;
        return fs.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_timestamp;
};
*/


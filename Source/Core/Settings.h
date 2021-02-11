#pragma once

#include <stdint.h>
#include <string>

//using namespace std;

#define WINDOWS_OS 0
#define LINUX_OS  1

#define VULKAN 0
#define DX12   1
    
#define RENDERING_API     VULKAN
#define GLFW_ENABLED  

#if defined(GLFW_ENABLED)
        
    // For Windows Message Box
    #if defined( _WIN32 )
    #undef APIENTRY
    #include <windows.h>
    #endif 

    #include <GLFW\glfw3.h>

#endif

#if defined( _WIN32 )
// this is always defined on windows platform
#define CURRENT_OS WINDOWS_OS

// LINUX ( Via XCB library )
#elif defined( __linux )
    #define CURRENT_OS LINUX_OS
#else
// platform not yet supported
#error Platform not yet supported
#endif

class Settings
{
public:
    static uint32_t windowWidth, windowHeight;
    static std::string windowName;
    static uint32_t swapBufferCount;
    static float depthClearValue, stencilClearValue;
    static float clearColorValue[4];
    static uint32_t maxFramesInFlight, currentFrameInFlight;
    static uint32_t maxFrameRate;
};


#pragma once
#include <stdint.h>
#include <string>
namespace Loops::Graphics
{
    class GraphicsManager
    {
    private:
        GraphicsManager(){}
        GraphicsManager(GraphicsManager const &) {}
        GraphicsManager const & operator= (GraphicsManager const &) {}

        static GraphicsManager* instance;

    public:
        void Init(uint32_t winWidth, uint32_t winHeight, std::string winName);
        void DeInit();
        void Update();
        static GraphicsManager* GetInstance();
        ~GraphicsManager();
    };
}

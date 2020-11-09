#pragma once

class EngineManager
{

private:
    EngineManager(){}
    EngineManager(EngineManager const &) {}
    EngineManager const & operator= (EngineManager const &) {}

    static EngineManager* instance;

public:
    void Init();
    void DeInit();
    void Update();
    static EngineManager* GetInstance();
    ~EngineManager();
};

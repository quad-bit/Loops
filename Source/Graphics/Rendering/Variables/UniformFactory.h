#pragma once

class UniformFactory
{

private:
    UniformFactory(){}
    UniformFactory(UniformFactory const &) {}
    UniformFactory const & operator= (UniformFactory const &) {}

    static UniformFactory* instance;

public:
    void Init();
    void DeInit();
    void Update();
    static UniformFactory* GetInstance();
    ~UniformFactory();
};

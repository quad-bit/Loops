#pragma once

#include <Scriptable.h>
class MouseDragEvent;

class CameraController : public Scriptable
{
private:

public:
    void Init() override;
    void Update(float dt) override;
    void DeInit() override;

    CameraController();
    virtual ~CameraController();

    void MouseDragEventHandler(MouseDragEvent * evt);
};

#pragma once

#include <Scriptable.h>
#include "Camera.h"
//#include "ComponentHandle.h"

struct MouseDragEvent;
struct KeyInputEvent;
class Camera;

class CameraController : public Scriptable
{
private:
    Camera * cam;
    float velocity = .07f;
    //ComponentHandle<Camera> camHandle;

    bool dragStartred = false;
    glm::vec2 currentMousePos, prevMousePos;
    void RotateCamera(glm::vec2 mousePosition);

public:
    void Activated() override;
    void Init() override;
    void Update(float dt) override;
    void DeInit() override;

    CameraController();
    virtual ~CameraController();

    void MouseDragEventHandler(MouseDragEvent * evt);
    void KeyBoardEventHandler(KeyInputEvent * evt);
};

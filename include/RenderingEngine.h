#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H

#include"GameObject.h"
#include"camera.h"
#include"InputHandler.h"
class RenderingEngine
{
public:
    RenderingEngine();
    void render(GameObject* object)
    {
        Transform t;
        object->render(*shader);
        shader->Update(t , *camera);
        camera->update(TheInputHandler::getInstance()->getMousePos());
    }
    virtual ~RenderingEngine();
private:
    BasicShader* shader;
    Camera* camera;
    RenderingEngine(const RenderingEngine& other) {}
    RenderingEngine& operator=(const RenderingEngine& other) {}
};

#endif // RENDERINGENGINE_H
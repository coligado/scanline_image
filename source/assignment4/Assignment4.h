#pragma once

#ifndef __ASSIGNMENT_4__
#define __ASSIGNMENT_4__

#include "common/Application.h"

class Assignment4: public Application
{
public:
    Assignment4(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera);

    static std::unique_ptr<Application> CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera);
    static std::shared_ptr<class Camera> CreateCamera();

    virtual glm::vec2 GetWindowSize() const;

    virtual void HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime);
    virtual void Tick(double deltaTime);
protected:
    virtual void HandleWindowResize(float x, float y);

private:
    virtual void SetupScene();

    virtual void SetupExample1();
    virtual void SetupExample2();
    virtual void SetupCamera();
    
    std::shared_ptr<class SceneObject> boto1;
    std::shared_ptr<class SceneObject> boto2;
    std::shared_ptr<class SceneObject> bottle;
    std::shared_ptr<class SceneObject> glass1;
    std::shared_ptr<class SceneObject> glass2;
    std::shared_ptr<class SceneObject> bubbles;
    std::shared_ptr<class SceneObject> ocean;
    std::shared_ptr<class SceneObject> sunset;
    std::shared_ptr<class SceneObject> surfboard;
    std::shared_ptr<class SceneObject> island;
    std::shared_ptr<class SceneObject> island2;
    std::shared_ptr<class SceneObject> island3;
    std::shared_ptr<class SceneObject> island4;
    std::shared_ptr<class SceneObject> island5;
    std::shared_ptr<class SceneObject> island6;
};

#endif

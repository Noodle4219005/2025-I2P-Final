#pragma once
#include "Engine/IScene.hpp"

class Start : public Engine::IScene 
{
public:
    Start() { ; }
    void Initialize() override;
    void Terminate() override;
    void Draw() const override;
    void OnKeyDown(int keyCode) override;
    void OnMouseDown(int button, int mx, int my) override;
};

#pragma once
#include "Engine/IScene.hpp"

class Start : public Engine::IScene 
{
public:
    Start() { ; }
    void Initialize() override {}
    void Terminate() override {}
};

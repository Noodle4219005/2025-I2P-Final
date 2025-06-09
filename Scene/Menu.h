#pragma once
#include "Engine/IScene.hpp"

class Menu : public Engine::IScene 
{
public:
    Menu() { ; }
    void Initialize() override {}
    void Terminate() override {}
};

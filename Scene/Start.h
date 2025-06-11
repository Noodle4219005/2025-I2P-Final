#pragma once
#include "Engine/IScene.hpp"

#include <memory>
#include <allegro5/allegro_audio.h>
#include <chrono>

class Start : public Engine::IScene 
{
public:
    Start() { ; }
    void Initialize() override;
    void Terminate() override;
    void Draw() const override;
    void OnKeyDown(int keyCode) override;
    void OnMouseScroll(int mx, int my, int delta) override;
    void OnMouseDown(int button, int mx, int my) override;
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> m_music;
    std::chrono::steady_clock::time_point m_audioAnimationStart;
};

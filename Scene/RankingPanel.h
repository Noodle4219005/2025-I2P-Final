#pragma once
#include "Beatmap/BeatmapParser.h"
#include "Engine/IScene.hpp"
#include "UI/Component/ImageButton.hpp"

#include <allegro5/allegro_audio.h>

class RankingPanel : public Engine::IScene 
{
public:
    RankingPanel() { ; }
    void Initialize() override;
    void Terminate() override;
    void OnMouseMove(int mx, int my) override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void Draw() const override;
    void Update(double deltaTime) override;
    void BackToMenu();
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> m_music;
    std::unique_ptr<Engine::ImageButton> m_backButton;
    std::unique_ptr<BeatmapParser> m_beatmap;
};

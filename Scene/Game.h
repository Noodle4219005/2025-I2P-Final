#pragma once
#include "Engine/IScene.hpp"
#include "Beatmap/BeatmapParser.h"
#include "UI/Component/ImageButton.hpp"

#include <allegro5/allegro_audio.h>
#include <vector>
#include <chrono>

class Game : public Engine::IScene
{
public:
    Game() { ; }
    void Initialize() override;
    void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;
    void OnMouseScroll(int mx, int my, int delta) override;
    void OnMouseMove(int mx, int my) override;
    void OnMouseDown(int button, int mx, int my) override;
    void Update(double deltaTime) override;
    void Draw() const override;
    void Terminate() override;
    void GetHitObject();
    void UpdateHitObjects();
    void Retry();
    void ContinueGame();
    void BackToMenu();

private:
    int m_firstObjectTime=0;
    int m_lastObjectTime=0;
    int m_timing=0;
    int m_isPlayed=false;
    double m_goalPosition=0;
    bool m_isAltKeyDown;
    std::chrono::steady_clock::time_point m_audioAnimationStart;
    std::chrono::steady_clock::time_point m_prevTimestamp;
    std::vector<std::chrono::steady_clock::time_point> m_prevAutoClicked;
    std::unique_ptr<HitObject> m_nextHitObject;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> m_music;
    std::unique_ptr<BeatmapParser> m_beatmap;
    std::unique_ptr<Engine::Image> m_bg;
    std::vector<std::list<std::unique_ptr<HitObject>>> m_activeObjectLists; // the object which is now need to be processed
    std::unique_ptr<Engine::ImageButton> m_continueButton;
    std::unique_ptr<Engine::ImageButton> m_retryButton;
    std::unique_ptr<Engine::ImageButton> m_backButton;
};

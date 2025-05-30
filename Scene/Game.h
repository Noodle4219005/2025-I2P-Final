#pragma once
#include "Engine/IScene.hpp"
#include "Beatmap/BeatmapParser.h"

#include <allegro5/allegro_audio.h>

class Game : public Engine::IScene
{
public:
    Game() { ; }
    void Initialize() override;
    void OnKeyDown(int keyCode) override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void Terminate() override;

private:
    float m_musicBarX;
    float m_musicBarY;
    float m_musicBarWidth;
    float m_musicBarHeight;
    HitObject m_nextHitObject;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> music;
    std::unique_ptr<BeatmapParser> m_beatmap;
    std::vector<std::list<HitObject>> activeObjectLists;
};

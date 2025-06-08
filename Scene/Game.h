#pragma once
#include "Engine/IScene.hpp"
#include "Beatmap/BeatmapParser.h"

#include <allegro5/allegro_audio.h>
#include <vector>

class Game : public Engine::IScene
{
public:
    Game() { ; }
    void Initialize() override;
    void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void Terminate() override;
    void GetHitObject();
    void UpdateHitObjects();

private:
    int m_firstObjectTime=0;
    int m_timing=0;
    std::unique_ptr<HitObject> m_nextHitObject;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> music;
    std::unique_ptr<BeatmapParser> m_beatmap;
    std::vector<std::list<std::unique_ptr<HitObject>>> m_activeObjectLists; // the object which is now need to be processed
};

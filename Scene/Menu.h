#pragma once
#include "Engine/IScene.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/SingleThreadWorker.h"
#include "Beatmap/BeatmapParser.h"
#include "UI/Component/BeatmapCard.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <allegro5/allegro_audio.h>
#include <chrono>
#include <thread>
#include <semaphore>

class Menu : public Engine::IScene 
{
public:
    Menu() { ; }
    void Initialize() override;
    void Terminate() override;
    void Update(double deltaTime) override;
    void OnMouseScroll(int mx, int my, int delta) override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;
    void Draw() const override;
private:
    struct MapInfo 
    {
        int id;
        std::string name;
        std::string author;
        std::string mapper;
        double minDiff, maxDiff;
        int previewTime;
        std::string audioPath;
        BeatmapCard card;
        BeatmapParser::BeatmapImageInfo imageInfo;
    };
    std::vector<MapInfo> m_mapList; 
    std::unordered_map<std::string, std::vector<BeatmapParser>> m_beatmapDifficultiesByName;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> m_music;
    std::unique_ptr<Engine::Image> m_bg;
    bool m_isPlayed;
    bool m_isAltKeyDown;
    std::chrono::steady_clock::time_point m_audioAnimationStart;
    std::chrono::steady_clock::time_point m_lastChangeTime;
    int m_nowMapIndex=-1;
    int m_nowDiffcultyIndex=0;
    int m_level=0; // 0 for map selection, 1 for difficulty selection
    std::string m_nowPlayingAudioPath="";
    std::string m_beatmapName="";
    std::vector<BeatmapCard> m_difficultyCards;
    SingleThreadWorker* m_worker;
    void ProcessDifficulty(int beatmapId, std::string beatmapPath);
    void MapCallBack(std::string beatmapName);
    void DiffCallBack();
};

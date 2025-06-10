#pragma once
#include "Engine/IScene.hpp"
#include "Engine/AudioHelper.hpp"
#include "Beatmap/BeatmapParser.h"
#include "UI/Component/BeatmapCard.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <allegro5/allegro_audio.h>

class Menu : public Engine::IScene 
{
public:
    Menu() { ; }
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void OnMouseScroll(int mx, int my, int delta) override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void Draw() const override;
private:
    struct MapInfo 
    {
        int id;
        std::string name;
        std::string author;
        std::string mapper;
        float minDiff, maxDiff;
        int previewTime;
        std::string audioPath;
        BeatmapCard card;
        std::shared_ptr<Engine::Image> image;
    };
    std::vector<MapInfo> m_mapList; 
    std::unordered_map<std::string, std::vector<BeatmapParser>> m_beatmapDifficultiesByName;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> m_music;
    std::shared_ptr<Engine::Image> m_bg;
    bool m_isPlayed;
    int m_nowMapIndex=-1;
    int m_nowDiffcultyIndex=0;
    int m_level=0; // 0 for map selection, 1 for difficulty selection
    std::string m_nowPlayingAudioPath="";
    std::string m_beatmapName="";
    std::vector<BeatmapCard> m_difficultyCards;
    void ProcessDifficulty(int beatmapId, std::string beatmapPath);
    void MapCallBack(std::string beatmapName);
    void DiffCallBack();
};

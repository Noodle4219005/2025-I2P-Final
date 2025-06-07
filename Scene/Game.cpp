#include "Game.h"
#include "Engine/IScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/AudioHelper.hpp"
#include "Beatmap/BeatmapParser.h"
#include "Objects/Hold.h"
#include "Objects/Node.h"
#include "util/GameData.h"
#include "util/Constant.h"
#include "UI/HUD.h"

#include <algorithm>
#include <string>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>

void Game::Initialize() 
{
    game_data::nowGameState=game_data::LOADING;
    m_beatmap=std::make_unique<BeatmapParser>(BeatmapParser(game_data::mapID, game_data::difficultyName));
    music=AudioHelper::PlaySample(m_beatmap->GetAudioFilePath(), false, AudioHelper::BGMVolume, 0.f);
    //AudioHelper::ChangeSampleSpeed(music, 1);
    m_activeObjectLists=std::vector<std::list<std::unique_ptr<HitObject>>>(m_beatmap->GetTotalColumns());
    m_nextHitObject=m_beatmap->GetNextHitObject();
    game_data::nowGameState=game_data::PLAYING;
}

void Game::Terminate()
{
    m_beatmap.release();
    m_activeObjectLists.clear();
}

void Game::OnKeyDown(int keyCode)  
{
    // determine the keydown
    for (int i=1; i<=m_beatmap->GetTotalColumns(); ++i) {
        std::string key=std::to_string(m_beatmap->GetTotalColumns())+"k"+std::to_string(i);
        if (keyCode!=constant::keyMap[key]) continue;
        if (m_activeObjectLists[i-1].empty()) continue;
        for (auto& object : m_activeObjectLists[i-1]) {
            if (object->IsAvailable()) {
                object->OnKeyDown();
                break;
            }
        }
    }

    // skip the front space of the music
    if (game_data::gamePosition<(m_nextHitObject->GetStartTime()-constant::kSkipTimeThreshold) && keyCode==constant::keyMap["skip"]) {
        std::cout<<"Skipped\n";
        m_nextHitObject->GetStartTime();
        AudioHelper::ChangeSamplePosition(music, m_nextHitObject->GetStartTime()-constant::kSkipTimeThreshold);
    }
}

void Game::OnKeyUp(int keyCode)  
{
    // determine the keyup
    for (int i=1; i<=m_beatmap->GetTotalColumns(); ++i) {
        std::string key=std::to_string(m_beatmap->GetTotalColumns())+"k"+std::to_string(i);
        if (keyCode!=constant::keyMap[key]) continue;
        if (m_activeObjectLists[i-1].empty()) continue;
        for (auto& object : m_activeObjectLists[i-1]) {
            if (object->IsAvailable()) {
                object->OnKeyUp();
                break;
            }
        }
    }
}

void Game::Update(float deltaTime)
{
    int baseScore=(1000000 * game_data::modMultiplier * 0.5 / m_beatmap->GetTotalNotes()) * (game_data::hitValue / 320);
    int bonusScore=(1000000 * game_data::modMultiplier * 0.5 / m_beatmap->GetTotalNotes()) * (game_data::hitBonusValue * sqrt(game_data::hitBonus) / 320);
    game_data::score=baseScore+bonusScore;
    game_data::gamePosition=AudioHelper::GetSamplePosition(music);
    if (game_data::nowGameState==game_data::PAUSE) {
        AudioHelper::StopSample(music);
        return;
    }
    m_beatmap->UpdateTiming(game_data::gamePosition);
    while (m_nextHitObject->GetStartTime() < game_data::gamePosition+game_data::GetScrollMilisecond()) {
        m_activeObjectLists[m_nextHitObject->GetColumn()].push_back(std::move(m_nextHitObject));
        if (!m_beatmap->IsMapEnded()) m_nextHitObject=std::move(m_beatmap->GetNextHitObject());
        else m_nextHitObject=std::make_unique<HitObject>(HitObject(0, INT_MAX, 0, 0, ""));
    }
    for (auto& objectList : m_activeObjectLists) {
        if (objectList.empty()) continue;
        for (auto& object : objectList) {
            object->Update();
        }
        if (objectList.size() && !objectList.front()->IsAlive()) {
            objectList.pop_front();
        }
    }
    game_data::scrollSpeedMultiplexer=m_beatmap->GetSpeedScale();
}

void Game::Draw() const
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    HUD::GetInstance().DrawBackground();
    if (game_data::nowGameState==game_data::LOADING) return;
    if (game_data::nowGameState==game_data::PAUSE) return;

    for (auto& objectList : m_activeObjectLists) {
        for (auto& hitObject : objectList) {
            hitObject->Draw();
        }
    }
    HUD::GetInstance().DrawForeground();
}

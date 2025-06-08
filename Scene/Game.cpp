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
    game_data::Refresh();
    game_data::nowGameState=game_data::LOADING;
    m_beatmap=std::make_unique<BeatmapParser>(BeatmapParser(game_data::mapID, game_data::difficultyName));
    //AudioHelper::ChangeSampleSpeed(music, 1);
    m_activeObjectLists=std::vector<std::list<std::unique_ptr<HitObject>>>(m_beatmap->GetTotalColumns());
    m_nextHitObject=m_beatmap->GetNextHitObject();
    m_firstObjectTime=m_nextHitObject->GetStartTime();
    while (m_nextHitObject->GetStartTime()<INT_MAX-5) {
        m_activeObjectLists[m_nextHitObject->GetColumn()].push_back(std::move(m_nextHitObject));
        if (!m_beatmap->IsMapEnded()) {
            m_nextHitObject=std::move(m_beatmap->GetNextHitObject());
            game_data::playtimeLength=m_lastObjectTime=m_nextHitObject->GetEndTime();
        }
        else m_nextHitObject=std::make_unique<HitObject>(HitObject(0, INT_MAX, 0, 0, ""));
    }
    HUD::GetInstance().Init();
    music=AudioHelper::PlaySample(m_beatmap->GetAudioFilePath(), false, AudioHelper::BGMVolume, 0.f);
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
        // HUD
        HUD::GetInstance().OnKeyDown(i-1);
        if (m_activeObjectLists[i-1].empty()) continue;
        // Key
        for (auto& object : m_activeObjectLists[i-1]) {
            if (object->IsAvailable()) {
                object->OnKeyDown();
                break;
            }
        }

    }

    // skip the front space of the music
    if (game_data::gamePosition<(m_firstObjectTime-constant::kSkipTimeThreshold) && keyCode==constant::keyMap["skip"]) {
        std::cout<<"Skipped\n";
        AudioHelper::ChangeSamplePosition(music, m_firstObjectTime-constant::kSkipTimeThreshold);
    }
}

void Game::OnKeyUp(int keyCode)  
{
    // determine the keyup
    for (int i=1; i<=m_beatmap->GetTotalColumns(); ++i) {
        std::string key=std::to_string(m_beatmap->GetTotalColumns())+"k"+std::to_string(i);
        if (keyCode!=constant::keyMap[key]) continue;
        // HUD
        HUD::GetInstance().OnKeyUp(i-1);
        if (m_activeObjectLists[i-1].empty()) continue;
        // Key
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
    if (game_data::nowGameState==game_data::PAUSE) {
        AudioHelper::StopSample(music);
        return;
    }
    int baseScore=(1000000 * game_data::modMultiplier * 0.5 / m_beatmap->GetTotalNotes()) * (game_data::hitValue / 320);
    int bonusScore=(1000000 * game_data::modMultiplier * 0.5 / m_beatmap->GetTotalNotes()) * (game_data::hitBonusValue * sqrt(game_data::hitBonus) / 320);
    game_data::score=baseScore+bonusScore;
    int goalPosition=AudioHelper::GetSamplePosition(music);
    int droppedTiming=0;
    while (m_beatmap->GetNextTiming()<=goalPosition) {
        droppedTiming++;
        game_data::gamePosition=m_beatmap->GetNextTiming();
        UpdateHitObjects();
        m_beatmap->PushTiming();
    }
    if (droppedTiming>1) std::cout<<"Dropped: "<<droppedTiming-1<<std::endl;
    game_data::gamePosition=goalPosition;
    UpdateHitObjects();
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

void Game::UpdateHitObjects()
{
    game_data::scrollSpeedMultiplier=m_beatmap->GetSpeedScale();
    for (auto& objectList : m_activeObjectLists) {
        if (objectList.empty()) continue;
        for (auto& object : objectList) {
            object->Update();
        }
        if (objectList.size() && !objectList.front()->IsAlive()) {
            objectList.pop_front();
        }
    }
}

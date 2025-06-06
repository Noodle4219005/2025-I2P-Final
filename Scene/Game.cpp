#include "Game.h"
#include "Engine/IScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/AudioHelper.hpp"
#include "Beatmap/BeatmapParser.h"
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
    m_activeObjectLists.assign(m_beatmap->GetTotalColumns(), std::list<HitObject>());
    m_nextHitObject=m_beatmap->GetNextHitObject();
    game_data::nowGameState=game_data::PLAYING;
}

void Game::Terminate()
{
    m_beatmap.release();
}

void Game::OnKeyDown(int keyCode)  
{
    // determine the keydown
    for (int i=1; i<=m_beatmap->GetTotalColumns(); ++i) {
        std::string key=std::to_string(m_beatmap->GetTotalColumns())+"k"+std::to_string(i);
        if (keyCode!=constant::keyMap[key]) continue;
        if (m_activeObjectLists[i-1].empty()) continue;
        for (auto object : m_activeObjectLists[i-1]) {
            if (object.IsAvailable()) {
                object.OnKeyDown();
                break;
            }
        }
    }

    // skip the front space of the music
    if (game_data::gamePosition<(m_nextHitObject.GetStartTime()-constant::kSkipTimeThreshold) && keyCode==constant::keyMap["skip"]) {
        std::cout<<"Skipped\n";
        m_nextHitObject.GetStartTime();
        AudioHelper::ChangeSamplePosition(music, m_nextHitObject.GetStartTime()-constant::kSkipTimeThreshold);
    }
}

void Game::OnKeyUp(int keyCode)  
{
}

void Game::Update(float deltaTime)
{
    game_data::gamePosition=AudioHelper::GetSamplePosition(music);
    if (game_data::nowGameState==game_data::PAUSE) {
        AudioHelper::StopSample(music);
        return;
    }
    while (m_nextHitObject.GetStartTime() < game_data::gamePosition+game_data::GetScrollMilisecond()) {
        m_activeObjectLists[m_nextHitObject.GetColumn()].push_back(m_nextHitObject);
        if (!m_beatmap->IsMapEnded()) m_nextHitObject=m_beatmap->GetNextHitObject();
    }
    for (auto& objectList : m_activeObjectLists) {
        if (objectList.empty()) continue;
        for (auto& object : objectList) {
            object.Update();
        }
        if (objectList.size() && !objectList.front().IsAlive()) {
            objectList.pop_front();
        }
    }
    game_data::scrollSpeedMultiplexer=m_beatmap->GetSpeedScale();
    m_beatmap->UpdateTiming(game_data::gamePosition);
}

void Game::Draw() const
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    if (game_data::nowGameState==game_data::LOADING) return;
    if (game_data::nowGameState==game_data::PAUSE) return;

    for (auto& objectList : m_activeObjectLists) {
        for (auto& hitObject : objectList) {
            hitObject.Draw();
        }
    }

    // Draw a hitbar for test.
    al_draw_line(0, game_data::hitPosition*constant::kPixelScale, constant::kScreenW, game_data::hitPosition*constant::kPixelScale,
                 al_map_rgb(255, 0, 0), 10);
}

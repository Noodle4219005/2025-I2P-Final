#include "Game.h"
#include "Engine/IScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/AudioHelper.hpp"
#include "Beatmap/BeatmapParser.h"
#include "util/GameData.h"
#include "util/Constant.h"
#include "UI/HUD.h"

#include <algorithm>
#include <allegro5/allegro_audio.h>

void Game::Initialize() 
{
    game_data::nowGameState=game_data::LOADING;
    int width=Engine::GameEngine::GetInstance().GetScreenWidth();
    int height=Engine::GameEngine::GetInstance().GetScreenHeight();
    m_musicBarX = width * 0.01f;
    m_musicBarY = height * 0.05f;
    m_musicBarWidth = std::max(width * 0.005f, 7.f);
    m_musicBarHeight = height * 0.9f;
    m_beatmap=std::make_unique<BeatmapParser>(BeatmapParser(game_data::mapID, game_data::difficultyName));
    music=AudioHelper::PlaySample(m_beatmap->GetAudioFilePath(), false, AudioHelper::BGMVolume, 0.f);
    activeObjectLists.assign(m_beatmap->GetTotalColumns(), std::list<HitObject>());
    m_nextHitObject=m_beatmap->GetNextHitObject();
    game_data::nowGameState=game_data::PLAYING;
}

void Game::Terminate()
{
    m_beatmap.release();
}

void Game::OnKeyDown(int keyCode)  
{
    if (m_beatmap->GetTotalColumns()==4) {
        if (keyCode==constant::key4k1 && activeObjectLists[0].size()) {
            activeObjectLists[0].front().OnKeyDown();
        }
    }
}

void Game::OnKeyUp(int keyCode)  
{
}

void Game::Update(float deltaTime)
{
    game_data::gamePosition=AudioHelper::GetSamplePosition(music);
    if (game_data::nowGameState==game_data::PAUSE) AudioHelper::StopSample(music);
    while (m_nextHitObject.GetStartTime()<game_data::gamePosition+game_data::GetScrollMilisecond(240)) {
        activeObjectLists[m_nextHitObject.GetColumn()].push_back(m_nextHitObject);
        if (!m_beatmap->IsMapEnded()) m_nextHitObject=m_beatmap->GetNextHitObject();
    }
    for (int i=0; i<activeObjectLists.size(); ++i) {
        if (activeObjectLists[i].empty()) continue;
        activeObjectLists[i].front().Update();
        if (!activeObjectLists[i].front().IsAlive()) {
            activeObjectLists[i].pop_front();
        }
    }
}

void Game::Draw() const
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    if (game_data::nowGameState==game_data::LOADING) return;
    if (game_data::nowGameState==game_data::PAUSE) return;

    for (auto objectList : activeObjectLists) {
        std::cout<<objectList.size()<<" ";
        for (auto hitObject : objectList) {
            hitObject.Draw();
        }
    }
    std::cout<<"\n";
}

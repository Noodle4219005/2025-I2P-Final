#include "Game.h"
#include "Engine/IScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/AudioHelper.hpp"
#include "Beatmap/BeatmapParser.h"
#include "util/GameData.h"
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
}

void Game::OnKeyDown(int keyCode)  
{
    m_beatmap.release();
}

void Game::Update(float deltaTime)
{
    game_data::gamePosition=AudioHelper::GetSamplePosition(music);
    if (game_data::nowGameState==game_data::PAUSE) AudioHelper::StopSample(music);
    while (m_nextHitObject.GetStartTime()<game_data::gamePosition) {
        activeObjectLists[m_nextHitObject.GetColumn()].push_back(m_nextHitObject);
        if (!m_beatmap->IsMapEnded()) m_nextHitObject=m_beatmap->GetNextHitObject();
    }
}

void Game::Draw() const
{
    if (game_data::nowGameState==game_data::LOADING) return;
    if (game_data::nowGameState==game_data::PAUSE) return;
}


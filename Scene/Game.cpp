#include "Game.h"
#include "Engine/IScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/Resources.hpp"
#include "Beatmap/BeatmapParser.h"
#include "Objects/Hold.h"
#include "Objects/Node.h"
#include "util/GameData.h"
#include "util/Constant.h"
#include "util/ErrorCalculator.h"
#include "UI/HUD.h"
#include "UI/Component/Label.hpp"

#include <algorithm>
#include <string>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <chrono>
#include <memory>

void Game::Initialize() 
{
    std::cout<<"MapID: "<<game_data::mapID<<" / Difficutly: "<<game_data::difficultyName<<std::endl;
    al_clear_to_color(al_map_rgb(0, 0, 0));
    Engine::Label loadingText{"Loading", "NotoCJK/noto-sans-cjk-black.ttf", 72, 1.f*constant::kScreenW/2, 1.f*constant::kScreenH/2, 255, 255, 255, 255, 0.5, 0.5};
    loadingText.Draw();
    al_flip_display();

    game_data::Refresh();
    game_data::nowGameState=game_data::LOADING;
    m_beatmap=std::make_unique<BeatmapParser>(game_data::mapID, game_data::difficultyName);
    if (game_data::isDoubleTime) {
        game_data::scrollSpeed/=1.5;
        game_data::modDivider=1.1;
    }
    if (game_data::isNoFailed) {
        game_data::modMultiplier=0.5;
    }
    
    // Load in the object list
    m_activeObjectLists=std::vector<std::list<std::unique_ptr<HitObject>>>(m_beatmap->GetTotalColumns());
    m_nextHitObject=m_beatmap->GetNextHitObject();
    m_firstObjectTime=m_nextHitObject->GetStartTime();
    int totalLoadedNotes=0;
    while (m_nextHitObject->GetStartTime()<INT_MAX-5) {
        al_draw_filled_rectangle(0, constant::kScreenH, 1.f*constant::kScreenW*totalLoadedNotes/m_beatmap->GetTotalNotes(), constant::kScreenH-10, al_map_rgb(255, 255, 255));
        al_flip_display();
        m_activeObjectLists[m_nextHitObject->GetColumn()].push_back(std::move(m_nextHitObject));
        if (!m_beatmap->IsMapEnded()) {
            m_nextHitObject=std::move(m_beatmap->GetNextHitObject());
            game_data::playtimeLength=m_lastObjectTime=m_nextHitObject->GetEndTime();
        }
        else m_nextHitObject=std::make_unique<HitObject>(HitObject(0, INT_MAX, 0, 0, ""));
        totalLoadedNotes++;
    }

    // Initialize singleton
    HUD::GetInstance().Init();
    (void)Skin::GetInstance();

    // Init timing and music playint
    m_music=AudioHelper::PlaySample(m_beatmap->GetAudioFilePath(), false, AudioHelper::BGMVolume, 0);
    AudioHelper::StopSample(m_music);
    m_prevTimestamp=std::chrono::steady_clock::now();
    m_goalPosition=game_data::gamePosition=-constant::kHitobjectPreviewThreshold;
    m_isPlayed=false;

    // Generate button
    std::string filename="";
    filename=constant::kSkinPath+'/'+"pause-back.png";
    m_backButton=std::move(std::make_unique<Engine::ImageButton>
        (
            filename, filename, 
            constant::kScreenW/2, constant::kPauseBackPosition*constant::kPixelScale,
            0, 0, 0.5, 0.5
        ));
    m_backButton->SetOnClickCallback(std::bind(&Game::BackToMenu, this));
    filename=constant::kSkinPath+'/'+"pause-retry.png";
    m_retryButton=std::move(std::make_unique<Engine::ImageButton>
        (
            filename, filename, 
            constant::kScreenW/2, constant::kPauseRetryPosition*constant::kPixelScale,
            0, 0, 0.5, 0.5
        ));
    m_retryButton->SetOnClickCallback(std::bind(&Game::Retry, this));
    filename=constant::kSkinPath+'/'+"pause-continue.png";
    m_continueButton=std::move(std::make_unique<Engine::ImageButton>
        (
            filename, filename, 
            constant::kScreenW/2, constant::kPauseContinuePosition*constant::kPixelScale,
            0, 0, 0.5, 0.5
        ));
    m_continueButton->SetOnClickCallback(std::bind(&Game::ContinueGame, this));

    // Init auto playing list
    m_prevAutoClicked.assign(game_data::nkey, std::chrono::steady_clock::now());

    game_data::nowGameState=game_data::PLAYING;

    m_isAltKeyDown=false;
    m_audioAnimationStart=std::chrono::steady_clock::now()-std::chrono::seconds(constant::kMusicBarDisplaySeconds+1);
    m_bg=std::make_unique<Engine::Image>(m_beatmap->GetBackgroundImage());
}

void Game::Terminate()
{
    m_isAltKeyDown=false;
    if (game_data::isDoubleTime) {
        game_data::scrollSpeed*=1.5;
    }
    AudioHelper::StopSample(m_music);
    decltype(m_activeObjectLists)().swap(m_activeObjectLists);
    m_music=nullptr;
}

void Game::OnKeyDown(int keyCode)  
{
    if (game_data::nowGameState==game_data::PAUSE) return;
    if (game_data::nowGameState==game_data::FAILED) return;

    // skip the front space of the music
    if (game_data::gamePosition<(m_firstObjectTime-constant::kSkipTimeThreshold) && keyCode==constant::keyMap["skip"]) {
        std::cout<<"Skipped\n";
        m_prevTimestamp=std::chrono::steady_clock().now();
        AudioHelper::ChangeSamplePosition(m_music, m_firstObjectTime-constant::kSkipTimeThreshold);
    }

    // pause the music
    if (keyCode==constant::keyMap["pause"]) {
        std::cout<<"Paused\n";
        game_data::nowGameState=game_data::PAUSE;
    }

    if (keyCode==ALLEGRO_KEY_ALT || keyCode==ALLEGRO_KEY_ALTGR) {
        m_isAltKeyDown=true;
    }

    // Auto mode
    if (game_data::isAuto) return;

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
}

void Game::OnKeyUp(int keyCode)  
{
    if (game_data::nowGameState==game_data::PAUSE) return;
    if (game_data::nowGameState==game_data::FAILED) return;

    if (keyCode==ALLEGRO_KEY_ALT || keyCode==ALLEGRO_KEY_ALTGR) {
        m_isAltKeyDown=false;
    }

    // Auto mode
    if (game_data::isAuto) return;

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

void Game::OnMouseMove(int mx, int my) 
{
    if (game_data::nowGameState==game_data::PAUSE) {
        m_backButton->OnMouseMove(mx, my);
        m_continueButton->OnMouseMove(mx, my);
        m_retryButton->OnMouseMove(mx, my);
    }
    if (game_data::nowGameState==game_data::FAILED) {
        m_backButton->OnMouseMove(mx, my);
        m_retryButton->OnMouseMove(mx, my);
    }

}
void Game::OnMouseDown(int button, int mx, int my) 
{
    std::cout<<"mouse: "<<button<<" "<<mx<<" "<<my<<"\n";
    if (game_data::nowGameState==game_data::PAUSE) {
        m_backButton->OnMouseDown(button, mx, my);
        m_continueButton->OnMouseDown(button, mx, my);
        m_retryButton->OnMouseDown(button, mx, my);
    }
    if (game_data::nowGameState==game_data::FAILED) {
        m_backButton->OnMouseDown(button, mx, my);
        m_retryButton->OnMouseDown(button, mx, my);
    }
}

void Game::OnMouseScroll(int mx, int my, int delta)
{
    if (m_isAltKeyDown) {
        AudioHelper::BGMVolume+=delta*0.01;
        AudioHelper::BGMVolume=std::clamp((double)AudioHelper::BGMVolume, 0.0, 1.0);
        AudioHelper::ChangeSampleVolume(m_music, AudioHelper::BGMVolume);
        m_audioAnimationStart=std::chrono::steady_clock::now();
        return;
    }
}

void Game::Update(double deltaTime)
{
    // std::cout<<game_data::gamePosition<<std::endl;
    if (game_data::nowGameState==game_data::PAUSE) {
        m_continueButton->Update(deltaTime);
        m_retryButton->Update(deltaTime);
        m_backButton->Update(deltaTime);
        if (m_isPlayed) {
            AudioHelper::StopSample(m_music);
            std::cout<<al_filename_exists((constant::kSkinPath+'/'+"pause-loop.wav").c_str());
            m_music=AudioHelper::PlaySample(constant::kSkinPath+'/'+"pause-loop.wav", true, AudioHelper::BGMVolume, 0);
            m_isPlayed=false;
        }
        return;
    }

    if (game_data::nowGameState==game_data::FAILED) {
        m_retryButton->Update(deltaTime);
        m_backButton->Update(deltaTime);
        if (m_isPlayed) {
            AudioHelper::StopSample(m_music);
            std::cout<<al_filename_exists((constant::kSkinPath+'/'+"failsound.mp3").c_str());
            m_music=AudioHelper::PlaySample(constant::kSkinPath+'/'+"failsound.mp3", true, AudioHelper::BGMVolume, 0);
            m_isPlayed=false;
        }
        return;
    }

    // Auto mode
    if (game_data::isAuto) {
        // Auto input for the key down
        for (int i=1; i<=m_beatmap->GetTotalColumns(); ++i) {
            if (m_activeObjectLists[i-1].empty()) continue;
            for (auto& object : m_activeObjectLists[i-1]) {
                if (object->IsAvailable()) {
                    if (game_data::gamePosition-object->GetStartTime()<-12) break;
                    HUD::GetInstance().OnKeyDown(i-1);
                    object->OnKeyDown();
                    m_prevAutoClicked[i-1]=std::chrono::steady_clock().now();
                    break;
                }
            }
        }

        // Auto input for the key up
        for (int i=1; i<=m_beatmap->GetTotalColumns(); ++i) {
            int duration=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-m_prevAutoClicked[i-1]).count();
            if (m_activeObjectLists[i-1].empty()) continue;
            // Key
            for (auto& object : m_activeObjectLists[i-1]) {
                if (duration > constant::kAutoDelay) {
                    if (object->GetType()&128 && object->GetStartTime()<game_data::gamePosition);
                    else HUD::GetInstance().OnKeyUp(i-1);
                }
                if (object->IsAvailable()) {
                    if (object->GetType()&128 && game_data::gamePosition-object->GetEndTime()>=-12) {
                        HUD::GetInstance().OnKeyUp(i-1);
                        object->OnKeyUp();
                    }
                    break;
                }
            }
        }
    }

    // passed
    if (game_data::gamePosition>m_lastObjectTime+constant::kHitobjectPreviewThreshold || 
        (game_data::gamePosition>=AudioHelper::GetSampleLength(m_music)-10 && game_data::gamePosition>=m_lastObjectTime)) {
        Engine::GameEngine::GetInstance().ChangeScene("ranking_panel");
    }

    int baseScore=(1000000 * game_data::modMultiplier * 0.5 / m_beatmap->GetTotalNotes()) * (game_data::hitValue / 320);
    int bonusScore=(1000000 * game_data::modMultiplier * 0.5 / m_beatmap->GetTotalNotes()) * (game_data::hitBonusValue * sqrt(game_data::hitBonus) / 320);
    game_data::score=baseScore+bonusScore;

    int droppedTiming=0;
    if (m_isPlayed) m_goalPosition=AudioHelper::GetSamplePosition(m_music);
    else m_goalPosition+=1.0*std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()-m_prevTimestamp).count()/1000.;
    m_prevTimestamp=std::chrono::steady_clock().now();
    // std::cout<<"Goal: "<<m_goalPosition<<std::endl;
    while (m_beatmap->GetNextTiming()<=m_goalPosition) {
        droppedTiming++;
        game_data::gamePosition=m_beatmap->GetNextTiming();
        UpdateHitObjects();
        m_beatmap->PushTiming();
    }
    game_data::gamePosition=m_goalPosition;
    if (game_data::gamePosition>=0.f && !m_isPlayed) {
        m_music=AudioHelper::PlaySample(m_beatmap->GetAudioFilePath(), false, AudioHelper::BGMVolume, game_data::gamePosition);
        if (game_data::isDoubleTime) AudioHelper::ChangeSampleSpeed(m_music, 1.5);
        m_isPlayed=true;
    }
    if (droppedTiming>1) std::cout<<"Dropped: "<<droppedTiming-1<<std::endl;
    UpdateHitObjects();

    game_data::maxCombo=std::max(game_data::maxCombo, game_data::combo);

    if (!game_data::isNoFailed && game_data::hp<=0) game_data::nowGameState=game_data::FAILED;
}

void Game::Draw() const
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    if (m_bg)m_bg->Draw();

    HUD::GetInstance().DrawBackground();
    if (game_data::nowGameState==game_data::LOADING) return;

    for (auto& objectList : m_activeObjectLists) {
        for (auto& hitObject : objectList) {
            if (hitObject->GetPositionY()<0) break;
            hitObject->Draw();
        }
    }

    if (game_data::gamePosition<(m_firstObjectTime-constant::kSkipTimeThreshold)) {
        Skin::GetInstance().DrawPlaySkip();
    }
    HUD::GetInstance().DrawForeground();

    if (game_data::nowGameState==game_data::PAUSE) {
        Skin::GetInstance().DrawPause();
        m_continueButton->Draw();
        m_retryButton->Draw();
        m_backButton->Draw();
    }

    if (game_data::nowGameState==game_data::FAILED) {
        Skin::GetInstance().DrawFailed();
        m_retryButton->Draw();
        m_backButton->Draw();

    }

    using std::chrono::duration_cast;
    using std::chrono::seconds;
    using std::chrono::steady_clock;
    if (duration_cast<seconds>(steady_clock::now()-m_audioAnimationStart).count() <= constant::kMusicBarDisplaySeconds) {
        int width=constant::kMusicBarDisplayWidth*constant::kPixelScale;
        int length=constant::kMusicBarDisplayLength*constant::kPixelScale;
        al_draw_filled_rectangle(constant::kScreenW, constant::kScreenH,
                                 constant::kScreenW-length, constant::kScreenH-width,
                                 al_map_rgb(20, 20, 20)
                                 );
        al_draw_filled_rectangle(constant::kScreenW, constant::kScreenH,
                                 constant::kScreenW-length*AudioHelper::BGMVolume, constant::kScreenH-width,
                                 al_map_rgb(255, 255, 255)
                                 );
        al_draw_rectangle(constant::kScreenW, constant::kScreenH,
                                 constant::kScreenW-length, constant::kScreenH-width,
                                 al_map_rgb(255, 255, 255), 5
                                 );
    }
}

void Game::UpdateHitObjects()
{
    game_data::scrollSpeedMultiplier=m_beatmap->GetSpeedScale();
    for (auto& objectList : m_activeObjectLists) {
        if (objectList.empty()) continue;
        if (objectList.size() && !objectList.front()->IsAlive()) {
            objectList.pop_front();
        }
        for (auto& object : objectList) {
            object->Update();
        }
    }
}

void Game::Retry() 
{
    game_data::nowGameState=game_data::PLAYING;
    m_prevTimestamp=std::chrono::steady_clock().now();
    AudioHelper::StopSample(m_music);
    Terminate();
    Initialize();
}

void Game::ContinueGame()
{
    game_data::nowGameState=game_data::PLAYING;
    m_prevTimestamp=std::chrono::steady_clock().now();
    AudioHelper::StopSample(m_music);
    if (game_data::gamePosition>=0) {
        m_music=AudioHelper::PlaySample(m_beatmap->GetAudioFilePath(), false, AudioHelper::BGMVolume, game_data::gamePosition);
        if (game_data::isDoubleTime) AudioHelper::ChangeSampleSpeed(m_music, 1.5);
    }
    m_isPlayed=true;
}

void Game::BackToMenu()
{
    Engine::GameEngine::GetInstance().ChangeScene("menu");
}

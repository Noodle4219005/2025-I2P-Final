#include "Engine/IScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/AudioHelper.hpp"
#include "util/GameData.h"

#include <algorithm>
#include <allegro5/allegro_audio.h>

class Game : public Engine::IScene
{
private:
    float m_musicBarX;
    float m_musicBarY;
    float m_musicBarWidth;
    float m_musicBarHeight;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> music;
public:
    void Initialize() override;
    void OnKeyDown(int keyCode) override;
    void Update(float deltaTime) override;
    void Draw() const override;
};

void Game::Initialize() 
{
    int width=Engine::GameEngine::GetInstance().GetScreenWidth();
    int height=Engine::GameEngine::GetInstance().GetScreenHeight();
    m_musicBarX = width * 0.01f;
    m_musicBarY = height * 0.05f;
    m_musicBarWidth = std::max(width * 0.005f, 7.f);
    m_musicBarHeight = height * 0.9f;
    music=AudioHelper::PlaySample()
}

void Game::OnKeyDown(int keyCode)  
{

}

void Game::Update(float deltaTime)
{
    game_data::gamePosition=AudioHelper::GetSamplePosition();
}

void Game::Draw() const
{
    if (game_data::nowGameState==game_data::LOADING) return;
}


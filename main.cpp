// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Engine/AudioHelper.hpp"
#include "Scene/Game.h"
#include "Scene/Menu.h"
#include "Scene/RankingPanel.h"
#include "Scene/Start.h"
#include "util/Constant.h"

#include <fstream>
#include <sstream>
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

void SetupVarialbes();

int main(int argc, char** argv) {
  Engine::LOG::SetConfig(true);
  SetupVarialbes();
  Engine::GameEngine& game = Engine::GameEngine::GetInstance();

  game.AddNewScene("game", new Game());
  game.AddNewScene("menu", new Menu());
  game.AddNewScene("ranking_panel", new RankingPanel());
  game.AddNewScene("start", new Start());

  game.Start("start", constant::kFPS, constant::kScreenW, constant::kScreenH);
  return 0;
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    main(0, 0);
    return 0;
}
#endif // _WIN32

void SetupVarialbes()
{
    std::fstream fs("./mania-config.ini");
    std::string token, val;
    while (getline(fs, token)) {
        std::stringstream ss(token);
        getline(ss, token, ':');
        getline(ss, val, ' ');
        getline(ss, val);
        if (token=="fps") constant::kFPS=stoi(val);
        if (token=="screen_width") constant::kScreenW=stoi(val);
        if (token=="screen_height") constant::kScreenH=stoi(val);
        if (token=="scroll_speed") game_data::scrollSpeed=stod(val);
        if (token=="hit_position") game_data::hitPosition=stoi(val);
        if (token=="offset") game_data::offset=stod(val);
        if (token=="background_dim") game_data::backgroundDim=stod(val);
        if (token=="BGM_Volume_Init") AudioHelper::BGMVolume=stod(val);
        if (token=="SFX_Volume_Init") AudioHelper::SFXVolume=stod(val);
    }
    auto pixelScalePtr=const_cast<double*>(&constant::kPixelScale);
    *pixelScalePtr=1.f*constant::kScreenH/480;
    auto offsetXPtr=const_cast<int*>(&constant::kOffsetX);
    *offsetXPtr=(constant::kScreenW-constant::kPixelScale*constant::kGameMaxPixelX)/2;
}

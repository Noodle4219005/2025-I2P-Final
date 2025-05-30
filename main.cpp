// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/Game.h"
#include "util/Constant.h"

int main(int argc, char** argv) {
  Engine::LOG::SetConfig(true);
  Engine::GameEngine& game = Engine::GameEngine::GetInstance();

  game.AddNewScene("game", new Game());

  game.Start("game", constant::kFPS, constant::kScreenW, constant::kScreenH);
  return 0;
}

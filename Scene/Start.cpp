#include "Start.h"
#include "Engine/GameEngine.hpp"
#include "Engine/AudioHelper.hpp"
#include "UI/Component/Image.hpp"
#include "util/Constant.h"

#include <allegro5/keycodes.h>

void Start::Initialize() 
{
}

void Start::Terminate() 
{
}

void Start::OnKeyDown(int keyCode)
{
    if (keyCode==ALLEGRO_KEY_ENTER) {
        Engine::GameEngine::GetInstance().ChangeScene("menu");
        AudioHelper::PlayAudio(constant::kSkinPath+"/welcome.wav");
    }
}

void Start::OnMouseDown(int button, int mx, int my)
{
    if (button==1) Engine::GameEngine::GetInstance().ChangeScene("menu");
}

void Start::Draw() const
{
    Engine::Image image("./res/start_background.jpg", 0, 0, constant::kScreenW);
    image.Draw();
}

#include "Start.h"
#include "Engine/GameEngine.hpp"
#include "Engine/AudioHelper.hpp"
#include "UI/Component/Image.hpp"
#include "util/Constant.h"

#include <allegro5/keycodes.h>
#include <allegro5/allegro_primitives.h>
#include <algorithm>

void Start::Initialize() 
{
    m_music=AudioHelper::PlaySample("./res/Nekodex - Circles!.wav");
}

void Start::Terminate() 
{
    AudioHelper::StopSample(m_music);
}

void Start::OnMouseScroll(int mx, int my, int delta)
{
    AudioHelper::BGMVolume+=delta*0.01;
    AudioHelper::BGMVolume=std::clamp((double)AudioHelper::BGMVolume, 0.0, 1.0);
    AudioHelper::ChangeSampleVolume(m_music, AudioHelper::BGMVolume);
    m_audioAnimationStart=std::chrono::steady_clock::now();
    return;

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

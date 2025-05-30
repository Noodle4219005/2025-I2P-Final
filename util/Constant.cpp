#include "Constant.h"

namespace constant 
{

    int key4k1=ALLEGRO_KEY_D;
    int key4k2=ALLEGRO_KEY_F;
    int key4k3=ALLEGRO_KEY_J;
    int key4k4=ALLEGRO_KEY_K;

    const std::string kSkinPath="./res/skin";
    
    const int kFPS=120;
    const int kScreenW=1600;
    const int kScreenH=832;

    const int kGameMaxPixelX=640;
    const int kGameMaxPixelY=480;

    const float pixelScale=1.f*kScreenH/480;
    const int offsetX=(kScreenW-pixelScale*kGameMaxPixelX)/2;
}

#include "Constant.h"

namespace constant 
{
    std::unordered_map<std::string, int> keyMap=
    {
        {"4k1", ALLEGRO_KEY_D},
        {"4k2", ALLEGRO_KEY_F},
        {"4k3", ALLEGRO_KEY_J},
        {"4k4", ALLEGRO_KEY_K},
        {"skip", ALLEGRO_KEY_SPACE}
    };

    const std::string kSkinPath="./res/skin";
    
    const int kFPS=120;
    const int kScreenW=1600;
    const int kScreenH=832;

    const int kGameMaxPixelX=640;
    const int kGameMaxPixelY=480;

    const float kPixelScale=1.f*kScreenH/480;
    const int kOffsetX=(kScreenW-kPixelScale*kGameMaxPixelX)/2;

    const int kHitobjectPreviewThreshold=1500; // ms
    const int kSkipTimeThreshold=5000-kHitobjectPreviewThreshold; // ms

    const int kBlockWidth=75;
    const int kBlockHeight=20;
    const int kHitlineThickness=5;
    const int kBorderlineThickness=5;
    const int kPlaytimeLineThickness=10;
    const int kHpLineThickness=10;
    const int kComboPosition=240;
    const int kScorePosition=150;
    const int kHitbarPosition=450;
    const int kHitbarWidth=15;

    const int kMaxScore=1000000;
}

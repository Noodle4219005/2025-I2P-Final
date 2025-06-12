#include "Constant.h"

namespace constant 
{
    std::unordered_map<std::string, int> keyMap=
    {
        {"1k1", ALLEGRO_KEY_SPACE},
        {"2k1", ALLEGRO_KEY_F},
        {"2k2", ALLEGRO_KEY_J},
        {"3k1", ALLEGRO_KEY_F},
        {"3k2", ALLEGRO_KEY_SPACE},
        {"3k3", ALLEGRO_KEY_J},
        {"4k1", ALLEGRO_KEY_D},
        {"4k2", ALLEGRO_KEY_F},
        {"4k3", ALLEGRO_KEY_J},
        {"4k4", ALLEGRO_KEY_K},
        {"5k1", ALLEGRO_KEY_D},
        {"5k2", ALLEGRO_KEY_F},
        {"5k3", ALLEGRO_KEY_SPACE},
        {"5k4", ALLEGRO_KEY_J},
        {"5k5", ALLEGRO_KEY_K},
        {"6k1", ALLEGRO_KEY_S},
        {"6k2", ALLEGRO_KEY_D},
        {"6k3", ALLEGRO_KEY_F},
        {"6k4", ALLEGRO_KEY_J},
        {"6k5", ALLEGRO_KEY_K},
        {"6k6", ALLEGRO_KEY_L},
        {"7k1", ALLEGRO_KEY_S},
        {"7k2", ALLEGRO_KEY_D},
        {"7k3", ALLEGRO_KEY_F},
        {"7k4", ALLEGRO_KEY_SPACE},
        {"7k5", ALLEGRO_KEY_J},
        {"7k6", ALLEGRO_KEY_K},
        {"7k7", ALLEGRO_KEY_L},
        {"8k1", ALLEGRO_KEY_A},
        {"8k2", ALLEGRO_KEY_S},
        {"8k3", ALLEGRO_KEY_D},
        {"8k4", ALLEGRO_KEY_F},
        {"8k5", ALLEGRO_KEY_J},
        {"8k6", ALLEGRO_KEY_K},
        {"8k7", ALLEGRO_KEY_L},
        {"8k8", ALLEGRO_KEY_SEMICOLON},
        {"9k1", ALLEGRO_KEY_A},
        {"9k2", ALLEGRO_KEY_S},
        {"9k3", ALLEGRO_KEY_D},
        {"9k4", ALLEGRO_KEY_F},
        {"9k5", ALLEGRO_KEY_SPACE},
        {"9k6", ALLEGRO_KEY_J},
        {"9k7", ALLEGRO_KEY_K},
        {"9k8", ALLEGRO_KEY_L},
        {"9k9", ALLEGRO_KEY_SEMICOLON},
        {"10k1", ALLEGRO_KEY_A},
        {"10k2", ALLEGRO_KEY_S},
        {"10k3", ALLEGRO_KEY_D},
        {"10k4", ALLEGRO_KEY_F},
        {"10k5", ALLEGRO_KEY_G},
        {"10k6", ALLEGRO_KEY_H},
        {"10k7", ALLEGRO_KEY_J},
        {"10k8", ALLEGRO_KEY_K},
        {"10k9", ALLEGRO_KEY_L},
        {"10k10", ALLEGRO_KEY_SEMICOLON},
        {"skip", ALLEGRO_KEY_SPACE},
        {"pause", ALLEGRO_KEY_ESCAPE},
        {"select_auto", ALLEGRO_KEY_A},
        {"select_dt", ALLEGRO_KEY_S},
        {"select_nf", ALLEGRO_KEY_D}
    };

    const std::string kSkinPath="./res/skin";
    
    const int kFPS=120;
    const int kScreenW=1600;
    const int kScreenH=832;

    const int kGameMaxPixelX=640;
    const int kGameMaxPixelY=480;

    const double kPixelScale=1.f*kScreenH/480;
    const int kOffsetX=(kScreenW-kPixelScale*kGameMaxPixelX)/2;

    const int kHitobjectPreviewThreshold=1000; // ms
    const int kSkipTimeThreshold=5000+kHitobjectPreviewThreshold; // ms

    const int kBlockWidth=75;
    const int kBlockHeight=20;
    const int kHitlineThickness=5;
    const int kBorderlineThickness=5;
    const int kPlaytimeLineThickness=10;
    const int kHpLineThickness=10;
    const int kComboPosition=240;
    const int kScorePosition=150;
    const int kHitbarPosition=450;
    const int kModSeletionPosition=100;
    const int kHitbarWidth=15;

    const int kPauseContinuePosition=200;
    const int kPauseRetryPosition=300;
    const int kPauseBackPosition=400;

    const int kAutoDelay=38; // in ms;

    const int kMaxScore=1000000;

    // For ranking
    const int kRankingScorePositionY=20;
    const int kRankingScorePositionX=50;
    const int kRankingResultsPosition=120;
    const int kRankingMaxComboPosition=325;
    const int kRankingAccuracyPosition=250;
    const int kRankingSignPosition=150;
    const int kRankingModPosition=200;

    // For menu
    extern const ALLEGRO_COLOR kSongNotSelectedColor=al_map_rgb(0, 150, 255);
    extern const ALLEGRO_COLOR kSongSelectedColor=al_map_rgb(255, 137, 244);
    extern const ALLEGRO_COLOR kDiffNotSelectedColor=al_map_rgb(255, 175, 14);
    extern const ALLEGRO_COLOR kDiffSelectedColor=al_map_rgb(255, 0, 0);
    extern const int kBeatmapCardHeight=50;
    extern const int kBeatmapCardWidth=550;

    extern const int kMusicBarDisplaySeconds=5;
    extern const int kMusicBarDisplayLength=150;
    extern const int kMusicBarDisplayWidth=50;
}

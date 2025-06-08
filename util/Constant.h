#pragma once

#include <string>
#include <unordered_map>
#include <allegro5/keycodes.h>

namespace constant
{
    extern std::unordered_map<std::string, int> keyMap;

    extern const std::string kSkinPath;

    extern const int kFPS;
    extern const int kScreenW;
    extern const int kScreenH;

    extern const int kGameMaxPixelX;
    extern const int kGameMaxPixelY;

    extern const float kPixelScale;
    extern const int kOffsetX;

    extern const int kSkipTimeThreshold; // ms
    extern const int kHitobjectPreviewThreshold; // ms

    extern const int kBlockWidth;
    extern const int kBlockHeight;
    extern const int kHitlineThickness;
    extern const int kBorderlineThickness;
    extern const int kPlaytimeLineThickness;

    extern const int kMaxScore;
}
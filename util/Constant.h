#pragma once

#include <string>
#include <unordered_map>
#include <allegro5/keycodes.h>
#include <allegro5/color.h>

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
    extern const int kHpLineThickness;
    extern const int kComboPosition;
    extern const int kScorePosition;
    extern const int kHitbarPosition;
    extern const int kModSeletionPosition;
    extern const int kHitbarWidth;

    extern const int kPauseContinuePosition;
    extern const int kPauseRetryPosition;
    extern const int kPauseBackPosition;

    extern const int kAutoDelay; // in ms;

    extern const int kMaxScore;

    // For ranking
    extern const int kRankingScorePositionY;
    extern const int kRankingScorePositionX;
    extern const int kRankingResultsPosition;
    extern const int kRankingMaxComboPosition;
    extern const int kRankingAccuracyPosition;
    extern const int kRankingSignPosition;
    extern const int kRankingModPosition;

    // For menu
    extern const ALLEGRO_COLOR kSongNotSelectedColor;
    extern const ALLEGRO_COLOR kSongSelectedColor;
    extern const ALLEGRO_COLOR kDiffNotSelectedColor;
    extern const ALLEGRO_COLOR kDiffSelectedColor;
    extern const int kBeatmapCardHeight;
    extern const int kBeatmapCardWidth;
}
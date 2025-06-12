#pragma once

#include <string>
#include <vector>

namespace game_data 
{
    enum GameState 
    {
        LOADING,
        PLAYING,
        PAUSE,
        FAILED
    };

    struct HitResult 
    {
        double time;
        double hitError;
        double acc;
        bool isErrorPositive; // positive is late
    };

    extern int combo;
    extern int maxCombo;
    extern int hit300;
    extern int hit300H;
    extern int hit100;
    extern int hit200;
    extern int hit50;
    extern int miss;
    extern int hp;
    extern double accuracy;
    extern double gamePosition;
    extern long long score;
    extern long long hitValue;
    extern long long hitBonusValue;
    extern long long hitPunishment;
    extern long long hitBonus;
    extern long long bonus;

    extern int nowGameState;

    void Refresh();

    extern long long mapID;
    extern std::string difficultyName; // which difficulty
    extern double starRate;
    extern double OD;
    extern double speed;
    extern int nkey;
    extern double scrollSpeed;
    extern double scrollSpeedMultiplier;
    extern double hitPosition;
    extern double modMultiplier;
    extern double modDivider;
    extern std::vector<HitResult> hitResults; // time acc
    extern double playtimeLength;
    extern double hitbarScale;

    extern double offset; // in ms
    extern double backgroundDim;

    // Mode
    extern bool isAuto;
    extern bool isNoFailed;
    extern bool isDoubleTime;
    extern bool isBPMScaled;

    double GetScrollMilisecond();
}


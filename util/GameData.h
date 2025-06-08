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
        float time;
        float hitError;
        float acc;
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
    extern float accuracy;
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
    extern float starRate;
    extern float OD;
    extern float speed;
    extern int nkey;
    extern float scrollSpeed;
    extern float scrollSpeedMultiplier;
    extern float hitPosition;
    extern float modMultiplier;
    extern float modDivider;
    extern std::vector<HitResult> hitResults; // time acc
    extern float playtimeLength;
    extern float hitbarScale;

    extern float offset; // in ms

    float GetScrollMilisecond();
}


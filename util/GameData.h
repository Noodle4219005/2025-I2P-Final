#pragma once

#include <string>

namespace game_data 
{
    enum GameState 
    {
        LOADING,
        PLAYING,
        PAUSE,
        FAILED
    };

    extern int combo;
    extern int hit300;
    extern int hit300H;
    extern int hit100;
    extern int hit100H;
    extern int hit50;
    extern int miss;
    extern float accuracy;
    extern long long gamePosition;
    extern long long score;

    extern int nowGameState;

    void Refresh();

    extern long long mapID;
    extern std::string difficultyName; // which difficulty
    extern float starRate;
    extern float OD;
    extern float speed;
}


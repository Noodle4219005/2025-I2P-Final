#include "GameData.h"

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
    extern int GamePosition;
    extern long long score;

    extern int nowGameState=LOADING;
    extern long long gamePosition=0;

    void Refresh();

    // about map
    extern long long mapID;
    extern std::string difficultyName; // which difficulty
    extern int starRate;
}

namespace game_data
{ 
    void Refresh() 
    {
        hit300=hit300H=hit100=hit100H=hit50=miss=0;
        score=0;
        gamePosition=0;
        accuracy=100.f;
        nowGameState=LOADING;
    }
}
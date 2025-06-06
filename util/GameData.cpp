#include "GameData.h"

#include <string>

namespace game_data 
{
    int combo=0;
    int hit300=0;
    int hit300H=0;
    int hit100=0;
    int hit100H=0;
    int hit50=0;
    int miss=0;
    float accuracy=100.f;
    long long gamePosition=0;
    long long score=0;

    int nowGameState=LOADING;

    long long mapID=622946;
    std::string difficultyName="HD"; // which difficulty
    float starRate=0;
    float OD=0;
    float speed=1;
    float scrollSpeed=15;
    float scrollSpeedMultiplexer=1.;
    float hitPosition=408; // from 240 to 480

    void Refresh() 
    {
        hit300=hit300H=hit100=hit100H=hit50=miss=0;
        score=0;
        gamePosition=0;
        OD=0;
        accuracy=100.f;
        nowGameState=LOADING;
    }

    float GetScrollMilisecond() {
        return (6860 + 6860 * (hitPosition/480)) / (scrollSpeed*scrollSpeedMultiplexer);
    }
}
#include "GameData.h"

#include <string>
#include <vector>

namespace game_data 
{
    int combo=0;
    int hit300=0;
    int hit300H=0;
    int hit100=0;
    int hit200=0;
    int hit50=0;
    int miss=0;
    int hp=100;
    float accuracy=100.f;
    double gamePosition=0;
    long long score=0;
    long long hitValue=0;
    long long hitBonusValue=0;
    long long hitPunishment=0;
    long long hitBonus=0;
    long long bonus=0;

    int nowGameState=LOADING;

    long long mapID=2252748;
    std::string difficultyName="Hard"; // which difficulty
    float starRate=0;
    float OD=0;
    int nkey=4;
    float speed=1;
    float scrollSpeed=22;
    float scrollSpeedMultiplier=1.;
    float hitPosition=408; // from 240 to 480
    float modMultiplier=1.;
    float modDivider=1.;
    std::vector<HitResult> hitResults{}; // time acc
    float playtimeLength=0;

    float offset=0;

    void Refresh() 
    {
        hit300=hit300H=hit100=hit200=hit50=miss=0;
        bonus=hitBonus=hitBonusValue=hitPunishment=hitValue=score=0;
        gamePosition=0;
        playtimeLength=0;
        OD=0;
        accuracy=100.f;
        nowGameState=LOADING;
        hp=100;
        decltype(hitResults)().swap(hitResults);
    }

    // Return the scroll speed without multiplexer
    float GetScrollMilisecond() 
    {
        return (6860 + 6860 * (hitPosition/480)) / scrollSpeed;
    }
}
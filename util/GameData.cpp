#include "GameData.h"

#include <string>
#include <vector>

namespace game_data 
{
    int combo=0;
    int maxCombo=0;
    int hit300=0;
    int hit300H=0;
    int hit100=0;
    int hit200=0;
    int hit50=0;
    int miss=0;
    int hp=1000;
    double accuracy=100.f;
    double gamePosition=0;
    long long score=0;
    long long hitValue=0;
    long long hitBonusValue=0;
    long long hitPunishment=0;
    long long hitBonus=0;
    long long bonus=0;

    int nowGameState=LOADING;

    long long mapID=2325151;
    std::string difficultyName="Final Hymn"; // which difficulty
    double starRate=0;
    double OD=0;
    int nkey=7;
    double speed=1;
    double scrollSpeed=22;
    double scrollSpeedMultiplier=1.;
    double hitPosition=408; // from 240 to 480
    double modMultiplier=1.;
    double modDivider=1.;
    std::vector<HitResult> hitResults{}; // time acc
    double firstObjectTime;
    double playtimeLength=0;
    double hitbarScale=3;

    double offset=0;
    double backgroundDim=0.7;

    // Mode
    bool isAuto=false;
    bool isNoFailed=false;
    bool isDoubleTime=false;
    bool isBPMScaled=false;

    void Refresh() 
    {
        combo=maxCombo=0;
        hit300=hit300H=hit100=hit200=hit50=miss=0;
        bonus=hitBonus=hitBonusValue=hitPunishment=hitValue=score=0;
        scrollSpeedMultiplier=1.;
        gamePosition=0;
        playtimeLength=0;
        OD=0;
        accuracy=100.f;
        nowGameState=LOADING;
        hp=1000;
        modMultiplier=modDivider=1.f;
        decltype(hitResults)().swap(hitResults);
    }

    // Return the scroll speed without multiplexer
    double GetScrollMilisecond() 
    {
        return (6860 + 6860 * (hitPosition/480)) / scrollSpeed;
    }
}
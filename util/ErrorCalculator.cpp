#include "ErrorCalculator.h" 
#include "GameData.h"

#include <math.h>
#include <algorithm>

int GetHitValue(int nodePerfectPosition)
{
    float hiterror=abs(game_data::gamePosition-nodePerfectPosition);

    // the magic numbers are from the website of osu judgement
    // https://osu.ppy.sh/wiki/en/Gameplay/Judgement/osu%21mania

    if (hiterror <= 16) {
        return PERFECT;
    }
    else if (hiterror <= 64 - 3*game_data::OD) {
        return GREAT;
    }
    else if (hiterror <= 97 - 3*game_data::OD) {
        return GOOD;
    }
    else if (hiterror <= 127 - 3*game_data::OD) {
        return OK;
    }
    else if (hiterror <= 151 - 3*game_data::OD) {
        return MEH;
    }
    else if (hiterror <= 188 - 3*game_data::OD) {
        return MISS;
    }
    else {
        return NONE;
    }
}

int GetHoldHeadValue(int headPerfectPosition) 
{
    return 0;
}

int GetHoldValue(int HeadJudgement, int tailPerfectPosition) 
{
    return 0;
}

void IncrementHitCounter(int judgement)
{
    // refering to this https://osu.ppy.sh/wiki/en/Gameplay/Score/ScoreV1/osu%21mania
    int preBonus=game_data::bonus;
    switch (judgement) {
    case PERFECT: 
        game_data::hit300H++;
        game_data::hitValue+=320;
        game_data::hitBonusValue+=32;
        game_data::hitBonus+=2;
        break;
    case GREAT: 
        game_data::hit300++;
        game_data::hitValue+=300;
        game_data::hitBonusValue+=32;
        game_data::hitBonus+=1;
        break;
    case GOOD:
        game_data::hit100H++;
        game_data::hitValue+=200;
        game_data::hitBonusValue+=16;
        game_data::hitPunishment+=8;
        break;
    case OK:
        game_data::hit100++;
        game_data::hitValue+=100;
        game_data::hitBonusValue+=8;
        game_data::hitPunishment+=24;
        break;
    case MEH:
        game_data::hit50++;
        game_data::hitValue+=50;
        game_data::hitBonusValue+=4;
        game_data::hitPunishment+=44;
        break;
    case MISS:
        game_data::miss++;
        game_data::hitPunishment+=100005; // INF
        break;
    default:
        break;
    }
    game_data::bonus=std::clamp(preBonus + game_data::hitBonus - game_data::hitPunishment/game_data::modDivider, 0.f, 100.f);
}


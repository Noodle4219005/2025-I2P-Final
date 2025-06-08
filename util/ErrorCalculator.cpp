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

int GetHoldValue(int headHitError, int tailPerfectPosition) 
{
    int tailHitError=game_data::gamePosition-tailPerfectPosition;
    int combinedError=headHitError+tailHitError;

    // the magic numbers are from the website of osu judgement
    // https://osu.ppy.sh/wiki/en/Gameplay/Judgement/osu%21mania

    if (headHitError <= 16*1.2 && combinedError <= 16*2.4) {
        return PERFECT;
    }
    else if (headHitError <= (64 - 3*game_data::OD)*1.1 && combinedError <= (64 - 3*game_data::OD)*2.2) {
        return GREAT;
    }
    else if (headHitError <= (97 - 3*game_data::OD) && headHitError <= (97 - 3*game_data::OD)*2) {
        return GOOD;
    }
    else if (headHitError <= 127 - 3*game_data::OD && combinedError <= (127 - 3*game_data::OD)*2) {
        return OK;
    }
    else {
        return MEH;
    }
}

void IncrementHitCounter(int judgement, float hitError, bool isPositive)
{
    // refering to this https://osu.ppy.sh/wiki/en/Gameplay/Score/ScoreV1/osu%21mania
    int preBonus=game_data::bonus;
    float acc=0;
    switch (judgement) {
    case PERFECT: 
        game_data::hit300H++;
        game_data::hitValue+=320;
        game_data::hitBonusValue+=32;
        game_data::hitBonus+=2;
        game_data::combo+=1;
        acc=100;
        break;
    case GREAT: 
        game_data::hit300++;
        game_data::hitValue+=300;
        game_data::hitBonusValue+=32;
        game_data::hitBonus+=1;
        game_data::combo+=1;
        acc=100;
        break;
    case GOOD:
        game_data::hit200++;
        game_data::hitValue+=200;
        game_data::hitBonusValue+=16;
        game_data::hitPunishment+=8;
        game_data::combo+=1;
        acc=66.67;
        break;
    case OK:
        game_data::hit100++;
        game_data::hitValue+=100;
        game_data::hitBonusValue+=8;
        game_data::hitPunishment+=24;
        game_data::combo+=1;
        acc=33.33;
        break;
    case MEH:
        game_data::hit50++;
        game_data::hitValue+=50;
        game_data::hitBonusValue+=4;
        game_data::hitPunishment+=44;
        game_data::combo+=1;
        acc=16.67;
        break;
    case MISS:
        game_data::miss++;
        game_data::hitPunishment+=100005; // INF
        game_data::combo=0;
        acc=0;
        break;
    default:
        break;
    }
    game_data::hitResults.push_back(game_data::HitResult{(float)game_data::gamePosition, hitError, acc, isPositive});
    game_data::bonus=std::clamp(preBonus + game_data::hitBonus - game_data::hitPunishment/game_data::modDivider, 0.f, 100.f);
    CalculateAcc();
}

void CalculateAcc()
{
    if (game_data::hitResults.size()==0) game_data::accuracy=100.;
    else game_data::accuracy=0.;
    for (auto i : game_data::hitResults) {
        game_data::accuracy+=i.acc/game_data::hitResults.size();
    }
}

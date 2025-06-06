#include "ErrorCalculator.h" 
#include "GameData.h"

#include <math.h>

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

int GetHoldValue(Judgement HeadJudgement, int tailPerfectPosition) 
{
    return 0;
}

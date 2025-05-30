#include "ErrorCalculator.h" 
#include "GameData.h"

#include <math.h>

int getHitValue(int nodePerfectPosition)
{
    int hiterror=abs(game_data::gamePosition-nodePerfectPosition);

    // the magic number are from the website of osu judgement
    // https://osu.ppy.sh/wiki/en/Gameplay/Judgement/osu%21mania

    if (hiterror<16) {
        return PERFECT;
    }
    else if (hiterror<64-3)
}

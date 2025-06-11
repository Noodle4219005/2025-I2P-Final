#pragma once

enum Judgement 
{
    PERFECT=0,
    GREAT,
    GOOD,
    OK,
    MEH,
    MISS,
    NONE
};

int GetHitValue(int nodePerfectPosition);
int GetHoldValue(int headHitError, int tailPerfectPosition);
void IncrementHitCounter(int headHitError, float hitError, bool isPositive);
void CalculateAcc();


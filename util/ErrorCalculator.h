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
void IncrementHitCounter(int headHitError, double hitError, bool isPositive);
void CalculateAcc();


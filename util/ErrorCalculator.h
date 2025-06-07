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
int GetHoldHeadValue(int headPerfectPosition);
int GetHoldValue(Judgement HeadJudgement, int tailPerfectPosition);
void IncrementHitCounter(int judgement);


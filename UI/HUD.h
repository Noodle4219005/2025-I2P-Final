#pragma once

#include "util/Constant.h"

#include <vector>

class HUD 
{
public:
    // Singleton
    HUD(HUD const&)=delete;
    void operator=(HUD const&)=delete;
    static HUD& GetInstance();

    void Init();
    void DrawForeground();
    void DrawBackground();
    void OnKeyDown(int column);
    void OnKeyUp(int column);

private:
    HUD();
    int screenMiddleX; 
    int startX;
    int playFieldWidth;
    float musicBarX;
    float musicBarY;
    float musicBarWidth;
    float musicBarHeight;
    std::vector<bool> isKeyPressed;
    int prevCombo;
    int prevHit300H;
    int prevHit300;
    int prevHit200;
    int prevHit100;
    int prevHit50;
    int prevMiss;
    int drawHitValue;
    int comboAnimationTick;
    int hitvalueAnimationTick;
    const int kMaxHitvalueAnimationTick=constant::kFPS*0.1;
};

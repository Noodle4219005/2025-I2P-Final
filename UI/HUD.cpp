#include "HUD.h"
#include "util/Constant.h"
#include "util/GameData.h"
#include "Skin/Skin.h"

#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <cmath>
#include <algorithm>

HUD& HUD::GetInstance()
{
    static HUD instance{};
    return instance;
}

HUD::HUD()
{
    Init();
}

void HUD::Init()
{
    screenMiddleX=constant::kScreenW/2;
    startX=screenMiddleX - constant::kBlockWidth*(game_data::nkey/2) - ((game_data::nkey%2)?(constant::kBlockWidth/2):(0));
    playFieldWidth=constant::kBlockWidth*(game_data::nkey);
    musicBarX = constant::kScreenW * 0.01f;
    musicBarY = constant::kScreenH * 0.05f;
    musicBarWidth = std::max(constant::kScreenW * 0.005f, 7.f);
    musicBarHeight = constant::kScreenH * 0.9f;
    decltype(isKeyPressed)().swap(isKeyPressed);
    isKeyPressed.assign(game_data::nkey, 0);
    prevCombo=0;
    prevHit300H=prevHit300=prevHit200=prevHit100=prevHit50=prevMiss=0;
}

void HUD::OnKeyDown(int column) 
{
    isKeyPressed[column]=true;
}

void HUD::OnKeyUp(int column) 
{
    isKeyPressed[column]=false;
}

void HUD::DrawForeground()
{
    // hitbar
    al_draw_line(startX, game_data::hitPosition*constant::kPixelScale, startX+playFieldWidth, game_data::hitPosition*constant::kPixelScale,
                 al_map_rgb(255, 0, 0), constant::kHitlineThickness);
    // border
    al_draw_line(startX, 0, startX, constant::kScreenH, al_map_rgb(255, 255, 255), constant::kBorderlineThickness);
    al_draw_line(startX+playFieldWidth, 0, startX+playFieldWidth, constant::kScreenH, al_map_rgb(255, 255, 255), constant::kBorderlineThickness);

    // key
    for (int i=0; i<isKeyPressed.size(); ++i) {
        if (!isKeyPressed[i]) continue;
        al_draw_filled_rectangle(startX + i*constant::kBlockWidth, game_data::hitPosition*constant::kPixelScale, 
                                 startX + (i+1)*constant::kBlockWidth, constant::kScreenH, 
                                 al_map_rgb(255, 255, 255));
    }

    // Draw score & acc
    Skin::GetInstance().DrawScore(game_data::score, game_data::accuracy);

    // Draw Combo (magic number 0.1 is total seconds of the animation)
    if (prevCombo<game_data::combo) comboAnimationTick=constant::kFPS*0.1;
    comboAnimationTick=std::max(0, comboAnimationTick);
    Skin::GetInstance().DrawCombo(game_data::combo, pow(1.5, 1.f*comboAnimationTick/(constant::kFPS*0.1)));
    comboAnimationTick-=1;
    prevCombo=game_data::combo;

    // Draw score hit
    if (prevHit300H!=game_data::hit300H) {
        prevHit300H=game_data::hit300H;
        drawHitValue=320;
        hitvalueAnimationTick=kMaxHitvalueAnimationTick;
    }
    else if (prevHit300!=game_data::hit300) {
        prevHit300=game_data::hit300;
        drawHitValue=300;
        hitvalueAnimationTick=kMaxHitvalueAnimationTick;
    }
    else if (prevHit200!=game_data::hit200) {
        prevHit200=game_data::hit200;
        drawHitValue=200;
        hitvalueAnimationTick=kMaxHitvalueAnimationTick;
    }
    else if (prevHit100!=game_data::hit100) {
        prevHit100=game_data::hit100;
        drawHitValue=100;
        hitvalueAnimationTick=kMaxHitvalueAnimationTick;
    }
    else if (prevHit50!=game_data::hit50) {
        prevHit50=game_data::hit50;
        drawHitValue=50;
        hitvalueAnimationTick=kMaxHitvalueAnimationTick;
    }
    else if (prevMiss!=game_data::miss) {
        prevMiss=game_data::miss;
        drawHitValue=0;
        hitvalueAnimationTick=kMaxHitvalueAnimationTick;
    }
    hitvalueAnimationTick=std::max(0, hitvalueAnimationTick);
    if (hitvalueAnimationTick>0) Skin::GetInstance().DrawHit(drawHitValue, pow(1.5, 1.f*hitvalueAnimationTick/kMaxHitvalueAnimationTick));
    hitvalueAnimationTick-=1;

    // playtime
    al_draw_line(0, constant::kScreenH, constant::kScreenW * (game_data::gamePosition/game_data::playtimeLength), constant::kScreenH, al_map_rgba(0, 255, 0, 150), constant::kPlaytimeLineThickness*2);

    // HP bar
    al_draw_line(startX+playFieldWidth*1.1, constant::kScreenH, startX+playFieldWidth*1.1, 1.f*constant::kScreenH*3/5 + 1.f*constant::kScreenH*2/5*(1.-game_data::hp/1000.), al_map_rgb(255, 192, 203), constant::kHpLineThickness);

    // hitbar(error bar)
    int halfX=constant::kScreenW/2;
    int perfectHitWindow=64 - 3*game_data::OD;
    perfectHitWindow*=game_data::hitbarScale;
    int goodHitWindow=127 - 3*game_data::OD;
    goodHitWindow*=game_data::hitbarScale;
    int mehHitWindow=151 - 3*game_data::OD;
    mehHitWindow*=game_data::hitbarScale;
    al_draw_line(screenMiddleX-mehHitWindow, constant::kHitbarPosition*constant::kPixelScale,
                 screenMiddleX+mehHitWindow, constant::kHitbarPosition*constant::kPixelScale, 
                 al_map_rgb(200, 170, 0), constant::kHitbarWidth);
    al_draw_line(screenMiddleX-goodHitWindow, constant::kHitbarPosition*constant::kPixelScale, 
                 screenMiddleX+goodHitWindow, constant::kHitbarPosition*constant::kPixelScale, 
                 al_map_rgb(0, 255, 0), constant::kHitbarWidth);
    al_draw_line(screenMiddleX-perfectHitWindow, constant::kHitbarPosition*constant::kPixelScale, 
                 screenMiddleX+perfectHitWindow, constant::kHitbarPosition*constant::kPixelScale, 
                 al_map_rgb(0, 255, 255), constant::kHitbarWidth);
    al_draw_line(screenMiddleX-5, constant::kHitbarPosition*constant::kPixelScale,
                 screenMiddleX+5, constant::kHitbarPosition*constant::kPixelScale, 
                 al_map_rgb(150, 150, 150), constant::kHitbarWidth*3);
    auto revHitResults{game_data::hitResults};
    std::reverse(revHitResults.begin(), revHitResults.end());
    double avergeHitError=0;
    int count=0;
    for (auto& result : revHitResults) {
        double a=1.f*(10000-(game_data::gamePosition-result.time))/10000*255;
        if (a<=0) break;
        int hitError=result.hitError*(result.isErrorPositive?1:-1)*game_data::hitbarScale;
        count++;
        avergeHitError+=hitError;
        if (abs(hitError)>mehHitWindow) continue;
        else if (abs(hitError)>goodHitWindow) {
            al_draw_line(screenMiddleX+hitError-2, constant::kHitbarPosition*constant::kPixelScale,
                         screenMiddleX+hitError+2, constant::kHitbarPosition*constant::kPixelScale, 
                         al_map_rgba(200, 170, 0, a), constant::kHitbarWidth*3);
        }
        else if (abs(hitError)>perfectHitWindow) {
            al_draw_line(screenMiddleX+hitError-2, constant::kHitbarPosition*constant::kPixelScale,
                         screenMiddleX+hitError+2, constant::kHitbarPosition*constant::kPixelScale, 
                         al_map_rgba(0, 255, 0, a), constant::kHitbarWidth*3);
        }
        else {
            al_draw_line(screenMiddleX+hitError-2, constant::kHitbarPosition*constant::kPixelScale,
                         screenMiddleX+hitError+2, constant::kHitbarPosition*constant::kPixelScale, 
                         al_map_rgba(0, 255, 255, a), constant::kHitbarWidth*3);
        }
    }
    if (count) avergeHitError/=count;
    avergeHitError=std::clamp(avergeHitError, -1.0*mehHitWindow, 1.0*mehHitWindow);
    al_draw_filled_triangle(screenMiddleX+avergeHitError, constant::kHitbarPosition*constant::kPixelScale,
                            screenMiddleX+avergeHitError-25, constant::kHitbarPosition*constant::kPixelScale-20,
                            screenMiddleX+avergeHitError+25, constant::kHitbarPosition*constant::kPixelScale-20,
                            al_map_rgb(255, 255, 255));

    // Mode Selection
    int selection=0;
    if (game_data::isAuto) selection+=1;
    if (game_data::isDoubleTime) selection+=2;
    if (game_data::isNoFailed) selection+=4;
    Skin::GetInstance().DrawMod(selection);
}

void HUD::DrawBackground() 
{
    al_draw_filled_rectangle(0, 0, constant::kScreenW, constant::kScreenH, al_map_rgba(0, 0, 0, 255*game_data::backgroundDim));
    al_draw_filled_rectangle(startX, 0, startX+playFieldWidth, constant::kScreenH, al_map_rgb(50, 50, 50));
}


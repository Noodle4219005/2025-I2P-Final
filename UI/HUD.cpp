#include "HUD.h"
#include "util/Constant.h"
#include "util/GameData.h"
#include "Skin/Skin.h"

#include <allegro5/allegro_primitives.h>
#include <iostream>

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
    startX=screenMiddleX - constant::kBlockWidth*(game_data::nkey/2);
    playFieldWidth=constant::kBlockWidth*(game_data::nkey);
    musicBarX = constant::kScreenW * 0.01f;
    musicBarY = constant::kScreenH * 0.05f;
    musicBarWidth = std::max(constant::kScreenW * 0.005f, 7.f);
    musicBarHeight = constant::kScreenH * 0.9f;
    decltype(isKeyPressed)().swap(isKeyPressed);
    isKeyPressed.assign(game_data::nkey, 0);
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

    // playtime
    al_draw_line(0, constant::kScreenH, constant::kScreenW * (game_data::gamePosition/game_data::playtimeLength), constant::kScreenH, al_map_rgba(0, 255, 0, 150), constant::kPlaytimeLineThickness*2);
}

void HUD::DrawBackground() 
{
    al_draw_filled_rectangle(startX, 0, startX+playFieldWidth, constant::kScreenH, al_map_rgb(50, 50, 50));
}


#include "HUD.h"

#include <util/Constant.h>
#include <util/GameData.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

HUD& HUD::GetInstance()
{
    static HUD instance{};
    return instance;
}

HUD::HUD()
{
    screenMiddleX=constant::kScreenW/2;
    startX=screenMiddleX - constant::kBlockWidth*(game_data::nkey/2);
    playFieldWidth=constant::kBlockWidth*(game_data::nkey);
    musicBarX = constant::kScreenW * 0.01f;
    musicBarY = constant::kScreenH * 0.05f;
    musicBarWidth = std::max(constant::kScreenW * 0.005f, 7.f);
    musicBarHeight = constant::kScreenH * 0.9f;
}

void HUD::DrawForeground()
{
    // hitbar
    al_draw_line(startX, game_data::hitPosition*constant::kPixelScale, startX+playFieldWidth, game_data::hitPosition*constant::kPixelScale,
                 al_map_rgb(255, 0, 0), constant::kHitlineThickness);
    // border
    al_draw_line(startX, 0, startX, constant::kScreenH, al_map_rgb(255, 255, 255), constant::kBorderlineThickness);
    al_draw_line(startX+playFieldWidth, 0, startX+playFieldWidth, constant::kScreenH, al_map_rgb(255, 255, 255), constant::kBorderlineThickness);

    // score
    // std::cout<<"score: "<<game_data::score<<std::endl;
}

void HUD::DrawBackground() 
{
    al_draw_filled_rectangle(startX, 0, startX+playFieldWidth, constant::kScreenH, al_map_rgb(50, 50, 50));
}


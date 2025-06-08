#include "Hold.h"
#include "util/Constant.h"
#include "util/GameData.h"
#include "util/ErrorCalculator.h"

#include <iostream>
#include <allegro5/allegro_primitives.h>

void Hold::Update() 
{
    double deltaTime=game_data::gamePosition-m_lastUpdateTime;
    m_positionY+=m_speed*deltaTime*game_data::scrollSpeedMultiplier;
    m_positionEndY+=m_speed*deltaTime*game_data::scrollSpeedMultiplier;
    m_lastUpdateTime=game_data::gamePosition;
    // TODO: isAvailable and isAlive
    if (game_data::gamePosition>m_time && (GetHitValue(m_time)==NONE || GetHitValue(m_time)==MISS)) {
        m_hitValue=MISS;
        game_data::miss++;
        m_isAvailable=0;
    }
    if (!m_isAvailable && m_positionEndY>constant::kScreenH) {
        m_isAlive=0;
    }
}


void Hold::Draw() const 
{
    ALLEGRO_COLOR color;
    int a=(m_isAvailable)?255:100;
    int screenMiddleX=constant::kScreenW/2;
    int startX=screenMiddleX - constant::kBlockWidth*(game_data::nkey/2) - ((game_data::nkey%2)?(constant::kBlockWidth/2):(0));
    if (game_data::nkey%2!=0 && m_column==game_data::nkey/2) {
        color=al_map_rgba(255, 255, 0, a);
    }
    else if (m_column<game_data::nkey/2) {
        if (m_column%2) { 
            color=al_map_rgba(255, 255, 255, a);
        }
        else {
            color=al_map_rgba(0, 255, 255, a);
        }
    }
    else {
        if (m_column%2) { 
            color=al_map_rgba(0, 255, 255, a);
        }
        else {
            color=al_map_rgba(255, 255, 255, a);
        }
    }
    // std::cout<<"hold: "<<m_positionY<<" "<<m_positionEndY<<"\n";
    al_draw_rectangle(startX + m_column*constant::kBlockWidth, m_positionY, 
                      startX + (m_column+1)*constant::kBlockWidth, m_positionEndY-constant::kBlockHeight, 
                      al_map_rgba(255, 0, 0, a), 5);
    al_draw_filled_rectangle(startX + m_column*constant::kBlockWidth, m_positionY, 
                             startX + (m_column+1)*constant::kBlockWidth, m_positionEndY-constant::kBlockHeight, 
                             color);
}

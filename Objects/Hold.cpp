#include "Hold.h"
#include "util/Constant.h"
#include "util/GameData.h"
#include "util/ErrorCalculator.h"

#include <iostream>
#include <allegro5/allegro_primitives.h>

void Hold::OnKeyDown()
{
    if (!m_isAvailable) return;
    if (!m_isFirstKeyDown) return;
    if (game_data::gamePosition<m_time && (GetHitValue(m_time)==MISS || GetHitValue(m_time)==NONE)) return;
    m_isKeyDown=true;
    m_isFirstKeyDown=false;
    m_firstKeyPosition=game_data::gamePosition;
}

void Hold::OnKeyUp()
{
    if (!m_isAvailable) return;
    if (m_isFirstKeyDown) return;
    m_isKeyDown=false;
    m_isAvailable=false;
    m_hitValue=GetHoldValue(abs(m_firstKeyPosition-m_time), m_endTime);
    m_hitError=std::min((abs(game_data::gamePosition-m_endTime)+abs(m_firstKeyPosition-m_time))/2, 151. - 3*game_data::OD);
    IncrementHitCounter(m_hitValue, m_hitError, (game_data::gamePosition-m_endTime+m_firstKeyPosition-m_time)>0);
}

void Hold::Update() 
{
    double deltaTime=std::max(game_data::gamePosition-m_lastUpdateTime, 0.);
    if (m_isFirstKeyDown && m_isKeyDown) m_positionY=game_data::hitPosition*constant::kPixelScale;
    else m_positionY+=m_speed*deltaTime*game_data::scrollSpeedMultiplier;
    m_positionEndY+=m_speed*deltaTime*game_data::scrollSpeedMultiplier;
    m_lastUpdateTime=game_data::gamePosition;
    if (m_isKeyDown && m_isAvailable && game_data::gamePosition-m_time>m_accumulatedCombo*100) {
        m_accumulatedCombo++;
        game_data::combo++;
    }
    // TODO: isAvailable and isAlive
    if (!m_isKeyDown && m_isAvailable && game_data::gamePosition>m_time && (GetHitValue(m_time)==NONE || GetHitValue(m_time)==MISS || GetHitValue(m_time)==MEH)) {
        // The magic number is from the ErrorCalculator and ppy osu mania judgement.
        IncrementHitCounter(MISS, 188 - 3*game_data::OD, true);
        m_isAvailable=0;
    }
    if (m_isKeyDown && m_isAvailable && game_data::gamePosition>m_endTime && (GetHitValue(m_endTime)==NONE || GetHitValue(m_endTime)==MISS || GetHitValue(m_endTime)==MEH)) {
        IncrementHitCounter(MEH, 151 - 3*game_data::OD, true);
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
    else if (m_column < (game_data::nkey/2)) {
        if (m_column%2) { 
            color=al_map_rgba(0, 255, 255, a);
        }
        else {
            color=al_map_rgba(255, 255, 255, a);
        }
    }
    else {
        if ((m_column + game_data::nkey%2)%2) {
            color=al_map_rgba(255, 255, 255, a);
        }
        else {
            color=al_map_rgba(0, 255, 255, a);
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

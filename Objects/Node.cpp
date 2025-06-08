#include "Node.h"
#include "util/GameData.h"
#include "util/Constant.h"
#include "util/ErrorCalculator.h"

#include <allegro5/allegro_primitives.h>

void Node::OnKeyDown() 
{
    if (!m_isAvailable) return;
    if (game_data::gamePosition<m_time && (GetHitValue(m_time)==MISS || GetHitValue(m_time)==NONE)) return;
    m_hitValue=GetHitValue(m_time);
    m_hitError=abs(game_data::gamePosition-m_time);
    IncrementHitCounter(m_hitValue, m_hitError, game_data::gamePosition>m_time);
    m_isAvailable=false;
    m_isAlive=false;
}

void Node::Draw() const 
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
    al_draw_rectangle(startX + m_column*constant::kBlockWidth, m_positionY, 
                      startX + (m_column+1)*constant::kBlockWidth, m_positionY-constant::kBlockHeight, 
                      al_map_rgba(255, 0, 0, a), 5);
    al_draw_filled_rectangle(startX + m_column*constant::kBlockWidth, m_positionY, 
                             startX + (m_column+1)*constant::kBlockWidth, m_positionY-constant::kBlockHeight, 
                             color);
}

#include "HitObject.h"
#include "util/GameData.h"
#include "util/Constant.h"
#include "util/ErrorCalculator.h"
#include <allegro5/allegro_primitives.h>
#include <iostream>

HitObject::HitObject(int x, int time, int type, int endTime, const std::string& hitSoundPath) :
    m_column(x), m_time(time), m_type(type), m_endTime(endTime), m_hitSoundPath(hitSoundPath) { ; }

HitObject::HitObject(int x, int time, int type, const std::string& hitSoundPath) :
    m_column(x), m_time(time), m_type(type), m_endTime(time), m_hitSoundPath(hitSoundPath) { ; }

int HitObject::GetStartTime()
{
    return m_time;
}

int HitObject::GetColumn() 
{
    return m_column;
}

void HitObject::Update() 
{
    if (game_data::gamePosition > m_time && GetHitValue(m_perfectHitPosition) == NONE) {
        m_isAlive=0;
    }
}

void HitObject::Draw() const
{
    float y=(game_data::gamePosition-m_time)/game_data::GetScrollMilisecond(240)*constant::kScreenH+constant::kScreenH-240;
    al_draw_filled_circle((m_column+1)*256, y, 60, al_map_rgb(0, 255, 255));
}

bool HitObject::IsAlive() {
    return m_isAlive;
}

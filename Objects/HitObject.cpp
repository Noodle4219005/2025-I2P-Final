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
    // TODO: isAvailable and isAlive
    if (game_data::gamePosition>m_time && GetHitValue(NONE)) {
        m_isAvailable=0;
    }
    if (!m_isAvailable && GetPositionY()>constant::kScreenH) {
        m_isAlive=0;
    }
}

float HitObject::GetPositionY() const
{
    return (game_data::gamePosition-m_time)/game_data::GetScrollMilisecond()*constant::kScreenH+game_data::hitPosition*constant::kPixelScale;
}

void HitObject::Draw() const
{
    int a=(m_isAvailable)?255:100;
    al_draw_filled_rectangle((m_column+1)*256, GetPositionY(), (m_column+2)*256, GetPositionY()-50, al_map_rgba(0, 255, 255, a));
}

bool HitObject::IsAlive() const
{
    return m_isAlive;
}

bool HitObject::IsAvailable() const
{
    return m_isAvailable;
}

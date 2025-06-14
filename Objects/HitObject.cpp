#include "HitObject.h"
#include "util/GameData.h"
#include "util/Constant.h"
#include "util/ErrorCalculator.h"

#include <allegro5/allegro_primitives.h>
#include <iostream>

HitObject::HitObject(int x, int time, int type, int endTime, double positionY, double positionEndY, const std::string& hitSoundPath) :
    m_column(x), m_time(time), m_type(type), m_endTime(endTime), m_positionY(positionY), m_positionEndY(positionEndY), m_hitSoundPath(hitSoundPath)
{
    Init();
}

HitObject::HitObject(int x, int time, int type, double positionY, const std::string& hitSoundPath) :
    m_column(x), m_time(time), m_type(type), m_endTime(time), m_positionY(positionY), m_positionEndY(positionY), m_hitSoundPath(hitSoundPath)
{
    Init();
}

HitObject::HitObject()
{
    Init();
}

void HitObject::Init()
{
    m_lastUpdateTime=-constant::kHitobjectPreviewThreshold;
    m_speed=constant::kScreenH/game_data::GetScrollMilisecond();
    m_accumulatedCombo=0;
}

int HitObject::GetStartTime() const
{
    return m_time;
}

int HitObject::GetEndTime() const
{
    return m_endTime;
}

int HitObject::GetColumn()  const
{
    return m_column;
}

void HitObject::Update() 
{
    double deltaTime=std::max(game_data::gamePosition-m_lastUpdateTime, 0.);
    m_positionY+=m_speed*deltaTime*game_data::scrollSpeedMultiplier;
    m_positionEndY+=m_speed*deltaTime*game_data::scrollSpeedMultiplier;
    m_lastUpdateTime=game_data::gamePosition;
    // TODO: isAvailable and isAlive
    if (m_isAvailable && game_data::gamePosition>m_time && (GetHitValue(m_time)==NONE || GetHitValue(m_time)==MISS || GetHitValue(m_time)==MEH)) {
        // The magic number is from the ErrorCalculator and ppy osu mania judgement.
        IncrementHitCounter(MISS, 188 - 3*game_data::OD, true);
        m_isAvailable=0;
    }
    if (!m_isAvailable && m_positionY>constant::kScreenH) {
        m_isAlive=0;
    }
}

double HitObject::GetPositionY() const
{
    return m_positionY;
}

void HitObject::Draw() const
{
    int a=(m_isAvailable)?255:100;
    if (m_isAvailable) {
        al_draw_filled_rectangle((m_column+1)*256, m_positionY, (m_column+2)*256, m_positionY-50, al_map_rgb(0, 255, 255));
    }
    else {
        al_draw_filled_rectangle((m_column+1)*256, m_positionY, (m_column+2)*256, m_positionY-50, al_map_rgb(255, 0, 0));
    }
}

int HitObject::GetType() const
{
    return m_type;
}

bool HitObject::IsAlive() const
{
    return m_isAlive;
}

bool HitObject::IsAvailable() const
{
    return m_isAvailable;
}

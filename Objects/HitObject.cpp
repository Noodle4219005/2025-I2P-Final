#include "HitObject.h"

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

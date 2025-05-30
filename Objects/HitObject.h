#pragma once
#include "Node.h"
#include "Hold.h"

#include <string>

class HitObject
{
public:
    HitObject(int x, int time, int type, int endTime, const std::string& hitSoundPath);
    HitObject(int x, int time, int type, const std::string& hitSoundPath);
    HitObject()=default;
    int GetStartTime();
    int GetColumn();
    bool isAlive=true;

protected:
    int m_column;
    int m_time;
    int m_endTime;
    int m_type;
    std::string m_hitSoundPath;
};
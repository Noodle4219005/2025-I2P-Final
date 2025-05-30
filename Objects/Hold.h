#pragma once

#include "HitObject.h"

class Hold : public HitObject
{
public:
    Hold(int x, int time, int type, int endTime, const std::string& hitSoundPath) :
        HitObject(x, time, type, endTime, hitSoundPath) { ; }
private:
};
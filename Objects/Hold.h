#pragma once

#include "HitObject.h"

class Hold : public HitObject
{
public:
    Hold(int x, int time, int type, int endTime, float positionY, float positionEndY, const std::string& hitSoundPath) :
        HitObject(x, time, type, endTime, positionY, positionEndY, hitSoundPath) { ; }
    void OnKeyDown() override;
    void OnKeyUp() override;
    void Update() override;
    void Draw() const override;
private:
};
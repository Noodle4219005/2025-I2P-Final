#pragma once
#include "Skin/Skin.h"
#include "UI/Component/Image.hpp"
#include "util/GameData.h"
#include "HitObject.h"

#include <iostream>

class Node : public HitObject
{
public:
    Node(int x, int time, int type, double positionY, const std::string& hitSoundPath) :
        HitObject(x, time, type, positionY, hitSoundPath) { ; }
    void OnKeyDown() override;
    // void OnKeyUp() override;
    // void Update() override;
    void Draw() const override;

private:
};

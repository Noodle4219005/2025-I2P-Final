#pragma once
#include "Skin/Skin.h"
#include "UI/Component/Image.hpp"
#include "util/GameData.h":
#include "HitObject.h"

class Node : public HitObject
{
public:
    Node(int x, int time, int type, const std::string& hitSoundPath) :
        HitObject(x, time, type, hitSoundPath) { ; }
    void OnKeyDown() override;
    void OnKeyUp() override;
    void Update() override;
    void Draw() const override;

private:
};

#pragma once
#include "Skin/Skin.h"
#include "UI/Component/Image.hpp"
#include "util/GameData.h":
#include "HitObject.h"

class Node
{
public:
    void Draw() const;
    void KeyDown();
    void Update();

private:
    int m_perfectHitPosition=0;
    bool m_isKeyDown=false;
    bool m_isFirstKeyDown=true;
    int m_firstKeyPosition=0;
    int m_column=-1;
};

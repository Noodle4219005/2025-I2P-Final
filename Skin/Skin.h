#pragma once
#include "SkinImage.h"
#include <vector>

class Skin 
{
public:
    void DrawHold(int start, int end) const;
    void Draw(int start, int end) const;
private:
    int m_keys;
    int m_columnStart=136;
    int m_columnRight=19;
    int m_columnSpacing=0;
    int m_columnWidth=30;
    int m_columnLineWidth=2;
    float m_barLineHeight=1.2;
    int m_lightPosition=413;
    std::vector<int> m_lightingNWidth;
};
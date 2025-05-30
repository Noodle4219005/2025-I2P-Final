#pragma once
#include "SkinImage.h"

#include <vector>
#include <string>
#include <unordered_map>

class Skin 
{
public:
    Skin();
    void DrawHold(int start, int end) const;
    void Draw(int start, int end) const;
private:
    enum Sections
    {
        GENERAL,
        FONTS,
        MANIA
    };

    void Parse(const std::string& str);

    int m_section;

    // General
    int m_animationFramerate=1;
    int m_comboBurstRandom=0;
    std::string m_customComboBurstSounds;

    //  Fonts
    std::string m_scorePrefix="score";
    int m_scoreOverlap=0;
    std::string m_comboPrefix="score";
    int m_comboOverlap=0;

    // Mania
    int m_keys;

    struct ManiaSkinData 
    {
        int columnStart=136;
        int columnRight=19;
        int columnSpacing=0;
        int columnWidth=30;
        int columnLineWidth=2;
        float barLineHeight=1.2;
        int lightPosition=413;
        std::vector<int> lightingNWidth;
    };

    std::unordered_map<int, ManiaSkinData> m_nkManiaSkin;
};
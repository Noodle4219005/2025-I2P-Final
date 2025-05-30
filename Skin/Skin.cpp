#include "Skin/Skin.h" 
#include "util/Constant.h"

#include <fstream>
#include <sstream>

// Watch out, the bitmap bottom hit the line is the zero point.
 
Skin::Skin() 
{
    std::ifstream skinFileStream(constant::kSkinPath+"/skin.ini");
    std::string token;
    while (getline(skinFileStream, token)) {
        // If commented, then skip this line.
        if (token.empty()) continue;
        while (token.front()==' ') token=token.substr(1, token.size()-1);
        if (token[0]=='/'&&token[1]=='/') continue;

        if (token=="[General]") {
            m_section=GENERAL;
        }
        else if (token=="[Colours]") {
            m_section=-1;
        }
        else if (token=="[Fonts]") {
            m_section=FONTS;
        }
        else if (token=="[CatchTheBeat]") {
            m_section=-1;
        }
        else if (token=="[Mania]") {
            m_section=MANIA;
        }
        else {
            Parse(token);
        }
    }
}

void Skin::Parse(const std::string& str)
{
    if (str.empty()) return;
    std::string token, value;
    std::istringstream iss(str);
    getline(iss, token, ':');
    getline(iss, value, ' ');
    getline(iss, value);

    if (m_section==GENERAL) {
        if (token=="AnimationFramerate") {
            if (stoi(value)==-1) return;
            m_animationFramerate=stoi(value);
        }
        else if (token=="ComboBurstRandom") {
            m_comboBurstRandom=stoi(value);
        }
        else if (token=="CustomComboBurstSounds") {
            m_customComboBurstSounds=value;
        }
    }
    else if (m_section==FONTS) {
        if (token=="ScorePrefix") {
            m_scorePrefix=value;
        }
        else if (token=="ScoreOverlap") {
            m_scoreOverlap=stoi(value);
        }
        else if (token=="ComboPrefix") {
            m_comboPrefix=value;
        }
        else if (token=="ComboOverlap") {
            m_comboOverlap=stoi(value);
        }
    }
    else if (m_section==MANIA) {
        if (token=="Keys") {
            m_keys=stoi(value);
            m_nkManiaSkin[m_keys];
        }
        else if (token=="ColumnStart") {
            m_nkManiaSkin[m_keys].columnStart=stoi(value);
        }
        else if (token=="ColumnRight") {
        }
    }
}
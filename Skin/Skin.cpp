#include "Skin/Skin.h" 
#include "util/Constant.h"
#include "UI/Component/Image.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// Watch out, the bitmap bottom hit the line is the zero point.

Skin& Skin::GetInstance()
{
    static Skin instance{};
    return instance;
}
 
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
            m_nkManiaSkin[m_keys].columnStart=stof(value);
        }
        else if (token=="ColumnRight") {
            m_nkManiaSkin[m_keys].columnRight=stof(value);
        }
        else if (token=="ColumnSpacing") {
            m_nkManiaSkin[m_keys].columnSpacing=stof(value);
        }
        else if (token=="ColumnWidth") {
            m_nkManiaSkin[m_keys].columnWidth=stof(value);
        }
        else if (token=="ColumnLineWidth") {
            m_nkManiaSkin[m_keys].columnLineWidth=stof(value);
        }
        else if (token=="BarlineHeight") {
            m_nkManiaSkin[m_keys].barLineHeight=stof(value);
        }
    }
}

void Skin::DrawScore(int score, float acc) const
{
    int digit;
    int count=constant::kMaxScore;
    int startX=constant::kScreenW;
    int startAccuracyY=0;
    // Score
    for (int i=count; i>0; i/=10) {
        digit=score%10;
        std::string filename=constant::kSkinPath+"\\"+m_scorePrefix+"-"+std::to_string(digit)+".png";
        Engine::Image image(filename, startX, 0, 0, 0, 1, 0);
        startX-=image.GetBitmapWidth() - m_scoreOverlap*constant::kPixelScale;
        startAccuracyY=std::max(startAccuracyY, image.GetBitmapHeight());
        image.Draw();
        score/=10;
    }

    // Accuracy
    acc=acc*100;
    // Prevent the acc will be override after counting numDigits
    float preAcc=acc;
    startX=constant::kScreenW;
    int numDigits=0;
    while ((int)acc) {
        numDigits++;
        acc=static_cast<int>(acc) / 10;
    }
    acc=preAcc;
    // percent
    {
        std::string filename=constant::kSkinPath+"\\"+m_scorePrefix+"-percent.png";
        Engine::Image image(filename, startX, startAccuracyY, 0, 0, 1, 0);
        startX-=image.GetBitmapWidth() - m_scoreOverlap*constant::kPixelScale;
        image.Draw();
    }
    for (int i=1; i<=5; ++i) {
        digit=static_cast<int>(acc)%10;
        if (i>numDigits && i>=4) break;
        std::string filename=constant::kSkinPath+"\\"+m_scorePrefix+"-"+std::to_string(digit)+".png";
        Engine::Image image(filename, startX, startAccuracyY, 0, 0, 1, 0);
        startX-=image.GetBitmapWidth() - m_scoreOverlap*constant::kPixelScale;
        image.Draw();
        // dot
        if (i==2) {
            std::string filename=constant::kSkinPath+"\\"+m_scorePrefix+"-dot.png";
            Engine::Image image(filename, startX, startAccuracyY, 0, 0, 1, 0);
            image.Position=Engine::Point{1.f*startX + (image.GetBitmapWidth() - m_scoreOverlap*constant::kPixelScale)*2, (float)startAccuracyY};
            startX-=(image.GetBitmapWidth() - m_scoreOverlap*constant::kPixelScale)*3;

            image.Draw();
        }
        acc=static_cast<int>(acc)/10;
    }
}

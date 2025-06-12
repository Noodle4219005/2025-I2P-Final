#include "Skin/Skin.h" 
#include "util/Constant.h"
#include "UI/Component/Image.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

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
        } else if (token=="[Colours]") {
            m_section=-1;
        } else if (token=="[Fonts]") {
            m_section=FONTS;
        } else if (token=="[CatchTheBeat]") {
            m_section=-1;
        } else if (token=="[Mania]") {
            m_section=MANIA;
        } else {
            Parse(token);
        }
    }

    // Init the pictures
    for (int i=0; i<=9; ++i) {
        std::string filename=constant::kSkinPath+"\\"+m_scorePrefix+"-"+std::to_string(i)+".png";
        Engine::Image image(filename, 0, 0, 0, 0, 1, 0);
    }
    {
        std::string filename=constant::kSkinPath+"\\"+m_scorePrefix+"-percent.png";
        Engine::Image image(filename, 0, 0, 0, 0, 1, 0);
    }
    {
        std::string filename=constant::kSkinPath+"\\"+m_scorePrefix+"-dot.png";
        Engine::Image image(filename, 0, 0, 0, 0, 1, 0);
    }
    for (int i=0; i<9; ++i) {
        std::string filename=constant::kSkinPath+"\\"+m_comboPrefix+"-"+std::to_string(i)+".png";
        Engine::Image image(filename, 0, 0, 0, 0, 1, 0);
    }
    {
        std::string filename=constant::kSkinPath+"\\"+"mania-hit300g-0.png";
        Engine::Image image(filename, 0, 0, 0, 0, 1, 0);
    }
    {
        std::string filename=constant::kSkinPath+"\\"+"mania-hit300.png";
        Engine::Image image(filename, 0, 0, 0, 0, 1, 0);
    }
    {
        std::string filename=constant::kSkinPath+"\\"+"mania-hit200.png";
        Engine::Image image(filename, 0, 0, 0, 0, 1, 0);
    }
    {
        std::string filename=constant::kSkinPath+"\\"+"mania-hit100.png";
        Engine::Image image(filename, 0, 0, 0, 0, 1, 0);
    }
    {
        std::string filename=constant::kSkinPath+"\\"+"mania-hit50.png";
        Engine::Image image(filename, 0, 0, 0, 0, 1, 0);
    }
    {
        std::string filename=constant::kSkinPath+"\\"+"mania-hit0.png";
        Engine::Image image(filename, 0, 0, 0, 0, 1, 0);
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
        } else if (token=="ComboBurstRandom") {
            m_comboBurstRandom=stoi(value);
        } else if (token=="CustomComboBurstSounds") {
            m_customComboBurstSounds=value;
        }
    } else if (m_section==FONTS) {
        if (token=="ScorePrefix") {
            m_scorePrefix=value;
        } else if (token=="ScoreOverlap") {
            m_scoreOverlap=stoi(value);
        } else if (token=="ComboPrefix") {
            m_comboPrefix=value;
        } else if (token=="ComboOverlap") {
            m_comboOverlap=stoi(value);
        }
    } else if (m_section==MANIA) {
        if (token=="Keys") {
            m_keys=stoi(value);
            m_nkManiaSkin[m_keys];
        } else if (token=="ColumnStart") {
            m_nkManiaSkin[m_keys].columnStart=stod(value);
        } else if (token=="ColumnRight") {
            m_nkManiaSkin[m_keys].columnRight=stod(value);
        } else if (token=="ColumnSpacing") {
            m_nkManiaSkin[m_keys].columnSpacing=stod(value);
        } else if (token=="ColumnWidth") {
            m_nkManiaSkin[m_keys].columnWidth=stod(value);
        } else if (token=="ColumnLineWidth") {
            m_nkManiaSkin[m_keys].columnLineWidth=stod(value);
        } else if (token=="BarlineHeight") {
            m_nkManiaSkin[m_keys].barLineHeight=stod(value);
        }
    }
}

void Skin::DrawScore(int score, double acc) const
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
    startX=constant::kScreenW;
    int numDigits=CalNumDigits(static_cast<int>(acc));
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
            image.Position=Engine::Point{1.f*startX - std::max((image.GetBitmapWidth() - m_scoreOverlap*constant::kPixelScale), 5.0)*2, (double)startAccuracyY};
            startX-=std::max(image.GetBitmapWidth() - m_scoreOverlap*constant::kPixelScale, 5.0)*3;

            image.Draw();
        }
        acc=static_cast<int>(acc)/10;
    }
}

void Skin::DrawCombo(int combo, double comboExpand) const
{
    int numDigits=CalNumDigits(combo);
    int pictureWidth=0;
    int pictureHeight=0;
    {
        std::string filename=constant::kSkinPath+"\\"+m_comboPrefix+"-0.png";
        Engine::Image image(filename, 0, 0, 0, 0, 1, 0);
        pictureWidth=image.GetBitmapWidth();
        pictureHeight=image.GetBitmapHeight();
    }
    int startX=(pictureWidth - constant::kPixelScale*m_comboOverlap)*numDigits/2 + constant::kScreenW/2;
    int digit=0;

    for (int i=0; i<numDigits; ++i) {
        digit=combo%10;
        std::string filename=constant::kSkinPath+"\\"+m_comboPrefix+"-"+std::to_string(digit)+".png";
        Engine::Image image(filename, startX, constant::kComboPosition*constant::kPixelScale, pictureWidth, pictureHeight*comboExpand, 0.5, 0.5);
        startX-=image.GetBitmapWidth() - m_comboOverlap*constant::kPixelScale;
        image.Draw();
        combo/=10;
    }
}

int Skin::CalNumDigits(int number) const
{
    int numDigits=0;
    while (number) {
        numDigits++;
        number=number/10;
    }
    return numDigits;
}
void Skin::DrawHit(int val, double expand) const
{
    int pictureWidth=0;
    int pictureHeight=0;
    std::string filename;
    switch (val)
    {
    case 320: 
        filename=constant::kSkinPath+"\\"+"mania-hit300g-0.png";
        break;
    case 300: 
        filename=constant::kSkinPath+"\\"+"mania-hit300.png";
        break;
    case 200: 
        filename=constant::kSkinPath+"\\"+"mania-hit200.png";
        break;
    case 100: 
        filename=constant::kSkinPath+"\\"+"mania-hit100.png";
        break;
    case 50: 
        filename=constant::kSkinPath+"\\"+"mania-hit50.png";
        break;
    case 0: 
        filename=constant::kSkinPath+"\\"+"mania-hit0.png";
        break;
    default:
        return;
    }
    Engine::Image image(filename, constant::kScreenW/2, constant::kScorePosition*constant::kPixelScale, 0, 0, 0.5, 0.5);
    pictureWidth=image.GetBitmapWidth();
    pictureHeight=image.GetBitmapHeight();
    Engine::Image imageScaled(filename, constant::kScreenW/2, constant::kScorePosition*constant::kPixelScale, pictureWidth*expand, pictureHeight*expand, 0.5, 0.5);
    imageScaled.Draw();
}

void Skin::DrawPlaySkip() const
{
    std::string filename=constant::kSkinPath+"\\"+"play-skip.png";
    Engine::Image image(filename, 0, constant::kScreenH, constant::kScreenW, 0, 0, 1);
    image.Draw();
}

void Skin::DrawPause() const
{
    std::string filename=constant::kSkinPath+'/'+"pause-overlay.png";
    Engine::Image image(filename, 0, 0, constant::kScreenW, constant::kScreenH, 0, 0);
    image.Draw();
}

void Skin::DrawFailed() const
{
    std::string filename=constant::kSkinPath+'/'+"fail-background.png";
    Engine::Image image(filename, 0, 0, constant::kScreenW, constant::kScreenH, 0, 0);
    image.Draw();

}

void Skin::DrawMod(int selection) const
{
    int startX=constant::kScreenW;
    int positionY=constant::kModSeletionPosition*constant::kPixelScale;
    if (selection&1) {
        std::string filename=constant::kSkinPath+'/'+"selection-mod-autoplay.png";
        Engine::Image image(filename, startX, positionY, 
                            0, 0, 1, 0);
        startX-=image.GetBitmapWidth();
        image.Draw();
    }
    if (selection&2) {
        std::string filename=constant::kSkinPath+'/'+"selection-mod-doubletime.png";
        Engine::Image image(filename, startX, positionY, 
                            0, 0, 1, 0);
        startX-=image.GetBitmapWidth();
        image.Draw();
    }
    if (selection&4) {
        std::string filename=constant::kSkinPath+'/'+"selection-mod-nofail.png";
        Engine::Image image(filename, startX, positionY, 
                            0, 0, 1, 0);
        startX-=image.GetBitmapWidth();
        image.Draw();
    }
}

void Skin::DrawRankingPanel() const 
{
    std::string filename=constant::kSkinPath+'/'+"ranking-panel.png";
    Engine::Image image(filename, 0, 0, 
                        constant::kScreenW, constant::kScreenH, 0, 0);
    image.Draw();
}

void Skin::DrawRankingScore(int score, int x, int y) const
{
    int digit;
    int count=constant::kMaxScore;
    int startX=x*constant::kPixelScale;
    std::vector<int> scoreStack;
    // Score
    for (int i=count; i>0; i/=10) {
        digit=score%10;
        scoreStack.push_back(digit);
        score/=10;
    }
    for (int i=count; i>0; i/=10) {
        digit=scoreStack.back(); scoreStack.pop_back();
        std::string filename=constant::kSkinPath+"\\"+m_scorePrefix+"-"+std::to_string(digit)+"@2x.png";
        Engine::Image image(filename, startX, y*constant::kPixelScale, 0, 0, 0, 0);
        startX+=image.GetBitmapWidth() - m_scoreOverlap*constant::kPixelScale;
        image.Draw();
    }
}

void Skin::DrawRankingNumber(int number, int x, int y) const
{
    int digit;
    int numDigits=std::max(CalNumDigits(number), 1);
    int count=constant::kMaxScore;
    int startX=constant::kOffsetX+x*constant::kPixelScale;
    std::string numString=std::to_string(number);
    // Score
    for (int i=0; i<numDigits; ++i) {
        int digit=numString[i]-'0';
        std::string filename=constant::kSkinPath+"\\"+m_scorePrefix+"-"+std::to_string(digit)+".png";
        Engine::Image image(filename, startX, y*constant::kPixelScale, 0, 0, 0, 0.5);
        startX+=image.GetBitmapWidth() - m_scoreOverlap*constant::kPixelScale;
        image.Draw();
    }
    std::string filename=constant::kSkinPath+"\\"+"score-x.png";
    Engine::Image image(filename, startX, y*constant::kPixelScale, 0, 0, 0, 0.5);
    image.Draw();
}

void Skin::DrawRankingAccuracy(double acc) const
{
    // Accuracy
    acc=acc*100;
    int digit=0;
    int startX=constant::kOffsetX+250*constant::kPixelScale; // 300 osu pixels
    int numDigits=CalNumDigits(static_cast<int>(acc));
    // percent
    {
        std::string filename=constant::kSkinPath+"\\"+m_scorePrefix+"-percent@2x.png";
        Engine::Image image(filename, startX, constant::kRankingMaxComboPosition*constant::kPixelScale, 0, 0, 1, 0);
        startX-=image.GetBitmapWidth() - m_scoreOverlap*constant::kPixelScale;
        image.Draw();
    }
    for (int i=1; i<=5; ++i) {
        digit=static_cast<int>(acc)%10;
        if (i>numDigits && i>=4) break;
        std::string filename=constant::kSkinPath+"\\"+m_scorePrefix+"-"+std::to_string(digit)+"@2x.png";
        Engine::Image image(filename, startX, constant::kRankingMaxComboPosition*constant::kPixelScale, 0, 0, 1, 0);
        startX-=image.GetBitmapWidth() - m_scoreOverlap*constant::kPixelScale;
        image.Draw();
        // dot
        if (i==2) {
            std::string filename=constant::kSkinPath+"\\"+m_scorePrefix+"-dot@2x.png";
            Engine::Image image(filename, startX, constant::kRankingMaxComboPosition*constant::kPixelScale, 0, 0, 1, 0);
            image.Position=Engine::Point{1.f*startX - std::max((image.GetBitmapWidth() - m_scoreOverlap*constant::kPixelScale), 5.0)*2, (double)constant::kRankingMaxComboPosition*constant::kPixelScale};
            startX-=std::max(image.GetBitmapWidth() - m_scoreOverlap*constant::kPixelScale, 5.0)*3;

            image.Draw();
        }
        acc=static_cast<int>(acc)/10;
    }

}

void Skin::DrawRankingMod(int selection) const
{
    int startX=constant::kScreenW-125*constant::kPixelScale;
    int positionY=constant::kRankingModPosition*constant::kPixelScale;
    if (selection&1) {
        std::string filename=constant::kSkinPath+'/'+"selection-mod-autoplay.png";
        Engine::Image image(filename, startX, positionY, 
                            0, 0, 1, 0);
        startX-=image.GetBitmapWidth();
        image.Draw();
    }
    if (selection&2) {
        std::string filename=constant::kSkinPath+'/'+"selection-mod-doubletime.png";
        Engine::Image image(filename, startX, positionY, 
                            0, 0, 1, 0);
        startX-=image.GetBitmapWidth();
        image.Draw();
    }
    if (selection&4) {
        std::string filename=constant::kSkinPath+'/'+"selection-mod-nofail.png";
        Engine::Image image(filename, startX, positionY, 
                            0, 0, 1, 0);
        startX-=image.GetBitmapWidth();
        image.Draw();
    }
}

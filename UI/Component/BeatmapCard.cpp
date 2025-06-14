﻿#include "BeatmapCard.h"
#include "Label.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "util/Constant.h"

#include <string>
#include <allegro5/allegro_primitives.h>
#include <sstream>
#include <functional>

using Engine::Collider, Engine::Point;

BeatmapCard::BeatmapCard(std::string name, std::string author, std::string mapper, double minDiffiulty, double maxDifficulty):
    m_name(name), m_author(author), m_mapper(mapper), m_minDiff(minDiffiulty), m_maxDiff(maxDifficulty)
{
}

BeatmapCard::BeatmapCard(std::string name, double diffiulty, int key):
    m_name(name), m_minDiff(diffiulty), m_maxDiff(-1), m_key(key)
{
}

double BeatmapCard::GetDifficulty() 
{
    return m_minDiff;
}

void BeatmapCard::OnMouseDown(int button, int mx, int my) 
{
    if (button!=1) return;
    if (m_idx!=0) return;
    int middleX=constant::kScreenW/2;
    int middleY=constant::kScreenH/2;
    int width=constant::kBeatmapCardWidth*constant::kPixelScale;
    int height=constant::kBeatmapCardHeight*constant::kPixelScale;
    int x=middleX-width/2;
    int y=middleY-height/2;
    bool mouseIn = Collider::IsPointInRect(Point{(double)mx, (double)my},
                                           Point{(double)x, (double)y},
                                           Point{(double)width, (double)height}
    );
    if (mouseIn) m_onClickedCallBack();

}

void BeatmapCard::SetOnClickCallback(std::function<void(void)> onClickCallback) {
    m_onClickedCallBack = onClickCallback;
}

void BeatmapCard::Update(int idx)
{
    m_idx=idx;
}

void BeatmapCard::Draw() const
{
    int middleX=constant::kScreenW/2;
    int middleY=constant::kScreenH/2;
    int width=constant::kBeatmapCardWidth*constant::kPixelScale;
    int height=constant::kBeatmapCardHeight*constant::kPixelScale;
    int x=middleX-width/2;
    int y=middleY-height/2+m_idx*height*1.5;
    // if out of screen, don't draw
    if (y+height<0 || y>constant::kScreenH) return;
    ALLEGRO_COLOR color;
    if (m_maxDiff>=0) {
        if (m_idx==0) color=constant::kSongSelectedColor;
        else color=constant::kSongNotSelectedColor;
    }
    else {
        if (m_idx==0) color=constant::kDiffSelectedColor;
        else color=constant::kDiffNotSelectedColor;
    }
    al_draw_rectangle(x, y, x+width, y+height, color, 5);
    al_draw_filled_rectangle(x, y, x+width, y+height, al_map_rgb(0, 0, 0));
    Engine::Label name{m_name, "NotoCJK/noto-sans-cjk-black.ttf", 36, (double)x, (double)y, 255, 255, 255};
    name.Draw();
    std::stringstream out;
    out.precision(2);
    if (m_maxDiff>=0) {
        out<<std::fixed<<m_minDiff;
        out<<" - ";
        out<<std::fixed<<m_maxDiff;
        out<<" STARS / "<<m_author<<" / "<<m_mapper;
    }
    else {
        out<<std::fixed<<m_minDiff;
        out<<" STARS / ";
        out<<m_key<<"k";
    }
    Engine::Label info{out.str(), "NotoCJK/noto-sans-cjk-black.ttf", 20, (double)x, (double)y+height*2/3, 255, 255, 255};
    info.Draw();
} 

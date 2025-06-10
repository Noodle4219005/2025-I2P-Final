#include "BeatmapCard.h"
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

BeatmapCard::BeatmapCard(std::string name, std::string author, std::string mapper, float minDiffiulty, float maxDifficulty):
    m_name(name), m_author(author), m_mapper(mapper), m_minDiff(minDiffiulty), m_maxDiff(maxDifficulty)
{
}
BeatmapCard::BeatmapCard(std::string name, float diffiulty):
    m_name(name), m_minDiff(diffiulty), m_maxDiff(-1)
{
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
    bool mouseIn = Collider::IsPointInRect(Point{(float)mx, (float)my},
                                           Point{(float)x, (float)y},
                                           Point{(float)width, (float)height}
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
    Engine::Label name{m_name, "NotoCJK/noto-sans-cjk-black.ttf", 36, (float)x, (float)y, 255, 255, 255};
    name.Draw();
    std::stringstream out;
    out.precision(2);
    if (m_maxDiff>=0) {
        out<<m_minDiff;
        out<<"-";
        out<<m_maxDiff;
        out<<" / "<<m_author<<" / "<<m_mapper;
        Engine::Label info{out.str(), "NotoCJK/noto-sans-cjk-black.ttf", 20, (float)x, (float)y+height*2/3, 255, 255, 255};
        info.Draw();
    }
} 

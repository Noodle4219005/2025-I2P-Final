#include "Engine/IScene.hpp"
#include <allegro5/keyboard.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <allegro5/allegro_font.h>
#include <string>
#include "InputBox.h"

using std::string;

InputBox::InputBox(int x, int y, int h, int w, ALLEGRO_COLOR deactiveColor, ALLEGRO_COLOR activeColor) :
    m_x(x), m_y(y), m_h(h), m_w(w), m_deactiveColor(deactiveColor), m_activeColor(activeColor) 
{
}

InputBox::InputBox(int x, int y, int h, int w) :
    m_x(x), m_y(y), m_h(h), m_w(w)
{
}

InputBox::InputBox(const InputBox& rhs) :
    m_x(rhs.m_h), m_y(rhs.m_y), m_h(rhs.m_h), m_w(rhs.m_w), 
    m_deactiveColor(rhs.m_deactiveColor), m_activeColor(rhs.m_activeColor) 
{
}

void InputBox::UpdateKey(int key) 
{
    if (key==ALLEGRO_KEY_ESCAPE){
        m_isSelected=false;
    }
    else if (key==ALLEGRO_KEY_ENTER){
        m_isSelected=true;
    }

    if (!m_isSelected) return;

    if (key==ALLEGRO_KEY_BACKSPACE) {
        if (m_value.empty()) return;
        m_value.pop_back();
    }
    else if (1<=key&&key<=26) {
        m_value.push_back(key+'A'-1);
    }
    else if (27<=key&&key<=36) {
        m_value.push_back(key+'0'-27);
    }
    else if (37<=key&&key<=46) {
        m_value.push_back(key+'0'-37);
    }
}

void InputBox::Draw() const
{
    std::shared_ptr<ALLEGRO_FONT> font=Engine::Resources::GetInstance().GetFont("pirulen.ttf", kFontSize);
    if (m_isSelected) al_draw_rectangle(m_x, m_y, m_x+m_w, m_y+m_h, m_activeColor, kThinkness);
    else al_draw_rectangle(m_x, m_y, m_x+m_w, m_y+m_h, m_deactiveColor, kThinkness);
    al_draw_filled_rectangle(m_x, m_y, m_x+m_w, m_y+m_h, al_map_rgb(255, 255, 255));
    al_draw_text(font.get(), al_map_rgb(0, 0, 0), m_x, m_y, 0, m_value.c_str());
}

void InputBox::SetValue(string str)
{
    m_value=str;
}

string InputBox::GetValue()
{
    return m_value;
}

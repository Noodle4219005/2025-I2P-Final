#pragma once
#include "Label.hpp"
#include "Engine/Resources.hpp"
#include <string>
#include <memory>
#include <allegro5/color.h>
#include <allegro5/allegro_font.h>
using std::string;

class InputBox
{ 
private:
    int m_x, m_y;
    int m_h, m_w;
    string m_value="";
    bool m_isSelected=false;
    ALLEGRO_COLOR m_deactiveColor=al_map_rgb(100, 100, 100);
    ALLEGRO_COLOR m_activeColor=al_map_rgb(255, 255, 0);
    static constexpr int kThinkness=8;
    static constexpr int kFontSize=16;

public:
    InputBox(int x, int y, int h, int w, ALLEGRO_COLOR deactiveColor, ALLEGRO_COLOR activeColor);
    InputBox(int x, int y, int h, int w);
    InputBox(const InputBox& rhs);
    InputBox()=default;
    void SetValue(string str);
    string GetValue();
    void UpdateKey(int key);
    void Draw() const;
    InputBox& operator=(const InputBox& rhs) 
    {
        if (this==&rhs) {
            return *this;
        }

        m_x=rhs.m_x;
        m_y=rhs.m_y;
        m_w=rhs.m_w;
        m_h=rhs.m_h;
        m_value=rhs.m_value;
        m_isSelected=rhs.m_isSelected;
        m_deactiveColor=rhs.m_deactiveColor;
        m_activeColor=rhs.m_activeColor;
    }
};

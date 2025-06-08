#pragma once

#include <vector>

class HUD 
{
public:
    // Singleton
    HUD(HUD const&)=delete;
    void operator=(HUD const&)=delete;
    static HUD& GetInstance();

    void Init();
    void DrawForeground();
    void DrawBackground();
    void OnKeyDown(int column);
    void OnKeyUp(int column);

private:
    HUD();
    int screenMiddleX; 
    int startX;
    int playFieldWidth;
    float musicBarX;
    float musicBarY;
    float musicBarWidth;
    float musicBarHeight;
    std::vector<bool> isKeyPressed;
};

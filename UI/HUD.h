#pragma once

class HUD 
{
public:
    // Singleton
    HUD(HUD const&)=delete;
    void operator=(HUD const&)=delete;
    static HUD& GetInstance();

    void DrawForeground();
    void DrawBackground();

private:
    HUD();
    int screenMiddleX; 
    int startX;
    int playFieldWidth;
    float musicBarX;
    float musicBarY;
    float musicBarWidth;
    float musicBarHeight;
};

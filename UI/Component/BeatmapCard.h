#pragma once

#include <string>
#include <functional>

class BeatmapCard 
{
public:
    BeatmapCard(std::string name, std::string author, std::string mapper, float minDiffiulty, float maxDifficulty);
    BeatmapCard(std::string name, float diffiulty);
    void OnMouseDown(int button, int mx, int my);

    /// <summary>
    /// Update the selected song and its object
    /// </summary>
    /// <param name="idx"> 
    /// idx=0 means the song now selected, 
    /// idx<0 is the song before the now one (draw on the above of current), 
    /// and idx>0 is the song after the now one (draw on the bottom of current).
    /// </param>
    void Update(int idx);

    void Draw() const;
    void SetOnClickCallback(std::function<void(void)> onClickCallback);
private:
    std::string m_name;
    std::string m_author;
    std::string m_mapper;
    float m_minDiff;
    float m_maxDiff;
    std::function<void(void)> m_onClickedCallBack;
    int m_idx=0;

};
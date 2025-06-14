#pragma once

class HitErrorGraph
{
public:
    HitErrorGraph(double x, double y, double width, double height);
    void Draw() const;
private:
    double m_x, m_y, m_w, m_h;
};
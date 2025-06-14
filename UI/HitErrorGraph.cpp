#include "HiterrorGraph.h"
#include "util/GameData.h"

#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <list>

HitErrorGraph::HitErrorGraph(double x, double y, double width, double height) :
    m_x(x), m_y(y), m_w(width), m_h(height) 
{
}

void HitErrorGraph::Draw() const 
{
    al_draw_rectangle(m_x, m_y, m_x+m_w, m_y+m_h, al_map_rgb(255, 255, 255), 5);
    const double maxHitError=188 - 3*game_data::OD;
    const int prevSize=1.0*game_data::hitResults.size()/50;
    std::list<std::pair<double, double>> plots;
    std::list<double> prev;
    for (auto const& hitResult : game_data::hitResults) {
        double percentage=std::min(abs(hitResult.hitError), maxHitError)/maxHitError;
        double timeFactor=(hitResult.time-game_data::firstObjectTime) / (game_data::gamePosition-game_data::firstObjectTime);
        prev.push_back(percentage);
        if (prev.size()>prevSize) {
            double average=0.0;
            for (auto i : prev) {
                average+=i;
            }
            average/=prev.size();
            plots.push_back({timeFactor, average});
            prev.clear();
        }
    }
    if (prev.size()) {
        double average=0.0;
        for (auto i : prev) {
            average+=i;
        }
        average/=prev.size();
        plots.push_back({1.0, average});
    }
    else {
        plots.push_back({1.0, plots.rbegin()->second});
    }

    double x0, y0;
    double threshold=(127 - 3*game_data::OD) / maxHitError;
    x0=y0=0;
    for (auto point: plots) {
        auto [x1, y1]=point;
        ALLEGRO_COLOR color;
        if (point.second<threshold) {
            color=al_map_rgb(0, 255, 100);
        }
        else {
            color=al_map_rgb(255, 0, 0);
        }
        al_draw_line(m_x + m_w*x0, m_y + m_h*y0, m_x + m_w*x1, m_y + m_h*y1, color, 5.0f);
        x0=x1;
        y0=y1;
    }
}

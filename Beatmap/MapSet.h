#pragma once
#include "BeatmapParser.h"

#include <string>
#include <vector>
#include <unordered_map>

class MapSet 
{
public:
    MapSet();
    void DrawBackground() const;
    void GetName() const;
    void NextBeatmap();
    void PrevBeatmap();
private:
    std::vector<std::string> m_mapNames; 
    std::unordered_map<std::string, std::vector<BeatmapParser>> m_beatmapDifficultiesByName;
    std::vector<std::string>::const_iterator m_mapNamesIter;
};
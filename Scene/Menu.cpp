#include "Menu.h"
#include "Engine/LOG.hpp"
#include "UI/Component/BeatmapCard.h"

#include <filesystem>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <allegro5/keycodes.h>

void Menu::Initialize() 
{
    std::string root;
    std::stringstream intSS;
    root="./Map";

    std::list<std::pair<int, std::string>> beatmapFilePathLists; // id, filepath
    try {
        for (auto const& entry : std::filesystem::recursive_directory_iterator(root)) {
            int beatmapId;
            if (entry.is_directory()) {
                intSS<<entry.path().filename().string();
                intSS>>beatmapId;
                beatmapFilePathLists.push_back(make_pair(beatmapId, "./Map/"+entry.path().filename().string()));
                decltype(intSS)().swap(intSS);
            }
        }
    }
    catch (std::filesystem::filesystem_error& e) {
        Engine::LOG(Engine::ERROR) << "Filesystem error: " << e.what() << '\n';
        throw e;
    }

    for (auto filepath : beatmapFilePathLists) {
        ProcessDifficulty(filepath.first, filepath.second);
    }


    // preload the music
    for (auto map : m_mapList) {
        m_music=AudioHelper::PlaySample(map.audioPath);
        AudioHelper::StopSample(m_music);
    }

    m_isPlayed=false;
}

void Menu::Update(float deltaTime)
{
    if (m_level==0) {
        if (!m_isPlayed) {
            if (m_nowPlayingAudioPath!=m_mapList[m_nowMapIndex].audioPath) {
                m_music=AudioHelper::PlaySample(m_mapList[m_nowMapIndex].audioPath, true, AudioHelper::BGMVolume, m_mapList[m_nowMapIndex].previewTime);
            }
            m_nowPlayingAudioPath=m_mapList[m_nowMapIndex].audioPath;
            m_isPlayed=true;
        }
        int idx=-m_nowMapIndex;
        for (int i=0; i<m_mapList.size(); ++i, ++idx) {
            m_mapList[i].card.Update(idx);
        }
    }
    else if (m_level==1) {
        if (!m_isPlayed) {
            std::string path=m_beatmapDifficultiesByName[m_beatmapName][m_nowDiffcultyIndex].GetAudioFilePath();
            if (m_nowPlayingAudioPath!=path) {
                m_music=AudioHelper::PlaySample(path, true, AudioHelper::BGMVolume, m_beatmapDifficultiesByName[m_beatmapName][m_nowDiffcultyIndex].GetPreviewTime());
            }
            m_nowPlayingAudioPath=path;
            m_isPlayed=true;
        }
        int idx=-m_nowDiffcultyIndex;
        for (int i=0; i<m_difficultyCards.size(); ++i, ++idx) {
            m_difficultyCards[i].Update(idx);
        }
        m_bg=m_beatmapDifficultiesByName[m_beatmapName][m_nowDiffcultyIndex].GetBackgroundImage();
    }
}

void Menu::Draw() const
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    if (m_level==0) {
        m_mapList[m_nowMapIndex].image->Draw();
        int idx=-m_nowMapIndex;
        for (int i=0; i<m_mapList.size(); ++i, ++idx) {
            if (idx!=0) m_mapList[i].card.Draw();
        }
        m_mapList[m_nowMapIndex].card.Draw();
    }
    else if (m_level==1) {
        if (m_bg) m_bg->Draw();
        int idx=-m_nowDiffcultyIndex;
        for (int i=0; i<m_difficultyCards.size(); ++i, ++idx) {
            m_difficultyCards[i].Draw();
        }
        m_difficultyCards[m_nowDiffcultyIndex].Draw();
    }
}


void Menu::OnMouseScroll(int mx, int my, int delta)
{
    if (m_level==0 && m_nowMapIndex-delta>=0 && m_nowMapIndex-delta<m_mapList.size()) {
        m_nowMapIndex-=delta;
        if (m_nowPlayingAudioPath!=m_mapList[m_nowMapIndex].audioPath) AudioHelper::StopSample(m_music);
        m_isPlayed=false;
    }
    if (m_level==1 && m_nowDiffcultyIndex-delta>=0 && m_nowDiffcultyIndex-delta<m_beatmapDifficultiesByName[m_beatmapName].size()) {
        m_nowDiffcultyIndex-=delta;
        std::string path=m_beatmapDifficultiesByName[m_beatmapName][m_nowDiffcultyIndex].GetAudioFilePath();
        if (m_nowPlayingAudioPath!=path) AudioHelper::StopSample(m_music);
        m_isPlayed=false;
    }
}

void Menu::OnMouseDown(int button, int mx, int my) 
{
    if (m_level==0) {
        for (auto map : m_mapList) {
            map.card.OnMouseDown(button, mx, my);
        }
    }
}

void Menu::OnKeyDown(int keyCode)
{
    int delta=0;
    if (keyCode==ALLEGRO_KEY_DOWN) delta=-1;
    else if (keyCode==ALLEGRO_KEY_UP) delta=1;
    else if (keyCode==ALLEGRO_KEY_ENTER) {
        if (m_level==0) mapCallBack(m_mapList[m_nowMapIndex].name);
        return;
    }
    else return;

    if (m_level==0 && m_nowMapIndex-delta>=0 && m_nowMapIndex-delta<m_mapList.size()) {
        m_nowMapIndex-=delta;
        if (m_nowPlayingAudioPath!=m_mapList[m_nowMapIndex].audioPath) AudioHelper::StopSample(m_music);
        m_isPlayed=false;
    }
    if (m_level==1 && m_nowDiffcultyIndex-delta>=0 && m_nowDiffcultyIndex-delta<m_beatmapDifficultiesByName[m_beatmapName].size()) {
        m_nowDiffcultyIndex-=delta;
        std::string path=m_beatmapDifficultiesByName[m_beatmapName][m_nowDiffcultyIndex].GetAudioFilePath();
        if (m_nowPlayingAudioPath!=path) AudioHelper::StopSample(m_music);
        m_isPlayed=false;
    }
}

void Menu::ProcessDifficulty (int beatmapId, std::string beatmapPath) 
{
    std::string beatmapName="";
    std::string author;
    std::string mapper;
    float minDiff=FLT_MAX, maxDiff=-1;
    int previewTime=0;
    std::string audioPath;
    std::vector<BeatmapParser> difficulties;
    std::shared_ptr<Engine::Image> image;
    for (auto const& entry : std::filesystem::recursive_directory_iterator(beatmapPath)) {
        std::string difficultyName;
        if (entry.is_regular_file() && entry.path().extension()==".osu") {
            std::stringstream ss(entry.path().filename().string());
            getline(ss, difficultyName, '[');
            getline(ss, difficultyName, ']');
            BeatmapParser beatmap(beatmapId, difficultyName);
            difficulties.push_back(beatmap);
            if (difficulties.size()==1) {
                beatmapName=difficulties.back().GetTitle();
                author=difficulties.back().GetAuthor();
                mapper=difficulties.back().GetMapper();
                previewTime=difficulties.back().GetPreviewTime();
                audioPath=difficulties.back().GetAudioFilePath();
                image=difficulties.back().GetBackgroundImage();
            }
            minDiff=std::min(minDiff, difficulties.back().GetStarRate());
            maxDiff=std::max(minDiff, difficulties.back().GetStarRate());
        }
    }
    m_beatmapDifficultiesByName[beatmapName].swap(difficulties);
    m_mapList.push_back(MapInfo{beatmapId, beatmapName, author, mapper, minDiff, maxDiff, previewTime, audioPath, 
                        BeatmapCard{beatmapName, author, mapper, minDiff, maxDiff}, image});
    m_mapList.back().card.SetOnClickCallback(std::bind(&Menu::mapCallBack, this, beatmapName));
}

void Menu::mapCallBack(std::string beatmapName) 
{
    std::cout<<"clicked\n";
    m_isPlayed=false;
    m_level=1;
    m_nowDiffcultyIndex=0;
    m_beatmapName=beatmapName;
    decltype(m_difficultyCards)().swap(m_difficultyCards);
    for (auto diff : m_beatmapDifficultiesByName[beatmapName]) {
        m_difficultyCards.push_back(BeatmapCard{diff.GetDifficultyName(), diff.GetStarRate()});
    }
    m_bg=m_beatmapDifficultiesByName[m_beatmapName][m_nowDiffcultyIndex].GetBackgroundImage();
}

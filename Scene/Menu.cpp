#include "Menu.h"
#include "Engine/LOG.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/BeatmapCard.h"
#include "UI/Component/Label.hpp"
#include "Skin/Skin.h"
#include "util/Constant.h"

#include <filesystem>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <allegro5/keycodes.h>
#include <allegro5/allegro_primitives.h>
#include <random>
#include <chrono>
#include <thread>
#include <functional>

void Menu::Initialize() 
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    Engine::Label loadingText{"Loading", "NotoCJK/noto-sans-cjk-black.ttf", 72, 1.f*constant::kScreenW/2, 1.f*constant::kScreenH/2, 255, 255, 255, 255, 0.5, 0.5};
    loadingText.Draw();
    al_flip_display();
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
    for (int i=0; i<m_mapList.size(); i++) {
        al_draw_filled_rectangle(0, constant::kScreenH, 1.f*constant::kScreenW*i/m_mapList.size(), constant::kScreenH-10, al_map_rgb(255, 255, 255));
        al_flip_display();
        m_music=AudioHelper::PlaySample(m_mapList[i].audioPath);
        AudioHelper::StopSample(m_music);
    }

    std::mt19937_64 engine(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
    if (m_nowMapIndex<0) m_nowMapIndex=engine()%m_mapList.size();
    m_isPlayed=false;
    m_isAltKeyDown=false;
    m_audioAnimationStart=std::chrono::steady_clock::now()-std::chrono::seconds(constant::kMusicBarDisplaySeconds+1);
}

void Menu::Terminate() 
{
    m_isAltKeyDown=false;
    m_bg.reset();
    AudioHelper::StopSample(m_music);
    m_mapList.clear();
    m_beatmapDifficultiesByName.clear();
    m_nowPlayingAudioPath="";
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

    int selection=0;
    if (game_data::isAuto) selection+=1;
    if (game_data::isDoubleTime) selection+=2;
    if (game_data::isNoFailed) selection+=4;
    Skin::GetInstance().DrawMod(selection);

    using std::chrono::duration_cast;
    using std::chrono::seconds;
    using std::chrono::steady_clock;
    if (duration_cast<seconds>(steady_clock::now()-m_audioAnimationStart).count() <= constant::kMusicBarDisplaySeconds) {
        int width=constant::kMusicBarDisplayWidth*constant::kPixelScale;
        int length=constant::kMusicBarDisplayLength*constant::kPixelScale;
        al_draw_filled_rectangle(constant::kScreenW, constant::kScreenH,
                                 constant::kScreenW-length, constant::kScreenH-width,
                                 al_map_rgb(20, 20, 20)
                                 );
        al_draw_filled_rectangle(constant::kScreenW, constant::kScreenH,
                                 constant::kScreenW-length*AudioHelper::BGMVolume, constant::kScreenH-width,
                                 al_map_rgb(255, 255, 255)
                                 );
        al_draw_rectangle(constant::kScreenW, constant::kScreenH,
                                 constant::kScreenW-length, constant::kScreenH-width,
                                 al_map_rgb(255, 255, 255), 5
                                 );
    }
}


void Menu::OnMouseScroll(int mx, int my, int delta)
{
    if (m_isAltKeyDown) {
        AudioHelper::BGMVolume+=delta*0.01;
        AudioHelper::BGMVolume=std::clamp((double)AudioHelper::BGMVolume, 0.0, 1.0);
        AudioHelper::ChangeSampleVolume(m_music, AudioHelper::BGMVolume);
        m_audioAnimationStart=std::chrono::steady_clock::now();
        return;
    }
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
    else if (m_level==1) {
        for (auto diff : m_difficultyCards) {
            diff.OnMouseDown(button, mx, my);
        }
    }
}

void Menu::OnKeyDown(int keyCode)
{
    int delta=0;
    if (keyCode==ALLEGRO_KEY_DOWN) delta=-1;
    else if (keyCode==ALLEGRO_KEY_UP) delta=1;
    else if (keyCode==ALLEGRO_KEY_ENTER) {
        if (m_level==0) MapCallBack(m_mapList[m_nowMapIndex].name);
        else if (m_level==1) DiffCallBack();
        return;
    }
    else if (keyCode==ALLEGRO_KEY_ESCAPE) {
        m_isPlayed=false;
        m_level=0;
        return;
    }
    else if (keyCode==constant::keyMap["select_auto"]) {
        game_data::isAuto=!game_data::isAuto;
        if (game_data::isAuto) {
            game_data::isNoFailed=false;
        }
        return;
    }
    else if (keyCode==constant::keyMap["select_dt"]) {
        game_data::isDoubleTime=!game_data::isDoubleTime;
        return;
    }
    else if (keyCode==constant::keyMap["select_nf"]) {
        game_data::isNoFailed=!game_data::isNoFailed;
        if (game_data::isNoFailed) {
            game_data::isAuto=false;
        }
        return;
    }
    else if (keyCode==ALLEGRO_KEY_ALT || keyCode==ALLEGRO_KEY_ALTGR) {
        m_isAltKeyDown=true;
    }
    else if (m_level==0 && keyCode==ALLEGRO_KEY_ESCAPE) {
        Engine::GameEngine::GetInstance().ChangeScene("start");
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

void Menu::OnKeyUp(int keyCode) 
{
    if (keyCode==ALLEGRO_KEY_ALT || keyCode==ALLEGRO_KEY_ALTGR) {
        m_isAltKeyDown=false;
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
            if (beatmap.GetMode()!=3) continue;
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
    m_mapList.back().card.SetOnClickCallback(std::bind(&Menu::MapCallBack, this, beatmapName));
}

void Menu::MapCallBack(std::string beatmapName) 
{
    m_isPlayed=false;
    m_level=1;
    m_nowDiffcultyIndex=0;
    m_beatmapName=beatmapName;
    decltype(m_difficultyCards)().swap(m_difficultyCards);
    for (auto diff : m_beatmapDifficultiesByName[beatmapName]) {
        m_difficultyCards.push_back(BeatmapCard{diff.GetDifficultyName(), diff.GetStarRate(), diff.GetTotalColumns()});
        m_difficultyCards.back().SetOnClickCallback(std::bind(&Menu::DiffCallBack, this));
    }
    m_bg=m_beatmapDifficultiesByName[m_beatmapName][m_nowDiffcultyIndex].GetBackgroundImage();
}

void Menu::DiffCallBack()
{
    game_data::mapID=m_beatmapDifficultiesByName[m_beatmapName][m_nowDiffcultyIndex].GetId();
    game_data::difficultyName=m_beatmapDifficultiesByName[m_beatmapName][m_nowDiffcultyIndex].GetDifficultyName();
    Engine::GameEngine::GetInstance().ChangeScene("game");
}

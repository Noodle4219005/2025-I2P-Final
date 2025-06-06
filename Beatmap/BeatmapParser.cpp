#include "BeatmapParser.h"
#include "Objects/Hold.h"
#include "Objects/Node.h"
#include "Engine/LOG.hpp"
#include "util/GameData.h"

#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <allegro5/bitmap.h>
#include <filesystem> 

BeatmapParser::BeatmapParser(const int& beatmapID, const std::string& difficultyName) 
{
    std::ifstream beatmapFileStream;
    std::stringstream intSS;
    std::string token;
    std::filesystem::path root;

    if (m_beatmapID==beatmapID && m_difficultyName==difficultyName) return;

    root="./Map";
    try {
        for (auto const& entry : std::filesystem::recursive_directory_iterator(root)) {
            if (entry.is_directory()) {
                intSS<<entry.path().filename().string();
                intSS>>m_beatmapID;
                if (m_beatmapID==beatmapID) {
                    m_beatmapPath="./Map/"+entry.path().filename().string();
                    break;
                }
                decltype(intSS)().swap(intSS);
            }
        }
    }
    catch (std::filesystem::filesystem_error& e) {
        Engine::LOG(Engine::ERROR) << "Filesystem error: " << e.what() << '\n';
        throw e;
    }

    for (auto const& entry : std::filesystem::recursive_directory_iterator(m_beatmapPath)) {
        if (entry.is_regular_file() && entry.path().extension()==".osu") {
            std::stringstream ss(entry.path().filename().string());
            getline(ss, m_difficultyName, '[');
            getline(ss, m_difficultyName, ']');
            if (m_difficultyName==difficultyName) {
                m_beatmapFile=entry.path().filename().string();
                break;
            }
        }
    }

    beatmapFileStream.open(m_beatmapPath+'/'+m_beatmapFile);
    while (getline(beatmapFileStream, token)) {
        // If commented, then skip this line.
        if (token.empty()) continue;
        while (token.front()==' ') token=token.substr(1, token.size()-1);
        if (token[0]=='/'&&token[1]=='/') continue;
        std::cout<<token<<std::endl;

        if (token=="[General]") {
            m_section=GENERAL;
        }
        else if (token=="[Editor]") { 
            m_section=-1;
        }
        else if (token=="[Metadata]") {
            m_section=METADATA;
        }
        else if (token=="[Difficulty]") {
            m_section=DIFFICULTY;
        }
        else if (token=="[Events]") {
            m_section=EVENTS;
        }
        else if (token=="[TimingPoints]") {
            m_section=TIMING_POINTS;
        }
        else if (token=="[Colours]") { 
            m_section=-1;
        }
        else if (token=="[HitObjects]") {
            m_section=HIT_OBJECTS;
        }
        else {
            Parse(token);
        }
    }
    m_timingIter=m_timingList.begin();
    m_nodeIter=m_nodeList.begin();
}

void BeatmapParser::Restart() 
{
    m_timingIter=m_timingList.begin();
    m_nodeIter=m_nodeList.begin();
}

void BeatmapParser::Parse(const std::string& str) 
{
    if (str.empty()) return;
    std::string token, value;
    if (m_section==GENERAL || m_section==METADATA || m_section==DIFFICULTY) {
        std::istringstream iss(str);
        getline(iss, token, ':');
        getline(iss, value, ' ');
        getline(iss, value);
    }

    if (m_section==GENERAL) {
        if (token=="AudioFilename") {
            m_audioFilename=value;
        }
        else if (token=="AudioLeadIn") {
            m_audioLeadIn=stoi(value);
        }
        else if (token=="PreviewTime") {
            m_previewTime=stoi(value);
        }
        else if (token=="Countdown") {
            m_countDown=stoi(value);
        }
        else if (token=="SampleSet") {
            m_sampleSet=value;
        }
        else if (token=="Mode") {
            m_mode=stoi(value);
        }
        else if (token=="SpecialStyle") {
            m_specialStyle=stoi(value);
        }
    }
    else if (m_section==METADATA) {
        if (token=="Title") {
            m_title=value;
        }
        else if (token=="Artist") {
            m_artist=value;
        }
        else if (token=="Creator") {
            m_creator=value;
        }
        else if (token=="Version") {
            m_version=value;
        }

    }
    else if (m_section==DIFFICULTY) {
        if (token=="CircleSize") {
            m_totalColumns=stoi(value);
        } 
        else if (token=="OverallDifficulty") {
            m_OD=stof(value);
            game_data::OD=m_OD;
        }
    }
    else if (m_section==EVENTS) {
    }
    else if (m_section==TIMING_POINTS) {
        std::istringstream iss(str);
        float beatLength;
        int volume;
        int time, meter;
        int sampleSet, sampleIndex;
        bool uninherited;
        int effects;

        getline(iss, token, ',');
        time=stoi(token);
        getline(iss, token, ',');
        beatLength=stof(token);
        getline(iss, token, ',');
        meter=stoi(token);
        getline(iss, token, ',');
        sampleSet=stoi(token);
        getline(iss, token, ',');
        sampleIndex=stoi(token);
        getline(iss, token, ',');
        volume=stoi(token);
        getline(iss, token, ',');
        uninherited=stoi(token);
        getline(iss, token, ',');
        effects=stoi(token);

        m_timingList.push_back({time, beatLength, meter, sampleSet, sampleIndex, volume, uninherited, effects});
    }
    else if (m_section==HIT_OBJECTS) {
        m_nodeList.push_back(str);
    }
}

bool BeatmapParser::IsMapEnded() 
{
    return m_nodeIter==m_nodeList.end();
}

HitObject BeatmapParser::GetNextHitObject()
{
    std::stringstream hitObjectTokenSS(*m_nodeIter);
    std::string token;
    int x, time, type;
    int endtime=-1;
    int hitSound; // from 0 to 3, normal, whistle, finish, clap
    std::cout<<"read: "<<*m_nodeIter<<std::endl;
    ++m_nodeIter;

    // the magic number is from ppy osu file format
    // https://osu.ppy.sh/wiki/en/Client/File_formats/osu_%28file_format%29#holds-(osu!mania-only)
    getline(hitObjectTokenSS, token, ',');
    x=floor(stoi(token) * m_totalColumns / 512);
    getline(hitObjectTokenSS, token, ',');
    getline(hitObjectTokenSS, token, ',');
    time=stoi(token);
    getline(hitObjectTokenSS, token, ',');
    type=stoi(token);
    getline(hitObjectTokenSS, token, ',');
    hitSound=stoi(token);
    if (type&128) {
        getline(hitObjectTokenSS, token, ',');
        endtime=stoi(token);
    }

    if (endtime>=0) return Node(x, time, type, "res/skin/normal-hitnormal.wav");
    return Hold(x, time, type, endtime,"res/skin/normal-hitnormal.wav");
}

void BeatmapParser::UpdateTiming(int musicPosition) 
{
    while (m_timingIter!=m_timingList.end() && m_timingIter->time<=musicPosition) {
        if (m_baseBPM==-1) m_baseBPM=GetBPM(m_timingIter->beatLength);
        if (m_timingIter->uninherited) {
            int bpm=GetBPM(m_timingIter->beatLength);
            m_bpmMultipliler=bpm/m_baseBPM;
        }
        else {
            m_bpmMultipliler=(-m_timingIter->beatLength)/100.;
            std::cout<<"BPM Mul: "<<m_bpmMultipliler<<std::endl;
        }
        switch (m_timingIter->sampleSet)
        {
        case 1: 
            m_sampleSet="Normal";
            break;
        case 2:
            m_sampleSet="Soft";
            break;
        case 3:
            m_sampleSet="Drum";
            break;
        default:
            break;
        }
        m_sampleIndex=m_timingIter->sampleIndex;
        m_volume=1.*m_timingIter->volume/100.;
        m_timingIter=next(m_timingIter);
    }
}

std::string BeatmapParser::GetAudioFilePath()
{
    return m_beatmapPath+'/'+m_audioFilename;
}

int BeatmapParser::GetTotalColumns()
{
    return m_totalColumns;
}

float BeatmapParser::GetBPM(float beatLength) 
{
    return 1 / beatLength * 1000 * 60;
}

float BeatmapParser::GetSpeedScale() 
{
    return m_bpmMultipliler;
}

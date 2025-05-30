#include "Objects/Node.h"
#include "Engine/ImageHelper.h"
#include "Engine/LOG.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <allegro5/bitmap.h>
#include <filesystem> 

class BeatmapParser
{
public:
    BeatmapParser(const int& beatmapID, const std::string& difficultyName);
    std::string GetAudioFilePath();
    int GetPreviewTime();
    int GetCountDown();
    Node GetNextNode();
    void UpdateEvent(int musicPosition);

private:
    enum Sections 
    {
        GENERAL=0,
        METADATA,
        DIFFICULTY,
        EVENTS,
        TIMING_POINTS,
        HIT_OBJECTS
    };

    struct Timing
    {
        int time; 
        float beatLength; 
        int meter; 
        int sampleSet;
        int sampleIndex;
        // skip the uninherited
        int volume;
        int effects;
    };

    int m_beatmapID=-1;
    int difficulty=-1;
    std::string m_beatmapPath;

    // General
    int m_section=-1;
    std::string m_audioFilename;
    int m_audioLeadIn;
    int m_previewTime;
    int m_countDown;
    int m_mode;
    std::string m_sampleSet="Normal"; // Normal Soft Drum
    int m_specialStyle;

    // Metadata
    std::string m_title;
    std::string m_artist;
    std::string m_creator;
    std::string m_version;

    // Difficulty
    int m_totalColumns=0;
    float m_OD=10;

    // Event
    std::shared_ptr<ALLEGRO_BITMAP> backgroudImage;

    // Timing Points
    std::list<Timing> m_timingList;

    // Node
    std::list<std::string > m_nodeList;

    void Parse(const std::string& str);
};

BeatmapParser::BeatmapParser(const int& beatmapID, const std::string& difficultyName) 
{
    std::ifstream beatmapFileStream;
    std::stringstream intSS;
    std::string token;
    std::filesystem::path root;

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
            std::string nowDifficulty;
            getline(ss, nowDifficulty, '[');
            getline(ss, nowDifficulty, ']');
            if (nowDifficulty==difficultyName) {
                m_beatmapPath="./Map/"+entry.path().filename().string();
                break;
            }
        }
    }

    beatmapFileStream.open("./Map/"+m_beatmapPath);
    while (getline(beatmapFileStream, token)) {
        // If commented, then skip this line.
        while (token.front()==' ') token=token.substr(1, token.size()-1);
        if (token[0]=='/'&&token[1]=='/') continue;

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
            m_audioFilename=value;
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
        }
    }
    else if (m_section==EVENTS) {
    }
    else if (m_section==TIMING_POINTS) {
        std::istringstream iss(str);
        float beatLength;
        int time, meter, volume;
        int sampleSet, sampleIndex;
        int uninherited;
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

        if (!uninherited) {
            beatLength=m_timingList.rbegin()->beatLength*(1.f+0.01f*beatLength);
        }
        m_timingList.push_back({time, beatLength, meter, sampleSet, sampleIndex, volume, effects});
    }
    else if (m_section==HIT_OBJECTS) {
        m_nodeList.push_back(token);
    }
}
 
Node BeatmapParser::GetNextNode()
{

}

void BeatmapParser::UpdateEvent(int musicPosition) 
{
}

std::string BeatmapParser::GetAudioFilePath()
{
    return m_beatmapPath+m_audioFilename;
}

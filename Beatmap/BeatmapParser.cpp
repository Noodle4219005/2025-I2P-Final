#include "BeatmapParser.h"
#include "DifficultyCounter.h"
#include "Objects/Hold.h"
#include "Objects/Node.h"
#include "Engine/LOG.hpp"
#include "UI/Component/Image.hpp"
#include "util/GameData.h"
#include "util/Constant.h"

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
        // std::cout<<token<<std::endl;

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
        getline(iss, value);
        if (value.size() && value.front()==' ') value=value.substr(1, value.size()-1);
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
            game_data::nkey=m_totalColumns;
        } 
        else if (token=="OverallDifficulty") {
            m_OD=stod(value);
            game_data::OD=m_OD;
        }
    }
    else if (m_section==EVENTS) {
        std::istringstream iss(str);
        std::string filename="";
        int xOffset=0;
        int yOffset=0;
        // Get the event type
        getline(iss, token, ',');
        if (token=="0") {
            getline(iss, token, ',');
            getline(iss, token, ',');
            filename=token;
            if (filename.front()=='"' && filename.back()=='"') {
                filename=filename.substr(1, filename.size()-2);
            }
            if (iss.str().size()) {
                getline(iss, token, ',');
                xOffset=stoi(token);
                getline(iss, token, ',');
                yOffset=stoi(token);
            }
            std::string path=m_beatmapPath+'/'+filename;
            m_bgImageInfo={path, xOffset*constant::kPixelScale, yOffset*constant::kPixelScale};
        }
        else if (token=="1" || token=="Video") {
        }
    }
    else if (m_section==TIMING_POINTS) {
        std::istringstream iss(str);
        double beatLength;
        int volume;
        int time, meter;
        int sampleSet, sampleIndex;
        bool uninherited;
        int effects;

        getline(iss, token, ',');
        time=stoi(token);
        getline(iss, token, ',');
        beatLength=stod(token);
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

std::unique_ptr<HitObject> BeatmapParser::GetNextHitObject()
{
    std::stringstream hitObjectTokenSS(*m_nodeIter);
    std::string token;
    int x, time, type;
    int endtime=-1;
    int hitSound; // from 0 to 3, normal, whistle, finish, clap
    // std::cout<<"read: "<<*m_nodeIter<<std::endl;
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

    if (endtime<0) return std::make_unique<Node>(Node(x, time, type, GetStartPosition(time, time+constant::kHitobjectPreviewThreshold), "res/skin/normal-hitnormal.wav"));
    return std::make_unique<Hold>(Hold(x, time, type, endtime, GetStartPosition(time, time+constant::kHitobjectPreviewThreshold), GetStartPosition(endtime, endtime+constant::kHitobjectPreviewThreshold), "res/skin/normal-hitnormal.wav"));
}

double BeatmapParser::GetNextTiming() 
{
    if (m_timingIter==m_timingList.end()) return FLT_MAX;
    else return m_timingIter->time;
}

void BeatmapParser::PushTiming() 
{
    if (m_timingIter->uninherited) {
        int bpm=GetBPM(m_timingIter->beatLength);
        if (m_baseBPM==-1) m_baseBPM=bpm;
        m_bpmMultipliler=bpm/m_baseBPM;
    }
    else {
        m_bpmMultipliler=1./((-m_timingIter->beatLength)/100.);
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
    std::cout<<"BPM Mul: "<<m_bpmMultipliler<<std::endl;
    m_sampleIndex=m_timingIter->sampleIndex;
    m_volume=1.*m_timingIter->volume/100.;
    m_timingIter=next(m_timingIter);
}

std::string BeatmapParser::GetAudioFilePath()
{
    return m_beatmapPath+'/'+m_audioFilename;
}

int BeatmapParser::GetTotalColumns()
{
    return m_totalColumns;
}

double BeatmapParser::GetBPM(double beatLength) 
{
    return 1 / beatLength * 1000 * 60;
}

double BeatmapParser::GetSpeedScale() 
{
    return m_bpmMultipliler;
}

double BeatmapParser::GetStartPosition(double perfectHitPosition, double deltaTime) 
{
    double ret=0;
    double speed=constant::kScreenH/game_data::GetScrollMilisecond();
    auto iter=m_timingIter;
    std::vector<std::pair<double, double>> changingPoint; // position, speedMultiplier
    changingPoint.push_back({perfectHitPosition-deltaTime, game_data::scrollSpeedMultiplier});
    while (iter!=m_timingList.end() && iter->time<=perfectHitPosition) {
        if (iter->time>=changingPoint.back().first) {
            changingPoint.push_back({iter->time, 0});
        }
        if (iter->uninherited) {
            int bpm=GetBPM(iter->beatLength);
            if (m_baseBPM==-1) m_baseBPM=bpm;
            changingPoint.back().second=bpm/m_baseBPM;
        }
        else {
            changingPoint.back().second=1./(-iter->beatLength/100.);
        }
        iter=next(iter);
    }
    changingPoint.push_back({perfectHitPosition, 1});
    for (int i=0; i<changingPoint.size()-1; ++i) {
        ret+=(changingPoint[i+1].first-changingPoint[i].first)*changingPoint[i].second*speed;
    }
    // return 0;
    return -ret+game_data::hitPosition*constant::kPixelScale;
}
int BeatmapParser::GetTotalNotes()
{
    return m_nodeList.size();
}

Engine::Image BeatmapParser::GetBackgroundImage()
{
    auto [path, xOffset, yOffset]=m_bgImageInfo;
    return Engine::Image(path, xOffset*constant::kPixelScale, yOffset*constant::kPixelScale, constant::kScreenW, 0, 0, 0);
}

BeatmapParser::BeatmapImageInfo BeatmapParser::GetBackgroundImageInfo()
{
    return m_bgImageInfo;
}

bool BeatmapParser::IsVideoAvailable()
{
    return m_hasVideo;
}

std::string BeatmapParser::GetTitle() 
{
    return m_title;
}

std::string BeatmapParser::GetAuthor() 
{
    return m_artist;
}

std::string BeatmapParser::GetMapper() 
{
    return m_creator;
}

double BeatmapParser::GetStarRate()
{ 
    if (m_starRate>0) return m_starRate;
    std::vector<DifficultyCounter::DifficultyHitObject> difficultyHitObjects;
    double latestTime=0;
    for (auto note : m_nodeList) {
        std::stringstream hitObjectTokenSS(note);
        std::string token;
        int x, time, type;
        int endtime=-1;
        int hitSound; // from 0 to 3, normal, whistle, finish, clap
        // std::cout<<"read: "<<*m_nodeIter<<std::endl;

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
        else {
            endtime=time;
        }
        // std::cout<<difficultyHitObjects.size()<<": "<<time<<" "<<endtime<<" "<<time-latestTime<<" "<<x<<"\n";
        difficultyHitObjects.push_back(DifficultyCounter::DifficultyHitObject{(double)time, (double)endtime, time-latestTime, x});
        latestTime=time;
    }
    DifficultyCounter difficultyCounter(GetTotalColumns(), difficultyHitObjects);
    return m_starRate=difficultyCounter.CalculateDifficulty();
}

int BeatmapParser::GetPreviewTime()
{
    return m_previewTime;
}

std::string BeatmapParser::GetDifficultyName()
{
    return m_difficultyName;
}

int BeatmapParser::GetMode() 
{
    return m_mode;
}

int BeatmapParser::GetId() 
{
    return m_beatmapID;
}

#pragma once

#include "DifficultyCounter.h"
#include "Objects/HitObject.h"
#include "Engine/LOG.hpp"
#include "UI/Component/Image.hpp"
#include "util/GameData.h"

#include <string>
#include <list>
#include <allegro5/bitmap.h>

class BeatmapParser
{
public:
    struct BeatmapImageInfo
    {
        std::string filepath;
        double xOffset;
        double yOffset;
    };
    BeatmapParser(const int& beatmapID, const std::string& difficultyName);
    ~BeatmapParser()=default;
    void Restart();
    std::string GetAudioFilePath();
    int GetPreviewTime();
    int GetCountDown();
    int GetTotalNotes();
    int GetTotalColumns();
    double GetBPM(double beatLength);
    double GetSpeedScale();
    double GetStartPosition(double perfectHitPosition, double deltaTime);
    double GetNextTiming();
    std::unique_ptr<HitObject> GetNextHitObject();
    Engine::Image GetBackgroundImage();
    BeatmapImageInfo GetBackgroundImageInfo();
    std::string GetTitle();
    std::string GetAuthor();
    std::string GetMapper();
    double GetStarRate();
    std::string GetDifficultyName();
    int GetMode();
    int GetId();
    void PushTiming();
    bool IsMapEnded();
    bool IsVideoAvailable();

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
        double beatLength; 
        int meter; 
        int sampleSet;
        int sampleIndex;
        int volume; // percentage
        bool uninherited;
        int effects;
    };

    int m_beatmapID=-1;
    std::string m_difficultyName;
    std::string m_beatmapPath;
    std::string m_beatmapFile;

    // General
    int m_section=-1;
    std::string m_audioFilename="";
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
    double m_OD=10;
    double m_starRate=0.;

    // Event
    BeatmapImageInfo m_bgImageInfo;
    std::shared_ptr<Engine::Image> m_backgroundImage;
    bool m_hasVideo=false;

    // Timing Points
    std::list<Timing> m_timingList;
    std::list<Timing>::const_iterator m_timingIter;
    double m_baseBPM=-1;
    double m_bpmMultipliler=1.;
    int m_sampleIndex=0;
    double m_volume=1.;

    // Node
    std::list<std::string> m_nodeList;
    std::list<std::string>::const_iterator m_nodeIter;

    void Parse(const std::string& str);
};

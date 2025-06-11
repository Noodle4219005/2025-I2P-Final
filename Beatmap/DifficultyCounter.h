#pragma once

#include "Objects/HitObject.h"

#include <memory>
#include <vector>
#include <list>
#include <functional>

class DifficultyCounter 
{
public:
    struct DifficultyHitObject
    {
        double startTime;
        double endTime;
        double delta;
        int column;
    };
    double CalculateDifficulty();
    DifficultyCounter(int totalColumn, std::vector<DifficultyHitObject> difficultyHitObjects);
    double StrainValueOf(const DifficultyHitObject& current);
    double StrainValueAt();
    double CalculateInitialStrain(double offset);
private:
    std::vector<DifficultyHitObject> m_difficultyHitObjects;

    const double kIndividualDecayBase = 0.125;
    const double kOverallDecayBase = 0.30;
    const double kReleaseThreshold = 30;
    const double kDifficultyMultiplier = 0.018;
    const double kDecayWeight=0.9;
    const int kSectionLength = 400;

    double m_difficultyValue=0;

    double m_skillMultiplier = 1;
    double m_strainDecayBase = 1;

    std::vector<double> m_startTimes;
    std::vector<double> m_endTimes;
    std::vector<double> m_individualStrains;

    double m_individualStrain;
    double m_overallStrain;
    double m_currentStrain=0;

    double m_currentSectionPeak=0;
    double m_currentSectionEnd=0;
    std::vector<double> m_objectStrains;
    std::vector<double> m_strainPeaks;

    int m_index=0;
    

    // helper
    static inline std::function<double(double, double, double)>lerp=[](double x, double y, double a)
    {
        return x * (1-a) + y * a;
    };

    DifficultyHitObject Previous(int backwardsIndex) const;
    void Process();
    void StartNewSectionFrom(double time);
    double StrainDecay(double time);
    std::vector<double> GetCurrentStrainPeaks();
};

#include "DifficultyCounter.h"
#include "Objects/HitObject.h"

#include <list>
#include <algorithm>

// Modified from osu mania difficulty counter
// Reference: https://github.com/ppy/osu/blob/master/osu.Game.Rulesets.Mania/Difficulty/Skills/Strain.cs

double Logistic(double x, double midpointOffset, double multiplier, double maxValue = 1);
double applyDecay(double value, double deltaTime, double decayBase);

DifficultyCounter::DifficultyCounter(int totalColumn, std::vector<DifficultyHitObject> difficultyHitObjects):
    m_difficultyHitObjects(difficultyHitObjects)
{
    m_startTimes = std::vector<double>(totalColumn, 0);
    m_endTimes = std::vector<double>(totalColumn, 0);
    m_individualStrains = std::vector<double>(totalColumn, 0);
    m_overallStrain = 1;
}

double DifficultyCounter::CalculateDifficulty()
{
    const double kReducedSectionCount = 10;
    const double kReducedStrainBaseline = 0.75;
    double difficultyValue=0;
    double weight=1.;
    std::vector<double> peaks;
    for (auto peak : GetCurrentStrainPeaks()) {
        if (peak>0) peaks.push_back(peak);
    }
    std::sort(peaks.begin(), peaks.end(), std::greater<double>());
    for (int i=0; i<std::min((double)peaks.size(), kReducedSectionCount); i++) {
        double scale=log10(lerp(1, 10, std::clamp(i/kReducedSectionCount, 0.0, 1.0)));
        peaks[i]*=lerp(kReducedStrainBaseline, 1, scale);
    }
    std::sort(peaks.begin(), peaks.end(), std::greater<double>());
    for (auto strain : peaks) {
        difficultyValue+=strain * weight;
        weight*=kDecayWeight;
    }

    return difficultyValue*kDifficultyMultiplier;
}

double DifficultyCounter::StrainValueOf(const DifficultyHitObject& current) 
{
    double startTime=current.startTime;
    double endTime=current.endTime;
    int column=current.column;
    bool isOverlapping=false;

    double closestEndTime=abs(endTime-startTime);
    double holdFactor=1.f;
    double holdAddition=0.f;

    for (int i=0; i<m_endTimes.size(); ++i) {
        isOverlapping |= (m_endTimes[i]>startTime) && (endTime>m_endTimes[i]) && (startTime>m_startTimes[i]);
        if (m_endTimes[i]>endTime && startTime>m_startTimes[i]) holdFactor=1.25;
        closestEndTime=std::min(closestEndTime, abs(endTime-m_endTimes[i]));
    }

    // Calculate the sigmoid curve
    if (isOverlapping) {
        holdAddition=Logistic(closestEndTime, kReleaseThreshold, 0.27);
    }

    m_individualStrains[column]=applyDecay(m_individualStrains[column], startTime-m_startTimes[column], kIndividualDecayBase);
    m_individualStrains[column]+=2.0*holdFactor;

    m_individualStrain=(current.delta <= 1)?(std::max(m_individualStrain, m_individualStrains[column])):(m_individualStrains[column]);

    m_overallStrain=applyDecay(m_overallStrain, current.delta, kOverallDecayBase);
    m_overallStrain+=(1+holdAddition) * holdFactor;

    return m_individualStrain + m_overallStrain - m_currentStrain;
}

double DifficultyCounter::StrainValueAt()
{
    DifficultyHitObject current=m_difficultyHitObjects[m_index];
    m_currentStrain *= StrainDecay(current.delta);
    m_currentStrain += StrainValueOf(current) * m_skillMultiplier;
    return m_currentStrain;
}

double DifficultyCounter::CalculateInitialStrain(double offset) {
    return applyDecay(m_individualStrain, offset - Previous(0).startTime, kIndividualDecayBase) 
        + applyDecay(m_overallStrain, offset - Previous(0).startTime, kOverallDecayBase);
}

void DifficultyCounter::Process()
{
    DifficultyHitObject current=m_difficultyHitObjects[m_index];
    if (m_index==0) {
        m_currentSectionEnd=__ceil(current.startTime / kSectionLength) * kSectionLength;
    }

    while (current.startTime > m_currentSectionEnd) {
        m_strainPeaks.push_back(m_currentSectionPeak); // save strain peak
        StartNewSectionFrom(m_currentSectionEnd);
        m_currentSectionEnd+kSectionLength;
    }

    double strain=StrainValueAt();
    m_currentSectionPeak=std::max(m_currentSectionPeak, strain);
    m_objectStrains.push_back(strain);
}

void DifficultyCounter::StartNewSectionFrom(double time) 
{
    m_currentSectionPeak=CalculateInitialStrain(time);

}

DifficultyCounter::DifficultyHitObject DifficultyCounter::Previous(int backwardsIndex) const
{
    int index = m_index - (backwardsIndex + 1);
    if (index>=0 && index<m_difficultyHitObjects.size()) return m_difficultyHitObjects[index];
    else return DifficultyCounter::DifficultyHitObject{};
}

std::vector<double> DifficultyCounter::GetCurrentStrainPeaks()
{
    std::vector<double> ret(m_strainPeaks);
    ret.push_back(m_currentSectionPeak);
    return ret;
}

/// <summary>
/// Calculates a S-shaped logistic function (https://en.wikipedia.org/wiki/Logistic_function)
/// </summary>
/// <param name="x">Value to calculate the function for</param>
/// <param name="maxValue">Maximum value returnable by the function</param>
/// <param name="multiplier">Growth rate of the function</param>
/// <param name="midpointOffset">How much the function midpoint is offset from zero <paramref name="x"/></param>
/// <returns>The output of logistic function of <paramref name="x"/></returns>
double Logistic(double x, double midpointOffset, double multiplier, double maxValue)
{
    return maxValue / (1 + exp(multiplier * (midpointOffset - x)));
}

double applyDecay(double value, double deltaTime, double decayBase)
{
    return value * pow(decayBase, deltaTime / 1000);
}

double DifficultyCounter::StrainDecay(double time)
{
    return pow(m_strainDecayBase, time / 1000);
}

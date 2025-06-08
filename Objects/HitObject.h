#pragma once

#include <string>

class HitObject
{
public:
    HitObject(int x, int time, int type, int endTime, float positionY, float positionEndY, const std::string& hitSoundPath);
    HitObject(int x, int time, int type, float positionY, const std::string& hitSoundPath);
    HitObject();
    virtual ~HitObject()=default;
    int GetStartTime();
    int GetEndTime();
    int GetColumn();
    float GetPositionY() const;
    int GetType() const;
    bool IsAlive() const;
    bool IsAvailable() const;
    void Init();
    virtual void OnKeyDown() { ; }
    virtual void OnKeyUp() { ; }
    virtual void Update();
    virtual void Draw() const;

protected:
    int m_column=0;
    int m_time=0;
    int m_endTime=0;
    int m_type=0;
    float m_speed=1.;
    float m_positionY=0;
    float m_positionEndY=0;
    double m_lastUpdateTime=0;
    std::string m_hitSoundPath="";
    int m_perfectHitPosition=0;
    bool m_isKeyDown=false;
    bool m_isFirstKeyDown=true;
    int m_firstKeyPosition=0;
    bool m_isAlive=true; // whether the key should be drawn.
    bool m_isAvailable=true; // whether the key can be hit.
    int m_hitValue=-1; // Judgement
    int m_hitError=-1;
    int m_accumulatedCombo=0;
};
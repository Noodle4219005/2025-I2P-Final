#pragma once

#include <string>

class HitObject
{
public:
    HitObject(int x, int time, int type, int endTime, const std::string& hitSoundPath);
    HitObject(int x, int time, int type, const std::string& hitSoundPath);
    HitObject()=default;
    int GetStartTime();
    int GetColumn();
    bool IsAlive();
    void virtual OnKeyDown() { ; }
    void virtual OnKeyUp() { ; }
    void virtual Update();
    void virtual Draw() const;

protected:
    int m_column=0;
    int m_time=0;
    int m_endTime=0;
    int m_type=0;
    std::string m_hitSoundPath="";
    int m_perfectHitPosition=0;
    bool m_isKeyDown=false;
    bool m_isFirstKeyDown=true;
    int m_firstKeyPosition=0;
    bool m_isAlive=true;
};
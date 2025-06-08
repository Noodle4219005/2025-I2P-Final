#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <allegro5/color.h>

class Skin 
{
public:
    // Singleton
    Skin();
    Skin(Skin const&)=delete;
    void operator=(Skin const&)=delete;
    static Skin& GetInstance();

    void DrawHold(int start, int end) const;
    void Draw(int start, int end) const;
    void DrawScore(int score, float acc) const;
    void DrawCombo(int combo, float comboExpand) const;
    void DrawHit(int val, float expand) const;
    void DrawPlaySkip() const;
    void DrawPause() const;
private:
    enum Sections
    {
        GENERAL,
        FONTS,
        MANIA
    };

    void Parse(const std::string& str);
    int CalNumDigits(int number) const;

    int m_section=-1;

    // General
    int m_animationFramerate=1;
    int m_comboBurstRandom=0;
    std::string m_customComboBurstSounds;
    // LayeredHitSound is not available for mania

    //  Fonts
    std::string m_scorePrefix="score";
    int m_scoreOverlap=0;
    std::string m_comboPrefix="score";
    int m_comboOverlap=0;

    // Mania
    int m_keys=0;

    struct ManiaSkinData
    {
        float columnStart=136; // Where does the left column start?
        float columnRight=19; // Up to which point can columns be drawn?
        float columnSpacing=0; // What is the distance between all columns individually?
        float columnWidth=30; // What widths do all columns have individually?
        float columnLineWidth=2; // How thick are the column separators individually? 
        float barLineHeight=1.2; // How thick is the barline?
        std::vector<float> lightingNWidth; // Which widths should LightingN use for all columns individually?
        std::vector<float> lightingLWidth; // Which widths should LightingL use for all columns individually?
        // If not defined, the height scale of the smallest column width is used
        float widthForNoteHeightScale; // Which height should all notes have if columns have individual widths?
        int hitPosition=402; // from 240 to 480
        int lightPosition=413; // On which height should the stage lights be drawn at?
        int scorePosition; // the height score burst appear
        int comboPosition; // the height combo appear
        int judgeLine=0;
        int lightFramePerSecond;
        int specialStyle=0;
        int comboBurstStyle=0;
        int splitStage=0;
        bool keysUnderNodes=0;
        bool upsideDown=0;
        bool keyFlipWhenUpsideDown=0;
        std::vector<ALLEGRO_COLOR> colour;
        std::vector<ALLEGRO_COLOR> colourLight;
        ALLEGRO_COLOR colourColumnLine=al_map_rgb(255, 255, 255);
        ALLEGRO_COLOR colourBarline=al_map_rgba(255, 255, 255, 255);
        ALLEGRO_COLOR colourJudgmentLine=al_map_rgb(255, 255, 255);
        ALLEGRO_COLOR colourKeyWarning=al_map_rgb(0, 0, 0);
        ALLEGRO_COLOR colourHold=al_map_rgba(255, 191, 51, 255);
        ALLEGRO_COLOR colourBreak=al_map_rgb(255, 0, 0);
        // The followings are all .png file by default.
        std::vector<std::string> keyImage={"mania-key1", "mania-key2", "mania-keyS"};
        std::vector<std::string> keyImageD={"mania-key1D", "mania-key2D", "mania-keySD"};
        std::vector<std::string> noteImage={"mania-note1", "mania-note2", "mania-noteS"};
        std::vector<std::string> noteImageH={"mania-note1H", "mania-note2H", "mania-noteSH"};
        std::vector<std::string> noteImageL={"mania-note1L", "mania-note2L", "mania-noteSL"};
        std::vector<std::string> noteImageT={"mania-note1T", "mania-note2T", "mania-noteST"};
        std::string stageLeft="mania-stage-left";
        std::string stageRight="mania-stage-right";
        std::string stageBottom="mania-stage-bottom"; // This element is 0.625x smaller than the stage width.
        std::string stageHint="mania-stage-hint";
        std::string stageLight="mania-stage-light";
        std::string lightingN="lightingN";
        std::string lightingL="lightingL";
        std::string warningArrow="mania-warningarrow";
        std::string hit0="mania-hit0";
        std::string hit50="mania-hit50";
        std::string hit100="mania-hit100";
        std::string hit200="mania-hit200";
        std::string hit300="mania-hit300";
        std::string hit300g="mania-hit300g";
    };

    std::unordered_map<int, ManiaSkinData> m_nkManiaSkin;
};
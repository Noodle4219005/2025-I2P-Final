#include "RankingPanel.h"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "util/Constant.h"
#include "util/GameData.h"
#include "Skin/Skin.h"

#include <iostream>
#include <allegro5/allegro_primitives.h>

void RankingPanel::Initialize()
{
    m_music=AudioHelper::PlaySample(constant::kSkinPath+'/'+"applause.mp3", false, AudioHelper::BGMVolume, 0);
    std::string filename="";
    filename=constant::kSkinPath+'/'+"menu-back-0@2x.png";
    m_backButton=std::move(std::make_unique<Engine::ImageButton>
                           (
                               Engine::ImageButton
                               (
                                   filename, filename,
                                   0, constant::kScreenH,
                                   0, 0, 0, 1
                               )
                           ));
    m_backButton->SetOnClickCallback(std::bind(&RankingPanel::BackToMenu, this));
    m_beatmap=std::make_unique<BeatmapParser>(BeatmapParser(game_data::mapID, game_data::difficultyName));
}

void RankingPanel::Terminate()
{
    AudioHelper::StopSample(m_music);
    m_beatmap.release();
}

void RankingPanel::OnMouseMove(int mx, int my)
{
    m_backButton->OnMouseMove(mx, my);
}

void RankingPanel::OnMouseDown(int button, int mx, int my)
{
    std::cout<<"clicked "<<button<<" "<<mx<<" "<<my<<"\n";
    m_backButton->OnMouseDown(button, mx, my);
}

void RankingPanel::OnKeyDown(int keyCode)
{
    if (keyCode==ALLEGRO_KEY_ESCAPE) BackToMenu();
}

void RankingPanel::Update(double deltaTime)
{
    m_backButton->Update(deltaTime);
}

void RankingPanel::Draw() const
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    m_beatmap->GetBackgroundImage().Draw();
    al_draw_filled_rectangle(0, 0, constant::kScreenW, constant::kScreenH, al_map_rgba(0, 0, 0, 255*game_data::backgroundDim));
    Engine::Image hit300(constant::kSkinPath+'/'+"hit300.png",
                         -50*constant::kPixelScale, constant::kRankingResultsPosition*constant::kPixelScale,
                          0, 0, 0, 0.5
    );
    Engine::Image hit300g(constant::kSkinPath+'/'+"hit300g.png",
                          150*constant::kPixelScale, constant::kRankingResultsPosition*constant::kPixelScale,
                          0, 0, 0, 0.5
    );
    Engine::Image hit100(constant::kSkinPath+'/'+"hit100.png",
                         -50*constant::kPixelScale, (constant::kRankingResultsPosition+75)*constant::kPixelScale,
                          0, 0, 0, 0.5
    );
    Engine::Image hit200(constant::kSkinPath+'/'+"hit100k.png",
                         150*constant::kPixelScale, (constant::kRankingResultsPosition+75)*constant::kPixelScale,
                          0, 0, 0, 0.5
    );
    Engine::Image hit50(constant::kSkinPath+'/'+"hit50.png",
                        -50*constant::kPixelScale, (constant::kRankingResultsPosition+150)*constant::kPixelScale,
                          0, 0, 0, 0.5
    );
    Engine::Image hit0(constant::kSkinPath+'/'+"hit0.png",
                       200*constant::kPixelScale, (constant::kRankingResultsPosition+150)*constant::kPixelScale,
                       0, 0, 0, 0.5
    );
    Skin::GetInstance().DrawRankingPanel();
    hit300g.Draw();
    Skin::GetInstance().DrawRankingNumber
    (
        game_data::hit300H,
        150, constant::kRankingResultsPosition
    );
    hit300.Draw();
    Skin::GetInstance().DrawRankingNumber
    (
        game_data::hit300,
        -50, constant::kRankingResultsPosition
    );
    hit200.Draw();
    Skin::GetInstance().DrawRankingNumber
    (
        game_data::hit200,
        150, constant::kRankingResultsPosition+75
    );
    hit100.Draw();
    Skin::GetInstance().DrawRankingNumber
    (
        game_data::hit100,
        -50, constant::kRankingResultsPosition+75
    );
    hit50.Draw();
    Skin::GetInstance().DrawRankingNumber
    (
        game_data::hit50,
        -50, constant::kRankingResultsPosition+150
    );
    hit0.Draw();
    Skin::GetInstance().DrawRankingNumber
    (
        game_data::miss,
        150, constant::kRankingResultsPosition+150
    );
    Skin::GetInstance().DrawRankingScore(game_data::score , constant::kRankingScorePositionX, constant::kRankingScorePositionY);

    // Draw max combo
    Skin::GetInstance().DrawRankingNumber
    (
        game_data::maxCombo,
        -100, constant::kRankingMaxComboPosition
    );

    Skin::GetInstance().DrawRankingAccuracy(game_data::accuracy);

    m_backButton->Draw();

    // Draw the sign
    int positionX=constant::kOffsetX+550*constant::kPixelScale;
    std::string filename="";
    if (game_data::accuracy==100) {
        filename=constant::kSkinPath+'/'+"ranking-X.png";
    }
    else if (game_data::accuracy>=95) {
        filename=constant::kSkinPath+'/'+"ranking-S.png";
    }
    else if (game_data::accuracy>=90) {
        filename=constant::kSkinPath+'/'+"ranking-A.png";
    }
    else if (game_data::accuracy>=80) {
        filename=constant::kSkinPath+'/'+"ranking-B.png";
    }
    else if (game_data::accuracy>=70) {
        filename=constant::kSkinPath+'/'+"ranking-C.png";
    }
    else {
        filename=constant::kSkinPath+'/'+"ranking-D.png";
    }
    Engine::Image image(filename,
                       positionX, constant::kRankingSignPosition*constant::kPixelScale,
                       0, 0, 0.5, 0.5
    );
    image.Draw();

    // Mode Selection
    int selection=0;
    if (game_data::isAuto) selection+=1;
    if (game_data::isDoubleTime) selection+=2;
    if (game_data::isNoFailed) selection+=4;
    Skin::GetInstance().DrawRankingMod(selection);
}

void RankingPanel::BackToMenu()
{
    Engine::GameEngine::GetInstance().ChangeScene("menu");
}

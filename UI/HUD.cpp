#include <util/Constant.h>
#include "HUD.h"

void HUD::Init()
{
    m_musicBarX = constant::kScreenW * 0.01f;
    m_musicBarY = constant::kScreenH * 0.05f;
    m_musicBarWidth = std::max(constant::kScreenW * 0.005f, 7.f);
    m_musicBarHeight = constant::kScreenH * 0.9f;
}

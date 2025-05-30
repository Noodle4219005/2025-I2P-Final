#include "Node.h"
#include "Skin/Skin.h"
#include "UI/Component/Image.hpp"
#include "util/GameData.h"
#include "util/ErrorCalculator.h"

void Node::OnKeyDown() 
{
    if (GetHitValue(m_perfectHitPosition) != NONE && m_isFirstKeyDown) {
        m_isFirstKeyDown=false;
        m_isKeyDown=true;
        m_firstKeyPosition=game_data::gamePosition;
    }
}

void Node::OnKeyUp() 
{
}

void Node::Update() 
{
}

void Node::Draw() const 
{
}
